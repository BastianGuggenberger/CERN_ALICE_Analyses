
#include "../resources/ppHelpers/ppHelpers.h"
#include "../resources/ppPaths.h"
#include <map>

std::vector<TH1D*> getvertexhistos(){
  TH1D* histo_vertex_x_k0 = new TH1D("Vx", ";V_{x};Number of events", 400, -0.1, 0.1); //0
  TH1D* histo_vertex_x_nonk0 = new TH1D("Vx", ";V_{x};Number of events", 400, -0.1, 0.1); //3

  TH1D* histo_vertex_y_k0  = new TH1D("Vy", ";V_{y};Number of events", 400, -0.1, 0.1); //1
  TH1D* histo_vertex_y_nonk0  = new TH1D("Vy", ";V_{y};Number of events", 400, -0.1, 0.1); //4

  TH1D* histo_vertex_z_k0 = new TH1D("Vz", ";V_{z};Number of events", 600, -30., 30.); //2
  TH1D* histo_vertex_z_nonk0 = new TH1D("Vz", ";V_{z};Number of events", 600, -30., 30.); //5

  std::vector<TH1D*> vertexhistos = {histo_vertex_x_k0, histo_vertex_y_k0, histo_vertex_z_k0, histo_vertex_x_nonk0, histo_vertex_y_nonk0, histo_vertex_z_nonk0};

  return vertexhistos;
}

void get_vertex_vs_runnumber()
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

  //prepare runnumber-histograms map and masses
  std::map<int,std::vector<TH1D*>> runhistos;
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

    //check if key exists
    if(runhistos.find(RunNumber) == runhistos.end()){
      //key does not exist - insert key
      runhistos[RunNumber] = getvertexhistos();
    }

    if((ivm_mass>=kaonmass-3*meansigma)&&(ivm_mass<=kaonmass+3*meansigma)){
      //k0 band
      runhistos[RunNumber][0]->Fill(PosX, 1.);
      runhistos[RunNumber][1]->Fill(PosY, 1.);
      runhistos[RunNumber][2]->Fill(PosZ, 1.);
    }else{
      //non k0 band
      runhistos[RunNumber][3]->Fill(PosX, 1.);
      runhistos[RunNumber][4]->Fill(PosY, 1.);
      runhistos[RunNumber][5]->Fill(PosZ, 1.);
    }
  }

  //save histograms and runnumbers
  std::cout <<"saving histograms as root files... " << std::endl;
  std::string histofilename = "results/histograms/histos_vertexvsrunnumber.root";
  TFile histofile(histofilename.c_str(), "RECREATE");
  ofstream runnumberfile("results/histograms/runnumbers.txt");

  std::vector<std::string> names = {"vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"};
  for (auto entry : runhistos){
    runnumberfile << entry.first << std::endl;
    for (int i=0; i<6; i++){
      std::string histoname = "histo_" + std::to_string(entry.first) + names[i];
      histofile.WriteObject(entry.second[i], histoname.c_str());
      delete entry.second[i];
    }
  }

  delete ch;
  delete ppc;
}