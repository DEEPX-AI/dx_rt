# ============================================================================
# dx_rt Build Script for Windows (PowerShell)
# Usage: build.ps1 [options]
# ============================================================================

function Request-AdminElevation {
    if (([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
            [Security.Principal.WindowsBuiltInRole]::Administrator)) {
        return
    }
    $scriptArgs = $script:ScriptArgs -join ' '
    Write-Host ""
    Write-Status ERROR "Administrator privileges required for this operation."
    Write-Host ""
    Write-Host "  Please run from an elevated (Administrator) command prompt:" -ForegroundColor Yellow
    Write-Host "    build.bat $scriptArgs" -ForegroundColor White
    Write-Host ""
    exit 1
}

# --- Script-level variables ---
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$ScriptArgs = $args
$BuildType = "Release"
$DoConfigure = $true
$DoBuild = $true
$DoInstall = $false
$DoClean = $false
$DoRebuild = $false
$DoDistclean = $false
$DoAll = $false
$DoVsProject = $false
$DoInstallTools = $false
$DoSetup = $false
$DoUnsetup = $false
$NoArgs = $false
$RestartDxrtd = $false

# Set VCPKG_DOWNLOADS to avoid SYSTEM account AppData path issues (CI/service)
if (-not $env:VCPKG_DOWNLOADS) {
    $env:VCPKG_DOWNLOADS = Join-Path $ScriptDir "out\vcpkg-downloads"
}

# Visual Studio paths (checked in order)
$VSPaths = [ordered]@{
    Community    = "C:\Program Files\Microsoft Visual Studio\2022\Community"
    Professional = "C:\Program Files\Microsoft Visual Studio\2022\Professional"
    Enterprise   = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
    BuildTools   = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
}

# ============================================================================
# Helper functions
# ============================================================================

function Write-Status {
    param(
        [ValidateSet('OK','ERROR','WARNING','INFO')]
        [string]$Level,
        [string]$Message
    )
    $colors = @{ OK = 'Green'; ERROR = 'Red'; WARNING = 'Yellow'; INFO = 'Cyan' }
    Write-Host "[$Level] " -ForegroundColor $colors[$Level] -NoNewline
    Write-Host $Message
}

function Write-Banner {
    param([string[]]$Lines)
    Write-Host ""
    Write-Host "============================================================" -ForegroundColor DarkCyan
    foreach ($line in $Lines) {
        Write-Host $line -ForegroundColor DarkCyan
    }
    Write-Host "============================================================" -ForegroundColor DarkCyan
    Write-Host ""
}

function Remove-DirectoryQuietly {
    param([string]$Path)
    if (Test-Path $Path) {
        Remove-Item -Path $Path -Recurse -Force
        Write-Status OK "Removed $Path"
    }
}

function Find-VSInstallation {
    # Try vswhere.exe first (shipped with VS 2017+)
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $installPath = & $vswhere -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
        if ($installPath -and (Test-Path (Join-Path $installPath "VC\Auxiliary\Build\vcvars64.bat"))) {
            return $installPath
        }
    }

    # Fallback to hardcoded paths
    foreach ($edition in $VSPaths.Keys) {
        $vsPath = $VSPaths[$edition]
        $vcvars = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
        if (Test-Path $vcvars) {
            return $vsPath
        }
    }
    return $null
}

function Import-VCVars {
    param([string]$VsPath)
    $vcvars = Join-Path $VsPath "VC\Auxiliary\Build\vcvars64.bat"
    $output = cmd /c "`"$vcvars`" >nul 2>&1 && set"
    foreach ($line in $output) {
        if ($line -match '^([^=]+)=(.*)$') {
            [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
        }
    }
}

function Send-SettingChange {
    if (-not ('Win32.NativeMethods' -as [type])) {
        Add-Type -Namespace Win32 -Name NativeMethods -MemberDefinition @'
[DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
public static extern IntPtr SendMessageTimeout(
    IntPtr hWnd, uint Msg, UIntPtr wParam, string lParam,
    uint fuFlags, uint uTimeout, out UIntPtr lpdwResult);
'@
    }
    $result = [UIntPtr]::Zero
    # HWND_BROADCAST=0xffff, WM_SETTINGCHANGE=0x001A, SMTO_ABORTIFHUNG=0x0002
    [Win32.NativeMethods]::SendMessageTimeout(
        [IntPtr]0xffff, 0x001A, [UIntPtr]::Zero,
        'Environment', 0x0002, 5000, [ref]$result) | Out-Null
}

function Stop-DxrtdService {
    $svc = Get-Service -Name dxrtd -ErrorAction SilentlyContinue
    if (-not $svc -or $svc.Status -ne 'Running') { return }

    Write-Status INFO "Stopping dxrtd service..."
    try {
        Stop-Service -Name dxrtd -Force -ErrorAction Stop
        Write-Status OK "dxrtd service stopped."
        $script:RestartDxrtd = $true
        return
    } catch {}

    # Fallback to sc.exe
    try {
        sc.exe stop dxrtd 2>$null | Out-Null
        Start-Sleep -Seconds 2
        $svc = Get-Service -Name dxrtd -ErrorAction SilentlyContinue
        if ($svc.Status -eq 'Stopped') {
            Write-Status OK "dxrtd service stopped."
            $script:RestartDxrtd = $true
            return
        }
    } catch {}

    Write-Status WARNING "Failed to stop dxrtd service. Run as administrator if files are locked."
}

function Start-DxrtdService {
    Write-Host ""
    Write-Status INFO "Restarting dxrtd service..."
    try {
        Start-Service -Name dxrtd -ErrorAction Stop
        Write-Status OK "dxrtd service restarted."
        return
    } catch {}

    # Fallback to sc.exe
    try {
        sc.exe start dxrtd 2>$null | Out-Null
        if ($LASTEXITCODE -eq 0) {
            Write-Status OK "dxrtd service restarted."
            return
        }
    } catch {}

    Write-Status WARNING "Failed to restart dxrtd service. Start manually: net start dxrtd"
}

function Show-Help {
    Write-Host @"

Usage: build.bat [options]
       build.ps1 [options]

Build Type:
  release            Release configuration (default)
  debug              Debug configuration

Actions:
  (no args)          Full Release build (= build.bat all)
  configure          CMake configure only
  build              Build only (skip configure)
  install            Build and install
  clean              Clean build directory
  rebuild            Clean + configure + build
  all                Clean + configure + build + install
  distclean          Remove all build artifacts (out/, build_vs/, vcpkg_installed/)
  vs                 Generate Visual Studio 2022 solution

Service/PATH (requires Administrator):
  setup              Add install bin to system PATH + register/start dxrtd service
  unsetup            Stop/remove dxrtd service + remove PATH entry

Other:
  install-tools      Install VS Build Tools 2022 via winget
  help               Show this help

Examples:
  build.bat                      Full Release build
  build.bat debug                Full Debug build
  build.bat configure            Configure only (Release)
  build.bat build                Build only, skip configure (Release)
  build.bat debug build          Build only, skip configure (Debug)
  build.bat install              Build + install (Release)
  build.bat debug install        Build + install (Debug)
  build.bat clean                Clean Release build directory
  build.bat debug clean          Clean Debug build directory
  build.bat rebuild              Clean + configure + build (Release)
  build.bat all                  Clean + configure + build + install (Release)
  build.bat debug all            Clean + configure + build + install (Debug)
  build.bat distclean            Remove all build artifacts
  build.bat vs                   Generate Visual Studio solution (build_vs\)
  build.bat setup                Register PATH + start dxrtd service (Release)
  build.bat debug setup          Register PATH + start dxrtd service (Debug)
  build.bat unsetup              Remove dxrtd service + PATH entry (Release)
  build.bat install-tools        Install VS Build Tools 2022

Note: build.bat is a thin wrapper that calls build.ps1.
      You can also run build.ps1 directly from PowerShell:
        .\build.ps1 debug build

"@
}

function Remove-CSharpBuildArtifacts {
    Write-Status INFO "Cleaning csharp_package build artifacts..."
    $csharpDirs = @(
        "csharp_package\bin"
        "csharp_package\packages"
        "csharp_package\.vs"
        "csharp_package\src\DxEngine\bin"
        "csharp_package\src\DxEngine\obj"
        "csharp_package\src\DxEngineNative\bin"
        "csharp_package\src\DxEngineNative\Release"
        "csharp_package\src\DxEngineNative\Debug"
        "csharp_package\examples\.vs"
        "csharp_package\examples\SimpleInference\bin"
        "csharp_package\examples\SimpleInference\obj"
        "csharp_package\examples\YoloV5\bin"
        "csharp_package\examples\YoloV5\obj"
    )
    foreach ($dir in $csharpDirs) {
        Remove-DirectoryQuietly (Join-Path $ScriptDir $dir)
    }
}

function Build-CSharpPackage {
    Write-Host ""
    Write-Status INFO "Building C# package (csharp_package)..."
    Write-Host ""

    $csproj = Join-Path $ScriptDir "csharp_package\src\DxEngine\DxEngine.csproj"
    if (-not (Test-Path $csproj)) {
        Write-Status INFO "C# project not found, skipping csharp_package build."
        return
    }

    # Detect .NET SDK
    $dotnetPath = $null
    $dotnetCmd = Get-Command dotnet -ErrorAction SilentlyContinue
    if ($dotnetCmd) {
        $dotnetPath = Split-Path $dotnetCmd.Source
    } elseif (Test-Path "C:\Program Files\dotnet\dotnet.exe") {
        $dotnetPath = "C:\Program Files\dotnet"
    }

    if (-not $dotnetPath) {
        Write-Status WARNING ".NET SDK not found, skipping C# package build."
        Write-Status WARNING "Install .NET 8 SDK from: https://dotnet.microsoft.com/download/dotnet/8.0"
        Write-Status WARNING "  or run: winget install Microsoft.DotNet.SDK.8"
        return
    }

    $env:PATH = "$dotnetPath;$env:PATH"
    Write-Status INFO ".NET SDK found at: $dotnetPath"

    # Set MSBuildSDKsPath for VS Build Tools MSBuild
    $sdkDirs = Get-ChildItem -Path (Join-Path $dotnetPath "sdk") -Filter "8.*" -Directory -ErrorAction SilentlyContinue |
        Sort-Object Name -Descending
    if ($sdkDirs) {
        $env:MSBuildSDKsPath = Join-Path $sdkDirs[0].FullName "Sdks"
        Write-Status INFO "MSBuildSDKsPath set to: $env:MSBuildSDKsPath"
    }

    # Read version from release.ver
    $csharpVersion = "0.0.0"
    $verFile = Join-Path $ScriptDir "release.ver"
    if (Test-Path $verFile) {
        $csharpVersion = (Get-Content $verFile -First 1).Trim()
    }
    if ($csharpVersion.StartsWith("v")) {
        $csharpVersion = $csharpVersion.Substring(1)
    }
    Write-Status INFO "Package version: $csharpVersion (from release.ver)"

    Push-Location (Join-Path $ScriptDir "csharp_package")
    try {
        # Build DxEngineNative C++/CLI wrapper
        Write-Status INFO "Building DxEngineNative C++/CLI wrapper..."
        $nativeProj = Join-Path $ScriptDir "csharp_package\src\DxEngineNative\DxEngineNative.vcxproj"
        if (Test-Path $nativeProj) {
            Push-Location (Join-Path $ScriptDir "csharp_package\src\DxEngineNative")
            try {
                msbuild DxEngineNative.vcxproj "/p:Configuration=$script:BuildType" /p:Platform=x64 /p:VcpkgEnableManifest=false /v:minimal
                if ($LASTEXITCODE -ne 0) {
                    Write-Status WARNING "DxEngineNative build failed, native DLL will not be available."
                } else {
                    Write-Status OK "DxEngineNative.dll built successfully."
                }
            } finally {
                Pop-Location
            }
        } else {
            Write-Status INFO "DxEngineNative.vcxproj not found, skipping native build."
        }

        # Build DxEngine NuGet package
        Write-Status INFO "Building DxEngine NuGet package..."
        Push-Location (Join-Path $ScriptDir "csharp_package\src\DxEngine")
        try {
            dotnet restore
            if ($LASTEXITCODE -ne 0) {
                Write-Status WARNING "C# DxEngine restore failed, skipping C# build."
                return
            }
            dotnet build -c $script:BuildType /p:Version=$csharpVersion
            if ($LASTEXITCODE -ne 0) {
                Write-Status WARNING "C# DxEngine build failed."
                return
            }
            $packagesDir = Join-Path $ScriptDir "csharp_package\packages"
            Get-ChildItem -Path $packagesDir -Filter "*.nupkg" -ErrorAction SilentlyContinue | Remove-Item -Force
            dotnet pack -c $script:BuildType /p:Version=$csharpVersion -o "..\..\packages"
            if ($LASTEXITCODE -ne 0) {
                Write-Status WARNING "C# DxEngine pack failed."
                return
            }
            Write-Status OK "DxEngine NuGet package created."
        } finally {
            Pop-Location
        }

        # Build C# examples
        Write-Status INFO "Building C# examples..."
        Push-Location (Join-Path $ScriptDir "csharp_package\examples")
        try {
            $examplesFailed = $false

            $simpleInference = "SimpleInference\SimpleInference.csproj"
            if (Test-Path $simpleInference) {
                Write-Status INFO "  Building SimpleInference..."
                dotnet build $simpleInference -c $script:BuildType
                if ($LASTEXITCODE -ne 0) { Write-Status WARNING "SimpleInference build failed."; $examplesFailed = $true }
            }

            $yoloProj = "YoloV5\YoloV5Example.csproj"
            if (Test-Path $yoloProj) {
                Write-Status INFO "  Building YoloV5Example..."
                dotnet build $yoloProj -c $script:BuildType
                if ($LASTEXITCODE -ne 0) { Write-Status WARNING "YoloV5Example build failed."; $examplesFailed = $true }
            }

            if (-not $examplesFailed) {
                Write-Status OK "C# examples build completed."
            }
        } finally {
            Pop-Location
        }
    } finally {
        Pop-Location
    }

    Write-Host ""
    Write-Status OK "C# package build completed successfully!"
    Write-Host "    - NuGet package: csharp_package\packages\"
    Write-Host "    - Examples:      csharp_package\examples\"
}

function Install-BuildTools {
    Write-Banner "  Visual Studio Build Tools 2022 Installer"

    # Check if already installed
    foreach ($edition in @('BuildTools', 'Community', 'Professional', 'Enterprise')) {
        $vcvars = Join-Path $VSPaths[$edition] "VC\Auxiliary\Build\vcvars64.bat"
        if (Test-Path $vcvars) {
            if ($edition -eq 'BuildTools') {
                Write-Status INFO "Visual Studio Build Tools 2022 is already installed at:"
                Write-Host "       $($VSPaths[$edition])"
            } else {
                Write-Status INFO "Visual Studio 2022 $edition is already installed."
                Write-Host "         Build Tools installation is not necessary."
            }
            return
        }
    }

    # Check for winget
    if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
        Write-Status ERROR "winget is not available on this system."
        Write-Host ""
        Write-Host "Please install Build Tools manually:"
        Write-Host "  1. Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/"
        Write-Host "  2. Run the installer"
        Write-Host "  3. Select `"Desktop development with C++`" workload"
        Write-Host ""
        exit 1
    }

    Write-Status INFO "Installing Visual Studio Build Tools 2022 via winget..."
    Write-Host ""
    Write-Host "This will install:"
    Write-Host "  - MSVC C++ compiler"
    Write-Host "  - Windows SDK"
    Write-Host "  - CMake"
    Write-Host "  - vcpkg"
    Write-Host ""
    Write-Host "The installation may take 10-30 minutes depending on your internet speed."
    Write-Host ""

    $confirm = Read-Host "Do you want to continue? [y/N]"
    if ($confirm -ne 'y' -and $confirm -ne 'Y') {
        Write-Host "Installation cancelled."
        return
    }

    Write-Host ""
    Write-Status INFO "Starting installation..."
    Write-Host ""

    winget install Microsoft.VisualStudio.2022.BuildTools --silent --accept-package-agreements --accept-source-agreements --override "--wait --quiet --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22621 --add Microsoft.VisualStudio.Component.VC.CMake.Project --add Microsoft.Component.VC.Runtime.UCRTSDK --includeRecommended"

    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Status ERROR "Installation failed!"
        Write-Host ""
        Write-Host "Please try manual installation:"
        Write-Host "  1. Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/"
        Write-Host "  2. Run the installer"
        Write-Host "  3. Select `"Desktop development with C++`" workload"
        Write-Host ""
        exit 1
    }

    Write-Host ""
    Write-Status OK "Visual Studio Build Tools 2022 installed successfully!"
    Write-Host ""
    Write-Host "[IMPORTANT] Please restart your terminal/command prompt before building." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "After restart, run:"
    Write-Host "  build.bat"
    Write-Host ""
}

# ============================================================================
# Parse arguments
# ============================================================================

if ($args.Count -eq 0) {
    $DoAll = $true
    $NoArgs = $true
} else {
    $i = 0
    while ($i -lt $args.Count) {
        switch ($args[$i].ToLower()) {
            'debug'         { $BuildType = "Debug" }
            'release'       { $BuildType = "Release" }
            'vs'            { $DoVsProject = $true; $DoConfigure = $false; $DoBuild = $false }
            'configure'     { $DoBuild = $false }
            'build'         { $DoConfigure = $false }
            'clean'         { $DoClean = $true; $DoConfigure = $false; $DoBuild = $false }
            'rebuild'       { $DoRebuild = $true }
            'install'       { $DoInstall = $true }
            'distclean'     { $DoDistclean = $true; $DoConfigure = $false; $DoBuild = $false }
            'all'           { $DoAll = $true }
            'install-tools' { $DoInstallTools = $true; $DoConfigure = $false; $DoBuild = $false }
            'setup'         { $DoSetup = $true; $DoConfigure = $false; $DoBuild = $false }
            'unsetup'       { $DoUnsetup = $true; $DoConfigure = $false; $DoBuild = $false }
            { $_ -in 'help', '-h', '--help', '/?' } { Show-Help; exit 0 }
            default {
                Write-Host "Unknown option: $($args[$i])"
                Show-Help
                exit 1
            }
        }
        $i++
    }
}

# If only build type was specified (no action), default to "all"
if (-not $DoAll -and -not $DoClean -and -not $DoRebuild -and -not $DoDistclean -and
    -not $DoVsProject -and -not $DoInstallTools -and -not $DoSetup -and -not $DoUnsetup -and
    -not $DoInstall -and $DoConfigure -and $DoBuild) {
    $DoAll = $true
}

# ============================================================================
# Handle install-tools command first
# ============================================================================
if ($DoInstallTools) {
    Install-BuildTools
    exit 0
}

# ============================================================================
# Handle setup/unsetup (no VS environment needed)
# ============================================================================
if ($DoSetup) {
    Request-AdminElevation
    Write-Banner "  dx_rt Setup (PATH + Service)"

    $installBin = Join-Path $ScriptDir "out\install\x64-$BuildType\bin"
    if (-not (Test-Path (Join-Path $installBin "dxrtd.exe"))) {
        Write-Status ERROR "dxrtd.exe not found at: $installBin"
        Write-Host "        Run 'build.bat all' or 'build.bat install' first."
        exit 1
    }

    Write-Host "  Build Type : $BuildType"
    Write-Host "  Bin Path   : $installBin"
    Write-Host ""

    # Step 1: Add to system PATH
    Write-Status INFO "Registering bin directory to system PATH..."
    $regKey = 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment'
    $currentPath = (Get-ItemProperty $regKey -Name Path).Path
    $pathEntries = $currentPath -split ';'
    if ($pathEntries | Where-Object { $_ -ieq $installBin }) {
        Write-Status INFO "PATH already contains: $installBin"
    } else {
        $newPath = $currentPath.TrimEnd(';') + ';' + $installBin
        Set-ItemProperty $regKey -Name Path -Value $newPath
        Write-Status OK "Added to system PATH: $installBin"
        Write-Status INFO "New terminal sessions will pick up the updated PATH."
    }

    # Broadcast WM_SETTINGCHANGE
    Send-SettingChange

    # Step 2: Install and start dxrtd service
    Write-Host ""
    $existingSvc = Get-Service -Name dxrtd -ErrorAction SilentlyContinue
    if ($existingSvc) {
        Write-Status INFO "dxrtd service already exists. Removing before re-register..."
        if ($existingSvc.Status -eq 'Running') {
            Stop-DxrtdService
            $script:RestartDxrtd = $false  # setup will start it explicitly
        }
        & "$installBin\dxrtd.exe" --uninstall 2>$null
        Start-Sleep -Seconds 1
    }
    Write-Status INFO "Registering dxrtd service..."
    & "$installBin\dxrtd.exe" --install
    if ($LASTEXITCODE -ne 0) {
        Write-Status ERROR "Service registration failed."
        exit 1
    }
    Write-Status OK "dxrtd service registered."

    Write-Status INFO "Starting dxrtd service..."
    & "$installBin\dxrtd.exe" --start
    if ($LASTEXITCODE -ne 0) {
        Write-Status ERROR "Failed to start dxrtd service."
        exit 1
    }
    Write-Status OK "dxrtd service started."

    Write-Banner @(
        "  Setup completed successfully!"
        "  - System PATH updated"
        "  - dxrtd service installed and running"
    )
    exit 0
}

if ($DoUnsetup) {
    Request-AdminElevation
    Write-Banner "  dx_rt Unsetup (Service + PATH)"

    $installBin = Join-Path $ScriptDir "out\install\x64-$BuildType\bin"

    Write-Host "  Build Type : $BuildType"
    Write-Host "  Bin Path   : $installBin"
    Write-Host ""

    # Step 1: Stop and uninstall dxrtd service
    if (Test-Path (Join-Path $installBin "dxrtd.exe")) {
        Write-Status INFO "Stopping dxrtd service..."
        & "$installBin\dxrtd.exe" --stop 2>$null
        Write-Status INFO "Uninstalling dxrtd service..."
        & "$installBin\dxrtd.exe" --uninstall 2>$null
        Write-Status OK "dxrtd service removed."
    } else {
        Write-Status WARNING "dxrtd.exe not found at: $installBin"
        Write-Status INFO "Attempting service removal via sc.exe..."
        sc.exe stop dxrtd 2>$null | Out-Null
        sc.exe delete dxrtd 2>$null | Out-Null
    }

    # Step 2: Remove from system PATH
    Write-Host ""
    Write-Status INFO "Removing bin directory from system PATH..."
    $regKey = 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment'
    $currentPath = (Get-ItemProperty $regKey -Name Path).Path
    $pathEntries = $currentPath -split ';'
    if (-not ($pathEntries | Where-Object { $_ -ieq $installBin })) {
        Write-Status INFO "PATH does not contain: $installBin"
    } else {
        $newPath = ($pathEntries | Where-Object { $_ -ne '' -and ($_ -ine $installBin) }) -join ';'
        Set-ItemProperty $regKey -Name Path -Value $newPath
        Write-Status OK "Removed from system PATH: $installBin"
    }

    # Broadcast WM_SETTINGCHANGE
    Send-SettingChange

    Write-Banner @(
        "  Unsetup completed successfully!"
        "  - dxrtd service stopped and removed"
        "  - System PATH entry removed"
    )
    exit 0
}

# ============================================================================
# Detect Visual Studio
# ============================================================================
$VsPath = Find-VSInstallation
if (-not $VsPath) {
    Write-Banner "  dx_rt Build Script"
    Write-Status ERROR "No Visual Studio 2022 or Build Tools found!"
    Write-Host ""
    Write-Host "Checked locations:"
    foreach ($p in $VSPaths.Values) {
        Write-Host "  - $p"
    }
    Write-Host ""
    Write-Host "To install Visual Studio Build Tools 2022 (CLI only, no IDE):"
    Write-Host "  build.bat install-tools"
    Write-Host ""
    Write-Host "Or download manually from:"
    Write-Host "  https://visualstudio.microsoft.com/visual-cpp-build-tools/"
    Write-Host ""
    exit 1
}

# ============================================================================
# Setup Visual Studio environment (before banner so tools are in PATH)
# ============================================================================
if (-not $env:VSCMD_VER) {
    Write-Host ""
    Write-Status INFO "Setting up Visual Studio 2022 x64 environment... (please wait)"
    Import-VCVars $VsPath
}

# ============================================================================
# Show banner
# ============================================================================

# Gather version info
$projectVersion = "unknown"
$verFile = Join-Path $ScriptDir "release.ver"
if (Test-Path $verFile) {
    $projectVersion = (Get-Content $verFile -First 1).Trim()
}

$gitBranch = git -C $ScriptDir rev-parse --abbrev-ref HEAD 2>$null
$gitCommit = git -C $ScriptDir rev-parse --short HEAD 2>$null
$gitDate = git -C $ScriptDir log -1 --format=%ci 2>$null
if ($gitDate -match '(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})') { $gitDate = $Matches[1] }
$gitInfo = if ($gitBranch -and $gitCommit) { "$gitBranch ($gitCommit, $gitDate)" } else { "N/A" }

$cmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
$cmakeVer = if ($cmakeCmd) {
    $v = cmake --version 2>$null | Select-Object -First 1
    if ($v -match '(\d+\.\d+\.\d+)') { $Matches[1] } else { "N/A" }
} else { "not found" }
$cmakePath = if ($cmakeCmd) { $cmakeCmd.Source } else { "N/A" }

$ninjaCmd = Get-Command ninja -ErrorAction SilentlyContinue
$ninjaVer = if ($ninjaCmd) {
    (ninja --version 2>$null).Trim()
} else { "not found" }
$ninjaPath = if ($ninjaCmd) { $ninjaCmd.Source } else { "N/A" }

$msvcVer = if ($env:VSCMD_VER) { $env:VSCMD_VER } else { "N/A" }

# vcpkg info from CMakePresets.json
$vcpkgTriplet = "x64-windows"
$vcpkgCrt = "dynamic"
$vcpkgLib = "dynamic"
$vcpkgManifestMode = "N/A"
$onnxVer = "unknown"
$presetFile = Join-Path $ScriptDir "CMakePresets.json"
if (Test-Path $presetFile) {
    $presetJson = Get-Content $presetFile -Raw | ConvertFrom-Json
    $cacheBase = $presetJson.configurePresets | Where-Object { $_.name -eq 'cache-base' }
    if ($cacheBase.cacheVariables) {
        $cv = $cacheBase.cacheVariables
        if ($cv.VCPKG_TARGET_TRIPLET)    { $vcpkgTriplet = $cv.VCPKG_TARGET_TRIPLET }
        if ($cv.VCPKG_CRT_LINKAGE)       { $vcpkgCrt = $cv.VCPKG_CRT_LINKAGE }
        if ($cv.VCPKG_LIBRARY_LINKAGE)    { $vcpkgLib = $cv.VCPKG_LIBRARY_LINKAGE }
        if ($cv.VCPKG_MANIFEST_MODE)      { $vcpkgManifestMode = $cv.VCPKG_MANIFEST_MODE }
        if ($cv.ONNXRUNTIME_ROOTDIR -match 'onnxruntime-win-x64-([\d.]+)') {
            $onnxVer = $Matches[1]
        }
    }
}

# vcpkg executable info
$vcpkgCmd = Get-Command vcpkg -ErrorAction SilentlyContinue
$vcpkgVer = if ($vcpkgCmd) {
    $v = vcpkg version 2>$null | Select-Object -First 1
    if ($v -match '(\d{4}[-\.]\d{2}[-\.]\d{2})') { $Matches[1] } else { "N/A" }
} else { "not found" }
$vcpkgPath = if ($vcpkgCmd) { $vcpkgCmd.Source } else { "N/A" }
$vcpkgRoot = if ($env:VCPKG_ROOT) { $env:VCPKG_ROOT } else { "not set" }
$vcpkgInstalledDir = Join-Path $ScriptDir "vcpkg_installed"
$vcpkgDownloadsDir = $env:VCPKG_DOWNLOADS

# vcpkg baseline from vcpkg-configuration.json
$vcpkgBaseline = "N/A"
$vcpkgConfigFile = Join-Path $ScriptDir "vcpkg-configuration.json"
if (Test-Path $vcpkgConfigFile) {
    $vcpkgConfig = Get-Content $vcpkgConfigFile -Raw | ConvertFrom-Json
    if ($vcpkgConfig.'default-registry'.baseline) {
        $bl = $vcpkgConfig.'default-registry'.baseline
        $vcpkgBaseline = $bl.Substring(0, [Math]::Min(10, $bl.Length))
    }
}

# vcpkg packages - read installed versions from status file
$vcpkgPackages = "N/A"
$vcpkgStatusFile = Join-Path $ScriptDir "vcpkg_installed\vcpkg\status"
if (Test-Path $vcpkgStatusFile) {
    $statusContent = Get-Content $vcpkgStatusFile -Raw
    $installedPkgs = @()
    foreach ($block in ($statusContent -split '\r?\n\r?\n')) {
        if ($block -match 'Package:\s*(.+)' -and $block -notmatch 'Package:\s*vcpkg-') {
            $pkgName = $Matches[1].Trim()
            $pkgVer = if ($block -match 'Version:\s*(.+)') { $Matches[1].Trim() } else { "?" }
            $installedPkgs += "$pkgName $pkgVer"
        }
    }
    if ($installedPkgs) { $vcpkgPackages = $installedPkgs -join ', ' }
} else {
    # Fallback to vcpkg.json manifest
    $vcpkgManifest = Join-Path $ScriptDir "vcpkg.json"
    if (Test-Path $vcpkgManifest) {
        $manifest = Get-Content $vcpkgManifest -Raw | ConvertFrom-Json
        $pkgNames = $manifest.dependencies | ForEach-Object {
            if ($_ -is [string]) { $_ } else { $_.name }
        } | Where-Object { $_ -notmatch '^vcpkg-' }
        if ($pkgNames) { $vcpkgPackages = $pkgNames -join ', ' }
    }
}

$ScriptVersion = "2026-04-09"

# Build/Install directory info
$buildDir = Join-Path $ScriptDir "out\build\x64-$BuildType"
$installDir = Join-Path $ScriptDir "out\install\x64-$BuildType"
$buildDirStatus = if (Test-Path $buildDir) { $buildDir } else { "$buildDir (not built)" }
$installDirStatus = if (Test-Path $installDir) { $installDir } else { "$installDir (not installed)" }

# dxrtd service info
$dxrtdSvc = Get-Service -Name dxrtd -ErrorAction SilentlyContinue
if ($dxrtdSvc) {
    $dxrtdStatus = $dxrtdSvc.Status.ToString()
    $dxrtdExePath = "N/A"
    try {
        $svcWmi = Get-CimInstance Win32_Service -Filter "Name='dxrtd'" -ErrorAction SilentlyContinue
        if ($svcWmi.PathName) {
            $dxrtdExePath = $svcWmi.PathName -replace '"', ''
        }
    } catch {}
} else {
    $dxrtdStatus = "not registered"
    $dxrtdExePath = $null
}

# --- Banner helpers ---
function Write-BannerLine {
    param(
        [string]$Label,
        [string]$Value,
        [int]$Indent = 2,
        [int]$LabelWidth = 14,
        [ConsoleColor]$LabelColor = [ConsoleColor]::Gray,
        [ConsoleColor]$ValueColor = [ConsoleColor]::White
    )
    $prefix = ' ' * $Indent
    $padded = $Label.PadRight($LabelWidth)
    Write-Host "$prefix$padded : " -ForegroundColor $LabelColor -NoNewline
    Write-Host $Value -ForegroundColor $ValueColor
}

function Write-BannerSection {
    param([string]$Title)
    Write-Host "  $Title" -ForegroundColor DarkCyan
    Write-Host ("  " + "-" * 56) -ForegroundColor DarkGray
}

# Service status color
$dxrtdStatusColor = switch ($dxrtdStatus) {
    'Running'        { [ConsoleColor]::Green }
    'Stopped'        { [ConsoleColor]::Yellow }
    'not registered' { [ConsoleColor]::DarkGray }
    default          { [ConsoleColor]::Red }
}

# Build/Install dir color
$buildDirColor   = if (Test-Path $buildDir)   { [ConsoleColor]::White } else { [ConsoleColor]::DarkGray }
$installDirColor = if (Test-Path $installDir)  { [ConsoleColor]::White } else { [ConsoleColor]::DarkGray }

Write-Host ""
Write-Host "  =============================================================" -ForegroundColor Cyan
Write-Host "   dx_rt Build Script" -ForegroundColor Cyan -NoNewline
Write-Host "                              $ScriptVersion" -ForegroundColor DarkGray
Write-Host "  =============================================================" -ForegroundColor Cyan
Write-Host ""

Write-BannerSection "Project"
Write-BannerLine "Version"    $projectVersion   -ValueColor Cyan
if ($DoVsProject) {
    Write-BannerLine "Action"     "Generate Visual Studio Project"
    Write-BannerLine "Output Dir" "${ScriptDir}\build_vs"
} else {
    Write-BannerLine "Build Type" $BuildType       -ValueColor Yellow
    Write-BannerLine "Preset"     "x64-$BuildType (Ninja)"
}
Write-BannerLine "Arch"       "x64"
Write-BannerLine "Source Dir" $ScriptDir
Write-Host ""

Write-BannerSection "Toolchain"
Write-BannerLine "VS Path"   "$VsPath (MSVC $msvcVer)"
Write-BannerLine "CMake"     "$cmakeVer ($cmakePath)"
Write-BannerLine "Ninja"     "$ninjaVer ($ninjaPath)"
Write-BannerLine "Git"       $gitInfo
Write-Host ""

Write-BannerSection "vcpkg"
Write-BannerLine "Version"   "$vcpkgVer ($vcpkgPath)"
Write-BannerLine "Root"      $vcpkgRoot
Write-BannerLine "Manifest"  $vcpkgManifestMode
Write-BannerLine "Installed" $vcpkgInstalledDir
Write-BannerLine "Downloads" $vcpkgDownloadsDir
Write-BannerLine "Baseline"  $vcpkgBaseline
Write-BannerLine "Triplet"   "$vcpkgTriplet (CRT: $vcpkgCrt, Lib: $vcpkgLib)"
Write-BannerLine "Packages"  $vcpkgPackages
Write-Host ""

Write-BannerSection "Output & Service"
Write-BannerLine "ONNX RT"   $onnxVer
Write-BannerLine "Build Dir" $buildDirStatus     -ValueColor $buildDirColor
Write-BannerLine "Install"   $installDirStatus   -ValueColor $installDirColor
Write-BannerLine "dxrtd Svc" $dxrtdStatus        -ValueColor $dxrtdStatusColor
if ($dxrtdExePath) {
    Write-BannerLine "  ExePath" $dxrtdExePath    -Indent 4
}

Write-Host ""
Write-Host "  =============================================================" -ForegroundColor Cyan
Write-Host ""

# --- Build timer ---
$BuildStartTime = Get-Date
Write-Status INFO "Build started at $($BuildStartTime.ToString('yyyy-MM-dd HH:mm:ss'))"

function Write-Report {
    param(
        [string]$Action,
        [string]$Result,       # "OK" or "FAILED"
        [string[]]$Details
    )
    $endTime = Get-Date
    $elapsed = $endTime - $script:BuildStartTime
    $elapsedStr = '{0:mm\:ss}' -f $elapsed
    if ($elapsed.TotalHours -ge 1) {
        $elapsedStr = '{0:hh\:mm\:ss}' -f $elapsed
    }
    $startStr = $script:BuildStartTime.ToString("yyyy-MM-dd HH:mm:ss")
    $endStr   = $endTime.ToString("yyyy-MM-dd HH:mm:ss")

    $resultColor = if ($Result -eq 'OK') { [ConsoleColor]::Green } else { [ConsoleColor]::Red }

    Write-Host ""
    Write-Host "  =============================================================" -ForegroundColor Cyan
    Write-Host "   Build Report" -ForegroundColor Cyan
    Write-Host "  -------------------------------------------------------------" -ForegroundColor DarkGray
    Write-BannerLine "Action"     $Action
    Write-BannerLine "Build Type" $script:BuildType
    Write-BannerLine "Result"     $Result          -ValueColor $resultColor
    Write-BannerLine "Started"    $startStr
    Write-BannerLine "Finished"   $endStr
    Write-BannerLine "Elapsed"    $elapsedStr
    if ($Details) {
        Write-Host "  -------------------------------------------------------------" -ForegroundColor DarkGray
        foreach ($line in $Details) {
            Write-Host "  $line"
        }
    }
    Write-Host "  =============================================================" -ForegroundColor Cyan
    Write-Host ""
}

# Change to script directory
Set-Location $ScriptDir

# ============================================================================
# Generate Visual Studio Project
# ============================================================================
if ($DoVsProject) {
    Write-Host ""
    Write-Status INFO "Generating Visual Studio 2022 solution/project files..."
    Write-Host ""
    cmake --preset x64-VS
    if ($LASTEXITCODE -ne 0) {
        Write-Report "Generate VS Project" "FAILED" @("CMake preset: x64-VS")
        exit 1
    }
    Write-Report "Generate VS Project" "OK" @(
        "Solution : ${ScriptDir}\build_vs\dx_rt.sln"
        "Open with: start build_vs\dx_rt.sln"
    )
    exit 0
}

# ============================================================================
# Distclean
# ============================================================================
if ($DoDistclean) {
    Write-Host ""
    Write-Status INFO "Full reset - removing all build artifacts..."
    Write-Host ""
    Stop-DxrtdService
    Remove-DirectoryQuietly (Join-Path $ScriptDir "out")
    Remove-DirectoryQuietly (Join-Path $ScriptDir "build_vs")
    Remove-DirectoryQuietly (Join-Path $ScriptDir "vcpkg_installed")
    Remove-CSharpBuildArtifacts
    Write-Report "Distclean" "OK" @("Workspace reset to source-only state.")
    exit 0
}

# ============================================================================
# All = Clean + Configure + Build + Install
# ============================================================================
if ($DoAll) {
    Write-Host ""
    Write-Status INFO "Full build: clean + configure + build + install ($BuildType)"
    Write-Host ""
    Stop-DxrtdService
    Remove-DirectoryQuietly (Join-Path $ScriptDir "out\build\x64-$BuildType")
    Remove-DirectoryQuietly (Join-Path $ScriptDir "out\install\x64-$BuildType")
    $DoConfigure = $true
    $DoBuild = $true
    $DoInstall = $true
}

# ============================================================================
# Clean
# ============================================================================
if ($DoClean) {
    Write-Host ""
    Write-Status INFO "Cleaning build directories..."
    Remove-DirectoryQuietly (Join-Path $ScriptDir "out\build\x64-$BuildType")

    $vcpkgDir = Join-Path $ScriptDir "vcpkg_installed"
    if (Test-Path $vcpkgDir) {
        $delVcpkg = Read-Host "[INFO] vcpkg_installed folder exists. Delete it too? [y/N]"
        if ($delVcpkg -eq 'y' -or $delVcpkg -eq 'Y') {
            Remove-DirectoryQuietly $vcpkgDir
        }
    }

    Write-Host ""
    Remove-CSharpBuildArtifacts
    Write-Report "Clean" "OK" @("Removed: out\build\x64-$BuildType")
    exit 0
}

# ============================================================================
# Rebuild = Clean + Configure + Build
# ============================================================================
if ($DoRebuild) {
    Write-Host ""
    Write-Status INFO "Rebuild: Cleaning build directory..."
    Remove-DirectoryQuietly (Join-Path $ScriptDir "out\build\x64-$BuildType")
    $DoConfigure = $true
    $DoBuild = $true
}

# ============================================================================
# Configure
# ============================================================================
if ($DoConfigure) {
    Write-Host ""
    Write-Status INFO "Running CMake configure with preset x64-$BuildType..."
    Write-Host ""
    cmake --preset "x64-$BuildType"
    if ($LASTEXITCODE -ne 0) {
        Write-Report "Configure" "FAILED" @("Preset: x64-$BuildType")
        exit 1
    }
    Write-Host ""
    Write-Status OK "CMake configure completed."
}

# ============================================================================
# Build
# ============================================================================
if ($DoBuild) {
    Write-Host ""
    Write-Status INFO "Building with preset x64-$BuildType..."
    Write-Host ""
    cmake --build --preset "x64-$BuildType"
    if ($LASTEXITCODE -ne 0) {
        Write-Report "Build" "FAILED" @("Preset: x64-$BuildType")
        exit 1
    }
    Write-Host ""
    Write-Status OK "Build completed successfully!"
}

# ============================================================================
# Install
# ============================================================================
if ($DoInstall) {
    Stop-DxrtdService
    Write-Host ""
    Write-Status INFO "Installing to out\install\x64-$BuildType..."
    Write-Host ""
    cmake --install "out\build\x64-$BuildType"
    if ($LASTEXITCODE -ne 0) {
        Write-Report "Install" "FAILED" @("Target: out\install\x64-$BuildType")
        exit 1
    }
    Write-Host ""
    Write-Status OK "Install completed successfully!"
}

# ============================================================================
# Build C# package (after install so dxrt.lib is available)
# ============================================================================
if ($DoBuild) {
    Build-CSharpPackage
}

# Restart dxrtd service if it was stopped before build
if ($RestartDxrtd) {
    Start-DxrtdService
}

# ============================================================================
# Final Report
# ============================================================================
$reportActions = @()
if ($DoAll)       { $reportActions += "Full Build (clean + configure + build + install)" }
else {
    if ($DoRebuild)   { $reportActions += "Rebuild (clean + configure + build)" }
    if ($DoConfigure -and -not $DoRebuild) { $reportActions += "Configure" }
    if ($DoBuild -and -not $DoRebuild)     { $reportActions += "Build" }
    if ($DoInstall -and -not $DoAll)       { $reportActions += "Install" }
}
$actionStr = $reportActions -join ' + '
if (-not $actionStr) { $actionStr = "Build" }

$reportDetails = @()
$reportDetails += "Build Dir  : out\build\x64-$BuildType"
if ($DoInstall) {
    $reportDetails += "Install Dir: out\install\x64-$BuildType"
}
if ($DoBuild -and (Test-Path (Join-Path $ScriptDir "csharp_package\src\DxEngine\DxEngine.csproj"))) {
    $reportDetails += "C# Package : csharp_package\packages\"
}

# dxrtd service status after build
$svcAfter = Get-Service -Name dxrtd -ErrorAction SilentlyContinue
if ($svcAfter) {
    $reportDetails += "dxrtd Svc  : $($svcAfter.Status)"
}

Write-Report $actionStr "OK" $reportDetails

if ($NoArgs) {
    Write-Host "Press any key to exit..."
    $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
}
