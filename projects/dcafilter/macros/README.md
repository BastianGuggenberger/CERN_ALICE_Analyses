# DCA FILTER:
DCA Filter is a project for filtering out kaon events from a dataset of proton-proton collisions registered by the CERN-ALICE Detector. The filter resticts the DCA-xy and DCA-z properties. The performance of the filter for varying xy and z DCA-tolerances is tested and also documented in this project.

## SUPPORT:
email: bastian.guggenberger@aon.at

## DEPENDENCIES:
### -ROOT:
software framework from CERN.

### -rapidjson

### -Python Libraries:
numpy, matplotlib.pyplot


## INSTALLATION:
### LINUX:

1. clone repository:
```bash
git clone https://github.com/BastianGuggenberger/Projektarbeit_SMI

#2. navigate to project directory
cd Projektarbeit_SMI/projects/dcafilter/macros

#3. install dependencies
conda config --set channel_priority strict
conda create -c conda-forge --name myenvironment root
conda activate myenvironment
conda install -c conda-forge rapidjson
conda install -c conda-forge numpy matplotlib
```

## Usesage
### execute C++ code:
C++ scripts have to be compiled and run in the Cling interpreter:
```bash
root
root [0] .L dcafilter.C
root [1] vary_xy_z()
#...
root [2] .q
```
### execute python code:
python code can be executed outside of Cling:
```bash
cd results/metrics
python plot_dca2d_metrics.py
```

    

## PROJECT STRUCTURE:

### ppHelpers.h:
helper function, where the dca filter is built in the .isGoodEvent() method
Dependencies:
ppDataItems.C, ppDataItems.h, ppBranchAssignment.h


### dcafilter.C:
the main functions "varyalpha()" and "vary_xy_z()" load proton-proton-Events and filter out kaon events by cutting out large dcas.
the "acceptance parameter" alpha responsible for the tolerance of the filter is varied in the varyalpha() function.
the ratio of xy-tolerance to z-tolerance is varied in the vary_xy_z() function.
the filter itself is built in the isGoodEvent() method in ppHelpers.h
Output:
results/histograms/vary.../histos_alpha..._xy..._z....root:
histograms for the IVM of the remaining events safed as root files.
Input:
ressources/rootfiles.txt:
    list of all root files of the dataset providing the event
Dependencies:
ppHelpers.h, ppConfig.json


### dcafilter_2.C:
same as dcafilter.C, but here the mainfunction dcafilter_2() builds a 11x11 grid of different xy and z tolerances.
Output:
results/histograms/2d/histos_alpha..._xy..._z....root:
    histograms for the IVM of the remaining events safed as root files.
Input:
ressources/rootfiles.txt:
    list of all root files of the dataset providing the event
Dependencies:
ppHelper_2_.h, ppDataItems.C, ppDataItems.h, ppBranchAssignment_2.h, ppConfig.json


### results/metrics/get_vsmetrics.C:
analyses the histograms built in dcafilter.C and dcafilter_2.C for their performance and calculates certain metrics of the remaining events. the definition of the metrics can be found in results/metrics/definitions.pdf
the main functions getalphasvsmetrics(), getxyzvsmetrics(), get2dvsmetrics() vary alpha,xy vs z or the 2d xy vs z grid in their metric analysis.
Output:
results/metrics/ressources/vary.../alpha_xy_z_a_b_c_t.txt:
    txt files containing alpha,xy,z and the metrics a,b,c,t
Input:
results/histograms/vary.../histos_alpha..._xy..._z....root:
    root files containing the root files with dca-filtered histograms
    from: dcafilter.C or dcafilter_2.C
    

### results/metrics/plot_vsgausmetrics.py:
plots alpha or xy/z ratio vs certain metrics for the filter performance
Output:
results/metrics/results/__.png
Input:
results/metrics/ressources/vary.../alpha_xy_z_a_b_c_t.txt:
    txt files containing alpha,xy,z and the metrics a,b,c,t
    from: results/metrics/get_vsmetrics.C


### results/metrics/plot_dca2d_metrics.py:
plots colormap for metrics for the 11x11 grid of xy and z tolerances
Input:
results/metrics/ressources/2d/alpha_xy_z_a_b_c_t.txt:
    txt files containing alpha,xy,z and the metrics a,b,c,t
    from: results/metrics/get_vsmetrics.C
Output:
results/metrics/results/__.png


### results/dcafilterplot.C:
script for plotting information of the remaining pp Events on a pdf
Output:
results/pdfs/ppEvents_dcacutoff_1________.pdf
Input:
results/histograms/histos_alpha..._xy..._z....root:
    from: dcafilter.C
Dependencies:
ppHelpers.h, ppConfig.json


### results/comparisons/comparison.sh
shell script for comparing IVM histograms of 8 different cutoffs
Input:
results/comparisons/pdfs/ppEvents_dcacutoff_1________.pdf
Output:
comparison.pdf
(results/comparisons/graph_pdfs/...)

