#!/bin/bash
echo "<MODEL_FILTER> must be modified manually."
if [ "$#" -ne 7 ]; then
    echo "Usage: $0 <MODELS_DIR> <LOOP> <ITERATION> <V_MIN> <V_MAX> <DDR_FREQ_MIN> <DDR_FREQ_MAX>"
    exit 1
fi
#MODEL_FILTER="3377_model_list.txt"
MODEL_FILTER="CHIP_TEST_MODEL_LIST.txt"

MODELS_DIR=$1
LOOP=$2
ITERATION=$3
V_MIN=$4
V_MAX=$5
DDR_FREQ_MIN=$6
DDR_FREQ_MAX=$7

mv BITMATCH_RESULTS.log "BITMATCH_RESULTS.log.bakcup"

for ((itr=0; itr<ITERATION; itr++)); do
    sudo killall -9 dxrtd
    sleep 1
    for ((DDR_FREQ=DDR_FREQ_MIN; DDR_FREQ<=DDR_FREQ_MAX; DDR_FREQ+=200)); do
        echo dxrt-cli-internal -t "$DDR_FREQ"
        dxrt-cli-internal -t "$DDR_FREQ"
        sleep 2

        for ((VOL=V_MAX; VOL>=V_MIN; VOL-=10)); do
            echo dxrt-cli-internal -c "$VOL" -c 1000 -c "$VOL" -c 1000 -c "$VOL" -c 1000
            dxrt-cli-internal -c "$VOL" -c 1000 -c "$VOL" -c 1000 -c "$VOL" -c 1000
            sleep 2
            dxrtd&
            sleep 1

            if [[ $itr -eq 0 && $DDR_FREQ -eq $DDR_FREQ_MIN && $VOL -eq $V_MAX ]]; then
                SECONDS=0  
                python ../python_package/bitmatch/bitmatch.py -d "$MODELS_DIR" -l "$LOOP" --model_filter "$MODEL_FILTER"
                INITIAL_TIMEOUT=$SECONDS  
                TIMEOUT_LIMIT=$((INITIAL_TIMEOUT * 2))  
                echo "First execution took $INITIAL_TIMEOUT seconds. Setting timeout limit to $TIMEOUT_LIMIT seconds."
            else
                timeout "$TIMEOUT_LIMIT" python ../python_package/bitmatch/bitmatch.py -d "$MODELS_DIR" -l "$LOOP" --model_filter "$MODEL_FILTER"
                if [ $? -eq 124 ]; then
                    echo "bitmatch.py timed out after $TIMEOUT_LIMIT seconds!"
                    sudo killall -9 python
                fi
            fi
            sudo killall -9 dxrtd
        done
    done
    mv BITMATCH_RESULTS.log "BITMATCH_RESULTS_${itr}.log"
done
