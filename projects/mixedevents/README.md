MIXEDEVENTS:
    the mixed events projects aims at building an IVM-histogram of background events by taking uncorresponded tracks and interpreting them as resulting from a particle with a certain IVM. 

DEPENDENCIES:
    -ROOT:
        software framework from CERN.

    -rapidjson

    -Python Libraries:
        numpy, matplotlib.pyplot


INSTALLATION:
    LINUX:
    ---bash
    # 1. clone repository:
    git clone https://github.com/BastianGuggenberger/Projektarbeit_SMI

    # 2. navigate to project directory
    cd Projektarbeit_SMI/projects/mixedevents

    # 3. install dependencies
    conda config --set channel_priority strict
    conda create -c conda-forge --name myenvironment root
    conda activate myenvironment
    conda install -c conda-forge rapidjson
    conda install -c conda-forge numpy matplotlib

    #4. execute scripts:
    #C++ scripts have to be compiled and run in the Cling interpreter:
    root
    root [0] .L mixedevents.C
    root [1] mixedevents()
    #...
    root [2] .q
    #python code can be executed outside of Cling

    

PROJECT STRUCTURE:

    ppHelpers_mixedevents.h:
        helper function
        Dependencies:
        ppDataItems.C, ppDataItems.h, ppBranchAssignment.h


    mixedevents.C:
        the main function mixedevents() builds the IVM histogram from events with mixed tracks.
        there is no scaling done at this point.
        Output:
        results/histograms/histo_mixedevents.root:
            IVM histogram of the mixedevents safed as root file
        Input:
        ressources/rootfiles.txt:
            list of all root files of the dataset providing the event
        Dependencies:
        ppHelpers_mixedevents.h, ppConfig.json

    
    results/mixedeventsplot.C
        plots the IVM histogram safed in the .root file
        Output:
        results/pdfs/ppEvents_mixed_events.pdf:
        Input:
        results/histograms/histo_mixedevents.root:
            IVM histogram of the mixedevents safed as root file
            from: mixedevents.C
        Dependencies:
        ppHelpers_mixedevents.h, ppConfig.json