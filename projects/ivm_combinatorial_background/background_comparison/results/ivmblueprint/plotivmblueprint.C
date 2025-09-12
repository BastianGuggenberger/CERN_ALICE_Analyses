#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "TLegend.h"
#include "TColor.h"
#include <vector>

//----------------------------------------------------------------------------------------

//plotivmblueprint: plots the ivmhistogram and of ALL pion pion events
void plotivmblueprint(TString fnconfig = "../../resources/ppHelpers/ppConfig.json"){
  
  //plot histogram
  TCanvas *cv = new TCanvas();
  TFile histofile("histo_ivmblueprint.root", "READ");
  TH1D* histo = static_cast<TH1D*>(histofile.Get("hs1d"));
  histo->Draw();
  cv->SaveAs("ivmblueprint.png");
  
}
