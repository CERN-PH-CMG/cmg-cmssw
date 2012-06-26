//       triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
//                           /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
//       triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
//                           /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());


void plotEfficiencyCurves(){
  TriggerEfficiency triggerEff_;

//   SelectionEfficiency selectionEff_;
//   float selectionEffWeight_;

  TCanvas C;
  C.Print("plotEfficiencyCurves.ps[");

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
  C.Print("plotEfficiencyCurves.ps");

  C.Clear();
  hTrigTauMCEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauDataEta2.SetTitle("Tau efficiency EndCap");
  hTrigTauMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigTauMCEta2.Draw("hist");
  hTrigTauDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurves.ps");

  C.Clear();
  hTrigTauDataEta1.GetYaxis()->SetRangeUser(.8,1.2);
  hTrigTauDataEta1.Divide(&hTrigTauDataEta2);
  hTrigTauDataEta1.Draw("hist");
  C.Print("plotEfficiencyCurves.ps");

  C.Clear();
  hTrigTauMCEta1.GetYaxis()->SetRangeUser(.8,1.2);
  hTrigTauMCEta1.Divide(&hTrigTauMCEta2);
  hTrigTauMCEta1.Draw("hist");
  C.Print("plotEfficiencyCurves.ps");



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
  C.Print("plotEfficiencyCurves.ps");

  C.Clear();
  hTrigMuMCEta2.SetTitle("Muon efficiency EndCap");
  hTrigMuMCEta2.GetYaxis()->SetRangeUser(0,1);
  hTrigMuMCEta2.Draw("hist");
  hTrigMuDataEta2.Draw("histpsame");
  C.Print("plotEfficiencyCurves.ps");

  C.Print("plotEfficiencyCurves.ps]");


}
