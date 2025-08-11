//several methods to analyse the performance of the dca kaon event filter
//ressources: 


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

std::vector<double> cutoffvec {0.3};

const double kaonmass = 0.497611; //GeV, source pdg.gov

double IVMmin = 0.43;
double IVMmax = 0.63;

// {d, k, amplitude_gauss, mean_gauss, sigma}
std::vector<double> vStart = {700,1000,180,0.497,0.04};
std::vector<double> vStepsize = {10,5,5,0.0005,0.001};
std::vector<double> vLow = {500,400, 50, 0.495,0.005};
std::vector<double> vHigh = {900,2000,10000,0.50,0.10};


TH1D* loadhisto(double cutoff, int entry){

  //LOAD DATA
  std::string label = "dcacutoff_1_" + std::to_string(cutoff);

  // prepare histograms
  TH1D* ivmhisto = new TH1D;
  //load histograms
  std::string histofilename = "../histograms/histos_cutoff" + std::to_string(cutoff) + ".root";
  TFile histofile(histofilename.c_str(), "OPEN");
  std::string histoname = "hs1d_" + std::to_string(entry);
  ivmhisto = dynamic_cast<TH1D*>(histofile.Get(histoname.c_str()));
  ivmhisto->SetDirectory(nullptr);

  return ivmhisto;
}





std::vector<double> getgausmetric(double cutoff){

  TH1D* ivmhisto = loadhisto(cutoff, 1);
  double totalevents = ivmhisto->Integral();

  TF1* f = new TF1("f","pol1(0)+gaus(2)", IVMmin, IVMmax);

  auto mMinimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

  auto chi = [&](const double* pars) -> double {
    for (int i=0; i<5; i++){
      f->SetParameter(i,pars[i]);
    }

    double_t chi = 0;
    for(int i=ivmhisto->FindBin(IVMmin); i<=ivmhisto->FindBin(IVMmax); i++){
      auto x = ivmhisto->GetBinCenter(i);
      auto y = ivmhisto->GetBinContent(i);

      double_t yfit = f->Eval(x);

      chi += (y-yfit)*(y-yfit);

    }
    return chi;
  };


  auto mFunctor = ROOT::Math::Functor(chi,5);
  mMinimizer->SetFunction(mFunctor);

  mMinimizer->SetMaxFunctionCalls(1000000);
  mMinimizer->SetTolerance(0.001);
  mMinimizer->SetPrintLevel(3);

  for(int i=0;i<5;i++){
    mMinimizer->SetLimitedVariable(i, Form("p%d", i), vStart[i], vStepsize[i], vLow[i], vHigh[i]);
  }

  mMinimizer->Minimize();
  const double* vs = mMinimizer->X();
  auto chimin = mMinimizer->MinValue();

  TF1* flinear = new TF1("flinear","pol1",IVMmin,IVMmax);
  TF1* fgaus = new TF1("fgaus","gaus",IVMmin,IVMmax);

  for (int i = 0; i<2; i++){
    f->SetParameter(i, vs[i]);
    flinear->SetParameter(i,vs[i]);
  }
  for (int i = 2; i<5; i++){
    f->SetParameter(i, vs[i]);
    fgaus->SetParameter(i-2, vs[i]);
  }

 
  ivmhisto->SetLineColor(kBlack);
  ivmhisto->Draw();

  f->SetLineColor(kRed);
  f->SetLineWidth(2);
  f->Draw("SAME");

  flinear->SetLineColor(kBlue);
  flinear->SetLineWidth(2);
  flinear->Draw("SAME");

  fgaus->SetLineColor(kGreen);
  fgaus->SetLineWidth(2);
  fgaus->Draw("SAME");

  double metric = fgaus->Integral(IVMmin,IVMmax);
  return {metric, totalevents};
}


//-----------------------------------------------------
//MAIN:



void cutvsmetric(){
    double metric;
    ofstream cutsvsmetrics("cutsvsmetrics.txt");

    for (double cutoff: cutoffvec){
        std::vector<double> mandt = getgausmetric(cutoff);
        double metric = mandt[0];
        double totalevents = mandt[1];
        cutsvsmetrics << cutoff << "," << metric << "," << totalevents << std::endl;
    }
}