#!/usr/bin/env bash

dir="pdfs"

files=("$dir"/*)

for file in "${files[@]}"; do
    echo $file
    gs -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -dSAFER -dFirstPage=2 -dLastPage=2 -sOutputFile=graph_$file $file
done 

pdfjam --nup 2x4 --outfile comparison.pdf "${files[@]/#/graph_}"
