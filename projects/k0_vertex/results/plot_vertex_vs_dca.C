void plot_vertex_vs_dca(){
  //load histograms
  std::string histofilename = "histograms/histos_vertexvsdca.root";
  TFile histofile(histofilename.c_str(), "READ");

  //plot histograms
  std::vector<std::string> histonames = {"histo_vt_k0band", "histo_vt_nonk0band", "histo_vz_k0band", "histo_vz_nonk0band"};

  for(std::string histoname : histonames){
    TCanvas *cv = new TCanvas();
    TH2D* histo = static_cast<TH2D*>(histofile.Get(histoname.c_str()));
    //histo->RebinX(10.);
    //histo->RebinY(10.);
    histo->Draw("COLZ");
    std::string pngname = histoname + ".png";
    cv->SaveAs(pngname.c_str());
  }
}