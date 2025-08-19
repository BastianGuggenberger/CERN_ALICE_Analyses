//Fills Histograms of pp Events with mixed events



#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "ppHelpers_mixedevents.h"


// run with
//  root -q -b 'processppEvents.C("LHC22_pass7_skimmed", "RF_LHC22_pass7_skimmed.txt")'
//  root -q -b 'processppEvents.C("LHC23_pass4_Thin_small", "RF_LHC23_pass4_Thin_small.txt")'
//  root -q -b 'processppEvents.C("LHC23_pass4_Thin", "RF_LHC23_pass4_Thin.txt")'

// Available datastes                  Events total    2pi candidates    2K candidates
//    LHC22_pass7_skimmed               32'467'114
//    LHC22_pass4_lowIR
//    LHC22_pass4_highIR_Thin_sampling
//    LHC22_pass4_highIR_Thin
//    LHC23_pass4_skimmed                13'824'860
//    LHC23_pass4_Thin_small             18'275'132
//    LHC23_pass4_Thin                  529'246'110
//    LHC24_pass1_skimmed               125'768'899
//    LHC24_pass1_MinBias
//
// auto fnames = std::string("rootfiles.txt");
void mixedevents(TString fnconfig = "ppConfig.json")
{

  int nbIVM = 4000;
  float bIVMmin = 0.;
  float bIVMmax = 5.;
  //safe data
  std::string histofilename = "results/histograms/histo_mixedevents.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("ressources/rootfiles.txt");

  // prepare histograms
  //std::vector<TH1D*> hs1d;
  //std::vector<TH2D*> hs2d;
  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax);

  //pph.getHistos(hs1d, hs2d);

  // prepare structures to hold variables as function of run number
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;

  std::vector<ROOT::Math::PxPyPzMVector> pxpypzm_archive;
  std::vector<double> trksign_archive;

  for (auto ii = 0; ii<nEvents2Process; ii+=100)
  {
      if((ii % 1000000)==0){
        std::cout << float(ii)/float(nEvents2Process)*100 << " % " << std::endl;
      }
    
    ch->GetEntry(ii);
    
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
