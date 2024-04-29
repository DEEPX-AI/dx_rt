#!/bin/bash

md_dir=docs
out=README.md

md_files=(
    Introduction
    Installation
    Build
    Getting-Started
    Inference-Guide
    Examples
    High-Level-Design
    # Low-Level-Design
    API-Reference
    Test
)

echo >$out
for chapter in ${md_files[*]}; do
    echo "# $chapter" >>$out
    cat $md_dir/$chapter.md >>$out
    echo >>$out
done
