//Fills Histograms of pp Events with mixed events
#include "../../resources/ppPaths.h"
#include "../../resources/ppHelpers/ppHelpers.h"

//fills and stores the IVM-histogram of the mixedevents-background
void mixedevents()
{
  //safe data
  std::string histofilename = "results/histograms/histo_mixedevents.root";
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

  //prepare archives for event combinations
  std::vector<ROOT::Math::PxPyPzMVector> pxpypzm_archive;
  std::vector<double> trksign_archive;

  //loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii+=100)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);
    
    // event selections 
    if (!pph.isGoodEvent(ppc)) continue;
    if (!pph.isGoodTuple(ppc,masses,true)) continue; // check if event has 2 tracks which are compatible with PID hypothesis

    //combine Track 0 with all possible Tracks 1 from the archive(previous events):

    //1. prepare Track 0
    double trksign = TrkSign[0];
    ROOT::Math::PxPyPzMVector ivm_0(TrkPx[0], TrkPy[0], TrkPz[0], masses[0]);
    ROOT::Math::PxPyPzMVector ivm;

    //2. loop over previous Tracks 1 in the archive
    for (int ii=0; ii<pxpypzm_archive.size(); ii++)
      {
        //calculate ivm
        ivm = ivm_0+pxpypzm_archive[ii];

        // ULS and LS
        if (trksign*trksign_archive[ii] < 0) {

          //fill histograms

          IVMhisto->Fill(ivm.M(), 1.);

          for(int j=0; j<2; j++){
          }
          auto v_0 = ROOT::Math::XYZVector(TrkPx[0], TrkPy[0], TrkPz[0]);
          hs2d[0]->Fill(v_0.Rho(), ivm.M(), 1.);
          hs2d[2]->Fill(v_0.eta(), ivm.M(), 1.);

          auto v_1 = ROOT::Math::XYZVector(pxpypzm_archive[ii].Px(),pxpypzm_archive[ii].Py(),pxpypzm_archive[ii].Pz());
          hs2d[0]->Fill(v_1.Rho(), ivm.M(), 1.);
          hs2d[2]->Fill(v_1.eta(), ivm.M(), 1.);

          auto v_0_Tvec = TVector3(TrkPx[0], TrkPy[0], TrkPz[0]);
          auto v_1_Tvec = TVector3(pxpypzm_archive[ii].Px(),pxpypzm_archive[ii].Py(),pxpypzm_archive[ii].Pz());
          double theta = v_0_Tvec.Angle(v_1_Tvec);
          hs2d[1]->Fill(theta, ivm.M(), 1.);
          hs2d[3]->Fill(sqrt(1.+cos(theta)),ivm.M(),1.);
        }
      }

    //3. store Track 1 in the archive
    pxpypzm_archive.push_back(ROOT::Math::PxPyPzMVector(TrkPx[1], TrkPy[1], TrkPz[1], masses[1]));
    trksign_archive.push_back(TrkSign[1]);
  }

  //safe histograms
  histofile.WriteObject(IVMhisto,"IVMhisto");
  histofile.WriteObject(hs2d[0],"pThisto");
  histofile.WriteObject(hs2d[1],"thetahisto");
  histofile.WriteObject(hs2d[2],"etahisto");
  histofile.WriteObject(hs2d[3],"sqrthisto");

  delete ch;
}