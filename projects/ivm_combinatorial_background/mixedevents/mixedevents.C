//Fills Histograms of pp Events with mixed events
#include "../../resources/ppPaths.h"
#include "../../resources/ppHelpers/ppHelpers.h"

void mixedevents()
{
  //safe data
  std::string histofilename = "results/histograms/histo_mixedevents_new.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles);

  // prepare histogram
  TH1D* hs1d = pph.getIVMhisto();

  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;

  std::vector<ROOT::Math::PxPyPzMVector> pxpypzm_archive;
  std::vector<double> trksign_archive;

  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);
    
    // event selections 
    if (!pph.isGoodEvent(ppc))
    {
      continue;
    }
    
    // check if event has 2 tracks which are compatible with PID hypothesis
    if (!pph.isGoodTuple(ppc, masses, true) ){
      continue;
    }
    
    ROOT::Math::PxPyPzMVector ivm_0(TrkPx[0], TrkPy[0], TrkPz[0], masses[0]);
    ROOT::Math::PxPyPzMVector ivm;
    double trksign = TrkSign[0];
    for (int ii=0; ii<pxpypzm_archive.size(); ii++)
      {
        ivm = ivm_0+pxpypzm_archive[ii];
        // ULS and LS
        if (trksign*trksign_archive[ii] < 0) {
          // count candidates
          hs1d->Fill(ivm.M(), 1.);
        }
      }
    pxpypzm_archive.push_back(ROOT::Math::PxPyPzMVector(TrkPx[1], TrkPy[1], TrkPz[1], masses[1]));
    trksign_archive.push_back(TrkSign[1]);
  }

  std::string histoname = "hs1d";
  histofile.WriteObject(hs1d, histoname.c_str());

  delete ch;

}
