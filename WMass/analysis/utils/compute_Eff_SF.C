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
 
 foutEff->cd();
 
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

