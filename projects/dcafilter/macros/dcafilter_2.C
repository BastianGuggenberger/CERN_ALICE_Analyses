//Fills Histograms of pp Events with kaon events filtered out by restricting the dca
//Version 2

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include<typeinfo>
#include "TH1.h"
#include<chrono>

#include "ppHelpers_2.h" //optimized for ivm usage only

const double alpha = 1.0;


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
void dcafilter_2(std::string fnames="ressources/rootfiles.txt", TString fnconfig = "ppConfig.json")
{
  //variables:
  bool goodtuple;
  double ivm_mass;
  double trksgnprdct;

  // get helpers and configuration
  TH1::AddDirectory(kFALSE);
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain(fnames);

  //prepare histograms
  //std::cout <<"analysing "<< i << "/11: " <<"preparing histograms... " << std::endl;
  std::array<std::array<std::vector<TH1D*>, 12>, 12> H1{};
  for(int i = 1; i<12; i++){
  for(int j=1; j<12; j++){
    std::vector<TH1D*> hs1d_vec;
    pph.getHistos(hs1d_vec);
    H1[i][j] = hs1d_vec;
  }}

  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  auto totalstart = std::chrono::system_clock::now();
  auto totalend = std::chrono::system_clock::now();

  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);
    if ((ii % 10000)== 0){
      totalend = std::chrono::system_clock::now();
      std::cout <<((static_cast<float>(ii))/nEvents2Process) * 100 << " %" << std::endl;
      //std::cout <<  "total: " << totalend-totalstart << std::endl;
      totalstart = std::chrono::system_clock::now();
    }

    //do j-independent stuff

    if (!pph.isGoodEvent_general(ppc)){
      continue;
    }

    trksgnprdct = TrkSign[0]*TrkSign[1];
    if(pph.isGoodTuple(ppc, masses, true)){
      goodtuple = true;
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<NumContrib; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
      }
      ivm_mass = ivm.M();
    }else{
      goodtuple = false;
    }
    

    //inner loop: vary xy and z
    for (int i=1; i<12; i++){
      for (int j=1; j<12; j++){
        double acc_xy = 0.0 + 0.0025* i;
        double acc_z = 0.0 + 0.0025* j;

        // dca filter
        if (!pph.isGoodEvent_dca(alpha, acc_xy, acc_z))
          {
            continue;
          }

        // check if event has 2 tracks which are compatible with PID hypothesis
        if (goodtuple)
        {
          // ULS and LS
          if (trksgnprdct < 0) { 
            (H1[i][j])[0]->Fill(ivm_mass, 1.);
          } else {
            (H1[i][j])[1]->Fill(ivm_mass, 1.);
          }
        }
      }
      }
    }
    
    for (int i=1; i<12; i++){
      for (int j=1; j<12; j++){
        std::cout <<"analysing "<< i << "/11: " <<"safing histograms as root files... " << std::endl;

        double acc_xy = 0.0 + 0.0025* i;
        double acc_z = 0.0 + 0.0025* j;
        int len_1 = (H1[i][j]).size();

        std::string histofilename = "results/histograms/2d/histos_alpha"+ std::to_string(alpha) + "_xy" + std::to_string(acc_xy) + "_z" + std::to_string(acc_z) + ".root";
        TFile histofile(histofilename.c_str(), "RECREATE");


        for (int a=0; a<len_1; a++){
          std::string histoname = "hs1d_" + std::to_string(a);
          histofile.WriteObject((H1[i][j])[a], histoname.c_str());
          delete (H1[i][j])[a];
        }

        (H1[i][j]).clear();
      }
    }
    

  delete ch;
  delete ppc;
}
