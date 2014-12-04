{

  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libCMGToolsCommon.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();

  TFile f("histograms.root");
  f.cd("JetMETAnalyzer");

  // change the following to get the fraction plot 
  // vs pt, eta, or phi
  // in barrel, endcaps, or HF
  //   string dim("RvsPt_endcaps");
  string dim("RvsEta");

 
  TProfile* p1 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/ChargedHadrons/")+dim).c_str() );
  TProfile* p2 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/Photons/")+dim).c_str());
  TProfile* p3 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/NeutralHadrons/")+dim).c_str());
  TProfile* p4 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/Electrons/")+dim).c_str());
  TProfile* p5 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/HFHAD/")+dim).c_str());
  TProfile* p6 = (TProfile*)gDirectory->Get( (string("pfJetHistograms/HFEM/")+dim).c_str());

  // converting the profiles into histograms
  TH1F* h1 = (TH1F*) p1->ProjectionX();
  TH1F* h2 = (TH1F*) p2->ProjectionX();
  TH1F* h3 = (TH1F*) p3->ProjectionX();
  TH1F* h4 = (TH1F*) p4->ProjectionX();
  TH1F* h5 = (TH1F*) p5->ProjectionX();
  TH1F* h6 = (TH1F*) p6->ProjectionX();

  THStack vsEta("vsEta","");
  
  Styles styles;
  
  bool solid=true;
  Styles::FormatHisto(h1, styles.spred, solid);
  Styles::FormatHisto(h2, styles.spblue, solid);
  Styles::FormatHisto(h3, styles.spgreen, solid);
  Styles::FormatHisto(h4, styles.spyellow, solid);
  Styles::FormatHisto(h5, styles.sppink1, solid);
  Styles::FormatHisto(h6, styles.sppink2, solid);
  

  vsEta.Add(h1);
  vsEta.Add(h2);
  vsEta.Add(h4);
  vsEta.Add(h3);
  vsEta.Add(h5);
  vsEta.Add(h6);

  vsEta.Draw("A,HIST");
  h1.Draw("AXIS,SAME");
  // other possible draw options: 
  // vsEta.Draw("nostack");
  // vsEta.Draw();
  // vsEta.Draw("hist");

  TLine l(-5,1,5,1);
  l.SetLineStyle(2);
  l.Draw("same");

 
}
