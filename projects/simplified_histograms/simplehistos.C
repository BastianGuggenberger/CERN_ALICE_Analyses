#include <iostream>
#include "TChain.h"

#include "ppDefinitions.h"

void simplehistos(){
    std::string fname = "/home/ubuma/Projektarbeit/Projektarbeit_SMI/projects/macros/../LHC22_pass7_skimmed/DGAnalysisTree_0002.root";
    TChain* ch = new TChain("chain");

    TFile ff(fname.c_str(),"READ");

    for (auto key : *ff.GetListOfKeys())
        {
        std::cout << std::endl << std::endl;
        std::string dname = std::string(((TKey*) key)->GetName());
        auto tname = fname + "/"+std::string(((TKey*) key)->GetName())+"/O2dgfewtrack";
        ch->Add(tname.c_str());
        }


//analyse TChain

    int n = ch->GetEntries();
    
    //set necessary branches
    #include "ppBranchAssignment.h"
    
    for (int i = 0; i<n; i++){
        ch->GetEntry(i);
        std::cout << "RunNumber: " << RunNumber << std::endl; 
    }
}