acc_xy=0.015;
acc_z=0.0125;
new_acc_xy=0.015000
new_acc_z=0.012500

alphas=(
  "0.100000" "0.750000" "1.350000" "2.100000"
  "0.200000" "0.900000" "1.500000" "2.250000"
  "0.300000" "100.000000" "1.650000" "0.450000"
  "1.050000" "1.800000" "0.600000" "1.200000"
  "1.950000"
)

for alpha in "${alphas[@]}"; do
    oldname="histos_alpha""$alpha""_xy""$acc_xy""_z""$acc_z"".root"
    newname="histos_alpha""$alpha""_xy""$new_acc_xy""_z""$new_acc_z"".root"
    mv "$oldname" "$newname"
    done