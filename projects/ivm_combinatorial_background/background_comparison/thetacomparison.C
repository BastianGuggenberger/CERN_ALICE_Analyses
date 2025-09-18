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

void thetacomparison(TString fnconfig = "../../resources/ppHelpers/ppConfig.json"){
  
  std::vector<std::string> folders = {"mixedevents","equalsign","rotatedtracks","morethan2tracks"};
  for (std::string name_folder : folders){
    //load histograms
    std::string histofilename = "../"+name_folder+"/results/histograms/histo_"+name_folder+".root";
    TFile histofile(histofilename.c_str(), "READ");

        //plot histograms
        gROOT->SetBatch(kTRUE);
        gStyle->SetOptStat(0);
        gStyle->SetLineWidth(2);
        gStyle->SetLabelSize(0.040,"XY");
        gStyle->SetTitleSize(0.045,"XY");
        gStyle->SetNumberContours(255);

        TCanvas *cv = new TCanvas("cv","",2400,1800);

        TH2D* histo = static_cast<TH2D*>(histofile.Get("thetahisto"));
        TH1D *histo1d = histo->ProjectionX("theta");

        histo1d->Draw();

        //safe plots
        std::string outputname = "results/solobackgrounds/theta1d/"+name_folder+"_"+"theta1d"+".png";
        cv->SaveAs(outputname.c_str());

}
}