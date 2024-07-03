#!/bin/bash

# 기본값 설정
target="all"
reg_num=1216
dir="/smbdir"
npu=1
# 매개변수 파싱
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --target) target="$2"; shift ;;
        --reg_num) reg_num="$2"; shift ;;
        --dir) dir="$2"; shift ;;
        --npu) npu="$2"; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# 현재 시간 및 분을 포함하는 로그 파일 이름 생성
current_time=$(date "+%m-%d-%H_%M")
log_tag="log_${reg_num}_${current_time}"
log_file="./$log_tag.txt"

# 특정 디렉토리 처리
process_model() {
    local model_name=$1
    local subdirectory="$dir/$reg_num/$model_name"

    sudo -E python python_package/examples/bitmatch_test.py --model "$subdirectory" --log $log_tag --npu $npu
}

# 모델 처리 시작
if [ "$target" != "all" ]; then
    if [ -f "$target" ]; then
        all_models=$(cat "$target")
    else
        echo "Model name list file not found: $target"
        exit 1
    fi
    for model_name in $all_models; do
        process_model "$model_name"
    done
else
    all_subdirectories=$(find "$dir"/"$reg_num"/ -mindepth 1 -maxdepth 1 -type d)
    for subdirectory in $all_subdirectories; do
        echo $subdirectory
        sudo -E python python_package/examples/bitmatch_test.py --model "$subdirectory" --log $log_tag --npu $npu
    done
fi
