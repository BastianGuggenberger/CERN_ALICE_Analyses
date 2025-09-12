#include "fitting.C"

void plot_vertex_vs_dca(){
  //loop over histograms
  std::string histofilename = "histograms/histos_vertexvsdca.root";
  TFile histofile(histofilename.c_str(), "READ");

  std::vector<std::string> histonames = {"histo_vt_k0band", "histo_vt_nonk0band", "histo_vz_k0band", "histo_vz_nonk0band"};
  ofstream mean_width_file("dcaplots/meanvswidth.csv");

  for(std::string histoname : histonames){

    //get right fitting id
    int id;
    if ((histoname == "histo_vt_k0band")||(histoname == "histo_vt_nonk0band")) id = 3;
    if ((histoname == "histo_vz_k0band")||(histoname == "histo_vz_nonk0band")) id = 2;

    //load histo
    TH2D* histo = static_cast<TH2D*>(histofile.Get(histoname.c_str()));

    //plot histo
    TCanvas *cv = new TCanvas();
    histo->Draw("COLZ");
    std::string pngname = histoname + ".png";
    cv->SaveAs(pngname.c_str());

    //further analysis

    std::vector<TH1D*> dca_specific_histos;
    double ymin;
    double ymax;
    for(int i=0; i<10; i++){
      ymin = 0.0 + i*0.025;
      ymax = 0.025 + i*0.025;

      int bin_ymin = histo->GetYaxis()->FindBin(ymin);
      int bin_ymax = histo->GetYaxis()->FindBin(ymax);

      TH1D* dca_specific_histo = histo->ProjectionX("hpx",bin_ymin,bin_ymax);

      TCanvas *cv = new TCanvas();
      dca_specific_histo->Draw();

      dca_specific_histos.push_back(dca_specific_histo);
      std::string fitpngname = "vertexvsdca/" + histoname + "_" + std::to_string(i) + ".png";
      std::array<double, 2> meanvswidth = get_mean_width(dca_specific_histo, id, fitpngname);
      
      mean_width_file << histoname << "," << i << "," << meanvswidth[0] << "," << meanvswidth[1] << std::endl; 
    
    }
  }
}