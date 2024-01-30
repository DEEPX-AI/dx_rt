#!/bin/bash
build_kernel_module() {
    local path=$1
    shift
    local flags=("$@")
    pushd $path
    make clean
    make "${flags[@]}"
    popd
}
check_file() {
    local file_path=$1
    if [ ! -e "$file_path" ]; then
        echo "Error: $file_path does not exist."
        exit 1
    fi
}
check_module_file() {
    local module_path=$1
    local ko_files=()
    if [ ! -d "$module_path" ]; then        
        echo ""
    fi
    while IFS= read -r -d $'\0' file; do
        ko_files+=("$file")
    done < <(find "$module_path" -type f -name "*.ko" -print0)
    if [ ${#ko_files[@]} -eq 0 ]; then        
        echo ""
    elif [ ${#ko_files[@]} -eq 1 ]; then
        echo "${ko_files[0]}"
    else
        echo ""
    fi
}
check_module_install() {
    local name=$1
    check_module=$(lsmod | grep -c $name)
    if [[ "$check_module" -ge "1" ]]; then
        echo "$1 loaded successfully!!"
    else
        echo "Error: $file_path does not exist."
        exit 1
    fi
}