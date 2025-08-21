//compares the histograms built by equalsign.C and mixedevents.C

#include "Riostream.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"
#include "TLegend.h"
#include "TColor.h"
#include <vector>

std::vector<std::string> names = {"equalsign","mixedevents","rotatedtracks"};
std::vector<Color_t> colors = {kBlue,kRed,kGreen};
std::vector<std::string> labels = {"equal-charge-based background","mixed-events-based background","rotated-tracks-based background"};


//----------------------------

//----------------------------

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


void backgroundcomparison(double comparison_minimum,double comparison_maximum){

  //load unscaled histograms

  std::string histoname = "hs1d";
  std::vector<TH1D*> histograms;
  for (std::string name: names){
    std::string filename = "../" + name +"/results/histograms/histo_" + name + ".root";
    TFile file(filename.c_str(),"READ");
    histograms.push_back(static_cast<TH1D*>(file.Get(histoname.c_str())));
    histograms[histograms.size()-1]->SetDirectory(nullptr);
  }
  std::string filename = "../mixedevents/results/histograms/histo_ivmblueprint.root";
  TFile blueprintfile(filename.c_str(),"READ");
  TH1D* blueprint = static_cast<TH1D*>(blueprintfile.Get(histoname.c_str()));

  //scale histograms
  bool repeat = true;
  int minbin = blueprint->FindBin(comparison_minimum);
  int maxbin = blueprint->FindBin(comparison_maximum);

  for (TH1D* background: histograms){
    bool downscale=false;
    for (int i = minbin; i<=maxbin; i++){
      if(background->GetBinContent(i) > blueprint->GetBinContent(i)){
        downscale=true;
      }
    }
    scale(background,blueprint,minbin,maxbin,downscale);
  }

  //safe scaled histos
  for (int i=0; i<names.size(); i++){
    std::string filename = "results/scaledhistograms/histo_scaled" + names[i] + ".root";
    TFile file(filename.c_str(),"RECREATE");
    file.WriteObject(histograms[i],histoname.c_str());
  }

  std::string scaledblueprintname = "results/scaledhistograms/histo_scaledblueprint.root";
  TFile scaledblueprintfile(scaledblueprintname.c_str(), "RECREATE");
  scaledblueprintfile.WriteObject(blueprint, histoname.c_str());


  //PLOT

  // prepare canvas
  TCanvas *cv = new TCanvas();

  blueprint->GetXaxis()->SetRangeUser(0.0, 2.5);
  blueprint->GetYaxis()->SetRangeUser(0.0, 5000);
  blueprint->SetLineColor(kBlack);
  blueprint->Draw();

  for (int i=0; i<names.size(); i++){
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->Draw("SAME");
  }

  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9); 

  leg->AddEntry(blueprint, "original ivm distribution", "l");
  for (int i=0; i<names.size(); i++){
    leg->AddEntry(histograms[i],labels[i].c_str(),"l");
  }
  leg->Draw();

  std::string pngname = "results/scaledbackgroundcomparison.png";
  cv->SaveAs(pngname.c_str());


  TCanvas *cv2 = new TCanvas("cv2","log");
  gPad->SetLogy();

  blueprint->SetMinimum(0.001);
  blueprint->GetXaxis()->SetRangeUser(0.0, 2.5);
  blueprint->GetYaxis()->SetRangeUser(0.001, 5000);
  blueprint->SetLineColor(kBlack);
  blueprint->Draw();

  for (int i=0; i<names.size(); i++){
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->Draw("SAME");
  }

  pngname = "results/log_scaledbackgroundcomparison.png";
  leg->Draw();
  cv2->SaveAs(pngname.c_str());
}
