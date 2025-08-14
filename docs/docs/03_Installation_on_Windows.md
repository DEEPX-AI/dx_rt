This chapter describes the instructions for installing and using **DX-RT** on a Windows system.  

## System Requirements  

This section describes the hardware and software requirements for running **DX-RT** on Windows.  

- **RAM:** 8GB RAM (16GB RAM or higher is recommended)
- **Storage:** 4GB or higher available disk space
- **OS:** Windows 10 / 11
- **Python:** Version 3.11 (for Python module support)
- **Compiler:** Visual Studio 2022 (required for building C++ examples)
- **Hardware:** The system **must** support connection to an **M1 M.2** module with the **M.2 interface** on the host PC.  

The current version **only** supports **Single-process** and does **not** support Multi-process.  

<div class="center-text">
<p align="center">
<img src="./../resources/02_DX-M1_M.2_LPDDR5x2.png" alt="DX-M1 M.2 Module" width="700px">  
<br>
Figure. DX-M1 M.2 Module  
<br><br>
</p>
</div>

---

## Execute Installer

DEEPX provides the Windows installer executable file for **DX-RT**.  
- `DXNN_Runtime_v[version]_windows_[architecture].exe`  

Here is an example of the execution file.  
- `DXNN_Runtime_vX.X.X_windows_amd64.exe`  

**Default Directory Path**  
- `C:/DevTools/DXNN/dxrt_v[version]`  

Once you install the `.exe` file, the driver will be installed automatically. So you can verify the installation via Device Manager under `DEEPX_DEVICE`.  

> **NOTE.**  
> If **Visual Studio 2022** is **not** installed, you may be prompted to install the **Microsoft Visual C++ Redistributable** (`VC_redist.x64.exe`) using administrator permissions.  

---

## File Structure

```
.
├── bin/
├── docs/
├── drivers/
├── examples/
├── include/
├── lib/
└── python_package/
```

- `bin/` Contains compiled binary executables and command-line tools.
- `docs/` Programming user guide include API documentation.
- `drivers/` Includes necessary driver files for Windows operating systems.
- `examples/` Provides example code how to use the DX-RT library for inference tasks.
- `include/` Contains C/C++ header files required for developing with the DX-RT libraries.
- `lib/` Contains pre-built libraries.
- `python_package/` Includes the Python modules for using DX-RT functionalities within Python.

---

## Running Examples

**DX-RT** includes sample programs in both C++ and Python.  

### Building C++ Examples  

Visual Studio 2022 should be installed on your PC.  

**Step 1.** Open the solution file in the following location.  
`examples\<example-name>\msvc\<example-name>.sln`

**Step 2.** In Visual Studio, Click Rebuild Solution.  
Once the build is complete, an `x64` directory is generated in the same location as the solution file. The executable file of the sample includes the Debug or Release sub-folder.  

### Running C++ Examples  

**Step 1.** Run the executable file of the sample at the following location.  
`examples\<example-name>\msvc\x64\[Debug|Release]\<example-name>.exe`

Example
```
C:\...> cd .\examples\run_async_model\msvc\x64\Release
C:\...\examples\run_async_model\msvc\x64\Release> .\run_async_model.exe model.dxnn 100
```

---

## Python Package Installation 

DEEPX provides a Python module named `dx_engine` for Python 3.11.  

**Step 1.** Build and Install the Package  
Navigate to the Python package directory and install the module.  

```
C:\...\dxrt_vX.X.X> cd python_package/
C:\...\dxrt_vX.X.X\python_package> pip install .
```

**Step 2.** Verify the Installation  
Open a Python shell and check the installed version.  

```
C:\...> python
... 
>>> from dx_engine import version
>>> print(version.__version__)
1.0.1
```

Examples 
```
cd examples/python
C:\...\examples\python> python run_async_model.py ...model.dxnn 10
```

---
