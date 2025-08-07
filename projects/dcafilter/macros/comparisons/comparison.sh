#!/usr/bin/env bash

files=("ppEvents_dcacutoff_0.500000.pdf" "ppEvents_dcacutoff_0.650000.pdf" "ppEvents_dcacutoff_1.000000.pdf" "ppEvents_Test_original.pdf")

for file in "${files[@]}"; do
    gs -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -dSAFER -dFirstPage=2 -dLastPage=2 -sOutputFile=graph_$file $file
done 

pdfjam --nup 2x2 --outfile comparison.pdf "graph_${files[0]}" "graph_${files[1]}" "graph_${files[2]}" "graph_${files[3]}"
