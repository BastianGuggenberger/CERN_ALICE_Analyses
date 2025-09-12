#include "fitting.C"

std::vector<int> getrunnumbers(){
  std::vector<int> runnumbers;
  ifstream runnumberfile("histograms/runnumbers.txt");
  std::string line;
  while (getline(runnumberfile,line)){
    runnumbers.push_back(std::stoi(line));
  }
  return runnumbers;
}


void plot_vertex_vs_runnumber_1d(){

  //load and plot histograms

  std::string histofilename = "histograms/histos_vertexvsrunnumber.root";
  TFile histofile(histofilename.c_str(), "READ");

  std::vector<std::string> names = {"vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"};

  for(int runnumber : getrunnumbers()){
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

void plot_vertex_vs_runnumber(){

  std::vector<double> vx;
  std::vector<double> vy;
  std::vector<double> vz;
  std::vector<double> widthx;
  std::vector<double> widthy;
  std::vector<double> widthz;


  //load and analyse histograms

  std::string ofname = "runnumberplots/meanvswidth.csv";
  ofstream meanvswidth_file(ofname);

  std::string histofilename = "histograms/histos_vertexvsrunnumber.root";
  TFile histofile(histofilename.c_str(), "READ");

  std::vector<std::string> names = {"vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"};

  for(int runnumber: getrunnumbers()){
    for(int i=0; i<6; i++){

      std::string histoname = "histo_" + std::to_string(runnumber) + names[i];
      TH1D* histo = static_cast<TH1D*>(histofile.Get(histoname.c_str()));

      std::string fitpngname = "vertexvsrunnumber/" + histoname;
      std::array<double, 2> mean_width = get_mean_width(histo,i%3,fitpngname);

      //meanvswidth_file: i, runnumber, mean, width
      meanvswidth_file << i << "," << runnumber << "," << mean_width[0] << "," << mean_width[1] << std::endl;
      
    }
  }

}