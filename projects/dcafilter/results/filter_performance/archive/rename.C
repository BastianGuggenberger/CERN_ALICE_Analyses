//several methods to analyse the performance of the dca kaon event filter
//resources: 


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"


void renamehisto(){

  // prepare histograms
  TH1D* ivmhisto = new TH1D;

  //load histograms
  std::string histofilename = "../histograms/varyalpha/old.root";
  TFile histofile(histofilename.c_str(), "OPEN");
  ivmhisto = dynamic_cast<TH1D*>(histofile.Get("hs1d_1"));

  TFile outputhisto("../histograms/varyalpha/histos_alpha100.000000_xy0.015000_z0.012500.root","RECREATE");
  outputhisto.WriteObject(ivmhisto,"ivmhisto");
}
