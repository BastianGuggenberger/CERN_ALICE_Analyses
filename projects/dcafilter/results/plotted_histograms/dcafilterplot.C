//plots the histograms built by dcafilter.C

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "../ppHelpers_2.h"

//----------------------------
//CUSTOM VARIABLES (important!)
double acc_xy = 0.015;
double acc_z = 0.0125;
double alpha = 1.0;
//----------------------------

void dcafilterplot(std::string folder, double IVMmin, double IVMmax, TString fnconfig = "../ppConfig.json"){

  //LOAD DATA

  std::string label = "_alpha" + std::to_string(alpha) +"_xy"+std::to_string(acc_xy) + "_z" + std::to_string(acc_z);

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // prepare histograms
  std::vector<TH1D*> hs1d;
  //std::vector<TH2D*> hs2d;
  pph.getHistos(hs1d);

  //load histograms
  std::string histofilename = "histograms/"+folder+"histos_alpha" + std::to_string(alpha) + "_xy" + std::to_string(acc_xy) + "_z" + std::to_string(acc_z) + ".root";
  TFile histofile(histofilename.c_str(), "READ");

  int len_1 = hs1d.size();
  //int len_2 = hs2d.size();


  for (int i=0; i<len_1; i++){
    std::string histoname = "hs1d_" + std::to_string(i);
    hs1d[i]= static_cast<TH1D*>(histofile.Get(histoname.c_str()));
    //std::cout << i << std::endl;
  }

  /*
  for (int i=0; i<len_2; i++){
    std::string histoname = "hs2d_" + std::to_string(i);
    hs2d[i] = static_cast<TH2D*>(histofile.Get(histoname.c_str()));
    //std::cout << i << std::endl;
  }
*/


  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();
  std::string pdfname = "pdfs/"+folder+"ppEvents_"+label+".pdf";
  cv->SaveAs((pdfname+"[").data());

  TLatex txt;
  txt.SetTextSize(0.03);
  txt.SetTextAlign(32);

  // plot histograms
  auto annText = (TString)label;
  pph.plotHistograms(cv, annText, hs1d,  pdfname, IVMmin, IVMmax);
 
  // clean up
  cv->SaveAs((pdfname+"]").data());

}


void analysealphas(double IVMmin, double IVMmax){
  double startalpha = 0.3;
  double stepsize = 0.15;
  double n_steps = 14;

  std::string folder = "varyalpha/";
  for (int i=0; i<n_steps; i++){
    alpha = startalpha + i*stepsize;
    dcafilterplot(folder, IVMmin, IVMmax);
    std::cout << "alpha " << alpha << " plotted." << std::endl ;
  }
}

void analysexyz(double IVMmin, double IVMmax){
  const double product = 15.0e-03 * 12.5e-03; //surface of ellipse will be kept constant
  double start_acc_xy = 3.0e-03;
  double stepsize = 2.0e-03;
  double n_steps = 15;
  std::string folder = "varyxyz/";
  for (int i=0; i<n_steps; i++){
    acc_xy = start_acc_xy + i*stepsize;
    acc_z = product/acc_xy;
    dcafilterplot(folder, IVMmin, IVMmax);
  }
}


void analyse2d(double IVMmin, double IVMmax){
  std::string folder = "2d/";
  for (int i=1; i<12; i++){
    for (int j=1; j<12; j++){
      acc_xy = 0.0 + 0.0025* i;
      acc_z = 0.0 + 0.0025* j;
    dcafilterplot(folder, IVMmin, IVMmax);
  }
}
}