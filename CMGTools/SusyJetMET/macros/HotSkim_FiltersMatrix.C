{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");
  gROOT->LoadMacro("cms35.C");

  double intLumi = 35;

  bool plotData = true;   // do data plots
  bool addData = false;    // superimpose data in Sample object
  bool weighted = true;   // do weighted plots
  bool plotPFPostProcessing = true;

  //  const char* passed = "inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0";

  // gROOT->Macro("DataSet_QCD_HotSkim_InconsistentMuon.C");
  gROOT->Macro("DataSet_QCD_HotSkim_LeptonVeto.C");
  gROOT->Macro("DataSet_Data_HotSkim_387.C");
  
  // gROOT->Macro("DataSet_QCD_HotSkim_UnWeighted.C");

  Styles styles;

  Samples sam; 
  sam.SetLegendCorners(0.65,0.8,0.8,0.88);

//   TCanvas cNumbers1D("cNumbers1D","cNumbers1D", 700, 700);
//   Styles::FormatPad( &cNumbers1D, false );

//   TH1F h1D("h1D", "", 4,0.5, 4.5);
//   Styles::FormatHisto( &h1D );

  all.SetAlias("b1","(greedyMuons.@obj.size()>0)");
  all.SetAlias("b1p","(greedyMuons.@obj.size()!=0)");
  
  all.SetAlias("b2","(inconsMuons.@obj.size()>0)+1");
  all.SetAlias("b2p","inconsMuons.@obj.size()!=0");

  all.SetAlias("b3","(wrongMomentumMuons.@obj.size()>0)+3");
  all.SetAlias("b3p","wrongMomentumMuons.@obj.size()!=0");

  all.SetAlias("b4","(pfPPFullSize>0)+2");
  all.SetAlias("b4p","pfPPFullSize!=0");

//   h1D.SetStats(0);
//   all.Project("h1D", "b1","b1p");
//   all.Project("+h1D", "b2","b2p");
//   all.Project("+h1D", "b3","b3p");
//   all.Project("+h1D", "b4","b4p");

//   h1D.Draw();

  
  TCanvas cNumbers2D("cNumbers2D","cNumbers2D", 700, 700);
  Styles::FormatPad( &cNumbers2D, false );
  cNumbers2D.SetLeftMargin(0.2);

  TH2F h2D("h2D", "", 4,0.5, 4.5, 4,0.5, 4.5);
  Styles::FormatHisto( &h2D );
 
  h2D.GetXaxis().SetBinLabel(1,"Greedy");
  h2D.GetXaxis().SetBinLabel(2,"Inconsistent");
  h2D.GetXaxis().SetBinLabel(4,"#Deltap_{T}");
  h2D.GetXaxis().SetBinLabel(3,"PFPostProc");

  h2D.GetYaxis().SetBinLabel(1,"Greedy");
  h2D.GetYaxis().SetBinLabel(2,"Inconsistent");
  h2D.GetYaxis().SetBinLabel(4,"#Deltap_{T}");
  h2D.GetYaxis().SetBinLabel(3,"PFPostProc");

 
  all.Project("+h2D", "b1:b1","b1p && b1");
  all.Project("+h2D", "b1:b2","b1p && b2 && b2p");
  all.Project("+h2D", "b1:b3","b1p && b3 && b3p");
  all.Project("+h2D", "b1:b4","b1p && b4 && b4p");

  all.Project("+h2D", "b2:b1","b2p && b1 && b1p");
  all.Project("+h2D", "b2:b2","b2p && b2");
  all.Project("+h2D", "b2:b3","b2p && b3 && b3p");
  all.Project("+h2D", "b2:b4","b2p && b4 && b4p");

  all.Project("+h2D", "b3:b1","b3p && b1 && b1p");
  all.Project("+h2D", "b3:b2","b3p && b2 && b2p");
  all.Project("+h2D", "b3:b3","b3p && b3");
  all.Project("+h2D", "b3:b4","b3p && b4 && b4p");

  all.Project("+h2D", "b4:b1","b4p && b1 && b1p");
  all.Project("+h2D", "b4:b2","b4p && b2 && b2p");
  all.Project("+h2D", "b4:b3","b4p && b3 && b3p");
  all.Project("+h2D", "b4:b4","b4p && b4");

  h2D.SetStats(0);
  h2D.Draw("boxtext");

  cNumbers2D.SaveAs("filters_matrix.png");
  cNumbers2D.SaveAs("filters_matrix.pdf");
  

}
