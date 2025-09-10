void plot_vertex_vs_runnumber_1d(){

  //get runnumbers
  std::vector<int> runnumbers;
  ifstream runnumberfile("histograms/runnumbers.txt");
  std::string line;
  while (getline(runnumberfile,line)){
    runnumbers.push_back(std::stoi(line));
  }

  //load and plot histograms

  std::string histofilename = "histograms/histos_vertexvsrunnumber.root";
  TFile histofile(histofilename.c_str(), "READ");

  std::vector<std::string> names = {"vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"};

  for(int runnumber:runnumbers){
    for(int i=0; i<6; i++){
      std::string histoname = "histo_" + std::to_string(runnumber) + names[i];
      TCanvas *cv = new TCanvas();
      TH1D* histo = static_cast<TH1D*>(histofile.Get(histoname.c_str()));

      histo->Draw();
      std::string pngname = "runnumberplots/1dhistos/" + histoname + ".png";
      cv->SaveAs(pngname.c_str());
    }
  }
}