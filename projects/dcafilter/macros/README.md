DCA FILTER:
    DCA Filter is a project for filtering out kaon events from a dataset of proton-proton collisions.

DEPENDENCIES:
    Cern-Root
    C++
    Python


STRUCTURE:

    ppHelpers.h:
        
        helper function, where the dca filter is built in the .isGoodEvent() method

        Dependencies:
        ppDataItems.C, ppDataItems.h, ppBranchAssignment.h


    dcafilter.C:

        the main function "varycutoff()" loads proton-proton-Events and filters out kaon events by cutting out large dcas.
        the "acceptance parameter" alpha responsible for the tolerance of the filter is varied.#
        the filter itself is built in the isGoodEvent() method in ppHelbers.h

        Output:
        results/histograms/histos_cutoff______.root:
            histograms for the IVM of the remaining events safed as root files.
        
        Input:
        ressources/rootfiles.txt:
            list of all root files of the dataset providing the event

        Dependencies:
        ppHelpers.h, ppConfig.json


    results/metrics/getalphavsmetrics.C:




    results/dcafilterplot.C:

        script for plotting information of the remaining pp Events on a pdf

        Input:
        results/histograms/histos_cutoff______.root:
            output of dcafilter.C

        Output:
        results/pdfs/ppEvents_dcacutoff_1________.pdf

        Dependencies:
        ppHelpers.h, ppConfig.json


    results/comparisons/comparison.sh

        shell script for comparing IVM histograms of 8 different cutoffs

        Input:
        results/comparisons/pdfs/ppEvents_dcacutoff_1________.pdf

        Output:
        comparison.pdf
        (results/comparisons/graph_pdfs/...)
