//Fills Histograms of pp Events with kaon events filtered out by restricting the dca

#include "../../resources/ppPaths.h"
#include "../../resources/ppHelpers/ppHelpers.h"

#include "TRandom3.h"
#include <random>
#include <cmath>

auto rnd = new TRandomMT64();

double get_theta(bool uniform){
  double u = rnd->Uniform();
  double theta = 0.0;
  if(uniform){
    // distribution of theta is uniform
    theta = 2*M_PI * u;
  }else{
    // distribution of x=sqrt(1+cos(theta)) is uniform
    double x = sqrt(2) * u;
    double y = x*x - 1;
    theta = (rnd->Uniform() < 0.5) ? acos(y) : (2*M_PI - acos(y));
  }
  return theta;
}


double rotate_in_plane(bool uniform, std::vector<double> p_0, std::vector<double>& p_1){

  //convert momenta to TVector3
  TVector3 vector_0(p_0[0],p_0[1],p_0[2]);
  TVector3 vector_1(p_1[0],p_1[1],p_1[2]);

  //define rotation axis
  TVector3 rotaxis = vector_0.Cross(vector_1).Unit();

  //rotate
  double theta = get_theta(uniform);

  //vector_0 is rotated because than theta defines the angle between p_0 and p_1
  vector_0.Rotate(theta, rotaxis);
  p_1 = {vector_0.X(),vector_0.Y(),vector_0.Z()};

  double theta_between;
  if(theta>M_PI){
    theta_between = 2*M_PI - theta;
  }else{
    theta_between = theta;
  }
  return theta_between;

}

void rotatedtracks(bool uniform = false)
{
  //safe data
  std::string histofilename = "results/histograms/histo_rotatedtracks_uniformsqrt_new.root";
  if(uniform){
    histofilename = "results/histograms/histo_rotatedtracks_uniformtheta_new.root";
  }
  TFile histofile(histofilename.c_str(), "RECREATE");

  histofilename = "results/histograms/histo_theta_uniformsqrt_new.root";
  TFile histofile_theta(histofilename.c_str(), "RECREATE");

  histofilename = "results/histograms/histo_sqrt_uniformsqrt_new.root";
  TFile histofile_sqrt(histofilename.c_str(), "RECREATE");

  histofilename = "results/histograms/histo_thetavsivm_new.root";
  TFile histofile_thetavsivm(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles);

  //create histograms
  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", pph.nbIVM, pph.bIVMmin, pph.bIVMmax);
  TH1D* hs1d_theta = new TH1D("theta", ";theta;Number of events", pph.nbIVM, 0., 6.4);
  TH1D* hs1d_sqrt = new TH1D("sqrt", ";sqrt-term ;Number of events", pph.nbIVM, 0., 2.);
  TH2D* hs2d = new TH2D("theta vs IVM", "theta vs IVM; theta; IVM [GeV/c^{2}]",pph.nbIVM, 0., 6.4,pph.nbIVM,pph.bIVMmin,pph.bIVMmax);
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;

  for (auto ii = 0; ii<nEvents2Process; ii+=1)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);

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
      double theta = rotate_in_plane(uniform, p_0,p_1);

      ivm += ROOT::Math::PxPyPzMVector(p_0[0],p_0[1],p_0[2],masses[0]);
      ivm += ROOT::Math::PxPyPzMVector(p_1[0],p_1[1],p_1[2],masses[0]);

      // ULS and LS
      if (TrkSign[0]*TrkSign[1] < 0) {
        // count candidates        
        hs1d->Fill(ivm.M(), 1.);
        hs1d_theta->Fill(theta,1.);
        hs1d_sqrt->Fill(sqrt(1.+cos(theta)),1.);
        hs2d->Fill(theta,ivm.M(),1.);
      }
    }
  }
  std::string histoname = "hs1d";
  histofile.WriteObject(hs1d, histoname.c_str());
  histofile_theta.WriteObject(hs1d_theta, histoname.c_str());
  histofile_sqrt.WriteObject(hs1d_sqrt, histoname.c_str());
  histofile_thetavsivm.WriteObject(hs2d, histoname.c_str());
  delete ch;
}