void TauEfficiency(Int_t Sample=1,Int_t EffOrFake=1,bool test=0){
  
  Int_t nfiles=0;
  Float_t ymax=1;
  TString ylabel="efficiency";
  float yoffset=0.34;
  float xoffset=0.3;
  TString eventweight="1";

  if(EffOrFake==2){
    ylabel="fake rate";
    yoffset=0.82 ;
    xoffset=0.2 ;
    ymax=0.1;
  }


  //bad gen jet but used for fake rate
  //TString localpath="/data/benitezj/PUStudies/Samples/deltaR1p5NoStatus1LepClean";
  //TString localpath="/data/benitezj/PUStudies/Samples/Mar13";
  //TString localpath="/data/benitezj/PUStudies/Samples/Mar14";
  //TString localpath="/data/benitezj/PUStudies/Samples/Mar15_NoChHadSub";  

  //fixed gen jet
  //TString localpath="/data/benitezj/PUStudies/Samples/Mar16NoChHadSubAllGen";
  TString localpath="/data/benitezj/PUStudies/Samples/Mar17AllGen";//with C.H.S  
  //TString localpath="/data/benitezj/PUStudies/Samples/Mar21NoChHadSubAllGenNoMatchCut";// Check on the Tau-Jet matchCone Cut

  if(Sample==1){//Fall11
    nfiles=115;
    if(EffOrFake==1)localpath=localpath+"/DYEff";
    if(EffOrFake==2)localpath=localpath+"/DYFake";
    eventweight="(genjetpt>15&&abs(genjeteta)<2.3)";
  }
  
  if(Sample==2){//Chamonix
    nfiles=380;
    if(EffOrFake==1)localpath=localpath+"/ChamonixEff";
    if(EffOrFake==2)localpath=localpath+"/ChamonixFake";
    //eventweight="ootweight*(genjetpt>15&&abs(genjeteta)<2.3)";
    eventweight="(genjetpt>15&&abs(genjeteta)<2.3)";
  }

  
  if(test)nfiles=10;
  TChain tree("flatNtp/tree");
  int countfiles=0;
  int fileidx=0;
  while(fileidx<nfiles){//115, 380
    fileidx++;
    TString fname=localpath+"/flatNtp_DY_"+(long)fileidx+".root";
    TFile file(fname.Data(),"read");
    if(file.IsZombie()) continue;
    if(!file.GetListOfKeys()) continue;
    if(file.GetListOfKeys()->GetSize()==0) continue;
    tree.Add(fname);
    countfiles++;
  }
  cout<<" chained "<<countfiles<<"  Entries = "<<tree.GetEntries()<<endl;


  
  ///create the histograms  
  TH1F HPU("HPU","",50,-0.5,49.5); HPU.GetXaxis()->SetTitle("# of pile up interactions"); HPU.Sumw2();
  tree.Draw("npv>>HPU");


  //eff vs eta
  TH1F HetaGen("HetaGen","",60,-3,3); HetaGen.GetXaxis()->SetTitle("gen. jet  #eta");  HetaGen.Sumw2();
  tree.Draw("genjeteta>>HetaGen",eventweight);
  TH1F Heta("Heta","",60,-3,3); Heta.GetXaxis()->SetTitle(" gen. jet  #eta  ");  Heta.Sumw2();
  tree.Draw("genjeteta>>Heta",eventweight+"*(recojetpt>15)");
  TH1F HetaVLooseIso("HetaVLooseIso","",60,-3,3); HetaVLooseIso.GetXaxis()->SetTitle(" gen. jet  #eta  ");  HetaVLooseIso.Sumw2();
  tree.Draw("genjeteta>>HetaVLooseIso",eventweight+"*(recojetpt>15&&recojetiso>1)");
  TH1F HetaLooseIso("HetaLooseIso","",60,-3,3); HetaLooseIso.GetXaxis()->SetTitle(" gen. jet  #eta  ");  HetaLooseIso.Sumw2();
  tree.Draw("genjeteta>>HetaLooseIso",eventweight+"*(recojetpt>15&&recojetiso>2)");
  TH1F HetaMediumIso("HetaMediumIso","",60,-3,3); HetaMediumIso.GetXaxis()->SetTitle(" gen. jet  #eta  ");  HetaMediumIso.Sumw2();
  tree.Draw("genjeteta>>HetaMediumIso",eventweight+"*(recojetpt>15&&recojetiso>3)");
  TH1F HetaTightIso("HetaTightIso","",60,-3,3); HetaTightIso.GetXaxis()->SetTitle(" gen. jet  #eta  ");  HetaTightIso.Sumw2();
  tree.Draw("genjeteta>>HetaTightIso",eventweight+"*(recojetpt>15&&recojetiso>4)");
  
  //pt resolution for LooseIso
  TH1F HptResLoose("HptResLoose","",80,-30,10); HptResLoose.GetXaxis()->SetTitle("reco. #tau p_{T} - gen. jet  p_{T} (GeV) "); HptResLoose.Sumw2();
  tree.Draw("recojetpt-genjetpt>>HptResLoose",eventweight+"*(recojetpt>15&&recojetiso>2&&30<genjetpt&&genjetpt<31)");

  //eta resolution for LooseIso
  TH1F HetaResLoose("HetaResLoose","",100,-0.05,0.05); HetaResLoose.GetXaxis()->SetTitle("reco. #tau #eta - gen. jet  #eta (GeV) "); HetaResLoose.Sumw2();
  tree.Draw("recojeteta-genjeteta>>HetaResLoose",eventweight+"*(recojetpt>15&&recojetiso>2&&2.0<abs(genjeteta)&&abs(genjeteta)<2.1)");



  Int_t nptbins=160; Float_t ptmin=0; Float_t ptmax=160;
  TH1F HptGenBin("HptGenBin","",nptbins,ptmin,ptmax); HptGenBin.GetXaxis()->SetTitle("gen. jet  p_{T} (GeV) "); HptGenBin.Sumw2();
  tree.Draw("genjetpt>>HptGenBin",eventweight);
  TH1F HptBin("HptBin","",nptbins,ptmin,ptmax); HptBin.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptBin.Sumw2();
  tree.Draw("genjetpt>>HptBin",eventweight+"*(recojetpt>15)");

  //efficiency vs pt //Simple binning
//   Int_t nptbins=160; Float_t ptmin=0; Float_t ptmax=160;
//   TH1F HptGen("HptGen","",nptbins,ptmin,ptmax); HptGen.GetXaxis()->SetTitle("gen. jet  p_{T} (GeV) "); HptGen.Sumw2();
//   tree.Draw("genjetpt>>HptGen",eventweight);
//   TH1F Hpt("Hpt","",nptbins,ptmin,ptmax); Hpt.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); Hpt.Sumw2();
//   tree.Draw("genjetpt>>Hpt",eventweight+"*(recojetpt>15)");
//   TH1F HptVLooseIso("HptVLooseIso","",nptbins,ptmin,ptmax); HptVLooseIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptVLooseIso.Sumw2();
//   tree.Draw("genjetpt>>HptVLooseIso",eventweight+"*(recojetpt>15&&recojetiso>1)");
//   TH1F HptLooseIso("HptLooseIso","",nptbins,ptmin,ptmax); HptLooseIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptLooseIso.Sumw2();
//   tree.Draw("genjetpt>>HptLooseIso",eventweight+"*(recojetpt>15&&recojetiso>2)");
//   TH1F HptMediumIso("HptMediumIso","",nptbins,ptmin,ptmax); HptMediumIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptMediumIso.Sumw2();
//   tree.Draw("genjetpt>>HptMediumIso",eventweight+"*(recojetpt>15&&recojetiso>3)");
//   TH1F HptTightIso("HptTightIso","",nptbins,ptmin,ptmax); HptTightIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptTightIso.Sumw2();
//   tree.Draw("genjetpt>>HptTightIso",eventweight+"*(recojetpt>15&&recojetiso>4)");
 
  //efficiency vs pt , changing binnning 
  Float_t ptbinsx[67];
  for(int b=0;b<=50;b++)ptbinsx[b]=b; for(int b=1;b<=10;b++)ptbinsx[50+b]=50+5*b; for(int b=1;b<=6;b++)ptbinsx[60+b]=100+10*b;
  TH1F HptGen("HptGen","",66,ptbinsx); HptGen.GetXaxis()->SetTitle("gen. jet  p_{T} (GeV) "); HptGen.Sumw2();
  tree.Draw("genjetpt>>HptGen",eventweight);
  TH1F Hpt("Hpt","",66,ptbinsx); Hpt.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); Hpt.Sumw2();
  tree.Draw("genjetpt>>Hpt",eventweight+"*(recojetpt>15)");
  TH1F HptVLooseIso("HptVLooseIso","",66,ptbinsx); HptVLooseIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptVLooseIso.Sumw2();
  tree.Draw("genjetpt>>HptVLooseIso",eventweight+"*(recojetpt>15&&recojetiso>1)");
  TH1F HptLooseIso("HptLooseIso","",66,ptbinsx); HptLooseIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptLooseIso.Sumw2();
  tree.Draw("genjetpt>>HptLooseIso",eventweight+"*(recojetpt>15&&recojetiso>2)");
  TH1F HptMediumIso("HptMediumIso","",66,ptbinsx); HptMediumIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptMediumIso.Sumw2();
  tree.Draw("genjetpt>>HptMediumIso",eventweight+"*(recojetpt>15&&recojetiso>3)");
  TH1F HptTightIso("HptTightIso","",66,ptbinsx); HptTightIso.GetXaxis()->SetTitle(" gen. jet  p_{T} (GeV) "); HptTightIso.Sumw2();
  tree.Draw("genjetpt>>HptTightIso",eventweight+"*(recojetpt>15&&recojetiso>4)");


  //isolation eff vs pileup (for 30<pt<40)
  TH1F Hpu3040Gen("Hpu3040Gen","",50,-0.5,49.5); Hpu3040Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040Gen.Sumw2();
  tree.Draw("npv>>Hpu3040Gen",eventweight+"*(30<genjetpt&&genjetpt<40)");
  TH1F Hpu3040("Hpu3040","",50,-0.5,49.5); Hpu3040.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040.Sumw2();
  tree.Draw("npv>>Hpu3040",eventweight+"*(recojetpt>15&&30<genjetpt&&genjetpt<40)");
  TH1F Hpu3040VLooseIso("Hpu3040VLooseIso","",50,-0.5,49.5); Hpu3040VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040VLooseIso.Sumw2();
  tree.Draw("npv>>Hpu3040VLooseIso",eventweight+"*(recojetpt>15&&recojetiso>1&&30<genjetpt&&genjetpt<40)");
  TH1F Hpu3040LooseIso("Hpu3040LooseIso","",50,-0.5,49.5); Hpu3040LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040LooseIso.Sumw2();
  tree.Draw("npv>>Hpu3040LooseIso",eventweight+"*(recojetpt>15&&recojetiso>2&&30<genjetpt&&genjetpt<40)");
  TH1F Hpu3040MediumIso("Hpu3040MediumIso","",50,-0.5,49.5); Hpu3040MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040MediumIso.Sumw2();
  tree.Draw("npv>>Hpu3040MediumIso",eventweight+"*(recojetpt>15&&recojetiso>3&&30<genjetpt&&genjetpt<40)");
  TH1F Hpu3040TightIso("Hpu3040TightIso","",50,-0.5,49.5); Hpu3040TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040TightIso.Sumw2();
  tree.Draw("npv>>Hpu3040TightIso",eventweight+"*(recojetpt>15&&recojetiso>4&&30<genjetpt&&genjetpt<40)");


  //isolation eff vs pileup (for 50<pt<60)
  TH1F Hpu5060Gen("Hpu5060Gen","",50,-0.5,49.5); Hpu5060Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060Gen.Sumw2();
  tree.Draw("npv>>Hpu5060Gen",eventweight+"*(50<genjetpt&&genjetpt<60)");
  TH1F Hpu5060("Hpu5060","",50,-0.5,49.5); Hpu5060.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060.Sumw2();
  tree.Draw("npv>>Hpu5060",eventweight+"*(recojetpt>15&&50<genjetpt&&genjetpt<60)");
  TH1F Hpu5060VLooseIso("Hpu5060VLooseIso","",50,-0.5,49.5); Hpu5060VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060VLooseIso.Sumw2();
  tree.Draw("npv>>Hpu5060VLooseIso",eventweight+"*(recojetpt>15&&recojetiso>1&&50<genjetpt&&genjetpt<60)");
  TH1F Hpu5060LooseIso("Hpu5060LooseIso","",50,-0.5,49.5); Hpu5060LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060LooseIso.Sumw2();
  tree.Draw("npv>>Hpu5060LooseIso",eventweight+"*(recojetpt>15&&recojetiso>2&&50<genjetpt&&genjetpt<60)");
  TH1F Hpu5060MediumIso("Hpu5060MediumIso","",50,-0.5,49.5); Hpu5060MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060MediumIso.Sumw2();
  tree.Draw("npv>>Hpu5060MediumIso",eventweight+"*(recojetpt>15&&recojetiso>3&&50<genjetpt&&genjetpt<60)");
  TH1F Hpu5060TightIso("Hpu5060TightIso","",50,-0.5,49.5); Hpu5060TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060TightIso.Sumw2();
  tree.Draw("npv>>Hpu5060TightIso",eventweight+"*(recojetpt>15&&recojetiso>4&&50<genjetpt&&genjetpt<60)");




  TCanvas Canv("Canv","TauEfficiency");
  TString psfile=localpath+"/TauEfficiency.ps";
  Canv.Print(psfile+"[");

  TText text;
  text.SetTextColor(1);
  text.SetTextSize(.03);
  char txt[100];
  // sprintf(txt,"y = %.4f %.4f * x",FPU.GetParameter(0),FPU.GetParameter(1));

  
  ///////////pile up 
  Canv.Clear();
  HPU.Draw("hist");
  Canv.Print(psfile);


  /////////////pt
  Canv.Clear();
  HptGenBin.GetYaxis()->SetRangeUser(0,1.2*HptGenBin.GetMaximum());
  HptGenBin.Draw("hist");
  HptBin.Draw("pesame");
  Canv.Print(psfile);
  
  //////////eta
  Canv.Clear();
  HetaGen.GetYaxis()->SetRangeUser(0,1.2*HetaGen.GetMaximum());
  HetaGen.Draw("hist");
  Heta.Draw("pesame");
  Canv.Print(psfile);


 
  ///efficiency vs eta with isolation
  TH1F* HetaRatio=(TH1F*)Heta.Clone("HetaRatio");  HetaRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HetaVLooseIsoRatio=(TH1F*)HetaVLooseIso.Clone("HetaVLooseIsoRatio");  HetaVLooseIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HetaLooseIsoRatio=(TH1F*)HetaLooseIso.Clone("HetaLooseIsoRatio");  HetaLooseIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HetaMediumIsoRatio=(TH1F*)HetaMediumIso.Clone("HetaMediumIsoRatio");  HetaMediumIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HetaTightIsoRatio=(TH1F*)HetaTightIso.Clone("HetaTightIsoRatio");  HetaTightIsoRatio->GetYaxis()->SetTitle(ylabel);
  delete HetaRatio->FindObject("stats"); HetaRatio->SetStats(0);
  delete HetaVLooseIsoRatio->FindObject("stats"); HetaVLooseIsoRatio->SetStats(0);
  delete HetaLooseIsoRatio->FindObject("stats"); HetaLooseIsoRatio->SetStats(0);
  delete HetaMediumIsoRatio->FindObject("stats"); HetaMediumIsoRatio->SetStats(0);
  delete HetaTightIsoRatio->FindObject("stats"); HetaTightIsoRatio->SetStats(0);
  HetaRatio->Divide(&HetaGen);   
  HetaVLooseIsoRatio->Divide(&HetaGen);   
  HetaLooseIsoRatio->Divide(&HetaGen);   
  HetaMediumIsoRatio->Divide(&HetaGen);   
  HetaTightIsoRatio->Divide(&HetaGen);   
  Canv.Clear();
  HetaRatio->GetYaxis()->SetRangeUser(0,ymax);
  HetaRatio->Draw("histpe");
  HetaVLooseIsoRatio->Draw("histpesame");
  HetaLooseIsoRatio->Draw("histpesame");
  HetaMediumIsoRatio->Draw("histpesame");
  HetaTightIsoRatio->Draw("histpesame");
  Canv.Print(psfile);
 
  ///efficiency vs pt with isolation
  gStyle->SetErrorX(0.5);
  TF1 FPTDep("FPTDep","[0]",30,160); 
  TH1F* HptRatio=(TH1F*)Hpt.Clone("HptRatio");  HptRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HptVLooseIsoRatio=(TH1F*)HptVLooseIso.Clone("HptVLooseIsoRatio");  HptVLooseIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HptLooseIsoRatio=(TH1F*)HptLooseIso.Clone("HptLooseIsoRatio");  HptLooseIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HptMediumIsoRatio=(TH1F*)HptMediumIso.Clone("HptMediumIsoRatio");  HptMediumIsoRatio->GetYaxis()->SetTitle(ylabel);
  TH1F* HptTightIsoRatio=(TH1F*)HptTightIso.Clone("HptTightIsoRatio");  HptTightIsoRatio->GetYaxis()->SetTitle(ylabel);
  delete HptRatio->FindObject("stats"); HptRatio->SetStats(0);
  delete HptVLooseIsoRatio->FindObject("stats"); HptVLooseIsoRatio->SetStats(0);
  delete HptLooseIsoRatio->FindObject("stats"); HptLooseIsoRatio->SetStats(0);
  delete HptMediumIsoRatio->FindObject("stats"); HptMediumIsoRatio->SetStats(0);
  delete HptTightIsoRatio->FindObject("stats"); HptTightIsoRatio->SetStats(0);
  HptRatio->Divide(&HptGen);   
  HptVLooseIsoRatio->Divide(&HptGen);   
  HptLooseIsoRatio->Divide(&HptGen);   
  HptMediumIsoRatio->Divide(&HptGen);   
  HptTightIsoRatio->Divide(&HptGen);   

  Canv.Clear();
  HptRatio->GetYaxis()->SetRangeUser(0,ymax);
  if(EffOrFake==1)HptRatio->Draw("histpe");

  HptVLooseIsoRatio->GetYaxis()->SetRangeUser(0,ymax);  
//   HptVLooseIsoRatio->Fit(&FPTDep,"",EffOrFake==1 ? "pesame" : "histpe",40,160);
//   sprintf(txt,"eff. = %.4f",FPTDep.GetParameter(0));
//   text.DrawText(165,FPTDep.GetParameter(0),txt);
  HptVLooseIsoRatio->Draw(EffOrFake==1 ? "pesame" : "histpe");

//   HptLooseIsoRatio->Fit(&FPTDep,"","pesame",40,160);
//   sprintf(txt,"eff. = %.3f",FPTDep.GetParameter(0));
//   text.DrawText(165,FPTDep.GetParameter(0),txt);
   HptLooseIsoRatio->Draw("pesame");

//   HptMediumIsoRatio->Fit(&FPTDep,"","pesame",40,160);
//   sprintf(txt,"eff. = %.3f",FPTDep.GetParameter(0));
//   text.DrawText(165,FPTDep.GetParameter(0),txt);
   HptMediumIsoRatio->Draw("pesame");

//   HptTightIsoRatio->Fit(&FPTDep,"","pesame",40,160);
//   sprintf(txt,"eff. = %.3f",FPTDep.GetParameter(0));
//   text.DrawText(165,FPTDep.GetParameter(0),txt); 
  HptTightIsoRatio->Draw("pesame");
  Canv.Print(psfile);
  gStyle->SetErrorX(0);


  ////////////efficiency vs pile up and isolation
  TF1 FPU("FPU","[0]+[1]*x",0,50);
  TH1F*Hpu3040Ratio=(TH1F*)Hpu3040.Clone("Hpu3040Ratio");
  TH1F*Hpu3040VLooseIsoRatio=(TH1F*)Hpu3040VLooseIso.Clone("Hpu3040VLooseIsoRatio"); 
  TH1F*Hpu3040LooseIsoRatio=(TH1F*)Hpu3040LooseIso.Clone("Hpu3040LooseIsoRatio"); 
  TH1F*Hpu3040MediumIsoRatio=(TH1F*)Hpu3040MediumIso.Clone("Hpu3040MediumIsoRatio");
  TH1F*Hpu3040TightIsoRatio=(TH1F*)Hpu3040TightIso.Clone("Hpu3040TightIsoRatio");
  delete Hpu3040Ratio->FindObject("stats"); Hpu3040Ratio->SetStats(0);
  delete Hpu3040VLooseIsoRatio->FindObject("stats"); Hpu3040VLooseIsoRatio->SetStats(0);
  delete Hpu3040LooseIsoRatio->FindObject("stats"); Hpu3040LooseIsoRatio->SetStats(0);
  delete Hpu3040MediumIsoRatio->FindObject("stats"); Hpu3040MediumIsoRatio->SetStats(0);
  delete Hpu3040TightIsoRatio->FindObject("stats"); Hpu3040TightIsoRatio->SetStats(0);
  Hpu3040Ratio->Divide(&Hpu3040Gen);   
  Hpu3040VLooseIsoRatio->Divide(&Hpu3040Gen);   
  Hpu3040LooseIsoRatio->Divide(&Hpu3040Gen);   
  Hpu3040MediumIsoRatio->Divide(&Hpu3040Gen);   
  Hpu3040TightIsoRatio->Divide(&Hpu3040Gen);   
  Canv.Clear();
  text.SetTextColor(2);
  Hpu3040Ratio->GetYaxis()->SetRangeUser(0,ymax);
  Hpu3040Ratio->GetYaxis()->SetTitle(ylabel);
  Hpu3040Ratio->Fit(&FPU,"","pe",10,30);
  if(EffOrFake==1){
    sprintf(txt,"y = %.5f %s %.5f * x",FPU.GetParameter(0),(FPU.GetParameter(1)>0 ? "+" : "-"),fabs(FPU.GetParameter(1)));
    text.DrawTextNDC(xoffset,yoffset,txt);
  }
  //Hpu3040Ratio->Draw("pe");
  Hpu3040VLooseIsoRatio->Fit(&FPU,"","pesame",10,30);
  sprintf(txt,"y = %.5f %s %.5f * x",FPU.GetParameter(0),(FPU.GetParameter(1)>0 ? "+" : "-"),fabs(FPU.GetParameter(1)));
  text.DrawTextNDC(xoffset,yoffset-0.04,txt);
  //Hpu3040VLooseIsoRatio->Draw("pesame");
  Hpu3040LooseIsoRatio->Fit(&FPU,"","pesame",10,30);
  sprintf(txt,"y = %.5f %s %.5f * x",FPU.GetParameter(0),(FPU.GetParameter(1)>0 ? "+" : "-"),fabs(FPU.GetParameter(1)));
  text.DrawTextNDC(xoffset,yoffset-0.08,txt);
  //Hpu3040LooseIsoRatio->Draw("pesame");
  Hpu3040MediumIsoRatio->Fit(&FPU,"","pesame",10,30);
  sprintf(txt,"y = %.5f %s %.5f * x",FPU.GetParameter(0),(FPU.GetParameter(1)>0 ? "+" : "-"),fabs(FPU.GetParameter(1)));
  text.DrawTextNDC(xoffset,yoffset-0.12,txt);
  //Hpu3040MediumIsoRatio->Draw("pesame");
  Hpu3040TightIsoRatio->Fit(&FPU,"","pesame",10,30);
  sprintf(txt,"y = %.5f %s %.5f * x",FPU.GetParameter(0),(FPU.GetParameter(1)>0 ? "+" : "-"),fabs(FPU.GetParameter(1)));
  text.DrawTextNDC(xoffset,yoffset-0.16,txt);
  //Hpu3040TightIsoRatio->Draw("pesame");
  Canv.Print(psfile);

 
  Canv.Print(psfile+"]");

  TFile RootFile(localpath+"/TauEfficiency.root","recreate");
  HetaRatio->Write();
  HetaLooseIsoRatio->Write();

  HptRatio->Write();
  HptLooseIsoRatio->Write();

  Hpu3040Ratio->Write();
  Hpu3040LooseIsoRatio->Write();

  HptResLoose.Write();
  HetaResLoose.Write();

  RootFile.ls();
  RootFile.Close();
}


