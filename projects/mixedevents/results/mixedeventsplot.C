//plots the histograms built by mixedevents.C

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "../ppHelpers_mixedevents.h"

//----------------------------

//----------------------------

void mixedeventsplot(double IVMmin, double IVMmax, TString fnconfig = "../ppConfig.json"){

  //LOAD DATA

  std::string label = "mixed_events";

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // prepare histograms
  std::vector<TH1D*> hs1d;
  std::vector<TH2D*> hs2d;
  pph.getHistos(hs1d,hs2d);

  //load histograms
  std::string histofilename = "histograms/histo_mixedevents.root";
  TFile histofile(histofilename.c_str(), "READ");

  int len_1 = hs1d.size();
  int len_2 = hs2d.size();


  for (int i=0; i<len_1; i++){
    std::string histoname = "hs1d_" + std::to_string(i);
    hs1d[i]= static_cast<TH1D*>(histofile.Get(histoname.c_str()));
    //std::cout << i << std::endl;
  }

  
  for (int i=0; i<len_2; i++){
    std::string histoname = "hs2d_" + std::to_string(i);
    hs2d[i] = static_cast<TH2D*>(histofile.Get(histoname.c_str()));
    //std::cout << i << std::endl;
  }



  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();
  std::string pdfname = "pdfs/ppEvents_"+label+".pdf";
  cv->SaveAs((pdfname+"[").data());

  TLatex txt;
  txt.SetTextSize(0.03);
  txt.SetTextAlign(32);

  // plot histograms
  auto annText = (TString)label;
  pph.plotHistograms(cv, annText, hs1d,hs2d, pdfname, IVMmin, IVMmax);
 
  // clean up
  cv->SaveAs((pdfname+"]").data());

}
