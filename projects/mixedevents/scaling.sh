for nr in 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 4.0 5.0
do
    root -l -q "scalebackground.C(0.1,$nr)"
    cd results || exit
    root -l -q "mixedeventsplot.C()"
    cd pngs || exit
    cp scaledivmbackground.png "scaledivmbackground_0.1_${nr}.png"
    cp log_scaledivmbackground.png "log_scaledivmbackground_0.1_${nr}.png"
    cd ..
    cd ..
done
