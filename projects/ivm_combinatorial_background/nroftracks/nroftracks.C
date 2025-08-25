//Fills Histograms of pp Events with kaon events filtered out by restricting the dca
#include "../../resources/ppPaths.h"
#include "../../resources/ppHelpers/ppHelpers.h"


void nroftracks()
{

  //safe data
  std::string histofilename = "results/histograms/histo_nroftracks_new.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles);
  
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
    helpers::coutpercentage(ii,nEvents2Process);
    
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

