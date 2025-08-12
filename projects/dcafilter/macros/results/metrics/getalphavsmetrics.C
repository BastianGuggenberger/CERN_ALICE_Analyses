//several methods to analyse the performance of the dca kaon event filter
//ressources: 


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

std::vector<double> alphavec {0.1,0.2,0.3,0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2.1,2.25};
ofstream historyfile("ressources/history.txt");
const double meansigma = 0.005449703125;

const double kaonmass = 0.497611; //GeV, source pdg.gov
double norm;
double IVMmin = 0.44;
double IVMmax = 0.60;
double plotmin = 0.3;
double plotmax = 0.8;

//0.3: 700,1000,180,0.487,0.04
//1.35: 1350,1000,1350,0.497,0.04
// {const., grad.,amplitude_gauss, mean_gauss, sigma}
std::vector<double> vStart = {100,820,124,kaonmass,0.04};
std::vector<double> vStepsize = {10,5,5,0.0005,0.001};
std::vector<double> vLow = {0,400, 50, 0.495,0.005};
std::vector<double> vHigh = {2000,2000,10000,0.50,0.10};


TH1D* loadhisto(double alpha, int entry){

  //LOAD DATA
  std::string label = "dcacutoff_1_" + std::to_string(alpha);

  // prepare histograms
  TH1D* ivmhisto = new TH1D;
  //load histograms
  std::string histofilename = "../histograms/histos_cutoff" + std::to_string(alpha) + ".root";
  TFile histofile(histofilename.c_str(), "OPEN");
  std::string histoname = "hs1d_" + std::to_string(entry);
  ivmhisto = dynamic_cast<TH1D*>(histofile.Get(histoname.c_str()));
  ivmhisto->SetDirectory(nullptr);

  return ivmhisto;
}





std::vector<double> get_abct(double alpha){

  TH1D* ivmhisto = loadhisto(alpha, 1);
  double t = ivmhisto->Integral();

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


  auto mFunctor = ROOT::Math::Functor(chi,6);
  mMinimizer->SetFunction(mFunctor);

  mMinimizer->SetMaxFunctionCalls(1000000);
  mMinimizer->SetTolerance(0.001);
  mMinimizer->SetPrintLevel(3);

  for(int i=0;i<5;i++){
    mMinimizer->SetLimitedVariable(i, Form("p%d", i), vStart[i], vStepsize[i], vLow[i], vHigh[i]);
  }

  historyfile << alpha << "," << vStart[0] << "," << vStart[1] << "," << vStart[2] << "," << vStart[3] << "," << vStart[4] << std::endl;
  mMinimizer->Minimize();
  const double* vs = mMinimizer->X();
  auto chimin = mMinimizer->MinValue();

  TF1* funderground = new TF1("funderground","pol1",IVMmin,IVMmax);
  TF1* fgaus = new TF1("fgaus","gaus",IVMmin,IVMmax);

  for (int i = 0; i<2; i++){
    f->SetParameter(i, vs[i]);
    vStart[i] = vs[i];
    funderground->SetParameter(i,vs[i]);
  }
  for (int i = 2; i<5; i++){
    f->SetParameter(i, vs[i]);
    vStart[i] = vs[i];
    fgaus->SetParameter(i-2, vs[i]);
  }

  TCanvas *c1 = new TCanvas("c1", "Plot", 800, 600);
 
  ivmhisto->GetXaxis()->SetRangeUser(plotmin,plotmax);
  ivmhisto->SetLineColor(kBlack);
  ivmhisto->Draw();

  f->SetLineColor(kRed);
  f->SetLineWidth(2);
  f->Draw("SAME");

  funderground->SetLineColor(kBlue);
  funderground->SetLineWidth(2);
  funderground->Draw("SAME");

  fgaus->SetLineColor(kGreen);
  fgaus->SetLineWidth(2);
  fgaus->Draw("SAME");

  std::string pngname = "fitpngs/" + std::to_string(alpha) + ".png";
  c1->SaveAs(pngname.c_str());

  double a = fgaus->Integral(vStart[3]-3*meansigma,vStart[3]+3*meansigma);
  double b = t-a;

  double area = f->Integral(vStart[3]-3*meansigma,vStart[3]+3*meansigma);
  double c = area - a;

  return {a, b, c, t};
}



//-----------------------------------------------------
//MAIN:



void getalphasvsmetrics(){
    double metric;
    ofstream alphavsmetrics("ressources/alpha_a_b_c_t.txt");
    ofstream originalmetrics("ressources/original_a_b_c_t.txt");

    std::vector<double> abct_0 = get_abct(100);
    originalmetrics << abct_0[0] << "," << abct_0[1] << "," << abct_0[2] << "," << abct_0[3];
    for (double alpha: alphavec){
        std::vector<double> abct_1 = get_abct(alpha);
        alphavsmetrics << alpha << "," << abct_1[0] << "," << abct_1[1] << "," << abct_1[2] << "," << abct_1[3] << std::endl;
    }
}