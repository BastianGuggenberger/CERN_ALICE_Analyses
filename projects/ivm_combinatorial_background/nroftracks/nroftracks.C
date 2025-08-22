//Fills Histograms of pp Events with kaon events filtered out by restricting the dca


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TRandom3.h"

#include "../ppHelpers/ppHelpers.h"
#include <random>
#include <cmath>

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

void nroftracks(TString fnconfig = "../ppHelpers/ppConfig.json")
{

  //safe data
  std::string histofilename = "results/histograms/histo_nroftracks.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("ressources/rootfiles.txt");
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  TH1D* hs1d_GoodEvents = new TH1D("NumContrib_GoodEvents", ";NumContrib;Number of tracks", 20, 0, 19);
  TH1D* hs1d_GoodTuples = new TH1D("NumContrib_GoodTuples", ";NumContrib;Number of tracks", 20, 0, 19);

  int nGoodEvents = 0;
  int nGoodTuples = 0;
  for (auto ii = 0; ii<nEvents2Process; ii+=100)
  {
    ch->GetEntry(ii);

      if((ii % 100000)==0){
        std::cout << float(ii)/float(nEvents2Process)*100 << " % " << std::endl;
      }

    // event selections 
    if (!pph.isGoodEvent(ppc,false))
    {
      continue;
    }
    nGoodEvents += 1;
    // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      nGoodTuples+=1;
      hs1d_GoodTuples->Fill(NumContrib,1.);
    }
    hs1d_GoodEvents->Fill(NumContrib,1.);
  }
  histofile.WriteObject(hs1d_GoodEvents, "hs1d_GoodEvents");
  histofile.WriteObject(hs1d_GoodTuples, "hs1d_GoodTuples");

  delete ch;
  std::cout << "GoodEvents: " << nGoodEvents << std::endl;
  std::cout << "GoodTuples: " << nGoodTuples << std::endl;
}

