//Fills Histograms of pp Events with kaon events filtered out by restricting the dca


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "ppHelpers.h"
double acc_xy = 15.0e-03;
double acc_z = 12.5e-03;

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
void processppEvents(std::string foldername, std::string fnames, double alpha, TString fnconfig = "ppConfig.json")
{

  //safe data
  std::string histofilename = "results/histograms/"+ foldername + "histos_alpha"+ std::to_string(alpha) + "_xy" + std::to_string(acc_xy) + "_z" + std::to_string(acc_z) + ".root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain(fnames);

  // prepare histograms
  std::vector<TH1D*> hs1d;
  std::vector<TH2D*> hs2d;

  pph.getHistos(hs1d, hs2d);

  // prepare structures to hold variables as function of run number
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);

    
    if ((ii % 10000)== 0){
      std::cout << ii << " of " << nEvents2Process << std::endl;
    }/* else {
      continue;
    }*/
   
    //
    // add here the event-by-event analysis
    //
    
    // update the event selection bits histogram
    // count processed events
    hs1d[0]->Fill(0., 1.);
    if (Trs == 1) hs1d[0]->Fill(1., 1.);
    if (Trofs == 1) hs1d[0]->Fill(2., 1.);
    if (Hmpr == 1) hs1d[0]->Fill(3., 1.);
    if (Tfb == 1) hs1d[0]->Fill(4., 1.);
    if (ItsRofb == 1) hs1d[0]->Fill(5., 1.);
    if (Sbp == 1) hs1d[0]->Fill(6., 1.);
    if (ZvtxFT0vsPv == 1) hs1d[0]->Fill(7., 1.);
    if (VtxITSTPC == 1) hs1d[0]->Fill(8., 1.);
    
    // event selections 
    if (!pph.isGoodEvent(ppc, alpha, acc_xy, acc_z))
    {
      continue;
    }



    // event characteristics
    hs1d[6]->Fill(OccupancyInTime, 1.);
    hs1d[7]->Fill(TotalFT0AmplitudeA, 1.);
    hs1d[8]->Fill(TotalFT0AmplitudeC, 1.);
    hs1d[9]->Fill(TotalFV0AmplitudeA, 1.);
    hs1d[10]->Fill(TimeFT0A, 1.);
    hs1d[11]->Fill(TimeFT0C, 1.);
    hs1d[12]->Fill(TimeFV0A, 1.);
    
    // number of tracks and vertex position
    hs2d[0]->Fill(NumContrib, TotalTracks, 1.);
    hs1d[3]->Fill(PosX, 1.);
    hs1d[4]->Fill(PosY, 1.);
    hs1d[5]->Fill(PosZ, 1.);
    hs2d[1]->Fill(PosZ, TMath::Sqrt(PosX*PosX+PosY*PosY), 1.);

    // track eta vs pT
    for (int ii=0; ii<2; ii++) {
      auto v = ROOT::Math::XYZVector(TrkPx[ii], TrkPy[ii], TrkPz[ii]);
      hs2d[15]->Fill(v.eta(), v.Rho(), 1.);
      if (TrkTOFexpMom[ii] >= 0.) {
        hs2d[16]->Fill(v.eta(), v.Rho(), 1.);
      }
    }
    
    // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<NumContrib; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
        
        // update track histograms
        hs2d[2]->Fill(TrkTPCinnerParam[ii], TrkTPCnSigmaEl[ii], 1.);
        hs2d[3]->Fill(TrkTPCinnerParam[ii], TrkTPCnSigmaPi[ii], 1.);
        hs2d[4]->Fill(TrkTPCinnerParam[ii], TrkTPCnSigmaKa[ii], 1.);
        hs2d[5]->Fill(TrkTPCinnerParam[ii], TrkTPCnSigmaPr[ii], 1.);
        
        hs2d[6]->Fill(TrkTOFexpMom[ii], TrkTOFnSigmaEl[ii], 1.);
        hs2d[7]->Fill(TrkTOFexpMom[ii], TrkTOFnSigmaPi[ii], 1.);
        hs2d[8]->Fill(TrkTOFexpMom[ii], TrkTOFnSigmaKa[ii], 1.);
        hs2d[9]->Fill(TrkTOFexpMom[ii], TrkTOFnSigmaPr[ii], 1.);

      }
      // ULS and LS
      if (TrkSign[0]*TrkSign[1] < 0) {
        // count candidates
        hs1d[0]->Fill(9., 1.);
        
        hs1d[1]->Fill(ivm.M(), 1.);
        hs2d[10]->Fill(ivm.M(), ivm.Pt(), 1.);

        for (int ii=0; ii<NumContrib; ii++)
        {
          hs2d[11]->Fill(ivm.M(), TrkTPCinnerParam[ii], 1.);
          hs2d[12]->Fill(ivm.M(), 1.E3 * TrkDCAxy[ii], 1.);
          hs2d[13]->Fill(ivm.M(), 1.E3 * TrkDCAz[ii], 1.);
          hs2d[17]->Fill(1.E3 * TrkDCAz[ii], 1.E3 * TrkDCAxy[ii]);
        }

      } else {
        hs1d[2]->Fill(ivm.M(), 1.);
      }
      
      hs2d[14]->Fill(ivm.M(), pph.acoplanarity(), 1.);
    }
  }


  int len_1 = hs1d.size();
  int len_2 = hs2d.size();

  for (int i=0; i<len_1; i++){
    std::string histoname = "hs1d_" + std::to_string(i);
    histofile.WriteObject(hs1d[i], histoname.c_str());
  }

  for (int i=0; i<len_2; i++){
    std::string histoname = "hs2d_" + std::to_string(i);
    histofile.WriteObject(hs2d[i], histoname.c_str());
  }

  delete ch;

  
}


void varyalpha(){
  double startalpha = 100.0;
  double stepsize = 0.10;
  double n_steps = 1;
  double alpha;
  for (int i=0; i<n_steps; i++){
    alpha = startalpha + i*stepsize;
    processppEvents("varyalpha/", "ressources/rootfiles.txt", alpha);
    std::cout << "alpha " << alpha << "done." << std::endl ;
  }
}


void vary_xy_z(){
  double alpha = 0.6;

  const double product = 15.0e-03 * 12.5e-03; //surface of ellipse will be kept constant

  double start_acc_xy = 3.0e-03;
  double stepsize = 2.0e-03;
  double n_steps = 15;
  
  for (int i=0; i<n_steps; i++){
    acc_xy = start_acc_xy + i*stepsize;
    acc_z = product/acc_xy;
    processppEvents("varyxyz/","ressources/rootfiles.txt", alpha);
    std::cout << "alpha " << alpha << "done." << std::endl ;
  }
}