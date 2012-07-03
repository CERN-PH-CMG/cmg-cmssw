
void compareDataCard(){
  
  TFile F1("/afs/cern.ch/user/b/bianchi/public/Roger/datacards2011v3/muTauSM.root","read");
  TFile F2("/afs/cern.ch/user/b/benitezj/public/datacards/June17/muTauSM_JoseJune17_tScale.root","read");
  
  F1.ls();
  F2.ls();
  

  TCanvas C("compareDataCard");

  
  C.Print("compareDataCard.ps[");
  
  //VBF----------------------------------------------------------------
  TH1F* H1 = (TH1F*) F1.Get("muTau_vbf/qqH125");
  TH1F* H2 = (TH1F*) F2.Get("muTau_vbf/qqH125");

  TH1F* HZTT1 = (TH1F*) F1.Get("muTau_vbf/ZTT");
  TH1F* HZTT2 = (TH1F*) F2.Get("muTau_vbf/ZTT");

  TH1F* HQCD1 = (TH1F*) F1.Get("muTau_vbf/QCD");
  TH1F* HQCD2 = (TH1F*) F2.Get("muTau_vbf/QCD");

  TH1F* HW1 = (TH1F*) F1.Get("muTau_vbf/W");
  TH1F* HW2 = (TH1F*) F2.Get("muTau_vbf/W");

  TH1F* HD1 = (TH1F*) F1.Get("muTau_vbf/data_obs");
  TH1F* HD2 = (TH1F*) F2.Get("muTau_vbf/data_obs");
  
  C.Clear();
  H1->GetYaxis()->SetRangeUser(0,H1->GetMaximum()*1.2);
  H1->Draw("hist");
  cout<<"H1 : "<<H1->Integral()<<" mean : "<<H1->GetMean()<<"  rms : "<<H1->GetRMS()<<endl;
  H2->Draw("histpesame");
  cout<<"H2 : "<<H2->Integral()<<" mean : "<<H2->GetMean()<<"  rms : "<<H2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  HZTT1->GetYaxis()->SetRangeUser(0,HZTT1->GetMaximum()*1.2);
  HZTT1->Draw("hist");
  cout<<"HZTT1 : "<<HZTT1->Integral()<<" mean : "<<HZTT1->GetMean()<<"  rms : "<<HZTT1->GetRMS()<<endl;
  HZTT2->Draw("histpesame");
  cout<<"HZTT2 : "<<HZTT2->Integral()<<" mean : "<<HZTT2->GetMean()<<"  rms : "<<HZTT2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  HQCD1->GetYaxis()->SetRangeUser(0,HQCD1->GetMaximum()*1.2);
  HQCD1->Draw("hist");
  cout<<"HQCD1 : "<<HQCD1->Integral()<<" mean : "<<HQCD1->GetMean()<<"  rms : "<<HQCD1->GetRMS()<<endl;
  HQCD2->Draw("histpesame");
  cout<<"HQCD2 : "<<HQCD2->Integral()<<" mean : "<<HQCD2->GetMean()<<"  rms : "<<HQCD2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  HW1->GetYaxis()->SetRangeUser(0,HW1->GetMaximum()*1.2);
  HW1->Draw("hist");
  cout<<"HW1 : "<<HW1->Integral()<<" mean : "<<HW1->GetMean()<<"  rms : "<<HW1->GetRMS()<<endl;
  HW2->Draw("histpesame");
  cout<<"HW2 : "<<HW2->Integral()<<" mean : "<<HW2->GetMean()<<"  rms : "<<HW2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");


  C.Clear();
  HD1->Draw("hist");
  HD2->Draw("histpesame");
  C.Print("compareDataCard.ps");


  //Boosted High----------------------------------------------------------------
  TH1F* HBoostHigh1 = (TH1F*) F1.Get("muTau_boost_high/qqH125");
  TH1F* HBoostHigh2 = (TH1F*) F2.Get("muTau_boost_high/qqH125");

  TH1F* HBoostHighZTT1 = (TH1F*) F1.Get("muTau_boost_high/ZTT");
  TH1F* HBoostHighZTT2 = (TH1F*) F2.Get("muTau_boost_high/ZTT");

  TH1F* HBoostHighD1 = (TH1F*) F1.Get("muTau_boost_high/data_obs");
  TH1F* HBoostHighD2 = (TH1F*) F2.Get("muTau_boost_high/data_obs");
  
  C.Clear();
  HBoostHigh1->GetYaxis()->SetRangeUser(0,HBoostHigh1->GetMaximum()*1.2);
  HBoostHigh1->Draw("hist");
  cout<<"HBoostHigh1 : "<<HBoostHigh1->Integral()<<" mean : "<<HBoostHigh1->GetMean()<<"  rms : "<<HBoostHigh1->GetRMS()<<endl;
  HBoostHigh2->Draw("histpesame");
  cout<<"HBoostHigh2 : "<<HBoostHigh2->Integral()<<" mean : "<<HBoostHigh2->GetMean()<<"  rms : "<<HBoostHigh2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  HBoostHighZTT1->GetYaxis()->SetRangeUser(0,HBoostHighZTT1->GetMaximum()*1.2);
  HBoostHighZTT1->Draw("hist");
  cout<<"HBoostHighZTT1 : "<<HBoostHighZTT1->Integral()<<" mean : "<<HBoostHighZTT1->GetMean()<<"  rms : "<<HBoostHighZTT1->GetRMS()<<endl;
  HBoostHighZTT2->Draw("histpesame");
  cout<<"HBoostHighZTT2 : "<<HBoostHighZTT2->Integral()<<" mean : "<<HBoostHighZTT2->GetMean()<<"  rms : "<<HBoostHighZTT2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  HBoostHighD1->Draw("hist");
  HBoostHighD2->Draw("histpesame");
  C.Print("compareDataCard.ps");


  //0Jet High----------------------------------------------------------------
  TH1F* H0JetHigh1 = (TH1F*) F1.Get("muTau_0jet_high/qqH125");
  TH1F* H0JetHigh2 = (TH1F*) F2.Get("muTau_0jet_high/qqH125");

  TH1F* H0JetHighZTT1 = (TH1F*) F1.Get("muTau_0jet_high/ZTT");
  TH1F* H0JetHighZTT2 = (TH1F*) F2.Get("muTau_0jet_high/ZTT");

  TH1F* H0JetHighD1 = (TH1F*) F1.Get("muTau_0jet_high/data_obs");
  TH1F* H0JetHighD2 = (TH1F*) F2.Get("muTau_0jet_high/data_obs");
  
  C.Clear();
  H0JetHigh1->GetYaxis()->SetRangeUser(0,H0JetHigh1->GetMaximum()*1.2);
  H0JetHigh1->Draw("hist");
  cout<<"H0JetHigh1 : "<<H0JetHigh1->Integral()<<" mean : "<<H0JetHigh1->GetMean()<<"  rms : "<<H0JetHigh1->GetRMS()<<endl;
  H0JetHigh2->Draw("histpesame");
  cout<<"H0JetHigh2 : "<<H0JetHigh2->Integral()<<" mean : "<<H0JetHigh2->GetMean()<<"  rms : "<<H0JetHigh2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  H0JetHighZTT1->GetYaxis()->SetRangeUser(0,H0JetHighZTT1->GetMaximum()*1.2);
  H0JetHighZTT1->Draw("hist");
  cout<<"H0JetHighZTT1 : "<<H0JetHighZTT1->Integral()<<" mean : "<<H0JetHighZTT1->GetMean()<<"  rms : "<<H0JetHighZTT1->GetRMS()<<endl;
  H0JetHighZTT2->Draw("histpesame");
  cout<<"H0JetHighZTT2 : "<<H0JetHighZTT2->Integral()<<" mean : "<<H0JetHighZTT2->GetMean()<<"  rms : "<<H0JetHighZTT2->GetRMS()<<endl;
  C.Print("compareDataCard.ps");

  C.Clear();
  H0JetHighD1->Draw("hist");
  H0JetHighD2->Draw("histpesame");
  C.Print("compareDataCard.ps");



  C.Print("compareDataCard.ps]");
  
}
