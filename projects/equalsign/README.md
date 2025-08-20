# EQUALSIGN:
the equalsign project aims at building an IVM-histogram of background events by taking only events with the same sign of charge and interpreting them as resulting from a particle with a certain IVM. 

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
cd Projektarbeit_SMI/projects/equalsign

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
root [0] .L equalsign.C
root [1] equalsign()
#...
root [2] .q
```
    

# PROJECT STRUCTURE:

## ppHelpers_equalsign.h:
helper function  
**Dependencies:**  
ppDataItems.C, ppDataItems.h, ppBranchAssignment.h  


## equalsign.C:
the main function equalsign() builds the IVM histogram from events with the same sign of charge.  
there is no scaling done at this point.  
**Output**  
results/histograms/histo_equalsign.root: IVM histogram of the equalsign events safed as root file  
**Input:**  
ressources/rootfiles.txt:  
    list of all root files of the dataset providing the events  
**Dependencies:**  
ppHelpers_equalsign.h, ppConfig.json  


## scalebackground.C
the main function scalebackground(double comparison_minimum, double comparison_maximum) scales the IVM background histogram built by equalsign.C (or alternatively the original IVM-histogram).  
the background will be scaled as high as possible without being higher than the original IVM-histogram (ivmblueprint) in the comparison interval [comparison_minimum,comparison_maximum].  
**Output:**  
results/histograms/histo_scaledequalsign.root  
results/histograms/histo_scaledblueprint.root  
**Input:**  
results/histograms/histo_equalsign.root  
results/histograms/histo_ivmblueprint.root  
**Dependencies:**  
ppHelpers_equalsign.h, ppConfig.json  

if not already available, the original ivm-histogram of the used dataset (ivmblueprint) can be obtained by the function getivmblueprint():  
**Output:**  
results/histograms/histo_ivmblueprint.root  
**Input:**  
ressources/rootfiles.txt: list of all root files of the dataset providing the event  
**Dependencies:**  
ppHelpers_equalsign.h, ppConfig.json  


## results/equalsignplot.C  
plots the histograms safed in the .root file.  

- plotbackground():  
**Output:**  
results/pngs/histo_equalsign.png    
**Input:**  
results/histograms/histo_equalsign.root   

- plotblueprint():  
**Output:**  
results/pngs/histo_ivmblueprint.png    
**Input:**  
results/histograms/histo_ivmblueprint.root   

- equalsignplot():  
**Output:**  
results/pngs/scaledivmequalsign.png    
results/pngs/log_scaledivmequalsign.png    
**Input:**  
results/histograms/histo_scaledequalsign.root   
results/histograms/histo_scaledblueprint.root   


## scaling.sh
shell script for combining the scalebackground() function with the equalsignplot() function.  
different comparison intervals [comparison_minimum,comparison_maximum] are used and the final plots are named after them.  
**Output:**  
results/histograms/histo_scaledequalsign.root  
results/histograms/histo_scaledblueprint.root  
results/pngs/scaledivmequalsign_.._...png    
results/pngs/log_scaledivmequalsign_.._...png   
**Input:**  
results/histograms/histo_equalsign.root  
results/histograms/histo_ivmblueprint.root  
**Dependencies:**  
ppHelpers_equalsign.h, ppConfig.json  