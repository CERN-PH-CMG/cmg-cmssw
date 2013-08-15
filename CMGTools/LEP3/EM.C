void EM() {

  //TCut cut1("Jet1pdgId+Jet2pdgId==0&&abs(Jet1pdgId-Jet2pdgId)==0&&abs(mMiss-95)<30&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50&&Jet1B7+Jet2B7>0.9");
  //TCut cut2("Jet1pdgId+Jet2pdgId==0&&abs(Jet1pdgId-Jet2pdgId)==0&&abs(mMiss-95)<30&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50&&Jet1B7+Jet2B7>0.9&&wwh==1");
  TCut cut1("abs(mMiss-95)<30&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50&&Jet1B7+Jet2B7>0.90");
  TCut cut2("abs(mMiss-95)<30&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50&&Jet1B7+Jet2B7>0.9&&wwh==1");
  TFile *h = TFile::Open("HZHA.root");
  TH1F* higgsbb = new TH1F("higgsbb","higgsbb",100,50.,150);
  TH1F* higgsww = (TH1F*)(higgsbb->Clone("higgsww"));
  EMissTreeProducer_EMissAnalyzer->Project("higgsbb","alpha*mVis",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("higgsww","alpha*mVis",cut2);
  higgsbb->Scale(1.03);
  higgsww->Scale(1.03);
  //higgsbb->Scale(0.);
  //higgsww->Scale(0.);

  TFile *z = TFile::Open("ZZ_Hinvis.root");
  TH1F* zzbb = (TH1F*)(higgsbb->Clone("zzbb"));
  EMissTreeProducer_EMissAnalyzer->Project("zzbb","alpha*mVis",cut1);
  zzbb->Scale(0.65);

  TFile *m = TFile::Open("MUMU_Hinvis.root");
  TH1F* mmbb = (TH1F*)(higgsbb->Clone("mmbb"));
  EMissTreeProducer_EMissAnalyzer->Project("mmbb","alpha*mVis",cut1);
  mmbb->Scale(2.2);
  mmbb->Draw("same");

  TFile *e = TFile::Open("ELEL_Hinvis.root");
  TH1F* eebb = (TH1F*)(higgsbb->Clone("eebb"));
  EMissTreeProducer_EMissAnalyzer->Project("eebb","alpha*mVis",cut1);
  eebb->Scale(2.2);

  TFile *t = TFile::Open("TAUTAU_Hinvis.root");
  TH1F* ttbb = (TH1F*)(higgsbb->Clone("ttbb"));
  EMissTreeProducer_EMissAnalyzer->Project("ttbb","alpha*mVis",cut1);
  ttbb->Scale(2.2);

  TFile *q = TFile::Open("QQ_Hinvis.root");
  TH1F* qqbb = (TH1F*)(higgsbb->Clone("qqbb"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb","alpha*mVis-0.5",cut1);
  qqbb->Scale(1.0);

  TH1F* qqbb0 = (TH1F*)(higgsbb->Clone("qqbb0"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb0","alpha*mVis+0.5",cut1);
  qqbb0->Scale(1.0);

  TH1F* qqbb1 = (TH1F*)(higgsbb->Clone("qqbb1"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb1","alpha*mVis-1.5",cut1);
  qqbb1->Scale(1.0);

  TH1F* qqbb2 = (TH1F*)(higgsbb->Clone("qqbb2"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb2","alpha*mVis+1.5",cut1);
  qqbb2->Scale(1.0);

  qqbb1->Add(qqbb0);
  qqbb2->Add(qqbb1);
  qqbb->Add(qqbb2);

  TFile *w = TFile::Open("WW_Hinvis.root");
  TH1F* wwbb = (TH1F*)(higgsbb->Clone("wwbb"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb","alpha*mVis-0.5",cut1);
  wwbb->Scale(1.0);

  TH1F* wwbb0 = (TH1F*)(higgsbb->Clone("wwbb0"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb0","alpha*mVis+0.5",cut1);
  wwbb0->Scale(1.0);

  TH1F* wwbb1 = (TH1F*)(higgsbb->Clone("wwbb1"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb1","alpha*mVis-1.5",cut1);
  wwbb1->Scale(1.0);

  TH1F* wwbb2 = (TH1F*)(higgsbb->Clone("wwbb2"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb2","alpha*mVis+1.5",cut1);
  wwbb2->Scale(1.0);

  wwbb1->Add(wwbb0);
  wwbb2->Add(wwbb1);
  wwbb->Add(wwbb2);

  TFile *wenu = TFile::Open("WENU_Hinvis.root");
  TH1F* wenubb = (TH1F*)(higgsbb->Clone("wenubb"));
  EMissTreeProducer_EMissAnalyzer->Project("wenubb","alpha*mVis",cut1);
  wenubb->Scale(0.65);

  TFile *zee = TFile::Open("ZEE_Hinvis.root");
  TH1F* zeebb = (TH1F*)(higgsbb->Clone("zeebb"));
  EMissTreeProducer_EMissAnalyzer->Project("zeebb","alpha*mVis",cut1);
  zeebb->Scale(1.);

  TFile *znnb = TFile::Open("ZNNB_Hinvis.root");
  TH1F* znnbbb = (TH1F*)(higgsbb->Clone("znnbbb"));
  EMissTreeProducer_EMissAnalyzer->Project("znnbbb","alpha*mVis",cut1);
  znnbbb->Scale(0.83);

  TCanvas* c0 = new TCanvas();
  mmbb->Add(ttbb);
  eebb->Add(mmbb);
  qqbb->Add(eebb);
  zeebb->Add(qqbb);
  wenubb->Add(zeebb);
  znnbbb->Add(wenubb);
  wwbb->Add(znnbbb);
  zzbb->Add(wwbb);
  higgsbb->Add(zzbb);
  higgsww->Add(zzbb);

  higgsww->Rebin(2);
  higgsbb->Rebin(2);
  zzbb->Rebin(2);
  wwbb->Rebin(2);
  znnbbb->Rebin(2);
  qqbb->Rebin(2);

  /* */
  TF1* fhbb = new TF1("fhbb","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5]))+[6]+[7]*x+[8]*x*x+[9]*x*x*x+[3]*[10]*exp(-(x-[4]*[11])*(x-[4]*[11])/(2*[12]*[12]))",60,150);

  fhbb->FixParameter(0,332.01);
  fhbb->FixParameter(1,93.68);
  fhbb->FixParameter(2,5.2735);
  fhbb->FixParameter(6,-1458.9);
  fhbb->FixParameter(7,37.64);
  fhbb->FixParameter(8,-0.285);
  fhbb->FixParameter(9,0.0006661);
  fhbb->SetParameter(3,500.);
  fhbb->SetParameter(4,125.);
  fhbb->SetParameter(5,3.2);
  fhbb->FixParameter(10,0.5);
  fhbb->SetParameter(11,1.);
  fhbb->SetParameter(12,10.);


  higgsbb->Fit("fhbb","EL","",70,146);  
  higgsbb->Fit("fhbb","EL","",70,146);  
  higgsbb->Fit("fhbb","E","",70,146);  
  /* */

  higgsbb->SetMarkerStyle(22);
  higgsbb->SetMarkerColor(2);
  higgsbb->SetMarkerSize(1);
  higgsbb->SetLineColor(2);
  higgsbb->SetLineWidth(4);

  higgsww->SetLineStyle(2);
  higgsww->SetLineColor(2);
  higgsww->SetLineWidth(3);

  zzbb->SetLineColor(4);
  zzbb->SetLineWidth(4);
  zzbb->SetFillColor(4);
  zzbb->SetFillStyle(3013);

  wwbb->SetLineColor(3);
  wwbb->SetLineWidth(4);
  znnbbb->SetLineColor(1);
  znnbbb->SetLineWidth(4);
  qqbb->SetLineColor(6);
  qqbb->SetLineWidth(4);

  higgsbb->SetTitle( "Missing Energy Channel" );
  higgsbb->SetXTitle( "Higgs mass (GeV)" );
  higgsbb->SetYTitle( "Events / 2 GeV" );
  higgsbb->GetYaxis()->SetTitleOffset(1.4);
  higgsbb->GetYaxis()->SetLabelSize(0.045);
  higgsbb->GetXaxis()->SetLabelSize(0.045);
  higgsbb->SetStats(0);
  higgsbb->SetMaximum(800);

  higgsbb->Draw();
  higgsww->Draw("same");
  zzbb->Draw("same");
  wwbb->Draw("same");
  znnbbb->Draw("same");
  //wenubb->Draw("same");
  //zeebb->Draw("same");
  qqbb->Draw("same");
  //eebb->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.52,0.40,0.85);
  leg0->AddEntry( higgsbb, "Signal (All)", "l0");
  leg0->AddEntry( higgsww, "Signal (WW fusion)", "l0");
  leg0->AddEntry( zzbb, "All backgrounds", "lf");
  leg0->AddEntry( zzbb, "ZZ", "l");
  leg0->AddEntry( wwbb, "WW and Wenu", "l");
  leg0->AddEntry( znnbbb, "Z nunubar", "l");
  leg0->AddEntry( qqbb, "qqbar", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(60,320,"L = 500 fb-1");
  //text->Draw("same");
  TText *cms = new TText(115,810,"CMS Preliminary");
  cms->Draw("same");

  //gPad->SetGridx();
  //gPad->SetGridy();
  gPad->SaveAs("EM.png");
  gPad->SaveAs("EM.pdf");

 
}
