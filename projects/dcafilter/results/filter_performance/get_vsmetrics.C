//several methods to analyse the performance of the dca kaon event filter
//resources: 


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
//---------------------------------------
//VARIABLES: (important)

//set acc_xy and acc_z when using getalphasvsmetrics
const double acc_xy_reset = 0.015;
const double acc_z_reset = 0.0125;

//set alpha when using getxyzvsmetrics()
const double alpha_reset_xyz = 0.6;
const double alpha_reset_2d = 1.0;

//---------------------------------------
double alpha;
double acc_xy;
double acc_z;

ofstream historyfile("resources/history.txt");
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


TH1D* loadhisto(std::string folder, int entry){

  // prepare histograms
  TH1D* ivmhisto = new TH1D;

  //load histograms
  std::string histofilename = "../histograms/" + folder + "histos_alpha"+ std::to_string(alpha) + "_xy" + std::to_string(acc_xy) + "_z" + std::to_string(acc_z) + ".root";
  TFile histofile(histofilename.c_str(), "OPEN");
  ivmhisto = dynamic_cast<TH1D*>(histofile.Get("ivmhisto"));
  ivmhisto->SetDirectory(nullptr);

  return ivmhisto;
}



std::vector<double> get_abct(std::string folder){
  int entry = 1;
  if(folder=="2d/"){
    entry = 0;
  }
  TH1D* ivmhisto = loadhisto(folder, entry);
  double t = ivmhisto->Integral();
  std::cout << t << std::endl;

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

  std::string pngname = "fitpngs/" + folder +"alpha" +std::to_string(alpha) + "_xy"+ std::to_string(acc_xy) + "_z" + std::to_string(acc_z)+ ".png";
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
    std::string folder = "varyalpha/";
    acc_xy = acc_xy_reset;
    acc_z = acc_z_reset;

    double metric;
    std::vector<double> alphavec {0.3}; //0.45,0.6,0.75,0.9,1.05,1.2,1.35,1.5,1.65,1.8,1.95,2.1,2.25
    ofstream alphaxyzvsmetrics("resources/varyalpha/alpha_xy_z_a_b_c_t.txt");
    ofstream originalmetrics("resources/original_a_b_c_t.txt");

    /*
    alpha = 100;
    std::vector<double> abct_0 = get_abct(folder);
    originalmetrics << abct_0[0] << "," << abct_0[1] << "," << abct_0[2] << "," << abct_0[3];
    */

    for (double nextalpha: alphavec){
        alpha = nextalpha;
        std::vector<double> abct_1 = get_abct(folder);
        alphaxyzvsmetrics << alpha << "," << acc_xy << "," << acc_z << "," << abct_1[0] << "," << abct_1[1] << "," << abct_1[2] << "," << abct_1[3] << std::endl;
    }
}


void getxyzvsmetrics(){
    std::string folder = "varyxyz/";
    alpha = alpha_reset_xyz;

    double metric;
    ofstream alphaxyzvsmetrics("resources/varyxyz/alpha_xy_z_a_b_c_t.txt");

    const double product = 15.0e-03 * 12.5e-03; //surface of ellipse will be kept constant
    double start_acc_xy = 3.0e-03;
    double stepsize = 2.0e-03;
    double n_steps = 15;
    for (int i=0; i<n_steps; i++){
        acc_xy = start_acc_xy + i*stepsize;
        acc_z = product/acc_xy;
        std::vector<double> abct_1 = get_abct(folder);
        alphaxyzvsmetrics << alpha << "," << acc_xy << "," << acc_z << "," << abct_1[0] << "," << abct_1[1] << "," << abct_1[2] << "," << abct_1[3] << std::endl;
    }
}

void get2dvsmetrics(){
    std::string folder = "2d/";
    alpha = alpha_reset_2d;

    double metric;
    ofstream alphaxyzvsmetrics("resources/2d/alpha_xy_z_a_b_c_t.txt");

    for (int i=1; i<12; i++){
      for(int j=1; j<12; j++){
        acc_xy = 0.0 + 0.0025*i;
        acc_z = 0.0 + 0.0025*j;
        std::vector<double> abct_1 = get_abct(folder);
        alphaxyzvsmetrics << alpha << "," << acc_xy << "," << acc_z << "," << abct_1[0] << "," << abct_1[1] << "," << abct_1[2] << "," << abct_1[3] << std::endl;
    }
    }
}