//       triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
//                           /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
//       triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
//                           /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());


void plotEfficiencyCurvesTauMu(){
  TriggerEfficiency triggerEff_;

//   SelectionEfficiency selectionEff_;
//   float selectionEffWeight_;

  TCanvas C;
  C.Print("plotEfficiencyCurvesTauMu.ps[");

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",100,0,100); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",100,0,100); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",100,0,100); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",100,0,100); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<100;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.effLooseTau15MC(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau2011AB(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.effLooseTau15MC(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau2011AB(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu.ps");

  C.Clear();
  hTrigTauDataEta1.GetYaxis()->SetRangeUser(.8,1.2);
  hTrigTauDataEta1.Divide(&hTrigTauDataEta2);
  hTrigTauDataEta1.Draw("hist");
  C.Print("plotEfficiencyCurvesTauMu.ps");

  C.Clear();
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(.8,1.2);
  hTrigTauMCEta1.Divide(&hTrigTauMCEta2);
  hTrigTauMCEta1.Draw("hist");
  C.Print("plotEfficiencyCurvesTauMu.ps");



  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",100,0,100);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",100,0,100);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",100,0,100);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",100,0,100);  
  for(Int_t p=0;p<100;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.effIsoMu15MC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effMu2011AB(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.effIsoMu15MC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effMu2011AB(p,2.0));
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Muon efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu.ps");

  C.Print("plotEfficiencyCurvesTauMu.ps]");


}


void plotEfficiencyCurvesTauMu2012(){
  TriggerEfficiency triggerEff_;

//   SelectionEfficiency selectionEff_;
//   float selectionEffWeight_;
//         triggerEffWeight_ *= triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
//           /triggerEff_.effTau2012MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
//         triggerEffWeight_ *= triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
//           /triggerEff_.effMu2012MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

  TCanvas C;
  C.Print("plotEfficiencyCurvesTauMu2012.ps[");

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",100,0,100); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",100,0,100); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",100,0,100); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",100,0,100); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<100;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.effTau2012MC(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau2012AB(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.effTau2012MC(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau2012AB(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");



  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",100,0,100);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",100,0,100);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",100,0,100);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",100,0,100);  
  for(Int_t p=0;p<100;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.effMu2012MC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effMu2012AB(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.effMu2012MC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effMu2012AB(p,2.0));
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  //zoom in
  C.Clear();
  hTrigMuMCEta1.GetXaxis()->SetRangeUser(15,30);
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  //TLine line;
  //line.DrawLine(17,0,17,1);
  //line.DrawLine(18,0,18,1);
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Muon efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Print("plotEfficiencyCurvesTauMu2012.ps]");


}

void plotEfficiencyCurvesTauMu2012HCP(){
  TriggerEfficiency triggerEff_;


  TCanvas C;
  C.Print("plotEfficiencyCurvesTauMu2012.ps[");

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",100,0,100); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",100,0,100); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",100,0,100); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",100,0,100); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<100;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.effTau2012MC(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau2012AB(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.effTau2012MC(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau2012AB(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");



  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",100,0,100);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",100,0,100);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",100,0,100);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",100,0,100);  
  for(Int_t p=0;p<100;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.effMu2012MC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effMu2012AB(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.effMu2012MC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effMu2012AB(p,2.0));
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  //zoom in
  C.Clear();
  hTrigMuMCEta1.GetXaxis()->SetRangeUser(15,30);
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  //TLine line;
  //line.DrawLine(17,0,17,1);
  //line.DrawLine(18,0,18,1);
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Muon efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012.ps");

  C.Print("plotEfficiencyCurvesTauMu2012.ps]");


}

void plotEfficiencyCurvesTauMu2012D(){
  TriggerEfficiency triggerEff_;


  TCanvas C;
  C.Print("plotEfficiencyCurvesTauMu2012D.ps[");

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",100,0,100); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",100,0,100); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",100,0,100); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",100,0,100); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<100;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.effTau_muTau_MC53X_2012D(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau_muTau_Data_2012D(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.effTau_muTau_MC53X_2012D(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau_muTau_Data_2012D(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012D.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.GetXaxis()->SetTitle("tau p_{T}");
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012D.ps");



  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",100,0,100);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",100,0,100);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",100,0,100);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",100,0,100);  
  for(Int_t p=0;p<100;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effMu_muTau_Data_2012D(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effMu_muTau_Data_2012D(p,2.0));                                                
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012D.ps");

  //zoom in
  C.Clear();
  hTrigMuMCEta1.GetXaxis()->SetRangeUser(15,30);
  hTrigMuMCEta1.SetTitle("Muon efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  //TLine line;
  //line.DrawLine(17,0,17,1);
  //line.DrawLine(18,0,18,1);
  C.Print("plotEfficiencyCurvesTauMu2012D.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Muon efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.GetXaxis()->SetTitle("mu p_{T}");
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauMu2012D.ps");

  C.Print("plotEfficiencyCurvesTauMu2012D.ps]");


}


void plotEfficiencyCurvesTauEle(){
  TriggerEfficiency triggerEff_;

//   SelectionEfficiency selectionEff_;
//   float selectionEffWeight_;

  TCanvas C;
  C.Print("plotEfficiencyCurvesTauEle.ps[");

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",100,0,100); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",100,0,100); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",100,0,100); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",100,0,100); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<100;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.effMediumIsoTau20MC(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau2011AB_TauEle(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.effMediumIsoTau20MC(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau2011AB_TauEle(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle.ps");

//   C.Clear();
//   hTrigTauDataEta1.GetYaxis()->SetRangeUser(.8,1.2);
//   hTrigTauDataEta1.Divide(&hTrigTauDataEta2);
//   hTrigTauDataEta1.Draw("hist");
//   C.Print("plotEfficiencyCurvesTauEle.ps");

//   C.Clear();
//   hTrigTauMCEta1.GetYaxis()->SetRangeUser(.8,1.2);
//   hTrigTauMCEta1.Divide(&hTrigTauMCEta2);
//   hTrigTauMCEta1.Draw("hist");
//   C.Print("plotEfficiencyCurvesTauEle.ps");



  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",100,0,100);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",100,0,100);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",100,0,100);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",100,0,100);  
  for(Int_t p=0;p<100;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.effEle18MC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effEle2011AB(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.effEle18MC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effEle2011AB(p,2.0));
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Electron efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Electron efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle.ps");

  C.Print("plotEfficiencyCurvesTauEle.ps]");


}


void plotEfficiencyCurvesTauEle2012(){
  TriggerEfficiency triggerEff_;


  TCanvas C;
  C.Print("plotEfficiencyCurvesTauEle2012.ps[");

  Int_t nbins=150;
  float xmin=0;
  float xmax=150;

  TH1F hTrigTauMCEta1("hTrigTauMCEta1","",nbins,xmin,xmax); hTrigTauMCEta1.SetStats(0);
  TH1F hTrigTauDataEta1("hTrigTauDataEta1","",nbins,xmin,xmax); hTrigTauDataEta1.SetStats(0);
  TH1F hTrigTauMCEta2("hTrigTauMCEta2","",nbins,xmin,xmax); hTrigTauMCEta2.SetStats(0);
  TH1F hTrigTauDataEta2("hTrigTauDataEta2","",nbins,xmin,xmax); hTrigTauDataEta2.SetStats(0);
  for(Int_t p=0;p<nbins;p++){
    hTrigTauMCEta1.SetBinContent(p,triggerEff_.eff2012Tau20MC_TauEle(p,0.5));
    hTrigTauDataEta1.SetBinContent(p,triggerEff_.effTau2012AB_TauEle(p,0.5));
    hTrigTauMCEta2.SetBinContent(p,triggerEff_.eff2012Tau20MC_TauEle(p,2.0));
    hTrigTauDataEta2.SetBinContent(p,triggerEff_.effTau2012AB_TauEle(p,2.0));
  }


  C.Clear();
  hTrigTauMCEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauDataEta1.SetTitle("Tau efficiency Barrel");
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta1.Draw("hist");
  hTrigTauDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle2012.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle2012.ps");

//   C.Clear();
//   hTrigTauDataEta1.GetYaxis()->SetRangeUser(.8,1.2);
//   hTrigTauDataEta1.Divide(&hTrigTauDataEta2);
//   hTrigTauDataEta1.Draw("hist");
//   C.Print("plotEfficiencyCurvesTauEle2012.ps");

//   C.Clear();
//   hTrigTauMCEta1.GetYaxis()->SetRangeUser(.8,1.2);
//   hTrigTauMCEta1.Divide(&hTrigTauMCEta2);
//   hTrigTauMCEta1.Draw("hist");
//   C.Print("plotEfficiencyCurvesTauEle2012.ps");


  TH1F hTrigMuMCEta1("hTrigMuMCEta1","",nbins,xmin,xmax);
  TH1F hTrigMuDataEta1("hTrigMuDataEta1","",nbins,xmin,xmax);  
  TH1F hTrigMuMCEta2("hTrigMuMCEta2","",nbins,xmin,xmax);
  TH1F hTrigMuDataEta2("hTrigMuDataEta2","",nbins,xmin,xmax);  
  for(Int_t p=0;p<nbins;p++){
    hTrigMuMCEta1.SetBinContent(p,triggerEff_.eff2012Ele20MC(p,0.5));
    hTrigMuDataEta1.SetBinContent(p,triggerEff_.effEle2012AB(p,0.5));
    hTrigMuMCEta2.SetBinContent(p,triggerEff_.eff2012Ele20MC(p,2.0));
    hTrigMuDataEta2.SetBinContent(p,triggerEff_.effEle2012AB(p,2.0));
  }

  C.Clear();
  hTrigMuMCEta1.SetTitle("Electron efficiency Barrel");
  hTrigMuMCEta1.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta1.Draw("hist");
  hTrigMuDataEta1.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle2012.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Electron efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurvesTauEle2012.ps");

  C.Print("plotEfficiencyCurvesTauEle2012.ps]");


}
