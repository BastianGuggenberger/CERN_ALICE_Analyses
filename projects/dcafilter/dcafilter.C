//Fills Histograms of pp Events with kaon events filtered out by restricting the dca
//Version 2

#include "../resources/ppHelpers/ppHelpers.h"
#include "../resources/ppPaths.h"

//accsettings[0]=alpha
//accsettings[1]=acc_xy
//accsettings[2]=acc_z

void dcafilter(std::string folder, std::vector<std::array<double,3>> accsettings)
{
  //variables:
  bool goodtuple;
  double ivm_mass;
  double trksgnprdct;

  // get helpers and configuration
  TH1::AddDirectory(kFALSE);
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(path_json);

  // create a TChain
  auto ch = pph.getChain(path_rootfiles);

  //prepare histograms and masses
  int n_settings = accsettings.size();
  std::vector<TH1D*> histograms;
  for(int i=0; i<n_settings; i++){
    TH1D* newhisto = pph.getIVMhisto();
    histograms.push_back(newhisto);
  }
  std::vector<float> masses;

  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  for (auto ii = 0; ii<nEvents2Process; ii++)
  {
    ch->GetEntry(ii);
    helpers::coutpercentage(ii,nEvents2Process);

    //do i,j-independent stuff:

    if (!pph.isGoodEvent(ppc)) continue; //does not check for dca yet
    if(!pph.isGoodTuple(ppc, masses, true)) continue;
    if(TrkSign[0]*TrkSign[1] > 0 ) continue;
    
    ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
    for (int ii=0; ii<NumContrib; ii++){
      ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
    }
    ivm_mass = ivm.M();
    
    
    //inner loop: vary acc_xy, acc_z, alpha accordint to accsettings vector
    for(int i=0; i<n_settings; i++){
      // dca filter
      if (!pph.isGoodEvent_dca(accsettings[i][0], accsettings[i][1], accsettings[i][2])) continue;

      //fill histo
      histograms[i]->Fill(ivm_mass, 1.);
    }
  }
    
  for (int i=1; i<n_settings; i++){

    //save histograms
    std::cout <<"saving histograms as root files... " << std::endl;
    std::string histofilename = "results/histograms/"+folder+"histos_alpha"+ std::to_string(accsettings[i][0]) + "_xy" + std::to_string(accsettings[i][1]) + "_z" + std::to_string(accsettings[i][2]) + ".root";
    TFile histofile(histofilename.c_str(), "RECREATE");

    histofile.WriteObject(histograms[i], "ivmhisto");
    delete histograms[i];
    }
    
  delete ch;
  delete ppc;
}

//-----------------------------------------------------------------
//MAIN: vary2d(), varyalpha(), vary_xy_z(), bestsetting()

void vary2d(){
  std::vector<std::array<double,3>> accsettings;

  const double alpha = 1.0;
  
  //loop over settings
  for (int i=1; i<12; i++){
    double acc_xy = 0.0 + 0.0025* i;
    for (int j=1; j<12; j++){
      double acc_z = 0.0 + 0.0025* j;

      //store setting in vector
      std::array<double,3> new_a_a_a = {alpha,acc_xy,acc_z};
      accsettings.push_back(new_a_a_a);
    }
  }
  
  //analyse all settings parallel
  dcafilter("2d/",accsettings);
}


void varyalpha(){
  std::vector<std::array<double,3>> accsettings;
  double acc_xy = 0.015;
  double acc_z = 0.0125;

  double startalpha = 0.3;
  double stepsize = 0.15;
  double n_steps = 14;

  //loop over settings
  for (int i=0; i<n_steps; i++){
    double alpha = startalpha + i*stepsize;

    //store setting in vector
    std::array<double,3> new_a_a_a = {alpha,acc_xy,acc_z};
    accsettings.push_back(new_a_a_a);
  }

  //analyse all settings parallel
  dcafilter("varyalpha/",  accsettings);
}


void vary_xy_z(){
  std::vector<std::array<double,3>> accsettings;
  double alpha = 0.6;

  const double product = 15.0e-03 * 12.5e-03; //surface of ellipse will be kept constant

  double start_acc_xy = 3.0e-03;
  double stepsize = 2.0e-03;
  double n_steps = 15;
  
  //loop over settings
  for (int i=0; i<n_steps; i++){
    double acc_xy = start_acc_xy + i*stepsize;
    double acc_z = product/acc_xy;

    //store setting in vector
    std::array<double,3> new_a_a_a = {alpha,acc_xy,acc_z};
    accsettings.push_back(new_a_a_a);
  }

  //analyse all settings parallel
  dcafilter("varyxyz/",  accsettings);
}

void bestsetting(){
  double alpha = 0.0;
  double acc_xy = 0.0;
  double acc_z = 0.0;

  std::vector<std::array<double,3>> accsettings = {{alpha,acc_xy,acc_z}};
  dcafilter("bestsetting/", accsettings);
}