#include "../../resources/ppHelpers/ppHelpers.h"
#include "../../resources/ppPaths.h"

//fills and stores the IVM-histogram of the equalsign-background
void equalsign()
{
  //safe data
  std::string histofilename = "results/histograms/histo_equalsign.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  //get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  //create a TChain
  auto ch = pph.getChain(path_rootfiles);

  //prepare histos and masses
  TH1D* IVMhisto = pph.getIVMhisto();
  std::vector<TH2D*> hs2d = pph.getparamvsivmhistos(); 
  std::vector<float> masses;

  //loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii+=1)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);
    
    //event selections 
    if (!pph.isGoodEvent(ppc)) continue;
    if (!pph.isGoodTuple(ppc, masses, true)) continue; // check if event has 2 tracks which are compatible with PID hypothesis

    //ULS and LS
    if (TrkSign[0]*TrkSign[1] > 0) {
      //fill histograms
      pph.FillHistos(ppc,IVMhisto,hs2d, masses);
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