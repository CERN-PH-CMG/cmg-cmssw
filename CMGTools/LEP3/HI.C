void HI() {

  TCut cut1("Jet1pdgId+Jet2pdgId==0&&abs(Jet1pdgId-Jet2pdgId)==0&&abs(mVis-85)<15&&acol>100&&cross>12&&ptMiss>15&&abs(pMiss*ctMiss)<50&&Jet1B7+Jet2B7>0.95");
  TCut cut2("Jet1pdgId+Jet2pdgId==0&&abs(Jet1pdgId-Jet2pdgId)==26&&abs(mVis-91.2)<4&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50");
  TCut cut3("Jet1pdgId+Jet2pdgId==0&&abs(Jet1pdgId-Jet2pdgId)==22&&abs(mVis-91.2)<6&&acol>100&&cross>10&&ptMiss>15&&abs(pMiss*ctMiss)<50");
  TFile *h = TFile::Open("Higgs_Hinvis.root");
  TH1F* higgsbb = new TH1F("higgsbb","higgsbb",50,50.,150);
  TH1F* higgsmm = (TH1F*)(higgsbb->Clone("higgsmm"));
  TH1F* higgsee = (TH1F*)(higgsbb->Clone("higgsee"));
  EMissTreeProducer_EMissAnalyzer->Project("higgsbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("higgsmm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("higgsee","mMissFit",cut3);
  higgsbb->Scale(1.0 * 1.00);
  higgsmm->Scale(1.0 * 1.00);
  higgsee->Scale(1.0 * 1.00);

  TFile *vis = TFile::Open("HZHA.root");
  TH1F* visbb = (TH1F*)(higgsbb->Clone("visbb"));
  TH1F* vismm = (TH1F*)(higgsbb->Clone("vismm"));
  TH1F* visee = (TH1F*)(higgsbb->Clone("visee"));
  EMissTreeProducer_EMissAnalyzer->Project("visbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("vismm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("visee","mMissFit",cut3);
  visbb->Scale(1.03 * 0.00);
  vismm->Scale(1.03 * 0.00);
  visee->Scale(1.03 * 0.00);

  TFile *z = TFile::Open("ZZ_Hinvis.root");
  TH1F* zzbb = (TH1F*)(higgsbb->Clone("zzbb"));
  TH1F* zzmm = (TH1F*)(higgsbb->Clone("zzmm"));
  TH1F* zzee = (TH1F*)(higgsbb->Clone("zzee"));
  EMissTreeProducer_EMissAnalyzer->Project("zzbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("zzmm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("zzee","mMissFit",cut3);
  zzbb->Scale(0.65);
  zzmm->Scale(0.65);
  zzee->Scale(0.65);

  TFile *m = TFile::Open("MUMU_Hinvis.root");
  TH1F* mmbb = (TH1F*)(higgsbb->Clone("mmbb"));
  TH1F* mmmm = (TH1F*)(higgsbb->Clone("mmmm"));
  TH1F* mmee = (TH1F*)(higgsbb->Clone("mmee"));
  EMissTreeProducer_EMissAnalyzer->Project("mmbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("mmmm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("mmee","mMissFit",cut3);
  mmbb->Scale(2.2);
  mmmm->Scale(2.2);
  mmee->Scale(2.2);
  mmbb->Draw("same");

  TFile *e = TFile::Open("ELEL_Hinvis.root");
  TH1F* eebb = (TH1F*)(higgsbb->Clone("eebb"));
  TH1F* eemm = (TH1F*)(higgsbb->Clone("eemm"));
  TH1F* eeee = (TH1F*)(higgsbb->Clone("eeee"));
  EMissTreeProducer_EMissAnalyzer->Project("eebb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("eemm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("eeee","mMissFit",cut3);
  eebb->Scale(2.2);
  eemm->Scale(2.2);
  eeee->Scale(2.2);

  TFile *t = TFile::Open("TAUTAU_Hinvis.root");
  TH1F* ttbb = (TH1F*)(higgsbb->Clone("ttbb"));
  TH1F* ttmm = (TH1F*)(higgsbb->Clone("ttmm"));
  TH1F* ttee = (TH1F*)(higgsbb->Clone("ttee"));
  EMissTreeProducer_EMissAnalyzer->Project("ttbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("ttmm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("ttee","mMissFit",cut3);
  ttbb->Scale(2.2);
  ttmm->Scale(2.2);
  ttee->Scale(2.2);

  TFile *q = TFile::Open("QQ_Hinvis.root");
  TH1F* qqbb = (TH1F*)(higgsbb->Clone("qqbb"));
  TH1F* qqmm = (TH1F*)(higgsbb->Clone("qqmm"));
  TH1F* qqee = (TH1F*)(higgsbb->Clone("qqee"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb","mMissFit-0.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("qqmm","mMissFit-0.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("qqee","mMissFit-0.5",cut3);
  qqbb->Scale(1.0);
  qqmm->Scale(1.0);
  qqee->Scale(1.0);

  TH1F* qqbb0 = (TH1F*)(higgsbb->Clone("qqbb0"));
  TH1F* qqmm0 = (TH1F*)(higgsbb->Clone("qqmm0"));
  TH1F* qqee0 = (TH1F*)(higgsbb->Clone("qqee0"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb0","mMissFit+0.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("qqmm0","mMissFit+0.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("qqee0","mMissFit+0.5",cut3);
  qqbb0->Scale(1.0);
  qqmm0->Scale(1.0);
  qqee0->Scale(1.0);

  TH1F* qqbb1 = (TH1F*)(higgsbb->Clone("qqbb1"));
  TH1F* qqmm1 = (TH1F*)(higgsbb->Clone("qqmm1"));
  TH1F* qqee1 = (TH1F*)(higgsbb->Clone("qqee1"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb1","mMissFit-1.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("qqmm1","mMissFit-1.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("qqee1","mMissFit-1.5",cut3);
  qqbb1->Scale(1.0);
  qqmm1->Scale(1.0);
  qqee1->Scale(1.0);

  TH1F* qqbb2 = (TH1F*)(higgsbb->Clone("qqbb2"));
  TH1F* qqmm2 = (TH1F*)(higgsbb->Clone("qqmm2"));
  TH1F* qqee2 = (TH1F*)(higgsbb->Clone("qqee2"));
  EMissTreeProducer_EMissAnalyzer->Project("qqbb2","mMissFit+1.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("qqmm2","mMissFit+1.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("qqee2","mMissFit+1.5",cut3);
  qqbb2->Scale(1.0);
  qqmm2->Scale(1.0);
  qqee2->Scale(1.0);

  qqbb1->Add(qqbb0);
  qqbb2->Add(qqbb1);
  qqbb->Add(qqbb2);
  qqee1->Add(qqee0);
  qqee2->Add(qqee1);
  qqee->Add(qqee2);
  qqmm1->Add(qqmm0);
  qqmm2->Add(qqmm1);
  qqmm->Add(qqmm2);

  TFile *w = TFile::Open("WW_Hinvis.root");
  TH1F* wwbb = (TH1F*)(higgsbb->Clone("wwbb"));
  TH1F* wwmm = (TH1F*)(higgsbb->Clone("wwmm"));
  TH1F* wwee = (TH1F*)(higgsbb->Clone("wwee"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb","mMissFit-0.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("wwmm","mMissFit-0.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("wwee","mMissFit-0.5",cut3);
  wwbb->Scale(1.0);
  wwmm->Scale(1.0);
  wwee->Scale(1.0);

  TH1F* wwbb = (TH1F*)(wwbb->Clone("wwbb0"));
  TH1F* wwmm = (TH1F*)(wwbb->Clone("wwmm0"));
  TH1F* wwee = (TH1F*)(wwbb->Clone("wwee0"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb0","mMissFit+0.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("wwmm0","mMissFit+0.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("wwee0","mMissFit+0.5",cut3);
  wwbb0->Scale(1.0);
  wwmm0->Scale(1.0);
  wwee0->Scale(1.0);

  TH1F* wwbb1 = (TH1F*)(higgsbb->Clone("wwbb1"));
  TH1F* wwmm1 = (TH1F*)(higgsbb->Clone("wwmm1"));
  TH1F* wwee1 = (TH1F*)(higgsbb->Clone("wwee1"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb1","mMissFit-1.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("wwmm1","mMissFit-1.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("wwee1","mMissFit-1.5",cut3);
  wwbb1->Scale(1.0);
  wwmm1->Scale(1.0);
  wwee1->Scale(1.0);

  TH1F* wwbb2 = (TH1F*)(higgsbb->Clone("wwbb2"));
  TH1F* wwmm2 = (TH1F*)(higgsbb->Clone("wwmm2"));
  TH1F* wwee2 = (TH1F*)(higgsbb->Clone("wwee2"));
  EMissTreeProducer_EMissAnalyzer->Project("wwbb2","mMissFit+1.5",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("wwmm2","mMissFit+1.5",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("wwee2","mMissFit+1.5",cut3);
  wwbb2->Scale(1.0);
  wwmm2->Scale(1.0);
  wwee2->Scale(1.0);

  wwbb1->Add(wwbb0);
  wwbb2->Add(wwbb1);
  wwbb->Add(wwbb2);
  wwee1->Add(wwee0);
  wwee2->Add(wwee1);
  wwee->Add(wwee2);
  wwmm1->Add(wwmm0);
  wwmm2->Add(wwmm1);
  wwmm->Add(wwmm2);

  TFile *wenu = TFile::Open("WENU_Hinvis.root");
  TH1F* wenubb = (TH1F*)(higgsbb->Clone("wenubb"));
  TH1F* wenumm = (TH1F*)(higgsbb->Clone("wenumm"));
  TH1F* wenuee = (TH1F*)(higgsbb->Clone("wenuee"));
  EMissTreeProducer_EMissAnalyzer->Project("wenubb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("wenumm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("wenuee","mMissFit",cut3);
  wenubb->Scale(0.65);
  wenumm->Scale(0.65);
  wenuee->Scale(0.65);

  TFile *zee = TFile::Open("ZEE_Hinvis.root");
  TH1F* zeebb = (TH1F*)(higgsbb->Clone("zeebb"));
  TH1F* zeemm = (TH1F*)(higgsbb->Clone("zeemm"));
  TH1F* zeeee = (TH1F*)(higgsbb->Clone("zeeee"));
  EMissTreeProducer_EMissAnalyzer->Project("zeebb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("zeemm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("zeeee","mMissFit",cut3);
  zeebb->Scale(1.);
  zeemm->Scale(1.);
  zeeee->Scale(1.);

  TFile *znnb = TFile::Open("ZNNB_Hinvis.root");
  TH1F* znnbbb = (TH1F*)(higgsbb->Clone("znnbbb"));
  TH1F* znnbmm = (TH1F*)(higgsbb->Clone("znnbmm"));
  TH1F* znnbee = (TH1F*)(higgsbb->Clone("znnbee"));
  EMissTreeProducer_EMissAnalyzer->Project("znnbbb","mMissFit",cut1);
  EMissTreeProducer_EMissAnalyzer->Project("znnbmm","mMissFit",cut2||cut3);
  EMissTreeProducer_EMissAnalyzer->Project("znnbee","mMissFit",cut3);
  znnbbb->Scale(0.83);
  znnbmm->Scale(0.83);
  znnbee->Scale(0.83);

  TCanvas* c0 = new TCanvas();
  mmbb->Add(ttbb);
  eebb->Add(mmbb);
  qqbb->Add(eebb);
  zeebb->Add(qqbb);
  wenubb->Add(zeebb);
  znnbbb->Add(wenubb);
  wwbb->Add(znnbbb);
  zzbb->Add(wwbb);
  visbb->Add(zzbb);
  higgsbb->Add(visbb);
  /*
  higgsbb->Rebin(2);
  wwbb->Rebin(2);
  zzbb->Rebin(2);
  znnbbb->Rebin(2);
  qqbb->Rebin(2);
  */

  TF1* fhbb = new TF1("fhbb","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5]))+[6]+[7]*x+[8]*x*x+[9]*x*x*x+[3]*[10]*exp(-(x-[4]*[11])*(x-[4]*[11])/(2*[12]*[12]))",100,150);
  fhbb->SetParameters(200.,100.,10.,500.,125.,4.,0.,0.,0.,0.);
  fhbb->FixParameter(0,126.4);
  fhbb->FixParameter(1,98.11);
  fhbb->FixParameter(2,8.406);
  fhbb->FixParameter(6,383.08);
  fhbb->FixParameter(7,-17.60.);
  fhbb->FixParameter(8,0.2492);
  fhbb->FixParameter(9,-0.0009909);
  fhbb->SetParameter(3,300.);
  fhbb->FixParameter(4,125.);
  fhbb->SetParameter(5,4.);
  fhbb->FixParameter(10,1.);
  fhbb->FixParameter(11,1.);
  fhbb->SetParameter(12,10.);
  //fhb->FixParameter(4,125.);
  //fhb->FixParameter(5,4.);
  //fhb->FixParameter(6,-7.073E-3);


  higgsbb->Fit("fhbb","EL","",60,148);  
  higgsbb->Fit("fhbb","EL","",60,148);  
  higgsbb->Fit("fhbb","E","",60,148);  
  //  higgs->Fit("fh","L","",102,147);  

  higgsbb->SetMarkerStyle(22);
  higgsbb->SetMarkerColor(2);
  higgsbb->SetMarkerSize(1);
  higgsbb->SetLineColor(2);
  higgsbb->SetLineWidth(4);

  visbb->SetMarkerStyle(22);
  visbb->SetLineStyle(2);
  visbb->SetLineColor(2);
  visbb->SetLineWidth(4);

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

  higgsbb->SetTitle( "Invisible Higgs with Z to bb" );
  higgsbb->SetXTitle( "Higgs mass (GeV)" );
  higgsbb->SetYTitle( "Events / 2 GeV" );
  higgsbb->GetYaxis()->SetTitleOffset(1.4);
  higgsbb->GetYaxis()->SetLabelSize(0.045);
  higgsbb->GetXaxis()->SetLabelSize(0.045);
  higgsbb->SetStats(0);
  higgsbb->SetMaximum(800);

  higgsbb->Draw();
  visbb->Draw("same");
  zzbb->Draw("same");
  wwbb->Draw("same");
  znnbbb->Draw("same");
  //wenubb->Draw("same");
  //zeebb->Draw("same");
  qqbb->Draw("same");
  //eebb->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.55,0.35,0.85);
  leg0->AddEntry( higgsbb, "Signal", "l0");
  leg0->AddEntry( zzbb, "All backgrounds", "lf");
  leg0->AddEntry( zzbb, "ZZ", "l");
  leg0->AddEntry( wwbb, "WW", "l");
  leg0->AddEntry( znnbbb, "Z nunubar", "l");
  leg0->AddEntry( qqbb, "qqbar", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(60,320,"L = 500 fb-1");
  //text->Draw("same");
  TText *cms = new TText(115,820,"CMS Preliminary");
  cms->Draw("same");

  //gPad->SetGridx();
  //gPad->SetGridy();
  gPad->SaveAs("HIbb.png");
  gPad->SaveAs("HIbb.pdf");


  TCanvas* c1 = new TCanvas();
  mmee->Add(ttee);
  eeee->Add(mmee);
  qqee->Add(eeee);
  zeeee->Add(qqee);
  wenuee->Add(zeeee);
  znnbee->Add(wenuee);
  wwee->Add(znnbee);
  zzee->Add(wwee);
  higgsee->Add(zzee);

  TF1* fhee = new TF1("fhee","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5]))+[6]+[7]*x+[8]*x*x+[9]*x*x*x+[3]*[10]*exp(-(x-[4]*[11])*(x-[4]*[11])/(2*[12]*[12]))",100,150);

  fhee->FixParameter(0,115.15);
  fhee->FixParameter(1,92.47);
  fhee->FixParameter(2,3.403);
  fhee->FixParameter(6,494.68);
  fhee->FixParameter(7,-18.532);
  fhee->FixParameter(8,0.22);
  fhee->FixParameter(9,-0.0007867);
  fhee->SetParameter(3,300.);
  fhee->SetParameter(4,125.);
  fhee->SetParameter(5,2.);
  fhee->SetParameter(10,0.1);
  fhee->SetParameter(11,1.0);
  fhee->SetParameter(12,3.);

  higgsee->Fit("fhee","EL","",70,120);  
  higgsee->Fit("fhee","EL","",60,135);  
  higgsee->Fit("fhee","E","",60,149);  

  higgsee->SetMarkerStyle(22);
  higgsee->SetMarkerColor(2);
  higgsee->SetMarkerSize(1);
  higgsee->SetLineColor(2);
  higgsee->SetLineWidth(4);

  zzee->SetLineColor(4);
  zzee->SetLineWidth(4);
  zzee->SetFillColor(4);
  zzee->SetFillStyle(3013);

  znnbee->SetLineColor(1);
  znnbee->SetLineWidth(4);
  wwee->SetLineColor(6);
  wwee->SetLineWidth(4);

  higgsee->SetTitle( "Invisible Higgs with Z to e+e-" );
  higgsee->SetXTitle( "Higgs mass (GeV)" );
  higgsee->SetYTitle( "Events / GeV" );
  higgsee->GetYaxis()->SetTitleOffset(1.4);
  higgsee->GetYaxis()->SetLabelSize(0.045);
  higgsee->GetXaxis()->SetLabelSize(0.045);
  higgsee->SetStats(0);
  //higgsee->SetMaximum(400);

  higgsee->Draw();
  zzee->Draw("same");
  wwee->Draw("same");
  znnbee->Draw("same");
  //wenuee->Draw("same");
  //zeeee->Draw("same");
  //qqee->Draw("same");
  //eeee->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.60,0.35,0.85);
  leg0->AddEntry( higgsbb, "Signal", "l0");
  leg0->AddEntry( zzee, "All backgrounds", "lf");
  leg0->AddEntry( zzee, "ZZ", "l");
  leg0->AddEntry( wwee, "WW", "l");
  leg0->AddEntry( znnbee, "Z nunubar", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(60,160,"L = 500 fb-1");
  text->Draw("same");

  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SaveAs("HIee.png");


  TCanvas* c2 = new TCanvas();
  mmmm->Add(ttmm);
  eemm->Add(mmmm);
  qqmm->Add(eemm);
  zeemm->Add(qqmm);
  wenumm->Add(zeemm);
  znnbmm->Add(wenumm);
  wwmm->Add(znnbmm);
  zzmm->Add(wwmm);
  vismm->Add(zzmm);
  higgsmm->Add(vismm);

  TF1 *fhmm = new TF1("fhmm",CrystalBall,100,150,9);
  fhmm->SetParameters(-0.66,4.,125.,1.9,800.,150.,10.,0.,0.);

  /*
  TF1* fhmm = new TF1("fhmm","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5]))+[6]+[7]*x+[8]*x*x+[9]*x*x*x+[3]*[10]*exp(-(x-[4]*[11])*(x-[4]*[11])/(2*[12]*[12]))",100,150);

  fhmm->FixParameter(0,110.98);
  fhmm->FixParameter(1,92.53);
  fhmm->FixParameter(2,4.288);
  fhmm->FixParameter(6,355.93);
  fhmm->FixParameter(7,-13.275);
  fhmm->FixParameter(8,0.1568);
  fhmm->FixParameter(9,-0.0005592);
  fhmm->SetParameter(3,0.);
  fhmm->SetParameter(4,125.);
  fhmm->SetParameter(5,2.);
  fhmm->SetParameter(10,0.1);
  fhmm->SetParameter(11,1.0);
  fhmm->SetParameter(12,3.);
  */

  higgsmm->Fit("fhmm","EL","",100,150);  
  higgsmm->Fit("fhmm","EL","",100,150);  
  higgsmm->Fit("fhmm","E","",100,150);  


  higgsmm->SetMarkerStyle(22);
  higgsmm->SetMarkerColor(2);
  higgsmm->SetMarkerSize(1);
  higgsmm->SetLineColor(2);
  higgsmm->SetLineWidth(4);

  vismm->SetMarkerStyle(22);
  vismm->SetLineStyle(2);
  vismm->SetLineColor(2);
  vismm->SetLineWidth(4);

  zzmm->SetLineColor(4);
  zzmm->SetLineWidth(4);
  zzmm->SetFillColor(4);
  zzmm->SetFillStyle(3013);

  znnbmm->SetLineColor(1);
  znnbmm->SetLineWidth(4);
  wwmm->SetLineColor(6);
  wwmm->SetLineWidth(4);

  higgsmm->SetTitle( "Invisible Higgs with Z to l+l-" );
  higgsmm->SetXTitle( "Higgs mass (GeV)" );
  higgsmm->SetYTitle( "Events / GeV" );
  higgsmm->GetYaxis()->SetTitleOffset(1.4);
  higgsmm->GetYaxis()->SetLabelSize(0.045);
  higgsmm->GetXaxis()->SetLabelSize(0.045);
  higgsmm->SetStats(0);
  //higgsmm->SetMaximum(400);


  higgsmm->Draw();
  vismm->Draw("same");
  zzmm->Draw("same");
  wwmm->Draw("same");
  znnbmm->Draw("same");
  //wenumm->Draw("same");
  //zeemm->Draw("same");
  //qqmm->Draw("same");
  //eemm->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.60,0.35,0.85);
  leg0->AddEntry( higgsbb, "Signal", "l0");
  leg0->AddEntry( zzee, "All backgrounds", "lf");
  leg0->AddEntry( zzee, "ZZ", "l");
  leg0->AddEntry( wwee, "WW", "l");
  leg0->AddEntry( znnbee, "Zvv", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(60,160,"L = 500 fb-1");
  //text->Draw("same");
  TText *cms = new TText(115,1700,"CMS Preliminary");
  cms->Draw("same");

  //gPad->SetGridx();
  //gPad->SetGridy();
  gPad->SaveAs("HImm.png");  
  gPad->SaveAs("HImm.pdf");  

 
}

Double_t CrystalBall(Double_t *x,Double_t *par) {

//Crystal ball function for signal, parameters are 0:alpha,1:n,2:mean,3:sigma,4:normalization;

  Double_t t = (x[0]-par[2])/par[3];
  if (par[0] < 0) t = -t;

  Double_t absAlpha = fabs((Double_t)par[0]);

  if (t >= -absAlpha) {
    return par[4]*exp(-0.5*t*t) + par[5] + t*par[6]+t*t*par[7] + t*t*t*par[8]; 
  }
  else {
    Double_t a =  TMath::Power(par[1]/absAlpha,par[1])*exp(-0.5*absAlpha*absAlpha);
    Double_t b= par[1]/absAlpha - absAlpha;

    return par[4]*(a/TMath::Power(b - t, par[1])) + par[5] + t*par[6] + t*t*par[7] + t*t*t*par[8];
  }
}
