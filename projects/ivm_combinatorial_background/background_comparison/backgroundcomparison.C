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


//names and descriptions of methods to be compared (names are necessary for paths)
std::vector<std::string> names_folders = {"equalsign","mixedevents","mixedevents_likesign","rotatedtracks","morethan2tracks"};
std::vector<Color_t> colors = {kBlue,kRed,kGreen,kOrange};
std::vector<std::string> labels = {"equal-charge-based background","mixed-events-based background","mixed-events-based background (equal charge)","rotated-tracks-based background","morethan2tracks-based background"};

//----------------------------------------------------------------------------------------

//plotsingle: plots the ivmhistogram and ivm vs pT, eta, theta histograms of a single method
void plotsingle(std::string name_folder, bool dim2 = false, TString fnconfig = "../../resources/ppHelpers/ppConfig.json"){
  
  //load histograms
  std::string histofilename = "../"+name_folder+"/results/histograms/histo_"+name_folder+".root";
  TFile histofile(histofilename.c_str(), "READ");
  std::vector<std::string> histonames = {"IVMhisto","pThisto","thetahisto","etahisto","sqrthisto"};

  for(std::string histoname: histonames){
    //plot histograms
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

    //safe plots
    std::string outputname = "results/solobackgrounds/"+name_folder+"/"+name_folder+"_"+histoname+".png";
    cv->SaveAs(outputname.c_str());
  }
}

//plots all IVM-histograms in the list and the blueprint IVM-histogram
void plotallhistos(TH1D* blueprint, std::vector<TH1D*> histograms){

  //rebin for better visibility
  blueprint->Rebin(4);
  for (auto* h : histograms) h->Rebin(4);

  // prepare canvas
  TCanvas *cv = new TCanvas();

  //plot blueprint
  blueprint->GetXaxis()->SetRangeUser(0.0, 2.0);
  blueprint->GetYaxis()->SetRangeUser(0.0, 30000);
  blueprint->SetLineColor(kBlack);
  blueprint->Draw();

  //plot histograms
  for (int i=0; i<names_folders.size(); i++){
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->SetFillColorAlpha(colors[i], 0.25);
    histograms[i]->Draw("SAME");
  }

  //add legend
  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9); 
  leg->AddEntry(blueprint, "original ivm distribution", "l");
  for (int i=0; i<names_folders.size(); i++){
    leg->AddEntry(histograms[i],labels[i].c_str(),"l");
  }
  leg->Draw();

  //safe plots
  std::string pngname = "results/scaledbackgroundcomparison.png";
  cv->SaveAs(pngname.c_str());

  //again in logscale
  TCanvas *cv2 = new TCanvas("cv2","log");
  gPad->SetLogy();

  blueprint->SetMinimum(0.001);
  blueprint->GetXaxis()->SetRangeUser(0.0, 3.5);
  blueprint->GetYaxis()->SetRangeUser(0.001, 30000);
  blueprint->SetLineColor(kBlack);
  blueprint->Draw();

  for (int i=0; i<names_folders.size(); i++){
    histograms[i]->SetFillColorAlpha(colors[i], 0.25);
    histograms[i]->SetLineColor(colors[i]);
    histograms[i]->Draw("SAME");
  }

  pngname = "results/log_scaledbackgroundcomparison.png";
  leg->Draw();
  cv2->SaveAs(pngname.c_str());
}


//scale all histos in histos2scale list to just fit under the blueprint curve in the [minbin,maxbin] interval
void scale(std::vector<TH1D*> histos2scale, TH1D* blueprint, int minbin, int maxbin, bool downscale){

  int iterations = 0;
  bool repeat = true;
  if(downscale){
    //downscaling algorithm:
    while(repeat){
      //count iterations
      iterations+=1;

      //scale down
      for(TH1D* histo2scale: histos2scale){
        histo2scale->Scale(0.99);
      }

      //find out if repeating is necessary
      repeat = false;
      for(TH1D* histo2scale: histos2scale){
        for (int i = minbin; i<=maxbin; i++){
          if((histo2scale->GetBinContent(i) > blueprint->GetBinContent(i)) && blueprint->GetBinContent(i)>0){repeat=true;}
        }
      }
    }
    std::cout<< "downscaling done, " << iterations << " iterations" <<std::endl;
  }else{
    //upscaling algorithm:
    while(repeat){
      //count iterations
      iterations+=1;

      //scale up
      for(TH1D* histo2scale: histos2scale){
        histo2scale->Scale(1.01);
      }

      //find out if repeating is necessary
      for(TH1D* histo2scale: histos2scale){
        for (int i = minbin; i<=maxbin; i++){
          if(histo2scale->GetBinContent(i) > blueprint->GetBinContent(i)){repeat=false;}
        }
      }
    }
    std::cout<< "upscaling done, " << iterations << " iterations" <<std::endl;
  }
}


//scales the histogram "bck2scale" in a way that its Integral is similar to the Integral of the histogram "bckdefault"
void scale_integral(TH1D* bck2scale, TH1D* bckdefault, int minbin, int maxbin){

  //count iterations
  int iterations = 0;
  if(bck2scale->Integral()>bckdefault->Integral()){

    //downscale as long as necessary:
    while(bck2scale->Integral()>bckdefault->Integral()){
      iterations+=1;
      bck2scale->Scale(0.99);
    }
    std::cout<< "integral-downscaling done, " << iterations << " iterations" <<std::endl;

  }else{

    //upscale as long as necessary:
    while(bck2scale->Integral()<bckdefault->Integral()){
      iterations+=1;
      bck2scale->Scale(1.01);
    }
    std::cout<< "integral-upscaling done, " << iterations << " iterations" <<std::endl;

  }
}

//main function: plot and store the scaled comparison of combinatorial backgrounds
void backgroundcomparison(double comparison_minimum, double comparison_maximum){

  //load unscaled IVM histograms
  std::string histoname = "IVMhisto";
  std::vector<TH1D*> histograms;
  for (int i=0; i<names_folders.size(); i++){
    std::string filename = "../" + names_folders[i] +"/results/histograms/histo_" + names_folders[i] + ".root";
    TFile file(filename.c_str(),"READ");
    histograms.push_back(static_cast<TH1D*>(file.Get(histoname.c_str())));
    histograms[histograms.size()-1]->SetDirectory(nullptr);
  }

  //load unscaled original IVM histogram
  std::string filename = "results/ivmblueprint/histo_ivmblueprint.root";
  TFile blueprintfile(filename.c_str(),"READ");
  TH1D* blueprint = static_cast<TH1D*>(blueprintfile.Get("hs1d"));

  int minbin = blueprint->FindBin(comparison_minimum);
  int maxbin = blueprint->FindBin(comparison_maximum);

  //SCALE HISTOGRAMS:

  //1. scale mixedeventshisto to fit under the blueprint
  bool downscale=false;
  for (int i = minbin; i<=maxbin; i++){
    if(histograms[1]->GetBinContent(i) > blueprint->GetBinContent(i)){
      downscale=true;
    }
  }
  std::vector<TH1D*> histos2scale {histograms[1]};
  scale(histos2scale,blueprint,minbin,maxbin,downscale);

  //2. scale histograms to have similar integrals
  for (int i=0; i<histograms.size(); i++){
    if(i==1) continue; //mixedevents histo is the default histo.
    scale_integral(histograms[i],histograms[1],minbin,maxbin);
  }

  //3. scale histograms to fit under the blueprint
  downscale=false;
  for(TH1D* histogram: histograms){
    for (int i = minbin; i<=maxbin; i++){
      if(histogram->GetBinContent(i) > blueprint->GetBinContent(i)){
        downscale=true;
      }
    }
  }
  scale(histograms,blueprint,minbin,maxbin,downscale);

  //PLOT:
  plotallhistos(blueprint, histograms);
}