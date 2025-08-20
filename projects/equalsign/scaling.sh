for nr in 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 4.0 5.0
do
    root -l -q "scalebackground.C(0.1,$nr)"
    cd results || exit
    root -l -q "equalsignplot.C()"
    cd pngs || exit
    cp scaledivmequalsign.png "scaledivmequalsign_0.1_${nr}.png"
    cp log_scaledivmequalsign.png "log_scaledivmequalsign_0.1_${nr}.png"
    cd ..
    cd ..
done
