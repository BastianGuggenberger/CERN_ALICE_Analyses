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

std::vector<std::string> names_folders = {"equalsign","mixedevents","rotatedtracks","morethan2tracks"};
std::vector<std::string> names_files = {"equalsign","mixedevents","rotatedtracks_uniformsqrt","morethan2tracks"};
std::vector<Color_t> colors = {kBlue,kRed,kGreen,kOrange};
std::vector<std::string> labels = {"equal-charge-based background","mixed-events-based background","rotated-tracks-based background","morethan2tracks-based background"};


//----------------------------

//----------------------------

void plot(std::string name_folder, std::string name_file, bool dim2 = false, TString fnconfig = "../../resources/ppHelpers/ppConfig.json"){
  //load histogram
  std::string histofilename = "../"+name_folder+"/results/histograms/histo_"+name_file+".root";
  TFile histofile(histofilename.c_str(), "READ");

  //PLOT
  std::string histoname = "hs1d";
  TCanvas *cv = new TCanvas();
  if(dim2){
    TH2D* histo = static_cast<TH2D*>(histofile.Get(histoname.c_str()));
    histo->RebinX(10.);
    histo->RebinY(20.);
    histo->Draw("COLZ");
  }else{
    TH1D* histo = static_cast<TH1D*>(histofile.Get(histoname.c_str()));
    histo->Draw();
  }
  std::string outputname = "results/solobackgrounds/"+name_file+".png";
  cv->SaveAs(outputname.c_str());

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


void backgroundcomparison(double comparison_minimum,double comparison_maximum){

  //load unscaled histograms

  std::string histoname = "hs1d";
  std::vector<TH1D*> histograms;
  for (int i=0; i<names_folders.size(); i++){
    std::string filename = "../" + names_folders[i] +"/results/histograms/histo_" + names_files[i] + ".root";
    TFile file(filename.c_str(),"READ");
    histograms.push_back(static_cast<TH1D*>(file.Get(histoname.c_str())));
    histograms[histograms.size()-1]->SetDirectory(nullptr);
  }
  std::string filename = "results/ivmblueprint/histo_ivmblueprint.root";
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
  for (int i=0; i<names_folders.size(); i++){
    std::string filename = "results/scaledhistograms/histo_scaled" + names_files[i] + ".root";
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

  for (int i=0; i<names_files.size(); i++){
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->Draw("SAME");
  }

  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9); 

  leg->AddEntry(blueprint, "original ivm distribution", "l");
  for (int i=0; i<names_folders.size(); i++){
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

  for (int i=0; i<names_files.size(); i++){
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->Draw("SAME");
  }

  pngname = "results/log_scaledbackgroundcomparison.png";
  leg->Draw();
  cv2->SaveAs(pngname.c_str());
}

