void compute_Eff_HLT_SF(){

  const int nbins=15;

  double eta_range[nbins+1]={ -2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};

  double Fall11[nbins]       ={0.456, 0.784, 0.800, 0.843, 0.886, 0.919, 0.893, 0.921, 0.892, 0.918, 0.883, 0.847, 0.796, 0.770, 0.443};
  double Fall11_err[nbins]   ={0.001, 0.001, 0.001, 0.001, 0.000, 0.000, 0.001, 0.000, 0.001, 0.000, 0.000, 0.001, 0.001, 0.001, 0.001};

  double incl2011[nbins]     ={0.582, 0.811, 0.844, 0.831, 0.875, 0.904,  0.857, 0.904, 0.859, 0.904, 0.877, 0.828, 0.828, 0.818, 0.589};
  double incl2011_err[nbins] ={0.002, 0.001, 0.001, 0.001, 0.001, 0.001,  0.002, 0.001, 0.002, 0.001, 0.001, 0.001, 0.001, 0.001, 0.002};

  TFile*fout = new TFile("","RECREATE");
  TGraphAsymmErrors /* *SF_HLT_eta_MC, *SF_HLT_eta_DATA,  */*SF_HLT_eta = new TGraphAsymmErrors(nbins);

  for(int i=0;i<nbins; i++){
    // double (eta_range[i]+eta_range[i+1])/2,incl2011[i];
    // double (eta_range[i]+eta_range[i+1])/2,Fall11[i];
    SF_HLT_eta->SetPoint(i,(eta_range[i]+eta_range[i+1])/2,incl2011[i]/Fall11[i]);
    double yerrLow,yerrHigh;
    yerrLow=(incl2011[i]/Fall11[i])*TMath::Hypot(incl2011_err[i],Fall11_err[i]);
    yerrHigh=(incl2011[i]/Fall11[i])*TMath::Hypot(incl2011_err[i],Fall11_err[i]);
    SF_HLT_eta->SetPointError(i,(eta_range[i]-eta_range[i+1])/2,(eta_range[i]+eta_range[i+1])/2,yerrLow,yerrHigh);
    cout << "xDATA= " << (eta_range[i]+eta_range[i+1])/2 << " xMC= " << (eta_range[i]+eta_range[i+1])/2 << " yDATA= " << incl2011[i] << " yMC= " << Fall11[i] << " SF= " << incl2011[i]/Fall11[i] << " - " << yerrLow << " + " << yerrHigh << endl;

  }




}