//script for scaling the background IVM based on the mixed events analysis
//an area is chosen where the background is dominating, to derive the correct scale for the background

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "ppHelpers_mixedevents.h"

void getivmblueprint(TString fnconfig = "ppConfig.json"){

  int nbIVM = 4000;
  float bIVMmin = 0.;
  float bIVMmax = 5.;

  //safe data
  std::string histofilename = "results/histograms/histo_ivmblueprint_short.root";
  TFile histofile(histofilename.c_str(), "RECREATE");

  // get helpers and configuration
  ppHelpers pph;
  ppConfiguration* ppc = new ppConfiguration(fnconfig);

  // create a TChain
  auto ch = pph.getChain("ressources/rootfiles.txt");

  TH1D* hs1d = new TH1D("ULS IVM", ";IVM [GeV/c^{2}];Number of events", nbIVM, bIVMmin, bIVMmax);

  // prepare structures to hold variables as function of run number
  
  // loop over events
  auto nEvents2Process = std::min(ch->GetEntries(), ppc->cc<Long64_t>("nEventsMax"));
  std::vector<float> masses;

  for (auto ii = 0; ii<nEvents2Process; ii+=10){

      if((ii % 10000)==0){
        std::cout << float(ii)/float(nEvents2Process) * 100 << " % " << std::endl;
      }

    ch->GetEntry(ii);
    
    // event selections 
    if (!pph.isGoodEvent(ppc))
    {
      continue;
    }
    
    // check if event has 2 tracks which are compatible with PID hypothesis
    if ( pph.isGoodTuple(ppc, masses, true) )
    {
      ROOT::Math::PxPyPzMVector ivm(0., 0., 0., 0.);
      for (int ii=0; ii<NumContrib; ii++)
      {
        ivm += ROOT::Math::PxPyPzMVector(TrkPx[ii], TrkPy[ii], TrkPz[ii], masses[ii]);
      }
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


void scale(TH1D* background, TH1D* blueprint, int minbin, int maxbin, bool downscale){
  int iterations = 0;
  bool repeat = true;
  if(downscale){
    //downscale
    while(repeat){
      iterations+=1;
      background->Scale(0.99);
      repeat = false;
      for (int i = minbin; i<=maxbin; i++){
        if((background->GetBinContent(i) > blueprint->GetBinContent(i)) && blueprint->GetBinContent(i)>0){repeat=true;}
      }
    }
    std::cout<< "downscaling done, " << iterations << " iterations" <<std::endl;
  }else{
    //upscale
    while(repeat){
      iterations+=1;
      background->Scale(1.01);
      for (int i = minbin; i<=maxbin; i++){
        if(background->GetBinContent(i) > blueprint->GetBinContent(i)){repeat=false;}
      }
    }
    std::cout<< "upscaling done, " << iterations << " iterations" <<std::endl;
  }
}


void scalebackground(double comparison_minimum,double comparison_maximum){

  //load histograms
  std::string backgroundfilename = "results/histograms/histo_mixedevents.root";
  TFile backgroundfile(backgroundfilename.c_str(), "READ");

  std::string blueprintfilename = "results/histograms/histo_ivmblueprint.root";
  TFile blueprintfile(blueprintfilename.c_str(), "READ");

  std::string histoname = "hs1d";
  TH1D* ivmbackground = static_cast<TH1D*>(backgroundfile.Get(histoname.c_str()));
  TH1D* ivmblueprint = static_cast<TH1D*>(blueprintfile.Get(histoname.c_str()));


  //scale
  int bin = ivmblueprint->FindBin(1.0);
  double size_blueprint = ivmblueprint->GetBinContent(bin);
  bin = ivmbackground->FindBin(1.0);
  double size_background = ivmbackground->GetBinContent(bin);

  bool repeat = true;
  int minbin = ivmblueprint->FindBin(comparison_minimum);
  int maxbin = ivmblueprint->FindBin(comparison_maximum);

  bool downscale=false;
  for (int i = minbin; i<=maxbin; i++){
    if(ivmbackground->GetBinContent(i) > ivmblueprint->GetBinContent(i)){
      downscale=true;
    }
  }

  scale(ivmbackground,ivmblueprint,minbin,maxbin,downscale);

  //safe scaled histo(s)
  std::string scaledbackgroundname = "results/histograms/histo_scaledbackground.root";
  TFile scaledbackgroundfile(scaledbackgroundname.c_str(), "RECREATE");
  scaledbackgroundfile.WriteObject(ivmbackground, histoname.c_str());

  std::string scaledblueprintname = "results/histograms/histo_scaledblueprint.root";
  TFile scaledblueprintfile(scaledblueprintname.c_str(), "RECREATE");
  scaledblueprintfile.WriteObject(ivmblueprint, histoname.c_str());
} 