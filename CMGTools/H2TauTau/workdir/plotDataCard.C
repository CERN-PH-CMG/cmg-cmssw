void combineData(){

  TFile F1("/afs/cern.ch/user/b/benitezj/public/datacards/June17/muTauSM_JoseJune17_tScale.root","read");

  TH1F* hVBF1_ZTT=(TH1F*)F1.Get("muTau_vbf/ZTT");
  TH1F* hVBF1_W=(TH1F*)F1.Get("muTau_vbf/W");
  TH1F* hVBF1_TT=(TH1F*)F1.Get("muTau_vbf/TT");
  TH1F* hVBF1_QCD=(TH1F*)F1.Get("muTau_vbf/QCD");
  TH1F* hVBF1_ZJ=(TH1F*)F1.Get("muTau_vbf/ZJ");
  TH1F* hVBF1_ZL=(TH1F*)F1.Get("muTau_vbf/ZL");
  TH1F* hVBF1_VV=(TH1F*)F1.Get("muTau_vbf/VV");
  TH1F* hVBF1_data=(TH1F*)F1.Get("muTau_vbf/data_obs");
  
  TH1F * hVBF1_Bkg =(TH1F*)hVBF1_ZTT->Clone("Bkg");
  hVBF1_Bkg->Add(hVBF1_W);
  hVBF1_Bkg->Add(hVBF1_TT);
  hVBF1_Bkg->Add(hVBF1_QCD);
  hVBF1_Bkg->Add(hVBF1_ZJ);
  hVBF1_Bkg->Add(hVBF1_ZL);
  hVBF1_Bkg->Add(hVBF1_VV);

  TCanvas C("combineData");

  C.Print("combineData.ps[");
  hVBF1_data->Draw("histpe");
  hVBF1_Bkg->Draw("histsame");
  C.Print("combineData.ps]");
}
