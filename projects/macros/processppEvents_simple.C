#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "ppHelpers.h"

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
void processppEvents(std::string label, std::string fnames, TString fnconfig = "ppConfig.json")
{
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
  rnMaps rnmaps;
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);
    if ((ii % 10000) == 0) std::cout << ii << " of " << nEvents2Process << std::endl;
    //
    // add here the event-by-event analysis
    //
    
    // ?? What are Trs, Trofs ...
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
    
    // charactarize each run by position (important for calculating mean position of each run)
    rnmaps.add(0, RunNumber, 1.);
    rnmaps.add(1, RunNumber, PosX);
    rnmaps.add(2, RunNumber, PosY);
    rnmaps.add(3, RunNumber, PosZ);
        
    // Add 1 weight to each histogram in the right area:
    
    // event characteristics
    hs1d[6]->Fill(OccupancyInTime, 1.);
    hs1d[7]->Fill(TotalFT0AmplitudeA, 1.);
    hs1d[8]->Fill(TotalFT0AmplitudeC, 1.);
    hs1d[9]->Fill(TotalFV0AmplitudeA, 1.);
    hs1d[10]->Fill(TimeFT0A, 1.);
    hs1d[11]->Fill(TimeFT0C, 1.);
    hs1d[12]->Fill(TimeFV0A, 1.);
    
    // number of tracks and vertex position
    // hs2d[0]->Fill(NumContrib, TotalTracks, 1.);
    hs1d[3]->Fill(PosX, 1.);
    hs1d[4]->Fill(PosY, 1.);
    hs1d[5]->Fill(PosZ, 1.);
    // hs2d[1]->Fill(PosZ, TMath::Sqrt(PosX*PosX+PosY*PosY), 1.);
    
 
/*
   // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<NumContrib; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);

      }
      // ULS and LS
      if (TrkSign[0]*TrkSign[1] < 0) {
        // count candidates
        hs1d[0]->Fill(9., 1.);
        hs1d[1]->Fill(ivm.M(), 1.);

      } else {
        hs1d[2]->Fill(ivm.M(), 1.);
    }
  }
}
  */




  // prepare canvas
  TCanvas *cv = new TCanvas();
  std::string pdfname = "ppEvents_"+label+".pdf";
  cv->SaveAs((pdfname+"[").data());

  TLatex txt;
  txt.SetTextSize(0.03);
  txt.SetTextAlign(32);

  // plot histograms
  auto annText = (TString)label;

  
  pph.plotHistograms(cv, annText, hs1d, hs2d, pdfname, false);
  

  
  // add special plots
  // ULS with scaled LS
  // the scaling is determined in the IVm range up to 0.8 GeV/c^2
  // The LS distribution is scale to touch the ULD distribution from below in this IVM range
  int imax = hs1d[1]->FindBin(0.8);
  float q = 0.0;
  for (int ii=1; ii<=imax; ii++) {
    auto a = hs1d[1]->GetBinContent(ii);
    if (a > 1.) {
      auto b = hs1d[2]->GetBinContent(ii);
      if (b/a > q) q = b/a;
    }
  }
  float fscale = q > 0. ? 1./q : 1.;
  std::cout << "fscale " << fscale << std::endl;
  auto h0 = (TH1D*)hs1d[2]->Clone("scaled");
  h0->Scale(fscale);
  h0->SetLineColor(c2u[3]);
  auto h1 = (TH1D*)hs1d[1]->Clone("diff");
  h1->Add(h0, -1.);
  h1->SetLineColor(c2u[4]);

  cv->SetLogy(false);
  hs1d[1]->Draw("HIST");
  h0->Draw("HIST SAME");
  h1->Draw("HIST SAME");
  txt.DrawLatexNDC(0.85,0.97,annText);
  cv->Update(); cv->SaveAs(pdfname.data());

  cv->SetLogy(true);
  hs1d[1]->Draw("HIST");
  h0->Draw("HIST SAME");
  h1->Draw("HIST SAME");
  txt.DrawLatexNDC(0.85,0.97,annText);
  cv->Update(); cv->SaveAs(pdfname.data());
  
  delete h0;
  delete h1;



  // clean up
  cv->SaveAs((pdfname+"]").data());
  delete ch;
  
}
