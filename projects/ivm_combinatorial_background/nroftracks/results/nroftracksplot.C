//plots the histograms built by equalsign.C

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "../../ppHelpers/ppHelpers.h"

//----------------------------

//----------------------------

void nroftracksplot(TString fnconfig = "../ppConfig.json"){

  //load histogram
  std::string histofilename = "histograms/histo_nroftracks.root";
  TFile histofile(histofilename.c_str(), "READ");

  TH1D* hs1d_GoodEvents = static_cast<TH1D*>(histofile.Get("hs1d_GoodEvents"));
  TH1D* hs1d_GoodTuples = static_cast<TH1D*>(histofile.Get("hs1d_GoodTuples"));

  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();
  hs1d_GoodEvents->Draw();
  std::string outputname = "pngs/GoodEvents.png";
  cv->SaveAs(outputname.c_str());

  TCanvas *cv_2 = new TCanvas();
  hs1d_GoodTuples->Draw();
  outputname = "pngs/GoodTuples.png";
  cv_2->SaveAs(outputname.c_str());

}