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


void rotate_in_plane(bool uniform, std::vector<double> p_0, std::vector<double>& p_1){

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
}

void rotatedtracks(bool uniform = false)
{
  //safe data
  std::string histofilename = "results/histograms/histo_rotatedtracks.root";
  if(uniform){
    histofilename = "results/histograms/histo_rotatedtracks_uniformtheta.root";
  }
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles);

  // prepare histos and masses
  TH1D* IVMhisto = pph.getIVMhisto();
  std::vector<TH2D*> hs2d = pph.getparamvsivmhistos(); 
  std::vector<float> masses;

  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii+=1)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);

    // event selections 
    if (!pph.isGoodEvent(ppc)) continue;
    if (!pph.isGoodTuple(ppc, masses, true) ) continue;// check if event has 2 tracks which are compatible with PID hypothesis

    ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
    std::vector<double> p_0 = {TrkPx[0],TrkPy[0],TrkPz[0]};
    std::vector<double> p_1 = {TrkPx[1],TrkPy[1],TrkPz[1]};

    //rotating operation
    rotate_in_plane(uniform, p_0,p_1); //rotates p_0

    ivm += ROOT::Math::PxPyPzMVector(p_0[0],p_0[1],p_0[2],masses[0]);
    ivm += ROOT::Math::PxPyPzMVector(p_1[0],p_1[1],p_1[2],masses[0]);

    // ULS and LS
    if (TrkSign[0]*TrkSign[1] < 0) {
      //Fill Histograms
      IVMhisto->Fill(ivm.M(), 1.);

      auto v_0 = ROOT::Math::XYZVector(p_0[0],p_0[1],p_0[2]);
      hs2d[0]->Fill(v_0.Rho(), ivm.M(), 1.);
      hs2d[2]->Fill(v_0.eta(), ivm.M(), 1.);

      auto v_1 = ROOT::Math::XYZVector(p_1[0],p_1[1],p_1[2]);
      hs2d[0]->Fill(v_1.Rho(), ivm.M(), 1.);
      hs2d[2]->Fill(v_1.eta(), ivm.M(), 1.);
      
      auto v_0_Tvec = TVector3(p_0[0],p_0[1],p_0[2]);
      auto v_1_Tvec = TVector3(p_1[0],p_1[1],p_1[2]);
      double theta = v_0_Tvec.Angle(v_1_Tvec);
      hs2d[1]->Fill(theta, ivm.M(), 1.);
      hs2d[3]->Fill(sqrt(1.+cos(theta)),ivm.M(),1.);
    }
  }

  //safe histograms
  histofile.WriteObject(IVMhisto,"IVMhisto");
  histofile.WriteObject(hs2d[0],"pThisto");
  histofile.WriteObject(hs2d[1],"thetahisto");
  histofile.WriteObject(hs2d[2],"etahisto");
  histofile.WriteObject(hs2d[3],"sqrthisto");

  delete ch;
}