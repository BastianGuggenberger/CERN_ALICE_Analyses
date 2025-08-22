//Fills Histograms of pp Events with kaon events filtered out by restricting the dca


#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TRandom3.h"

#include "../ppHelpers/ppHelpers.h"
#include <random>
#include <cmath>

// run with
//  root -q -b 'processppEvents.C("LHC22_pass7_skimmed", "RF_LHC22_pass7_skimmed.txt")'
//  root -q -b 'processppEvents.C("LHC23_pass4_Thin_small", "RF_LHC23_pass4_Thin_small.txt")'
//  root -q -b 'processppEvents.C("LHC23_pass4_Thin", "RF_LHC23_pass4_Thin.txt")'

// Available datastes                  Events total    2pi candidates    2K candidates
//    LHC22_pass7_skimmed               32'467'114
//    LHC22_pass4_lowIR
//    LHC22_pass4_highIR_Thin_sampling
//    LHC22_pass4_highIR_Thin
//    LHC23_pass4_skimmed                13'824'860
//    LHC23_pass4_Thin_small             18'275'132
//    LHC23_pass4_Thin                  529'246'110
//    LHC24_pass1_skimmed               125'768'899
//    LHC24_pass1_MinBias
//
// auto fnames = std::string("rootfiles.txt");
auto rnd = new TRandomMT64();

std::double get_theta(bool uniform){
  double u = rnd->Uniform();
  double theta = 0.0;
  if(uniform){
    theta = 2*M_PI * u;
  }else{
    // distribution of sqrt(1+cos(alpha)) is uniform
    double x = 2 * u;
    double y = x*x - 1;
    theta = (rnd->Uniform() < 0.5) ? acos(y) : (2*M_PI - acos(y));
  }
  return theta;
}


std::vector<double> rotate_in_plane(std::vector<double> p_0,std::vector<double> p_1){

  //convert momenta to TVector3
  TVector3 vector_0(p_0[0],p_0[1],p_0[2]);
  TVector3 vector_1(p_1[0],p_1[1],p_1[2]);

  //define rotation axis
  TVector3 rotaxis = vector_0.Cross(vector_1).Unit();

  //rotate

  vector_1.Rotate(theta, rotaxis);

  std::vector<double> rotated = {vector_1.X(),vector_1.Y(),vector_1.Z()};

  return rotated;

}

std::vector<double> rotate_in_plane_uniformalpha(std::vector<double> p_0,std::vector<double> p_1){

  //convert momenta to TVector3
  TVector3 vector_0(p_0[0],p_0[1],p_0[2]);
  TVector3 vector_1(p_1[0],p_1[1],p_1[2]);

  //define rotation axis
  TVector3 rotaxis = vector_0.Cross(vector_1).Unit();

  //rotate
  // distribution of sqrt(1+cos(alpha)) is uniform
  x = 2 * u;
  y = x*x - 1;
  alpha = (rnd->Uniform() < 0.5) ? acos(y) : (2*M_PI - acos(y));
  double theta = dist(gen);
  theta = theta * M_PI / 180.0;
  vector_1.Rotate(theta, rotaxis);

  std::vector<double> rotated = {vector_1.X(),vector_1.Y(),vector_1.Z()};

  return rotated;

}


void rotatedtracks(TString fnconfig = "../ppHelpers/ppConfig.json")
{
  int nbIVM = 4000;
  float bIVMmin = 0.;
  float bIVMmax = 5.;

  //safe data
  std::string histofilename = "results/histograms/histo_rotatedtracks.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("ressources/rootfiles.txt");
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;
  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax);
  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);

      if((ii % 100000)==0){
        std::cout << float(ii)/float(nEvents2Process)*100 << " % " << std::endl;
      }

    // event selections 
    if (!pph.isGoodEvent(ppc))
    {
      continue;
    }
    
    // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);

      std::vector<double> p_0 = {TrkPx[0],TrkPy[0],TrkPz[0]};
      std::vector<double> p_1 = {TrkPx[1],TrkPy[1],TrkPz[1]};

      //rotating operation
      p_1 = rotate_in_plane(p_0,p_1);

      ivm += ROOT::Math::PxPyPzMVector(p_0[0],p_0[1],p_0[2],masses[0]);
      ivm += ROOT::Math::PxPyPzMVector(p_1[0],p_1[1],p_1[2],masses[0]);

      // ULS and LS
      if (TrkSign[0]*TrkSign[1] < 0) {
        // count candidates        
        hs1d->Fill(ivm.M(), 1.);
      }
    }
  }
  std::string histoname = "hs1d";
  histofile.WriteObject(hs1d, histoname.c_str());
  delete ch;
}
