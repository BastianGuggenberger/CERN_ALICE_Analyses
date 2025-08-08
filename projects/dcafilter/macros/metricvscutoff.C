#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

std::vector<double> cutoffvec {0.1,0.2,0.3,0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2.1,2.25};

const double kaonmass = 0.497611; //GeV, source pdg.gov
const double tolerance = 0.02;

double IVMmin = kaonmass - tolerance;
double IVMmax = kaonmass + tolerance;


std::vector<double> getmetric(double cutoff, TString fnconfig = "ppConfig.json"){

  //LOAD DATA

  std::string label = "dcacutoff_1_" + std::to_string(cutoff);

  // prepare histograms
  TH1D* ivmhisto;

  //load histograms
  std::string histofilename = "histograms/histos_cutoff" + std::to_string(cutoff) + ".root";
  TFile histofile(histofilename.c_str(), "OPEN");
  std::string histoname = "hs1d_1";
  ivmhisto = static_cast<TH1D*>(histofile.Get(histoname.c_str()));
  


  //CALCULATE METRIC;

  double totalevents = ivmhisto->Integral();

  int binmin = ivmhisto->FindBin(IVMmin);
  int binmax = ivmhisto->FindBin(IVMmax);
  double kaon_and_background = ivmhisto->Integral(binmin,binmax);
  double metric = kaon_and_background/totalevents;

  std::vector<double> mandt = {metric, totalevents};

  return mandt;
}

void cutvsmetric(){
    double metric;
    ofstream cutsvsmetrics("cutsvsmetrics.txt");

    for (double cutoff: cutoffvec){
        std::vector<double> mandt = getmetric(cutoff);
        double metric = mandt[0];
        double totalevents = mandt[1];
        cutsvsmetrics << cutoff << "," << metric << "," << totalevents << std::endl;
    }
}