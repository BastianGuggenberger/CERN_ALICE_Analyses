for nr in 1.0 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 4.0 5.0
do
    root -l -q "backgroundcomparison.C(0.3,$nr)"
    cd results || exit
    cp scaledbackgroundcomparison.png "scaledbackgroundcomparison_0.3_${nr}.png"
    cp log_scaledbackgroundcomparison.png "log_scaledbackgroundcomparison_0.3_${nr}.png"
    cd ..
done
