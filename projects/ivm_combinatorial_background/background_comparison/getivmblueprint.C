#include "../../resources/ppHelpers/ppHelpers.h"
#include "../../resources/ppPaths.h"

//fills and stores the original IVM-histogram ("ivmblueprint")
void getivmblueprint(){

  //safe data
  std::string histofilename = "results/ivmblueprint/histo_ivmblueprint_new.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  //get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  //create a TChain
  auto ch = pph.getChain(path_rootfiles);

  //get histogram and masses
  TH1D* hs1d = pph.getIVMhisto();
  std::vector<float> masses;

  //loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii++){

    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);
    
    //event selections 
    if (!pph.isGoodEvent(ppc)) continue;
    if (!pph.isGoodTuple(ppc, masses, true) ) continue; // check if event has 2 tracks which are compatible with PID hypothesis

    //calculate ivm
    ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
    for (int ii=0; ii<NumContrib; ii++)
    {
      ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
    }

    //ULS and LS
    if (TrkSign[0]*TrkSign[1] < 0) {
      //fill histo
      hs1d->Fill(ivm.M(), 1.);
    }
  }

  //safe histogram
  std::string histoname = "hs1d";
  histofile.WriteObject(hs1d, histoname.c_str());

  delete ch;
}