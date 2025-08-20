//plots the histograms built by equalsign.C

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "../ppHelpers_equalsign.h"

//----------------------------

//----------------------------

void plot(std::string label, TString fnconfig = "../ppConfig.json"){

  //load histogram
  std::string histofilename = "histograms/"+label+".root";
  TFile histofile(histofilename.c_str(), "READ");

  std::string histoname = "hs1d";
  TH1D* histo = static_cast<TH1D*>(histofile.Get(histoname.c_str()));

  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();
  histo->Draw();
  std::string outputname = "pngs/"+label+".png";
  cv->SaveAs(outputname.c_str());

}


void plotbackground(){
  std::string label = "histo_equalsign";
  plot(label);
}

void plotblueprint(){
  std::string label = "histo_ivmblueprint";
  plot(label);
}

void equalsignplot(){
  //LOAD DATA

  //load histograms
  std::string backgroundfilename = "histograms/histo_scaledequalsign.root";
  TFile backgroundfile(backgroundfilename.c_str(), "READ");

  std::string blueprintfilename = "histograms/histo_scaledblueprint.root";
  TFile blueprintfile(blueprintfilename.c_str(), "READ");

  std::string histoname = "hs1d";
  TH1D* ivmbackground = static_cast<TH1D*>(backgroundfile.Get(histoname.c_str()));
  TH1D* ivmblueprint = static_cast<TH1D*>(blueprintfile.Get(histoname.c_str()));

  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();

  ivmblueprint->SetLineColor(kBlue);
  ivmblueprint->Draw();

  ivmbackground->SetLineColor(kRed);
  ivmbackground->Draw("SAME");
  std::string pngname = "pngs/scaledivmequalsign.png";
  cv->SaveAs(pngname.c_str());

  TCanvas *cv2 = new TCanvas("cv2","log");
  gPad->SetLogy();
  ivmblueprint->SetLineColor(kBlue);
  ivmblueprint->Draw();

  ivmbackground->SetLineColor(kRed);
  ivmbackground->Draw("SAME");
  pngname = "pngs/log_scaledivmequalsign.png";
  cv2->SaveAs(pngname.c_str());
}