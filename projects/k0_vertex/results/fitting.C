//Fitting parameters: {{amplitude, mean, sigma} (vx), {amplitude, mean, sigma} (vy), {amplitude, mean, sigma} (vz), {amplitude, mean, sigma} (vt)}
std::vector<std::vector<double>> vStart = {{3800,-0.04,0.05},{3800,0.025,0.05},{50,0.0,3.0},{3800,0.04,0.05}};
std::vector<std::vector<double>> vStepsize = {{1,0.001,0.001},{1,0.001,0.001},{1,0.001,0.1},{1,0.001,0.001}};
std::vector<std::vector<double>> vLow = {{10,-0.045,0.001},{10,0.01,0.001},{1,-3.0,1.0},{1,0.0,0.001}};
std::vector<std::vector<double>> vHigh = {{9000,-0.035,0.01},{9000,0.03,0.01},{2500,3.0,20.0},{22000,0.06,0.01}};

std::vector<double> comparemin = {-0.1, 0.0, -20.0, 0.0};
std::vector<double> comparemax = {-0.0, 0.04, 20.0, 0.1};

std::vector<double> plotmin = {-0.5, 0.0, -30.0, 0.0};
std::vector<double> plotmax = {0.5, 0.1, 30.0, 0.2};


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

  //save parameters for next fitting
  for(int i=0; i<3; i++){
    vStart[id][i] = vs[i];
  }

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