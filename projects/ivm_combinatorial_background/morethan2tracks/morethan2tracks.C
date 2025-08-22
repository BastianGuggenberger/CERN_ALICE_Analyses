//Fills Histograms of pp Events with mixed events



#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include <set>

#include "../../resources/ppHelpers/ppHelpers.h"


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

std::vector<std::vector<int>> goodtuples_3 = {{1,2},{1,3},{2,3}};
std::vector<std::vector<int>> goodtuples_4 = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};


std::vector<std::vector<int>> getgoodtuples(ppHelpers& pph,ppConfiguration* ppc,std::vector<std::vector<float>>& masses_vec){
  std::vector<std::vector<int>> goodtuples;
  if(NumContrib==3){
    goodtuples = goodtuples_3;
    for(int i=0; i<3; i++){
      std::vector<float> masses;
      masses_vec.push_back(masses);
    }
  }
  if(NumContrib==4){
    goodtuples = goodtuples_4;
    for(int i=0; i<6; i++){
      std::vector<float> masses;
      masses_vec.push_back(masses);
    }
  }

  // check if the tracks are compatible with PID hypothesis
  int i=0;
  while(i<goodtuples.size()){
    if (!pph.isGoodTuple(ppc, masses_vec[i], true, goodtuples[i])){
      goodtuples.erase(goodtuples.begin()+i);
      masses_vec.erase(masses_vec.begin()+i);
    }else{
      i++;
    }
  }

  // check if tracks are present in more than 1 tuple
  i=0;
  std::set<int> tracks;
  bool unique;
  while(i<goodtuples.size()){
    unique = true;
    for(int j=0;j<2;j++){
      if(tracks.find(goodtuples[i][j])!=tracks.end()){
        //track not unique
        unique = false;
        goodtuples.erase(goodtuples.begin()+1);
        masses_vec.erase(masses_vec.begin()+i);
      }
      if(unique){
        tracks.insert(goodtuples[i][0]);
        tracks.insert(goodtuples[i][1]);
      }
    }
      i++;
    }

    return goodtuples;
  }





void morethan2tracks(TString fnconfig = "ppConfig_morethan2tracks.json")
{

  int nbIVM = 4000;
  float bIVMmin = 0.;
  float bIVMmax = 5.;
  //safe data
  std::string histofilename = "results/histograms/histo_morethan2tracks.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("../../resources/rootfiles.txt");

  // prepare histograms
  //std::vector<TH1D*> hs1d;
  //std::vector<TH2D*> hs2d;
  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax);
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));

  for (auto ii = 0; ii<nEvents2Process; ii+=1)
  {
      if((ii % 1000000)==0){
        std::cout << float(ii)/float(nEvents2Process)*100 << " % " << std::endl;
      }
    
    ch->GetEntry(ii);
    
    // event selections 
    if (!pph.isGoodEvent(ppc,true)) //only keeps events with 3 or 4 tracks
    {
      continue;
    }

    std::vector<std::vector<float>> masses_vec;
    std::vector<std::vector<int>> goodtuples = getgoodtuples(pph,ppc,masses_vec);

    for(int i=0; i<goodtuples.size(); i++){
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<2; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[goodtuples[i][ii]], TrkPy[goodtuples[i][ii]], TrkPz[goodtuples[i][ii]], masses_vec[i][ii]); //masses_vec[i] only has 2 entries!
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
