#!/bin/bash
dxrt_ver=`cat release.ver`

SCRIPT_DIR=$(realpath "$(dirname "$0")")

# color env settings
source "${SCRIPT_DIR}/scripts/color_env.sh"

help() {
    echo -e "Usage: ${COLOR_CYAN}$0 [OPTIONS]${COLOR_RESET}"
    echo -e "Build the dx-rt project with various options."
    echo -e ""
    echo -e "${COLOR_BOLD}Options:${COLOR_RESET}"
    echo -e "  ${COLOR_GREEN}--help${COLOR_RESET}            Display this help message and exit."
    echo -e "  ${COLOR_GREEN}--clean${COLOR_RESET}           Perform a clean build, removing previous build artifacts."
    echo -e "  ${COLOR_GREEN}--verbose${COLOR_RESET}         Show detailed build commands during the process."
    echo -e "  ${COLOR_GREEN}--type <TYPE>${COLOR_RESET}     Specify the CMake build type. Valid options: [Release, Debug, RelWithDebInfo]."
    echo -e "  ${COLOR_GREEN}--arch <ARCH>${COLOR_RESET}     Specify the target CPU architecture. Valid options: [x86_64, aarch64]."
    echo -e "  ${COLOR_GREEN}--install <PATH>${COLOR_RESET}  Specify the installation path for dx-rt files."
    echo -e "  ${COLOR_GREEN}--uninstall${COLOR_RESET}       Remove previously installed dx-rt files."
    echo -e "  ${COLOR_GREEN}--docker${COLOR_RESET}          Build the project within a Docker environment."
    echo -e "  ${COLOR_GREEN}--clang${COLOR_RESET}           Use Clang as the compiler for the build."
    echo -e "  ${COLOR_GREEN}--coverage${COLOR_RESET}        Enable code coverage collection (automatically enabled for Debug builds)."
    echo -e ""
    echo -e "  ${COLOR_GREEN}--python_exec <PATH>${COLOR_RESET} Specify the Python executable to use for the build."
    echo -e "                            If omitted, the default system 'python3' will be used."
    echo -e "  ${COLOR_GREEN}--venv_path <PATH>${COLOR_RESET}  Specify the path to a virtual environment to activate for the build."
    echo -e "                            If omitted, no virtual environment will be activated."
    echo -e ""
    echo -e "${COLOR_BOLD}Examples:${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --type Release --arch x86_64${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --clean --install /opt/dx-rt${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --docker --verbose${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --type Debug --coverage${COLOR_RESET}"
    echo -e ""
    echo -e "  ${COLOR_YELLOW}$0 --python_exec /usr/local/bin/python3.8${COLOR_RESET}"
    echo -e "  ${COLOR_YELLOW}$0 --venv_path ./venv-dxnn${COLOR_RESET}"

    if [ "$1" == "error" ] && [[ ! -n "$2" ]]; then
        echo -e "${TAG_ERROR} Invalid or missing arguments."
        exit 1
    elif [ "$1" == "error" ] && [[ -n "$2" ]]; then
        echo -e "${TAG_ERROR} $2"
        exit 1
    elif [[ "$1" == "warn" ]] && [[ -n "$2" ]]; then
        echo -e "${TAG_WARN} $2"
        return 0
    fi
    exit 0
}

# parsing dxrt.cfg.cmake
CMAKE_USE_ORT=false
CMAKE_USE_SERVICE=true
CMAKE_USE_PYTHON=true
parse_cmake_options() {
  local file="$1"
  while IFS= read -r line; do
    parsed=$(echo "$line" | sed -E 's/option\(([^)]+)\)/\1/' )
    IFS= read -r -a parts <<< "$(echo "$parsed" | sed -E 's/[[:space:]]+/ /g')"
    var_name=$(echo "$parsed" | awk -F'"' '{print $1}' | xargs | cut -d' ' -f1)
    default_val=$(echo "$parsed" | awk -F'"' '{print $2; print $3}' | tail -n1 | xargs)
    if [ "$var_name" == 'USE_ORT' ]; then
        if [ "$default_val" == 'ON' ]; then
            CMAKE_USE_ORT=true
        else
            CMAKE_USE_ORT=false
        fi
        echo CMAKE_USE_ORT=$CMAKE_USE_ORT
    elif [ "$var_name" == 'USE_SERVICE' ]; then
        if [ "$default_val" == 'ON' ]; then
            CMAKE_USE_SERVICE=true
        else
            CMAKE_USE_SERVICE=false
        fi
        echo CMAKE_USE_SERVICE=$CMAKE_USE_SERVICE
    elif [ "$var_name" == 'USE_PYTHON' ]; then
        if [ "$default_val" == 'ON' ]; then
            CMAKE_USE_PYTHON=true
        else
            CMAKE_USE_PYTHON=false
        fi
        echo CMAKE_USE_PYTHON=$CMAKE_USE_PYTHON
    fi
  done < "$file"
}

# read cmake/dxrt.cfg.cmake file
CMAKE_FILE="cmake/dxrt.cfg.cmake"
if [ -f "$CMAKE_FILE" ]; then
  parse_cmake_options "$CMAKE_FILE"
else
  echo "Error: File '$CMAKE_FILE' not found."
fi

# cmake command
cmd=()
clean_build=false
install_ort=false
verbose=false
build_type=release
host_arch=$(uname -m)
target_arch=$(uname -m)
build_mode="Release Build"
install=""
uninstall=false
clang=false
build_in_docker=false
python_exec="python3"
venv_path=""
enable_coverage=false

[ $# -gt 0 ] && \
while (( $# )); do
    case "$1" in
        --help)  help; exit 0;;
        --clean) clean_build=true; install_ort=$CMAKE_USE_ORT; shift;;
        --verbose) verbose=true; shift;;
        --type)
            shift
            build_type="${1,,}"
            shift;;
        --arch)
            shift
            target_arch=$1
            shift;;
        --python_exec)
            shift
            python_exec=$1
            shift;;
        --venv_path)
            shift
            venv_path=$1
            shift;;
        --install)
            shift
            install=$1
            shift;;
        --uninstall) uninstall=true; shift;;
        --clang) clang=true; shift;;
        --docker) build_in_docker=true; shift;;
        --coverage) enable_coverage=true; shift;;
        *)
            help "error" "Invalid argument : $1"
            exit 1;;
    esac
done

# install onnx runtime
if [ $install_ort == "true" ]; then
    ./install.sh --onnxruntime --arch $target_arch
fi

if [ $target_arch == "arm64" ]; then
    target_arch=aarch64
fi

if [ $target_arch == $host_arch ]; then
    if [ -z $install ]; then
        install=/usr/local
    fi
    CROSS_COMPILE="native"
else
    CROSS_COMPILE=$target_arch
fi


build_dir=build_"$target_arch"
out_dir=bin

cmd+=(-DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.$target_arch.cmake)
if [ $clang == "true" ]; then
    cmd+=(-DUSE_CLANG=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++)
fi

cmd+=(-DCMAKE_VERBOSE_MAKEFILE=$verbose)
if [ $build_type == "release" ] || [ $build_type == "debug" ] || [ $build_type == "relwithdebinfo" ]; then
    cmd+=(-DCMAKE_BUILD_TYPE=$build_type);
else
    cmd+=(-DCMAKE_BUILD_TYPE=release);
fi

# Enable coverage for Debug builds or when explicitly requested
if [ $enable_coverage == "true" ]; then
    echo -e "${COLOR_YELLOW}Enabling code coverage collection...${COLOR_RESET}"
    cmd+=(-DENABLE_COVERAGE=ON)
    
    if [ $clang == "true" ]; then
        # Clang coverage flags
        export CXXFLAGS="--coverage -fprofile-instr-generate -fcoverage-mapping -g $CXXFLAGS"
        export CFLAGS="--coverage -fprofile-instr-generate -fcoverage-mapping -g $CFLAGS"
        export LDFLAGS="--coverage $LDFLAGS"
    else
        # GCC coverage flags
        export CXXFLAGS="--coverage -fprofile-arcs -ftest-coverage -g -O0 $CXXFLAGS"
        export CFLAGS="--coverage -fprofile-arcs -ftest-coverage -g -O0 $CFLAGS"
        export LDFLAGS="--coverage $LDFLAGS"
    fi
else
    cmd+=(-DENABLE_COVERAGE=OFF)
fi

cmd+=(-DCMAKE_GENERATOR=Ninja)

if [ ! -z $install ]; then
    cmd+=(-DCMAKE_INSTALL_PREFIX=$install)
fi
cmd+=(-DPython_EXECUTABLE=$(which ${python_exec}))

echo cmake args : ${cmd[@]}
[ $clean_build == "true" ] && sudo rm -rf $build_dir

if [ $uninstall == "true" ]; then
    cd $build_dir
    sudo ninja uninstall
else
    if [ -n "$venv_path" ]; then
        . ${venv_path}/bin/activate;
    fi
    mkdir -p $build_dir
    mkdir -p $out_dir
    sudo rm -rf $build_dir/bin
    
    # Clean coverage data from previous runs if coverage is enabled
    if [ $enable_coverage == "true" ]; then
        echo -e "${COLOR_YELLOW}Cleaning previous coverage data...${COLOR_RESET}"
        find . -name "*.gcda" -delete 2>/dev/null || true
        find . -name "*.gcno" -delete 2>/dev/null || true
    fi
    
    cd $build_dir
    cmake .. ${cmd[@]}
    ninja
    sudo ninja install
fi

return_code=$?
#echo $return_code

if [ $return_code -ne 0 ]; then
  exit $return_code
fi

if [ $CROSS_COMPILE != "native" ]; then
    echo "dxtop is not intended for cross-compilation. If required, please build it natively on the target system."
fi

# install python package
if [ $CMAKE_USE_PYTHON == "true" ]; then
    if [ $CROSS_COMPILE == "native" ]; then
        echo '[Install python package ...]'
        cd ../python_package
        pip install .
        PIP_RETURN=$?
        cd ..
   
        if [ $PIP_RETURN -eq 0 ]; then
            pip list | grep dx-engine
            echo -e ${COLOR_YELLOW}"[INFO] The Python package has been installed in the following Python version folder."${COLOR_RESET}
            echo -e ${COLOR_YELLOW}"[INFO] "$(pip --version)${COLOR_RESET}
            echo -e ${COLOR_YELLOW}"[INFO] To install it in a Python virtual environment,\nyou need to activate the virtual environment and then\nreinstall it by running 'pip install .' from the python_package folder."${COLOR_RESET}
            echo '[Installed python package]'
            if [ -n "$venv_path" ]; then
                echo -e "${TAG_INFO} To activate the virtual environment, run:"
                echo -e "${COLOR_BRIGHT_YELLOW_ON_BLACK}  source ${venv_path}/bin/activate ${COLOR_RESET}"
            fi
        else
            echo -e ${COLOR_RED}"[ERROR] Fail to install python package"${COLOR_RESET}
            exit 1
        fi
    else
        echo "Manually install the dxrt Python module on the target system."
    fi
fi


if [ $build_in_docker == "false" ]; then
    # start or restart dxrt system service (dxrtd)
    if [ $CROSS_COMPILE == "native" ]; then

        if [ $CMAKE_USE_SERVICE == "true" ]; then
            
            DXRTD_LOG=$(sudo systemctl restart dxrt.service)
            if [ $? -ne 0 ]; then
                # Register the dxrt service (dxrtd)
                sudo cp ./service/dxrt.service /etc/systemd/system/
                DXRTD_LOG=$(sudo systemctl start dxrt.service)
                if [ $? -eq 0 ]; then
                    DXRTD_LOG=$(sudo systemctl enable dxrt.service)
                    echo '[Registered the dxrt service (dxrtd)]'
                    echo '[Started the dxrt service (dxrtd)]'
                else
                    echo '[Fail to register the dxrt service (dxrtd)]'
                fi
            else
                DXRTD_LOG=$(sudo systemctl enable dxrt.service)
                echo '[Restarted the dxrt service (dxrtd)]'
            fi
        else
            
            DXRTD_LOG=$(sudo systemctl stop dxrt.service)
            if [ $? -eq 0 ]; then
                DXRTD_LOG=$(sudo systemctl disable dxrt.service)
                echo '[Stoped the dxrt service (dxrtd)]'
            else
                echo '[No registered the dxrt service (dxrtd)]'
            fi
        fi
    else
        echo "Manually start the dxrtd service on the target system."
    fi # native compile

fi # not build_in_docker

# Coverage information
if [ $enable_coverage == "true" ]; then
    echo -e "${COLOR_GREEN}========================================${COLOR_RESET}"
    echo -e "${COLOR_GREEN}Coverage collection is ENABLED${COLOR_RESET}"
    echo -e "${COLOR_GREEN}========================================${COLOR_RESET}"
    echo -e "${COLOR_YELLOW}After running tests, you can generate coverage reports with:${COLOR_RESET}"
    echo -e "${COLOR_CYAN}  gcovr --root . --filter \"lib/.*\" --filter \"examples/.*\" --exclude \".*test.*\" --exclude \".*3rdparty.*\" --exclude \".*build.*\" --html-details coverage_report.html${COLOR_RESET}"
    echo -e "${COLOR_CYAN}  gcovr --root . --filter \"lib/.*\" --filter \"examples/.*\" --exclude \".*test.*\" --exclude \".*3rdparty.*\" --exclude \".*build.*\" --sonarqube coverage.xml${COLOR_RESET}"
    echo -e "${COLOR_GREEN}========================================${COLOR_RESET}"
fi
