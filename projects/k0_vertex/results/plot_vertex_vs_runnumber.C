
//Fitting parameters: {{amplitude, mean, sigma} (vx), {amplitude, mean, sigma} (vy), {amplitude, mean, sigma} (vz)}
std::vector<std::vector<double>> vStart = {{100,-0.04,0.0005},{100,-0.04,0.0005},{100,-0.04,0.0005}};
std::vector<std::vector<double>> vStepsize = {{1,0.001,0.0001},{1,0.001,0.0001},{1,0.001,0.0001}};
std::vector<std::vector<double>> vLow = {{10,-4.0,100},{10,-4.0,100},{10,-4.0,100}};
std::vector<std::vector<double>> vHigh = {{1000,10.0,10.0},{1000,10.0,10.0},{1000,10.0,10.0}};

std::vector<double> comparemin = {0.0, 0.0, 0.0};
std::vector<double> comparemax = {1000.0, 1000.0, 1000.0};

std::vector<double> plotmin = {0.0, 0.0, 0.0};
std::vector<double> plotmax = {1000.0, 1000.0, 1000.0};



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


std::array<double, 2> get_mean_width(TH1D* histo, int id, std::string filename){
  TF1* gaus = new TF1("fgaus","gaus", comparemin[id], comparemax[id]);

  auto mMinimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

  auto chi = [&](const double* pars) -> double {
    for (int i=0; i<3; i++){
      gaus->SetParameter(i,pars[i]);
    }

    double_t chi = 0;
    for(int i=histo->FindBin(comparemin[id]); i<=histo->FindBin(comparemax[id]); i++){
      auto x = histo->GetBinCenter(i);
      auto y = histo->GetBinContent(i);

      double_t yfit = gaus->Eval(x);

      chi += (y-yfit)*(y-yfit);

    }
    return chi;
  };


  auto mFunctor = ROOT::Math::Functor(chi,3);
  mMinimizer->SetFunction(mFunctor);

  mMinimizer->SetMaxFunctionCalls(1000000);
  mMinimizer->SetTolerance(0.001);
  mMinimizer->SetPrintLevel(3);

  for(int i=0;i<3;i++){
    mMinimizer->SetLimitedVariable(i, Form("p%d", i), vStart[id][i], vStepsize[id][i], vLow[id][i], vHigh[id][i]);
  }

  //historyfile << alpha << "," << vStart[0] << "," << vStart[1] << "," << vStart[2] << "," << vStart[3] << "," << vStart[4] << std::endl;
  mMinimizer->Minimize();
  const double* vs = mMinimizer->X();
  auto chimin = mMinimizer->MinValue();

  TCanvas *c1 = new TCanvas("c1", "Plot", 800, 600);
 
  histo->GetXaxis()->SetRangeUser(plotmin[id],plotmax[id]);
  histo->SetLineColor(kBlack);
  histo->Draw();

  gaus->SetLineColor(kGreen);
  gaus->SetLineWidth(2);
  gaus->Draw("SAME");

  std::string pngname = "fitpngs/"+filename+".png";
  c1->SaveAs(pngname.c_str());

  std::array<double, 2> mean_width = {vs[1],vs[2]};
  return mean_width;
}

void plot_vertex_vs_runnumber(){

  std::vector<double> vx;
  std::vector<double> vy;
  std::vector<double> vz;
  std::vector<double> widthx;
  std::vector<double> widthy;
  std::vector<double> widthz;


  //load and analyse histograms

  std::string histofilename = "histograms/histos_vertexvsrunnumber.root";
  TFile histofile(histofilename.c_str(), "READ");

  std::vector<std::string> names = {"vx_k0band","vy_k0band","vz_k0band","vx_nonk0band","vy_nonk0band","vz_nonk0band"};

  for(int runnumber: getrunnumbers()){
    for(int i=0; i<6; i++){
      std::string histoname = "histo_" + std::to_string(runnumber) + names[i];
      TH1D* histo = static_cast<TH1D*>(histofile.Get(histoname.c_str()));

      std::array<double, 2> mean_width = get_mean_width(histo,i%3,histoname);
      std::cout << mean_width[0] << " , " << mean_width[1] << std::endl;
      
    }
  }

}