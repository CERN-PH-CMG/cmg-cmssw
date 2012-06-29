void TTLL() {

  //  TCut cut1("Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<8&&(abs(mMiss)>20||Tau1NEle+Tau2NEle<2||Tau1NMu+Tau2NMu<2)");
  TCut cut1("Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10");
  TCut cut2("Tau1NEle+Tau1NMu+Tau1Ntrk<5&&Tau2NEle+Tau2NMu+Tau2Ntrk<5&&min(Lep1REnergy,Lep2REnergy)>5&&min(Tau1REnergy,Tau2REnergy)>5&&abs(ZMass-91.2)<6&&(abs(mMiss)>10||(Tau1NEle+Tau2NEle<1&&Tau1NMu+Tau2NMu<2&&Tau1Ntrk+Tau2Ntrk<7))&&ZRMass+HRMass-91.2-sqrt(240**2-2*240*ZEnergy+ZMass**2)>-10&&tautaull==0");
  TFile *h = TFile::Open("HZHA_ttll.root");

  TH1F* higgs = new TH1F("higgs","higgs",100,50,150);
  TH1F* other = (TH1F*)(higgs->Clone("other"));
  //TauTauLLTreeProducer_TauTauLLAnalyzer->Project("higgs","HRMass+ZRMass-91.2",cut1);
  //TauTauLLTreeProducer_TauTauLLAnalyzer->Project("other","HRMass+ZRMass-91.2",cut2);
  TauTauLLTreeProducer_TauTauLLAnalyzer->Project("higgs","sqrt(240**2-2*240*ZEnergy+ZMass**2)-1.0",cut1);
  TauTauLLTreeProducer_TauTauLLAnalyzer->Project("other","sqrt(240**2-2*240*ZEnergy+ZMass**2)-1.0",cut2);
  higgs->Scale(1.03);
  //higgs->Scale(0.0);

  TFile *z = TFile::Open("ZZ_ttll.root");
  TH1F* zz = (TH1F*)(higgs->Clone("zz"));
  //TauTauLLTreeProducer_TauTauLLAnalyzer->Project("zz","HRMass+ZRMass-91.2",cut1);
  TauTauLLTreeProducer_TauTauLLAnalyzer->Project("zz","sqrt(240**2-2*240*ZEnergy+ZMass**2)",cut1);
  zz->Scale(0.65);

  TFile *q = TFile::Open("QQ_ttll.root");
  TH1F* qq = (TH1F*)(higgs->Clone("qq"));
  //TauTauLLTreeProducer_TauTauLLAnalyzer->Project("qq","HRMass+ZRMass-91.2",cut1);
  TauTauLLTreeProducer_TauTauLLAnalyzer->Project("qq","sqrt(240**2-2*240*ZEnergy+ZMass**2)",cut1);
  qq->Scale(4.0);

  TFile *w = TFile::Open("WW_ttll.root");
  TH1F* ww = (TH1F*)(higgs->Clone("ww"));
  //TauTauLLTreeProducer_TauTauLLAnalyzer->Project("ww","HRMass+ZRMass-91.2",cut1);
  TauTauLLTreeProducer_TauTauLLAnalyzer->Project("ww","sqrt(240**2-2*240*ZEnergy+ZMass**2)",cut1);
  ww->Scale(4.0);
 

  TCanvas* c0 = new TCanvas();
  ww->Add(qq);
  zz->Add(ww);
  higgs->Add(zz);
  other->Add(zz);

  TF1* fh = new TF1("fh","[0]*exp(-(x-95)/[1])+[2]*exp(-(x-[3])*(x-[3])/(2*[4]*[4]))+[5]+[6]*x+[7]*x*x+[8]*x*x*x",50,150);
  /*
  fh->FixParameter(0,34.63);
  fh->FixParameter(1,5.1915);
  fh->FixParameter(5,625.0);
  fh->FixParameter(6,-14.24);
  fh->FixParameter(7,0.1088);
  fh->FixParameter(8,-2.776E-4);
  fh->SetParameter(2,20.);
  fh->SetParameter(3,125.);
  fh->SetParameter(4,3.);
  */

  /*
  fh->SetParameter(0,6.39);
  fh->SetParameter(1,6.318);
  fh->SetParameter(5,299.57);
  fh->SetParameter(6,-7.0198);
  fh->SetParameter(7,0.05535);
  fh->SetParameter(8,-1.4553E-4);
  */

  fh->SetParameter(0,82.912);
  fh->SetParameter(1,5.7382);
  fh->SetParameter(5,20.454);
  fh->SetParameter(6,-1.298E-1);
  //fh->SetParameter(7,-1.378E-1);
  //fh->SetParameter(8,3.1587E-4);
  fh->FixParameter(7,0);
  fh->FixParameter(8,0);
  fh->SetParameter(2,0.);
  fh->SetParameter(3,125.);
  fh->SetParameter(4,3.);
  higgs->Fit("fh","EL","",95,148);  
  higgs->Fit("fh","EL","",95,148);  
  higgs->Fit("fh","E","",95,148);  

  higgs->SetMarkerStyle(22);
  higgs->SetMarkerColor(2);
  higgs->SetMarkerSize(1);
  higgs->SetLineColor(2);
  higgs->SetLineWidth(2);

  other->SetLineStyle(2);
  other->SetLineColor(2);
  other->SetLineWidth(3);

  zz->SetLineColor(4);
  zz->SetLineWidth(2);
  zz->SetFillColor(4);
  zz->SetFillStyle(3013);

  ww->SetLineColor(1);
  ww->SetLineWidth(2);
  qq->SetLineColor(6);
  qq->SetLineWidth(2);

  higgs->SetTitle( "H -> tau tau with Z to l+l-" );
  higgs->SetXTitle( "Higgs mass (GeV)" );
  higgs->SetYTitle( "Events / GeV" );
  higgs->GetYaxis()->SetTitleOffset(1.4);
  higgs->GetYaxis()->SetLabelSize(0.045);
  higgs->GetXaxis()->SetLabelSize(0.045);
  higgs->SetStats(0);
  //higgs->SetMaximum(800);

  higgs->Draw();
  //other->Draw("same");
  zz->Draw("same");
  ww->Draw("same");
  qq->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.55,0.35,0.85);
  leg0->AddEntry( higgs, "Signal", "l0");
  leg0->AddEntry( zz, "All backgrounds", "lf");
  leg0->AddEntry( zz, "ZZ", "l");
  leg0->AddEntry( ww, "WW", "l");
  leg0->AddEntry( qq, "qqbar", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(60,320,"L = 500 fb-1");
  text->Draw("same");

  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SaveAs("TTLL.png");


 
}
