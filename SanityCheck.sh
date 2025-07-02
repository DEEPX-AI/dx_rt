#!/bin/bash
# Sanity Check script to verify deepx sdk environments
SANITY_DIR="dx_report/sanity"

function pushd () {
    command pushd "$@" > /dev/null
}

function popd () {
    command popd "$@" > /dev/null
}

pushd $SANITY_DIR
source Sanity.sh
popd