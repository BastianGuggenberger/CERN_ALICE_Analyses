//Fills Histograms of pp Events with mixed events
#include <set>
#include "../../resources/ppHelpers/ppHelpers.h"
#include "../../resources/ppPaths.h"

//potential tuples:
std::vector<std::vector<int>> goodtuples_3 = {{1,2},{1,3},{2,3}};
std::vector<std::vector<int>> goodtuples_4 = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};

//returns all track-tuples that are approved by the pph.isGoodTuple function
//also, each track can only appear in one tuple
std::vector<std::vector<int>> getgoodtuples(ppHelpers& pph,ppConfiguration* ppc,std::vector<std::vector<float>>& masses_vec){
 
  //prepare goodtuples vector vector
  std::vector<std::vector<int>> goodtuples_3 = {{1,2},{1,3},{2,3}};
  std::vector<std::vector<int>> goodtuples_4 = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
  std::vector<std::vector<int>> goodtuples;

  //prepare masses vector
  if(NumContrib==3){
    goodtuples = goodtuples_3;
    for(int i=0; i<3; i++){
      std::vector<float> masses;
      masses_vec.push_back(masses);
    }
  }
  if(NumContrib==4){
    goodtuples = goodtuples_4;
    for(int i=0; i<6; i++){
      std::vector<float> masses;
      masses_vec.push_back(masses);
    }
  }

  //check if the tracks are compatible with PID hypothesis
  int i=0;
  while(i<goodtuples.size()){
    if (!pph.isGoodTuple(ppc, masses_vec[i], true, goodtuples[i])){
      goodtuples.erase(goodtuples.begin()+i);
      masses_vec.erase(masses_vec.begin()+i);
    }else{
      i++;
    }
  }

  //check if tracks are present in more than 1 tuple
  i=0;
  std::set<int> tracks;
  bool unique;
  while(i<goodtuples.size()){

    //check if at least one of the two tracks is not unique
    unique = true;
    for(int j=0;j<2;j++){
      if(tracks.find(goodtuples[i][j])!=tracks.end()){
        //track not unique
        unique = false;
        goodtuples.erase(goodtuples.begin()+i);
        masses_vec.erase(masses_vec.begin()+i);
        continue;
      }
    }
    
    if(unique){
      //both tracks unique, insert tracks in set, move on to next tuple
      tracks.insert(goodtuples[i][0]);
      tracks.insert(goodtuples[i][1]);
      i++;
    }
  }

  return goodtuples;
}




//fills and stores the IVM-histogram of the morethan2tracks-background
void morethan2tracks()
{
  //safe data
  std::string histofilename = "results/histograms/histo_morethan2tracks.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  //get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration("ppConfig_morethan2tracks.json"); //unique json file for 3 or 4 tracks

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
    if (!pph.isGoodEvent(ppc,true)) continue; //only keeps events with 3 or 4 tracks

    //prepare good tuples
    std::vector<std::vector<float>> masses_vec;
    std::vector<std::vector<int>> goodtuples = getgoodtuples(pph,ppc,masses_vec);
    
    for(int i=0; i<goodtuples.size(); i++){

      //calculate ivm
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int j=0; j<2; j++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[goodtuples[i][j]], TrkPy[goodtuples[i][j]], TrkPz[goodtuples[i][j]], masses_vec[i][j]); //masses_vec[i] only has 2 entries!
      }

      //ULS and LS
      if (TrkSign[goodtuples[i][0]]*TrkSign[goodtuples[i][1]] > 0) {

        //fill histograms
        IVMhisto->Fill(ivm.M(), 1.);

        for(int j=0; j<2; j++){
          auto v = ROOT::Math::XYZVector(TrkPx[goodtuples[i][j]], TrkPy[goodtuples[i][j]], TrkPz[goodtuples[i][j]]);
          hs2d[0]->Fill(v.Rho(), ivm.M(), 1.);
          hs2d[2]->Fill(v.eta(), ivm.M(), 1.);
        }
        auto v_0 = TVector3(TrkPx[goodtuples[i][0]], TrkPy[goodtuples[i][0]], TrkPz[goodtuples[i][0]]);
        auto v_1 = TVector3(TrkPx[goodtuples[i][1]], TrkPy[goodtuples[i][1]], TrkPz[goodtuples[i][1]]);
        double theta = v_0.Angle(v_1);
        hs2d[1]->Fill(theta, ivm.M(), 1.);
        hs2d[3]->Fill(sqrt(1.+cos(theta)),ivm.M(),1.);

      } 
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