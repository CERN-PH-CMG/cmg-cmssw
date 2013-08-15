void FJ() {

  TCanvas* c0 = new TCanvas();
  
  TCut cut1("zzMin>10&&wwMin>10&&ZMass>82.&&chi2<15");
  TCut cut2("abs(ZMass+HMass-91.2-125)<5&&zzMin>10&&wwMin>10&&ZMass>83.");
  std::cout << "Apres cut1" << std::endl;
  TFile *h = TFile::Open("Higgs.root");
  TH1F* higgs = new TH1F("higgs","higgs",50,100.5,150.5);
  TH1F* heta = new TH1F("heta","heta",30,-3.,3.);
  FourJetTreeProducer_FourJetAnalyzer->Project("higgs","ZMass+HMass-91.2",cut1);
  FourJetTreeProducer_FourJetAnalyzer->Project("heta","HEta",cut2);
  higgs->Scale(1.03);
  //higgs->Scale(0.00);

  TFile *z = TFile::Open("ZZ.root");
  TH1F* zz = (TH1F*)(higgs->Clone("zz"));
  FourJetTreeProducer_FourJetAnalyzer->Project("zz","ZMass+HMass-91.2",cut1);
  zz->Scale(0.65);
  TH1F* zeta = (TH1F*)(heta->Clone("zeta"));
  FourJetTreeProducer_FourJetAnalyzer->Project("zeta","HEta",cut2);
  zeta->Scale(0.65);

  TFile *w = TFile::Open("WW.root");
  TH1F* ww = (TH1F*)(higgs->Clone("ww"));
  FourJetTreeProducer_FourJetAnalyzer->Project("ww","ZMass+HMass-91.2",cut1);
  ww->Scale(4.0);
  TH1F* weta = (TH1F*)(heta->Clone("weta"));
  FourJetTreeProducer_FourJetAnalyzer->Project("weta","HEta",cut2);
  weta->Scale(4.0);

  TFile *q = TFile::Open("QQ.root");
  TH1F* qq = (TH1F*)(higgs->Clone("qq"));
  FourJetTreeProducer_FourJetAnalyzer->Project("qq","ZMass+HMass-91.2",cut1);
  qq->Scale(4.0);
  TH1F* qeta = (TH1F*)(heta->Clone("qeta"));
  FourJetTreeProducer_FourJetAnalyzer->Project("qeta","HEta",cut2);
  qeta->Scale(4.0);

  qq->Add(zz);
  ww->Add(qq);
  higgs->Add(ww);

  TF1* fh = new TF1("fh","[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[0]*[7]*exp(-(x-[8])*(x-[8])/(2*[9]*[9]))",100,150);
  fh->SetParameters(500,125.,3.,0.,0.,0.,0.,0.54,125.,10.);
  fh->FixParameter(7,0.54);
  fh->FixParameter(3,2242.);
  fh->FixParameter(4,-97.32);
  fh->FixParameter(5,1.186);
  fh->FixParameter(6,-4.236E-3);


  higgs->Fit("fh","EL","",102,147);  
  higgs->Fit("fh","EL","",102,147);  
  higgs->Fit("fh","E","",102,147);  
  //  higgs->Fit("fh","L","",102,147);  

  ww->SetLineColor(4);
  ww->SetLineWidth(4);
  ww->SetFillColor(4);
  ww->SetFillStyle(3013);

  higgs->SetMarkerStyle(22);
  higgs->SetMarkerColor(2);
  higgs->SetMarkerSize(1);
  higgs->SetLineColor(2);
  higgs->SetLineWidth(4);
 
  zz->SetLineColor(1);
  zz->SetLineWidth(4);
  qq->SetLineColor(6);
  qq->SetLineWidth(4);

  higgs->SetTitle( "Four-Jet Channel" );
  higgs->SetXTitle( "Higgs mass (GeV)" );
  higgs->SetYTitle( "Events / GeV" );
  higgs->GetYaxis()->SetTitleOffset(1.4);
  higgs->GetYaxis()->SetLabelSize(0.045);
  higgs->GetXaxis()->SetLabelSize(0.045);
  higgs->SetStats(0);

  //hmass_before->SetMaximum(6000);
  higgs->Draw("");
  higgs->Draw("sameerro");
  qq->Draw("same");
  zz->Draw("same");
  ww->Draw("same");

  TLegend *leg0=new TLegend(0.15,0.60,0.35,0.85);
  leg0->AddEntry( higgs, "Signal", "l0");
  leg0->AddEntry( ww, "All backgrounds", "lf");
  leg0->AddEntry( ww, "WW", "l");
  leg0->AddEntry( qq, "qqbar", "l");
  leg0->AddEntry( zz, "ZZ", "l");
  leg0->SetTextSize(0.03);
  leg0->Draw();

  TText *text = new TText(137,1100,"L = 500 fb-1");
  //text->Draw("same");
  TText *cms = new TText(133,1250,"CMS Preliminary");
  cms->Draw("same");

//   heta->SetLineColor(2);
//   zeta->SetLineColor(1);
//   qeta->Add(zeta);
//   weta->Add(qeta);
//   heta->Add(weta);
//   weta->SetLineColor(4);
//   qeta->SetLineColor(6);
//   heta->Draw();
//   zeta->Draw("same");
//   qeta->Draw("same");
//   weta->Draw("same");


  //gPad->SetGridx();
  //gPad->SetGridy();
  gPad->SaveAs("4J.png");
  gPad->SaveAs("4J.pdf");
}
