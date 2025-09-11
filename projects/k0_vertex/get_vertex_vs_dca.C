//plots the vertex position (xy and z) versus the track dca(xy, z) for events in the K0 mass band and outside of this band

#include "../resources/ppHelpers/ppHelpers.h"
#include "../resources/ppPaths.h"


void get_vertex_vs_dca()
{
  //variables:
  const double meansigma = 0.005449703125;
  const double kaonmass = 0.497611; //GeV, source pdg.gov

  bool goodtuple;
  double ivm_mass;
  double trksgnprdct;

  //get helpers and configuration
  TH1::AddDirectory(kFALSE);
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles,false);

  //prepare histograms and masses
  TH2D* histo_vt_k0band = new TH2D("Transversal Vertex position vs dca_xy (in k0 mass band)", "Number of events; V_{T} ; track dca_{xyz}", 200, -0.15, 0.15, 200, 0., 10.0);
  TH2D* histo_vt_nonk0band = new TH2D("Transversal Vertex position vs dca_xy (outside k0 mass band)", "Number of events; V_{T} ; track dca_{xyz}", 400, -0.15, 0.15, 200, 0., 10.0);
  TH2D* histo_vz_k0band = new TH2D("Z Vertex position vs dca_xy (in k0 mass band)", "Number of events; V_{z} ; track dca_{xyz}", 200, -25., 25., 200, 0., 10.0);
  TH2D* histo_vz_nonk0band = new TH2D("Z Vertex position vs dca_xy (outside k0 mass band)", "Number of events; V_{z} ; track dca_{xyz}", 200, -25., 25., 200, 0., 10.0);
  std::vector<float> masses;

  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii+=1)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);

    if (!pph.isGoodEvent(ppc)) continue;
    if(!pph.isGoodTuple(ppc, masses, true)) continue;
    if(TrkSign[0]*TrkSign[1] > 0 ) continue;
    
    ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
    for (int ii=0; ii<NumContrib; ii++){
      ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
    }
    ivm_mass = ivm.M();

    double maxdca = std::max(TrkDCAxy[0]*TrkDCAxy[0],TrkDCAxy[1]*TrkDCAxy[1]);
    if((ivm_mass>=kaonmass-3*meansigma)&&(ivm_mass<=kaonmass+3*meansigma)){
      histo_vt_k0band->Fill(sqrt(PosX*PosX+PosY*PosY),sqrt(maxdca), 1.);
      histo_vz_k0band->Fill(PosZ,sqrt(maxdca), 1.);
    }else{
      histo_vt_nonk0band->Fill(sqrt(PosX*PosX+PosY*PosY),sqrt(maxdca), 1.);
      histo_vz_nonk0band->Fill(PosZ,sqrt(maxdca), 1.);
    }
  }

  //save histograms
  std::cout <<"saving histograms as root files... " << std::endl;
  std::string histofilename = "results/histograms/histos_vertexvsdca.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  histofile.WriteObject(histo_vt_k0band, "histo_vt_k0band");
  histofile.WriteObject(histo_vt_nonk0band, "histo_vt_nonk0band");
  histofile.WriteObject(histo_vz_k0band, "histo_vz_k0band");
  histofile.WriteObject(histo_vz_nonk0band, "histo_vz_nonk0band");

  delete histo_vt_k0band;
  delete histo_vt_nonk0band;
  delete histo_vz_k0band;
  delete histo_vz_nonk0band;

  delete ch;
  delete ppc;
}