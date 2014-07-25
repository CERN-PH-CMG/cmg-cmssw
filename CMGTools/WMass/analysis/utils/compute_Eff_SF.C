void compute_Eff_SF(){

  TFile*finEff = new TFile("MuonEfficiencies2011_44X_DataMC.root");
  TFile*foutEff = new TFile("MuonEfficiencies_SF_2011_44X_DataMC.root","RECREATE");

  TGraphAsymmErrors *SF_MuID_eta_MC[2],*SF_Iso_eta_MC[2],*SF_Iso_vtx_MC[2];
  TGraphAsymmErrors *SF_MuID_eta_DATA[2],*SF_Iso_eta_DATA[2],*SF_Iso_vtx_DATA[2];

  // ===============================================
  SF_MuID_eta_MC[0] = (TGraphAsymmErrors *) finEff->Get("MC_TIGHT_nL8_2011A_eta__pt>20");
  SF_MuID_eta_MC[1] = (TGraphAsymmErrors *) finEff->Get("MC_TIGHT_nL8_2011B_eta__pt>20");

  SF_Iso_eta_MC[0] = (TGraphAsymmErrors *) finEff->Get("MC_combRelPFISO12_2011A_eta__pt>20");
  SF_Iso_eta_MC[1] = (TGraphAsymmErrors *) finEff->Get("MC_combRelPFISO12_2011B_eta__pt>20");

  SF_Iso_vtx_MC[0] = (TGraphAsymmErrors *) finEff->Get("MC_combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
  SF_Iso_vtx_MC[1] = (TGraphAsymmErrors *) finEff->Get("MC_combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");

  // ===============================================

  SF_MuID_eta_DATA[0] = (TGraphAsymmErrors *) finEff->Get("DATA_TIGHT_nL8_2011A_eta__pt>20");
  SF_MuID_eta_DATA[1] = (TGraphAsymmErrors *) finEff->Get("DATA_TIGHT_nL8_2011B_eta__pt>20");

  SF_Iso_eta_DATA[0] = (TGraphAsymmErrors *) finEff->Get("DATA_combRelPFISO12_2011A_eta__pt>20");
  SF_Iso_eta_DATA[1] = (TGraphAsymmErrors *) finEff->Get("DATA_combRelPFISO12_2011B_eta__pt>20");

  SF_Iso_vtx_DATA[0] = (TGraphAsymmErrors *) finEff->Get("DATA_combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
  SF_Iso_vtx_DATA[1] = (TGraphAsymmErrors *) finEff->Get("DATA_combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");

  // ===============================================

  TGraphAsymmErrors *SF_MuID_eta[2]; 
  SF_MuID_eta[0]=new TGraphAsymmErrors(SF_MuID_eta_MC[0]->GetN());
  SF_MuID_eta[0]->SetName("SF_TIGHT_nL8_2011A_eta__pt>20"); SF_MuID_eta[0]->SetTitle("SF_TIGHT_nL8_2011A_eta__pt>20");
  SF_MuID_eta[1]=new TGraphAsymmErrors(SF_MuID_eta_MC[1]->GetN());
  SF_MuID_eta[1]->SetName("SF_TIGHT_nL8_2011B_eta__pt>20"); SF_MuID_eta[1]->SetTitle("SF_TIGHT_nL8_2011B_eta__pt>20");
  
  TGraphAsymmErrors *SF_Iso_eta[2];
  SF_Iso_eta[0]=new TGraphAsymmErrors(SF_Iso_eta_MC[0]->GetN());
  SF_Iso_eta[0]->SetName("combRelPFISO12_2011A_eta__pt>20"); SF_Iso_eta[0]->SetTitle("combRelPFISO12_2011A_eta__pt>20");
  SF_Iso_eta[1]=new TGraphAsymmErrors(SF_Iso_eta_MC[1]->GetN());
  SF_Iso_eta[1]->SetName("combRelPFISO12_2011B_eta__pt>20"); SF_Iso_eta[1]->SetTitle("combRelPFISO12_2011B_eta__pt>20");
  TGraphAsymmErrors *SF_Iso_vtx[2];
  SF_Iso_vtx[0]=new TGraphAsymmErrors(SF_Iso_vtx_MC[0]->GetN());
  SF_Iso_vtx[0]->SetName("combRelPFISO12_2011A_vtx__pt>20_abseta<2.4"); SF_Iso_vtx[0]->SetTitle("combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
  SF_Iso_vtx[1]=new TGraphAsymmErrors(SF_Iso_vtx_MC[1]->GetN());
  SF_Iso_vtx[1]->SetName("combRelPFISO12_2011B_vtx__pt>20_abseta<2.4"); SF_Iso_vtx[1]->SetTitle("combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");
  
  // ===============================================

  cout << SF_MuID_eta[0]->GetName() << endl;
  for(int i=0; i<SF_MuID_eta_DATA[0]->GetN(); i++){
    double xDATA,yDATA;
    SF_MuID_eta_DATA[0]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_MuID_eta_MC[0]->GetPoint(i,xMC,yMC);
    SF_MuID_eta[0]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_MuID_eta_DATA[0]->GetErrorYlow(i),SF_MuID_eta_MC[0]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_MuID_eta_DATA[0]->GetErrorYhigh(i),SF_MuID_eta_MC[0]->GetErrorYhigh(i));
    SF_MuID_eta[0]->SetPointError(i,SF_MuID_eta_DATA[0]->GetErrorXlow(i),SF_MuID_eta_DATA[0]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
  cout << SF_MuID_eta[1]->GetName() << endl;
  for(int i=0; i<SF_MuID_eta_DATA[1]->GetN(); i++){
    double xDATA,yDATA;
    SF_MuID_eta_DATA[1]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_MuID_eta_MC[1]->GetPoint(i,xMC,yMC);
    SF_MuID_eta[1]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_MuID_eta_DATA[1]->GetErrorYlow(i),SF_MuID_eta_MC[1]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_MuID_eta_DATA[1]->GetErrorYhigh(i),SF_MuID_eta_MC[1]->GetErrorYhigh(i));
    SF_MuID_eta[1]->SetPointError(i,SF_MuID_eta_DATA[1]->GetErrorXlow(i),SF_MuID_eta_DATA[1]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
 
  cout << SF_Iso_eta[0]->GetName() << endl;
  for(int i=0; i<SF_Iso_eta_DATA[0]->GetN(); i++){
    double xDATA,yDATA;
    SF_Iso_eta_DATA[0]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_Iso_eta_MC[0]->GetPoint(i,xMC,yMC);
    SF_Iso_eta[0]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_Iso_eta_DATA[0]->GetErrorYlow(i),SF_Iso_eta_MC[0]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_Iso_eta_DATA[0]->GetErrorYhigh(i),SF_Iso_eta_MC[0]->GetErrorYhigh(i));
    SF_Iso_eta[0]->SetPointError(i,SF_Iso_eta_DATA[0]->GetErrorXlow(i),SF_Iso_eta_DATA[0]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
  cout << SF_Iso_eta[1]->GetName() << endl;
  for(int i=0; i<SF_Iso_eta_DATA[1]->GetN(); i++){
    double xDATA,yDATA;
    SF_Iso_eta_DATA[1]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_Iso_eta_MC[1]->GetPoint(i,xMC,yMC);
    SF_Iso_eta[1]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_Iso_eta_DATA[1]->GetErrorYlow(i),SF_Iso_eta_MC[1]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_Iso_eta_DATA[1]->GetErrorYhigh(i),SF_Iso_eta_MC[1]->GetErrorYhigh(i));
    SF_Iso_eta[1]->SetPointError(i,SF_Iso_eta_DATA[1]->GetErrorXlow(i),SF_Iso_eta_DATA[1]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
 
  cout << SF_Iso_vtx[0]->GetName() << endl;
  for(int i=0; i<SF_Iso_vtx_DATA[0]->GetN(); i++){
    double xDATA,yDATA;
    SF_Iso_vtx_DATA[0]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_Iso_vtx_MC[0]->GetPoint(i,xMC,yMC);
    SF_Iso_vtx[0]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_Iso_vtx_DATA[0]->GetErrorYlow(i),SF_Iso_vtx_MC[0]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_Iso_vtx_DATA[0]->GetErrorYhigh(i),SF_Iso_vtx_MC[0]->GetErrorYhigh(i));
    SF_Iso_vtx[0]->SetPointError(i,SF_Iso_vtx_DATA[0]->GetErrorXlow(i),SF_Iso_vtx_DATA[0]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
  cout << SF_Iso_vtx[1]->GetName() << endl;
  for(int i=0; i<SF_Iso_vtx_DATA[1]->GetN(); i++){
    double xDATA,yDATA;
    SF_Iso_vtx_DATA[1]->GetPoint(i,xDATA,yDATA);
    double xMC,yMC;
    SF_Iso_vtx_MC[1]->GetPoint(i,xMC,yMC);
    SF_Iso_vtx[1]->SetPoint(i,xDATA,yDATA/yMC);
    double yerrLow,yerrHigh;
    yerrLow=(yDATA/yMC)*TMath::Hypot(SF_Iso_vtx_DATA[1]->GetErrorYlow(i),SF_Iso_vtx_MC[1]->GetErrorYlow(i));
    yerrHigh=(yDATA/yMC)*TMath::Hypot(SF_Iso_vtx_DATA[1]->GetErrorYhigh(i),SF_Iso_vtx_MC[1]->GetErrorYhigh(i));
    SF_Iso_vtx[1]->SetPointError(i,SF_Iso_vtx_DATA[1]->GetErrorXlow(i),SF_Iso_vtx_DATA[1]->GetErrorXhigh(i),yerrLow,yerrHigh);
    cout << "xDATA= " << xDATA << " xMC= " << xMC << " yDATA= " << yDATA << " yMC= " << yMC << " SF= " << yDATA/yMC << " - " << yerrLow << " + " << yerrHigh << endl;
  }
 
 
  const int nbins=15;

  double eta_range[nbins+1]={ -2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};

  double Fall11[nbins]       ={0.456, 0.784, 0.800, 0.843, 0.886, 0.919, 0.893, 0.921, 0.892, 0.918, 0.883, 0.847, 0.796, 0.770, 0.443};
  double Fall11_err[nbins]   ={0.001, 0.001, 0.001, 0.001, 0.000, 0.000, 0.001, 0.000, 0.001, 0.000, 0.000, 0.001, 0.001, 0.001, 0.001};

  double incl2011[nbins]     ={0.582, 0.811, 0.844, 0.831, 0.875, 0.904,  0.857, 0.904, 0.859, 0.904, 0.877, 0.828, 0.828, 0.818, 0.589};
  double incl2011_err[nbins] ={0.002, 0.001, 0.001, 0.001, 0.001, 0.001,  0.002, 0.001, 0.002, 0.001, 0.001, 0.001, 0.001, 0.001, 0.002};

  TGraphAsymmErrors /* *SF_HLT_eta_MC, *SF_HLT_eta_DATA,  */*SF_HLT_eta = new TGraphAsymmErrors(nbins);
  SF_HLT_eta->SetName("SF_HLT_MuIso24_2011_eta__pt>30"); SF_HLT_eta->SetTitle("SF_HLT_MuIso24_2011_eta__pt>30");
  cout << SF_HLT_eta->GetTitle() << endl;
  for(int i=0;i<nbins; i++){
    // double (eta_range[i]+eta_range[i+1])/2,incl2011[i];
    // double (eta_range[i]+eta_range[i+1])/2,Fall11[i];
    SF_HLT_eta->SetPoint(i,(eta_range[i]+eta_range[i+1])/2,incl2011[i]/Fall11[i]);
    double yerrLow,yerrHigh;
    yerrLow=(incl2011[i]/Fall11[i])*TMath::Hypot(incl2011_err[i],Fall11_err[i]);
    yerrHigh=(incl2011[i]/Fall11[i])*TMath::Hypot(incl2011_err[i],Fall11_err[i]);
    SF_HLT_eta->SetPointError(i,(eta_range[i+1]-eta_range[i])/2,(eta_range[i+1]-eta_range[i])/2,yerrLow,yerrHigh);
    cout << "xDATA= " << (eta_range[i]+eta_range[i+1])/2 << " xMC= " << (eta_range[i]+eta_range[i+1])/2 << " xerr= " << (eta_range[i+1]-eta_range[i])/2 << " yDATA= " << incl2011[i] << " yMC= " << Fall11[i] << " SF= " << incl2011[i]/Fall11[i] << " - " << yerrLow << " + " << yerrHigh << endl;

  }
 
 
 
 foutEff->cd();
 
 SF_HLT_eta->Write();
 SF_MuID_eta[0]->Write();
 SF_MuID_eta[1]->Write();
 SF_Iso_eta[0]->Write();
 SF_Iso_eta[1]->Write();
 SF_Iso_vtx[0]->Write();
 SF_Iso_vtx[1]->Write();
 
 foutEff->Write();
 foutEff->Close();
 
}

// void TGraphAsymmRatio(TGraphAsymmErrors *hDATA, TGraphAsymmErrors *MC, TGraphAsymmErrors *SF){
// }

