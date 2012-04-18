void CompareChHadSub(){

  ////eff
  TFile FNoSub("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/DYEff/TauEfficiency.root","read");
  TFile FSub("/data/benitezj/PUStudies/Samples/Mar17AllGen/DYEff/TauEfficiency.root","read");
  int effOrFake=1;
  
//   //fake
//   TFile FNoSub("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/DYFake/TauEfficiency.root","read");
//   TFile FSub("/data/benitezj/PUStudies/Samples/Mar17AllGen/DYFake/TauEfficiency.root","read");
//   int effOrFake=2;


  TH1F* HNoSubPUDecay=(TH1F*)FNoSub.Get("Hpu3040Ratio");
  TH1F* HSubPUDecay=(TH1F*)FSub.Get("Hpu3040Ratio");

  TH1F* HNoSubPtDecay=(TH1F*)FNoSub.Get("HptRatio");
  TH1F* HSubPtDecay=(TH1F*)FSub.Get("HptRatio");

  TH1F* HNoSubEtaDecay=(TH1F*)FNoSub.Get("HetaRatio");
  TH1F* HSubEtaDecay=(TH1F*)FSub.Get("HetaRatio");


  TH1F* HNoSubPU=(TH1F*)FNoSub.Get("Hpu3040LooseIsoRatio");
  TH1F* HSubPU=(TH1F*)FSub.Get("Hpu3040LooseIsoRatio");

  TH1F* HNoSubPt=(TH1F*)FNoSub.Get("HptLooseIsoRatio");
  TH1F* HSubPt=(TH1F*)FSub.Get("HptLooseIsoRatio");

  TH1F* HNoSubEta=(TH1F*)FNoSub.Get("HetaLooseIsoRatio");
  TH1F* HSubEta=(TH1F*)FSub.Get("HetaLooseIsoRatio");

  
  TCanvas C("CompareChHadSub");
  C.Print("CompareChHadSub.ps[");

  //PU
  C.Clear();
  if(effOrFake==1)HNoSubPU->GetYaxis()->SetRangeUser(0.4,0.7);
  if(effOrFake==2)HNoSubPU->GetYaxis()->SetRangeUser(0.0,0.05);
  if(effOrFake==1)HNoSubPU->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoSubPU->GetYaxis()->SetTitle("fake rate");
  HNoSubPU->Draw("histpe");
  HSubPU->SetMarkerColor(2);
  HSubPU->SetLineColor(2);
  HSubPU->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.4,.3,"Pile-Up Sub.");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.4,.2,"Not Pile-Up Sub.");
  C.Print("CompareChHadSub.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HSubPU->GetNbinsX();b++)
    HSubPU->SetBinError(b,0);//Data sets are correlated.
  HSubPU->Divide(HNoSubPU);
  if(effOrFake==1)HSubPU->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HSubPU->GetYaxis()->SetRangeUser(0.8,1.2);
  HSubPU->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
  HSubPU->Draw("pe");
  TLine line;
  line.DrawLine(HSubPU->GetXaxis()->GetXmin(),1,HSubPU->GetXaxis()->GetXmax(),1);
  C.Print("CompareChHadSub.ps");



  //pt
  C.Clear();
  if(effOrFake==1)HNoSubPt->GetYaxis()->SetRangeUser(0.0,0.7);
  if(effOrFake==2)HNoSubPt->GetYaxis()->SetRangeUser(0.0,0.05);
  if(effOrFake==1)HNoSubPt->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoSubPt->GetYaxis()->SetTitle("fake rate");
  HNoSubPt->Draw("histpe");
  HSubPt->SetMarkerColor(2);
  HSubPt->SetLineColor(2);
  HSubPt->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.4,"Pile-Up Sub.");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.3,"Not Pile-Up Sub.");
  C.Print("CompareChHadSub.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HSubPt->GetNbinsX();b++)
    HSubPt->SetBinError(b,0);//Data sets are correlated.
  HSubPt->Divide(HNoSubPt);
  if(effOrFake==1)HSubPt->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HSubPt->GetYaxis()->SetRangeUser(0.8,1.2);
  HSubPt->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
  HSubPt->Draw("pe");
  TLine line;
  line.DrawLine(HSubPt->GetXaxis()->GetXmin(),1,HSubPt->GetXaxis()->GetXmax(),1);
  C.Print("CompareChHadSub.ps");


  //eta
  C.Clear();
  if(effOrFake==1)HNoSubEta->GetYaxis()->SetRangeUser(0.0,0.7);
  if(effOrFake==2)HNoSubEta->GetYaxis()->SetRangeUser(0.0,0.05);
  if(effOrFake==1)HNoSubEta->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoSubEta->GetYaxis()->SetTitle("fake rate");
  HNoSubEta->Draw("histpe");
  HSubEta->SetMarkerColor(2);
  HSubEta->SetLineColor(2);
  HSubEta->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.4,.3,"Pile-Up Sub.");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.4,.2,"Not Pile-Up Sub.");
  C.Print("CompareChHadSub.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HSubEta->GetNbinsX();b++)
    HSubEta->SetBinError(b,0);//Data sets are correlated.
  HSubEta->Divide(HNoSubEta);
  if(effOrFake==1)HSubEta->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HSubEta->GetYaxis()->SetRangeUser(0.8,1.2);
  HSubEta->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
  HSubEta->Draw("pe");
  TLine line;
  line.DrawLine(HSubEta->GetXaxis()->GetXmin(),1,HSubEta->GetXaxis()->GetXmax(),1);
  C.Print("CompareChHadSub.ps");




  if(effOrFake==1){
    //PU
    C.Clear();
    HNoSubPUDecay->GetYaxis()->SetRangeUser(0.,1.0);
    HNoSubPUDecay->GetYaxis()->SetTitle("efficiency");
    HNoSubPUDecay->Draw("histpe");
    HSubPUDecay->SetMarkerColor(2);
    HSubPUDecay->SetLineColor(2);
    HSubPUDecay->Draw("histpesame");
    TText txt;
    txt.SetTextSize(.05);
    txt.SetTextColor(2);
    txt.DrawTextNDC(.4,.3,"Pile-Up Sub.");
    txt.SetTextColor(1);
    txt.DrawTextNDC(.4,.2,"Not Pile-Up Sub.");
    C.Print("CompareChHadSub.ps");
  

    C.Clear();
    for(Int_t b=1;b<=HSubPUDecay->GetNbinsX();b++)
      HSubPUDecay->SetBinError(b,0);//Data sets are correlated.
    HSubPUDecay->Divide(HNoSubPUDecay);
    HSubPUDecay->GetYaxis()->SetRangeUser(0.9,1.1);
    HSubPUDecay->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
    HSubPUDecay->Draw("pe");
    TLine line;
    line.DrawLine(HSubPUDecay->GetXaxis()->GetXmin(),1,HSubPUDecay->GetXaxis()->GetXmax(),1);
    C.Print("CompareChHadSub.ps");



    //pt
    C.Clear();
    HNoSubPtDecay->GetYaxis()->SetRangeUser(0.0,1.0);
    HNoSubPtDecay->GetYaxis()->SetTitle("efficiency");
    HNoSubPtDecay->Draw("histpe");
    HSubPtDecay->SetMarkerColor(2);
    HSubPtDecay->SetLineColor(2);
    HSubPtDecay->Draw("histpesame");
    TText txt;
    txt.SetTextSize(.05);
    txt.SetTextColor(2);
    txt.DrawTextNDC(.5,.4,"Pile-Up Sub.");
    txt.SetTextColor(1);
    txt.DrawTextNDC(.5,.3,"Not Pile-Up Sub.");
    C.Print("CompareChHadSub.ps");
  

    C.Clear();
    for(Int_t b=1;b<=HSubPtDecay->GetNbinsX();b++)
      HSubPtDecay->SetBinError(b,0);//Data sets are correlated.
    HSubPtDecay->Divide(HNoSubPtDecay);
    HSubPtDecay->GetYaxis()->SetRangeUser(0.9,1.1);
    HSubPtDecay->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
    HSubPtDecay->Draw("pe");
    TLine line;
    line.DrawLine(HSubPtDecay->GetXaxis()->GetXmin(),1,HSubPtDecay->GetXaxis()->GetXmax(),1);
    C.Print("CompareChHadSub.ps");


    //eta
    C.Clear();
    HNoSubEtaDecay->GetYaxis()->SetRangeUser(0.0,1.0);
    HNoSubEtaDecay->GetYaxis()->SetTitle("efficiency");
    HNoSubEtaDecay->Draw("histpe");
    HSubEtaDecay->SetMarkerColor(2);
    HSubEtaDecay->SetLineColor(2);
    HSubEtaDecay->Draw("histpesame");
    TText txt;
    txt.SetTextSize(.05);
    txt.SetTextColor(2);
    txt.DrawTextNDC(.4,.3,"Pile-Up Sub.");
    txt.SetTextColor(1);
    txt.DrawTextNDC(.4,.2,"Not Pile-Up Sub.");
    C.Print("CompareChHadSub.ps");
  

    C.Clear();
    for(Int_t b=1;b<=HSubEtaDecay->GetNbinsX();b++)
      HSubEtaDecay->SetBinError(b,0);//Data sets are correlated.
    HSubEtaDecay->Divide(HNoSubEtaDecay);
    HSubEtaDecay->GetYaxis()->SetRangeUser(0.9,1.1);
    HSubEtaDecay->GetYaxis()->SetTitle("PileUp Sub. / Not PileUp Sub.");
    HSubEtaDecay->Draw("pe");
    TLine line;
    line.DrawLine(HSubEtaDecay->GetXaxis()->GetXmin(),1,HSubEtaDecay->GetXaxis()->GetXmax(),1);
    C.Print("CompareChHadSub.ps");

  }


  C.Print("CompareChHadSub.ps]");
}


void CompareChamonix(){
  TFile FFall11("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/DYEff/TauEfficiency.root","read");
  TFile FCham("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/ChamonixEff/TauEfficiency.root","read");
  //TFile FCham("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/ChamonixEff/TauEfficiency_OOTW.root","read");


  TH1F* HFall11PtDecay=(TH1F*)FFall11.Get("HptRatio");
  TH1F* HChamPtDecay=(TH1F*)FCham.Get("HptRatio");

  TH1F* HFall11Pt=(TH1F*)FFall11.Get("HptLooseIsoRatio");
  TH1F* HChamPt=(TH1F*)FCham.Get("HptLooseIsoRatio");

  TH1F* HFall11EtaDecay=(TH1F*)FFall11.Get("HetaRatio");
  TH1F* HChamEtaDecay=(TH1F*)FCham.Get("HetaRatio");

  TH1F* HFall11Eta=(TH1F*)FFall11.Get("HetaLooseIsoRatio");
  TH1F* HChamEta=(TH1F*)FCham.Get("HetaLooseIsoRatio");

  TH1F* HFall11PU=(TH1F*)FFall11.Get("Hpu3040LooseIsoRatio");
  TH1F* HChamPU=(TH1F*)FCham.Get("Hpu3040LooseIsoRatio");


  TH1F* HFall11PtRes=(TH1F*)FFall11.Get("HptResLoose");
  TH1F* HChamPtRes=(TH1F*)FCham.Get("HptResLoose");

  TH1F* HFall11EtaRes=(TH1F*)FFall11.Get("HetaResLoose");
  TH1F* HChamEtaRes=(TH1F*)FCham.Get("HetaResLoose");

  TText txt;
  TCanvas C("CompareCham");
  C.Print("CompareCham.ps[");


  C.Clear();
  HFall11PtDecay->GetYaxis()->SetRangeUser(0.,1.);
  HFall11PtDecay->Draw("pe");
  HChamPtDecay->SetMarkerColor(2);
  HChamPtDecay->SetLineColor(2);
  HChamPtDecay->Draw("pesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.4,"Fall11");
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.3,"Chamonix");
  C.Print("CompareCham.ps");

  C.Clear();
  HFall11Pt->GetYaxis()->SetRangeUser(0.,1.);
  HFall11Pt->Draw("pe");
  HChamPt->SetMarkerColor(2);
  HChamPt->SetLineColor(2);
  HChamPt->Draw("pesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.4,"Fall11");
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.3,"Chamonix");
  C.Print("CompareCham.ps");
  

  C.Clear();
  HFall11EtaDecay->GetYaxis()->SetRangeUser(0.,1.);
  HFall11EtaDecay->Draw("pe");
  HChamEtaDecay->SetMarkerColor(2);
  HChamEtaDecay->SetLineColor(2);
  HChamEtaDecay->Draw("pesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.4,"Fall11");
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.3,"Chamonix");
  C.Print("CompareCham.ps");
  

  C.Clear();
  HFall11Eta->GetYaxis()->SetRangeUser(0.,1.0);
  HFall11Eta->Draw("pe");
  HChamEta->SetMarkerColor(2);
  HChamEta->SetLineColor(2);
  HChamEta->Draw("pesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.4,"Fall11");
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.3,"Chamonix");
  C.Print("CompareCham.ps");
  

  C.Clear();
  HFall11PU->GetYaxis()->SetRangeUser(0.,.6);
  HFall11PU->GetYaxis()->SetTitle("efficiency");
  HFall11PU->Draw("histpe");
  HChamPU->SetMarkerColor(2);
  HChamPU->SetLineColor(2);
  HChamPU->Draw("histpesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.5,.4,"Fall11");
  txt.SetTextColor(2);
  txt.DrawTextNDC(.5,.3,"Chamonix");
  C.Print("CompareCham.ps");

  C.Clear();
  delete HFall11PtRes->FindObject("stats");
  HFall11PtRes->SetStats(0);
  HFall11PtRes->Scale(1./HFall11PtRes->Integral());
  HChamPtRes->Scale(1./HChamPtRes->Integral());
  HFall11PtRes->GetXaxis()->SetRangeUser(-20,10);
  HFall11PtRes->Draw("histpe");
  HChamPtRes->SetMarkerColor(2);
  HChamPtRes->SetLineColor(2);
  HChamPtRes->Draw("histpesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  char text[100];
  txt.DrawTextNDC(.2,.7,"Fall11");
  sprintf(text," rms=%.2f GeV",HFall11PtRes->GetRMS());
  txt.DrawTextNDC(.2,.65,text);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.2,.5,"Chamonix");
  sprintf(text," rms=%.2f GeV",HChamPtRes->GetRMS());
  txt.DrawTextNDC(.2,.45,text);
  C.Print("CompareCham.ps");


  C.Clear();
  delete HFall11EtaRes->FindObject("stats");
  HFall11EtaRes->SetStats(0);
  HFall11EtaRes->Scale(1./HFall11EtaRes->Integral());
  HChamEtaRes->Scale(1./HChamEtaRes->Integral());
  HFall11EtaRes->Draw("histpe");
  HChamEtaRes->SetMarkerColor(2);
  HChamEtaRes->SetLineColor(2);
  HChamEtaRes->Draw("histpesame");
  txt.SetTextSize(.05);
  txt.SetTextColor(1);
  txt.DrawTextNDC(.55,.7,"Fall11");
  sprintf(text," rms=%.4f",HFall11EtaRes->GetRMS());
  txt.DrawTextNDC(.55,.65,text);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.55,.5,"Chamonix");
  sprintf(text," rms=%.4f",HChamEtaRes->GetRMS());
  txt.DrawTextNDC(.55,.45,text);
  C.Print("CompareCham.ps");
  



  C.Print("CompareCham.ps]");
}


void CompareNoMatchingCut(){

  ////eff
  TFile F("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/DYEff/TauEfficiency.root","read");
  TFile FNo("/data/benitezj/PUStudies/Samples/Mar21NoChHadSubAllGenNoMatchCut/DYEff/TauEfficiency.root","read");
  int effOrFake=1;
  
//   //fake
//   TFile F("/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen/DYFake/TauEfficiency.root","read");
//   TFile FNo("/data/benitezj/PUStudies/Samples/Mar21NoChHadSubAllGenNoMatchCut/DYFake/TauEfficiency.root","read");
//   int effOrFake=2;


  TH1F* HNoPU=(TH1F*)FNo.Get("Hpu3040LooseIsoRatio");
  TH1F* HPU=(TH1F*)F.Get("Hpu3040LooseIsoRatio");
  TH1F* HNoPU=(TH1F*)FNo.Get("Hpu3040Ratio");
  TH1F* HPU=(TH1F*)F.Get("Hpu3040Ratio");
  HNoPU->SetMarkerColor(2);
  HNoPU->SetLineColor(2);

  TH1F* HNoPt=(TH1F*)FNo.Get("HptLooseIsoRatio");
  TH1F* HPt=(TH1F*)F.Get("HptLooseIsoRatio");
  HNoPt->SetMarkerColor(2);
  HNoPt->SetLineColor(2);

  TH1F* HNoEta=(TH1F*)FNo.Get("HetaLooseIsoRatio");
  TH1F* HEta=(TH1F*)F.Get("HetaLooseIsoRatio");
  HNoEta->SetMarkerColor(2);
  HNoEta->SetLineColor(2);
  
  TCanvas C("CompareMatchingCut");
  C.Print("CompareMatchingCut.ps[");

  //PU
  C.Clear();
  if(effOrFake==1)HNoPU->GetYaxis()->SetRangeUser(0.7,1.0);
  if(effOrFake==2)HNoPU->GetYaxis()->SetRangeUser(0.0,0.1);
  if(effOrFake==1)HNoPU->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoPU->GetYaxis()->SetTitle("fake rate");
  HNoPU->Draw("histpe");
  HPU->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.3,.75," matchingCone Not Applied ");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.3,.65," matchingCone Applied ");
  C.Print("CompareMatchingCut.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HPU->GetNbinsX();b++)
    HNoPU->SetBinError(b,0);//Data sets are correlated.
  HNoPU->Divide(HPU);
  if(effOrFake==1)HNoPU->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HNoPU->GetYaxis()->SetRangeUser(0.8,1.2);
  HNoPU->GetYaxis()->SetTitle("No matchingCone / matchingCone ");
  HNoPU->Draw("pe");
  TLine line;
  line.DrawLine(HPU->GetXaxis()->GetXmin(),1,HPU->GetXaxis()->GetXmax(),1);
  C.Print("CompareMatchingCut.ps");



  //pt
  C.Clear();
  if(effOrFake==1)HNoPt->GetYaxis()->SetRangeUser(0.0,0.7);
  if(effOrFake==2)HNoPt->GetYaxis()->SetRangeUser(0.0,0.1);
  if(effOrFake==1)HNoPt->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoPt->GetYaxis()->SetTitle("fake rate");
  HNoPt->Draw("histpe");
  HPt->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.2,.4," matchingCone Not Applied");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.2,.3," matchingCone Applied");
  C.Print("CompareMatchingCut.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HNoPt->GetNbinsX();b++)
    HNoPt->SetBinError(b,0);//Data sets are correlated.
  HNoPt->Divide(HPt);
  if(effOrFake==1)HNoPt->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HNoPt->GetYaxis()->SetRangeUser(0.8,1.2);
  HNoPt->GetYaxis()->SetTitle("No matchingCone / matchingCone ");
  HNoPt->Draw("pe");
  TLine line;
  line.DrawLine(HNoPt->GetXaxis()->GetXmin(),1,HNoPt->GetXaxis()->GetXmax(),1);
  C.Print("CompareMatchingCut.ps");


  //eta
  C.Clear();
  if(effOrFake==1)HNoEta->GetYaxis()->SetRangeUser(0.0,0.7);
  if(effOrFake==2)HNoEta->GetYaxis()->SetRangeUser(0.0,0.1);
  if(effOrFake==1)HNoEta->GetYaxis()->SetTitle("efficiency");
  if(effOrFake==2)HNoEta->GetYaxis()->SetTitle("fake rate");
  HNoEta->Draw("histpe");
  HEta->Draw("histpesame");
  TText txt;
  txt.SetTextSize(.05);
  txt.SetTextColor(2);
  txt.DrawTextNDC(.4,.3," matchingCone Not Applied");
  txt.SetTextColor(1);
  txt.DrawTextNDC(.4,.2," matchingCone Applied");
  C.Print("CompareMatchingCut.ps");
  

  C.Clear();
  for(Int_t b=1;b<=HNoEta->GetNbinsX();b++)
    HNoEta->SetBinError(b,0);//Data sets are correlated.
  HNoEta->Divide(HEta);
  if(effOrFake==1)HNoEta->GetYaxis()->SetRangeUser(0.9,1.1);
  if(effOrFake==2)HNoEta->GetYaxis()->SetRangeUser(0.8,1.2);
  HNoEta->GetYaxis()->SetTitle("No matchingCone / matchingCone  ");
  HNoEta->Draw("pe");
  TLine line;
  line.DrawLine(HNoEta->GetXaxis()->GetXmin(),1,HNoEta->GetXaxis()->GetXmax(),1);
  C.Print("CompareMatchingCut.ps");



  C.Print("CompareMatchingCut.ps]");
}
