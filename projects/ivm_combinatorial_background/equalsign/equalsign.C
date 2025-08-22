
#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "../ppHelpers/ppHelpers.h"


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
void equalsign(TString fnconfig = "../ppHelpers/ppConfig.json")
{
  int nbIVM = 4000;
  float bIVMmin = 0.;
  float bIVMmax = 5.;

  //safe data
  std::string histofilename = "results/histograms/histo_equalsign.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("ressources/rootfiles.txt");

  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax);

  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);

    if ((ii % 10000)== 0){
      std::cout << float(ii)/float(nEvents2Process)*100 << " % " << std::endl;
    }

    
    // event selections 
    if (!pph.isGoodEvent(ppc))
    {
      continue;
    }
    
    // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<NumContrib; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
      }
      // ULS and LS
      if (TrkSign[0]*TrkSign[1] > 0) {
        hs1d->Fill(ivm.M(), 1.);
      } 
    }
  }
  std::string histoname = "hs1d";
  histofile.WriteObject(hs1d, histoname.c_str());
  delete ch;
}

