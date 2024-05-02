#!/bin/bash
models_root_dir=$1
run=./bin/run_model
models=()
while IFS= read -r -d '' directory; do
    models+=("$directory")
done < <(find "$models_root_dir" -mindepth 1 -type d -print0)
for model in ${models[*]}
do
    echo $model
    sudo $run -m $model
done
