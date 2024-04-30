#!/bin/bash
models=(
    "MobileNetV1_12" #    "mv1_best_argmax"
    "SSDMobileNetV1_1" #    "fix_shape_mobilenetv1-ssd_onnx"
    "EyenixSSDLite_1" #    "eyenix_ssdlite320"
    "SSDMobileNetV2Lite_1" #    "mv2_ssd_lite"
    "SSDMobileNetV1_3" #    "mv1ssd512"
    "YOLOV5S_4" #    "yolov5s_320"
    "YOLOV5S_3" #    "yolov5s_512"
    "YOLOV5S_6" #    "yolov5s_640"
    "YOLOV7_3" #    "YOLOv7_640"
    "YOLOX-S_1" #    "yolox_s"
    "YOLOV7_1" #    "yolov7_640_ppu"
)

######################################################

function get_model()
{
    model=$1
    src=$2
    dest=$3
    echo Get model [ $model ]  from $src to $dest
    if [ ! -d $src ]; then
		echo "Error: " $src/$model "doesn't exist."
		return 1
	fi
    rm -rf $dest
    cp -r $src $dest
}

######################################################
if [ $# -lt 2 ]; then
    echo "Invalid arguments."
    echo "    ./get_model.sh src_dir dest_dir"
    exit 1
fi
# set -x #echo on for debug
src_dir=$1
dest_dir=$(realpath $2)
mkdir -p $dest_dir
for model in ${models[*]}
do
    get_model $model $src_dir/$model $dest_dir/$model
    cp $dest_dir/$model/model_*.txt $model.cfg    
    sed -i "s|$src_dir|$dest_dir|g" "$model.cfg"
done
tree $dest_dir -L 1
