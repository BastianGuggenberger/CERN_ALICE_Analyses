declare -A ids_to_paths

ids_to_paths[0]="/media/ubuma/PortableSSD/LHC22_pass4_highIR_Thin"
ids_to_paths[1]="/media/ubuma/PortableSSD/LHC22_pass4_highIR_Thin_sampling"
ids_to_paths[2]="/media/ubuma/PortableSSD/LHC22_pass4_lowIR"
ids_to_paths[3]="/media/ubuma/PortableSSD/LHC22_pass7_skimmed"
ids_to_paths[4]="/media/ubuma/PortableSSD/LHC23_pass4_skimmed"
ids_to_paths[5]="/media/ubuma/PortableSSD/LHC23_pass4_Thin"
ids_to_paths[6]="/media/ubuma/PortableSSD/LHC23_pass4_Thin_small"
ids_to_paths[7]="/media/ubuma/PortableSSD/LHC24_pass1_MinBias"
ids_to_paths[8]="/media/ubuma/PortableSSD/LHC24_pass1_skimmed"

for ((dbid=0; dbid<9; dbid++));do  
    outputfile="rootfiles/rootfile"$dbid".txt"
    > "$outputfile"
    inputdir=${ids_to_paths[$dbid]}
    files=("$inputdir"/*)
    for file in "${files[@]}"; do
        echo $file >> "$outputfile"
    done
done