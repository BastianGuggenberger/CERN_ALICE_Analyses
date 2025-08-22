# MIXEDEVENTS:
the mixed events projects aims at building an IVM-histogram of background events by taking uncorresponded tracks and interpreting them as resulting from a particle with a certain IVM. 

# SUPPORT:
email: bastian.guggenberger@aon.at

# DEPENDENCIES:
- ROOT: software framework from CERN.
- rapidjson 
- Python Libraries: numpy, matplotlib.pyplot

# INSTALLATION:
## LINUX:
```bash
#1. clone repository:
git clone https://github.com/BastianGuggenberger/Projektarbeit_SMI

#2. navigate to project directory
cd Projektarbeit_SMI/projects/mixedevents

#3. install dependencies
conda config --set channel_priority strict
conda create -c conda-forge --name myenvironment root
conda activate myenvironment
conda install -c conda-forge rapidjson
conda install -c conda-forge numpy matplotlib
```

# USAGE:
```bash
#1. quickstart
chmod +x scaling.sh
./scaling.sh

#2. execute scripts seperately:
#C++ scripts have to be compiled and run in the Cling interpreter:
root
root [0] .L mixedevents.C
root [1] mixedevents()
#...
root [2] .q
```
    

# PROJECT STRUCTURE:

## ppHelpers_mixedevents.h:
helper function  
**Dependencies:**  
ppDataItems.C, ppDataItems.h, ppBranchAssignment.h  


## mixedevents.C:
the main function mixedevents() builds the IVM histogram from events with mixed tracks.  
there is no scaling done at this point.  
**Output**  
results/histograms/histo_mixedevents.root: IVM histogram of the mixedevents safed as root file  
**Input:**  
ressources/rootfiles.txt:  
    list of all root files of the dataset providing the event  
**Dependencies:**  
ppHelpers_mixedevents.h, ppConfig.json  


## scalebackground.C
the main function scalebackground(double comparison_minimum, double comparison_maximum) scales the IVM background histogram built by mixedevents.C (or alternatively the original IVM-histogram).  
the background will be scaled as high as possible without being higher than the original IVM-histogram (ivmblueprint) in the comparison interval [comparison_minimum,comparison_maximum].  
**Output:**  
results/histograms/histo_scaledbackground.root  
results/histograms/histo_scaledblueprint.root  
**Input:**  
results/histograms/histo_mixedevents.root  
results/histograms/histo_ivmblueprint.root  
**Dependencies:**  
ppHelpers_mixedevents.h, ppConfig.json  

if not already available, the original ivm-histogram of the used dataset (ivmblueprint) can be obtained by the function getivmblueprint():  
**Output:**  
results/histograms/histo_ivmblueprint.root  
**Input:**  
ressources/rootfiles.txt: list of all root files of the dataset providing the event  
**Dependencies:**  
ppHelpers_mixedevents.h, ppConfig.json  


## results/mixedeventsplot.C  
plots the histograms safed in the .root file.  

- plotbackground():  
**Output:**  
results/pngs/histo_mixedevents.png    
**Input:**  
results/histograms/histo_mixedevents.root   

- plotblueprint():  
**Output:**  
results/pngs/histo_ivmblueprint.png    
**Input:**  
results/histograms/histo_ivmblueprint.root   

- mixedeventsplot():  
**Output:**  
results/pngs/scaledivmbackground.png    
results/pngs/log_scaledivmbackground.png    
**Input:**  
results/histograms/histo_scaledbackground.root   
results/histograms/histo_scaledblueprint.root   

## scaling.sh
shell script for combining the scalebackground() function with the mixedeventsplot() function.  
different comparison intervals [comparison_minimum,comparison_maximum] are used and the final plots are named after them.  
**Output:**  
results/histograms/histo_scaledbackground.root  
results/histograms/histo_scaledblueprint.root  
results/pngs/scaledivmbackground_.._...png    
results/pngs/log_scaledivmbackground_.._...png   
**Input:**  
results/histograms/histo_mixedevents.root  
results/histograms/histo_ivmblueprint.root  
**Dependencies:**  
ppHelpers_mixedevents.h, ppConfig.json  