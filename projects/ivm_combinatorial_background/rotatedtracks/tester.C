void runner()
{
  auto rnd = new TRandomMT64();

  auto h0 = new TH1D("alpha1", "alpha1", 1000, 0., 3.2);
  auto h1 = new TH1D("alpha2", "alpha2", 1000, 0., 3.2);
  auto h2 = new TH1D("alpha1", "alpha1", 1000, 0., 3.2);

  Double_t u, x, y, alpha, val;
  for (Int_t i=0; i<1000000; ++i)
  {
    // random variable u is uniform between 0 and 1
    u = rnd->Uniform();
    h0->Fill(u, 1.);

    // distribution of sqrt(1+cos(alpha)) peaks at large values
    alpha = 2*M_PI * u;
    val = TMath::Sqrt(1.+TMath::Cos(alpha));
    h1->Fill(val, 1.);

    // distribution of sqrt(1+cos(alpha)) is uniform
    x = 2 * u;
    y = x*x - 1;
    alpha = (rnd->Uniform() < 0.5) ? acos(y) : (2*M_PI - acos(y));
    val = TMath::Sqrt(1.+TMath::Cos(alpha));
    h2->Fill(val, 1.);
  }

  // plot distributions
  TCanvas *c = new TCanvas("c", "Three Pads Example", 800, 600);

  TPad *pad0 = new TPad("pad1", "Pad 1", 0.0, 0.66, 1.0, 1.0);
  TPad *pad1 = new TPad("pad2", "Pad 2", 0.0, 0.33, 1.0, 0.66);
  TPad *pad2 = new TPad("pad3", "Pad 3", 0.0, 0.0,  1.0, 0.33);

  pad0->Draw();
  pad1->Draw();
  pad2->Draw();

  pad0->cd(); h0->Draw("HIST");
  pad1->cd(); h1->Draw("HIST");
  pad2->cd(); h2->Draw("HIST");
  
  c->Print("distributions.pdf");
}

