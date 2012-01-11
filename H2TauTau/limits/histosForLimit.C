
TH1F* changeBinning(TH1F* hinput, int rebin,float xmax){
  int nbins=(int)(xmax);
  TString hname=hinput->GetName();
  TH1F* houtput=new TH1F("htmp",hinput->GetTitle(),nbins,0,xmax);
  for(Int_t b=1;b<nbins;b++){
    houtput->SetBinContent(b,hinput->GetBinContent(b));
    houtput->SetBinError(b,hinput->GetBinError(b));
  }
  delete hinput;

  houtput->Rebin(rebin);
  houtput->SetName(hname);
  return houtput;
}

std::string fillString(std::string input){
  if(input.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=input;
  for(Int_t i=0;i<15;i++)
    if(i>=input.size())output+=" ";
  
  return output;
}

std::string fillStringLong(std::string input){
  if(input.size()>40){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=input;
  for(Int_t i=0;i<40;i++)
    if(i>=input.size())output+=" ";
  
  return output;
}

std::string fillInt(int input){

  char  inchar[100];
  sprintf(inchar,"%d",input);
  std::string newinput(inchar);
  if(newinput.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=newinput;
  for(Int_t i=0;i<15;i++)
    if(i>=newinput.size())output+=" ";
  
  return output;
}

std::string fillFloat(float input){

  char  inchar[100];
  sprintf(inchar,"%.3f",input);
  std::string newinput(inchar);
  if(newinput.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=newinput;
  for(Int_t i=0;i<15;i++)
    if(i>=newinput.size())output+=" ";
    
 return output;
}

void histosForLimit(){
  //defines samples
  gROOT->ProcessLine(".x ../workdir/TauMuConfig_limits.C");
  
  //normalizes backgrounds for the data
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}  
  if(!analysis.scaleHistos())return 0;
  

  /* ***************************************
    -access backgrounds like this (updated Dec 2)
    -everything should be obtained from class interface/TauMuAnalysis.h
    -individual backgrounds obtained using getSample(), except for ZtoTauTau and QCD 
    -methods return ownership of object --> delete objects
    ****************************************** 
    analysis.getTotalDataSS(TString histoname);//sum of SS Data samples 
    analysis.getTotalData(TString histoname);
    analysis.getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
    analysis.getQCD(TString histoname);
    analysis.getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
    analysis.getTotalBackground(TString histoname);
  */




  //histogram used for fit in limit calcualtion
  TString histoname="diTauMassHisto_SM0";//"diTauMassHisto_SM1" "diTauMassHisto_SM2"
  

  TCanvas C("Canvas");
  C.Print("plots.ps[");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("HiggsGG115",histoname));
  TH1F* SM115 =(TH1F*)(htmp->Clone("SM115"));//rename otherwise default name may conflict below
  SM115=changeBinning(SM115,10,300);//use 10GeV wide bins from 0-->300
  delete htmp;
  C.Clear();
  SM115->SetTitle("SM115");
  SM115->Draw();
  C.Print("plots.ps");
  
//   TH1F* SM115_CMS_scale_tUp = (TH1F*)SM115->Clone("SM115_CMS_scale_tUp");
//   TH1F* SM115_CMS_scale_tDown = (TH1F*)SM115->Clone("SM115_CMS_scale_tDown");
//   TH1F* SM115_CMS_scale_jUp = (TH1F*)SM115->Clone("SM115_CMS_scale_jUp");
//   TH1F* SM115_CMS_scale_jDown = (TH1F*)SM115->Clone("SM115_CMS_scale_jDown");

  TH1F* htmp = (TH1F*)(analysis.getSample("HiggsVBF115",histoname));
  TH1F* VBF115 =(TH1F*)(htmp->Clone("VBF115"));//rename otherwise default name may conflict below
  VBF115=changeBinning(VBF115,10,300);
  delete htmp;
  C.Clear();
  VBF115->SetTitle("VBF115");
  VBF115->Draw();
  C.Print("plots.ps");
  
//   TH1F* VBF115_CMS_scale_tUp = (TH1F*)VBF115->Clone("VBF115_CMS_scale_tUp");
//   TH1F* VBF115_CMS_scale_tDown = (TH1F*)VBF115->Clone("VBF115_CMS_scale_tDown");
//   TH1F* VBF115_CMS_scale_jUp = (TH1F*)VBF115->Clone("VBF115_CMS_scale_jUp");
//   TH1F* VBF115_CMS_scale_jDown = (TH1F*)VBF115->Clone("VBF115_CMS_scale_jDown");

  
  TH1F* htmp = (TH1F*)(analysis.getZToTauTau(histoname));
  TH1F* ZTT =(TH1F*)(htmp->Clone("ZTT"));//rename otherwise default name may conflict below
  ZTT=changeBinning(ZTT,10,300);
  delete htmp;
  C.Clear();
  ZTT->SetTitle("ZTT");
  ZTT->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZTT_CMS_scale_tUp = (TH1F*)ZTT->Clone("ZTT_CMS_scale_tUp");
//   TH1F* ZTT_CMS_scale_tDown = (TH1F*)ZTT->Clone("ZTT_CMS_scale_tDown");
//   TH1F* ZTT_CMS_scale_jUp = (TH1F*)ZTT->Clone("ZTT_CMS_scale_jUp");
//   TH1F* ZTT_CMS_scale_jDown = (TH1F*)ZTT->Clone("ZTT_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToMuMu",histoname));
  TH1F* ZL =(TH1F*)(htmp->Clone("ZL"));//rename otherwise default name may conflict below
  ZL=changeBinning(ZL,10,300);
  delete htmp;
  C.Clear();
  ZL->SetTitle("ZL");
  ZL->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZL_CMS_scale_tUp = (TH1F*)ZL->Clone("ZL_CMS_scale_tUp");
//   TH1F* ZL_CMS_scale_tDown = (TH1F*)ZL->Clone("ZL_CMS_scale_tDown");
//   TH1F* ZL_CMS_scale_jUp = (TH1F*)ZL->Clone("ZL_CMS_scale_jUp");
//   TH1F* ZL_CMS_scale_jDown = (TH1F*)ZL->Clone("ZL_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToLJet",histoname));
  TH1F* ZJ =(TH1F*)(htmp->Clone("ZJ"));//rename otherwise default name may conflict below
  ZJ=changeBinning(ZJ,10,300);
  delete htmp;
  C.Clear();
  ZJ->SetTitle("ZJ");
  ZJ->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZJ_CMS_scale_tUp = (TH1F*)ZJ->Clone("ZJ_CMS_scale_tUp");
//   TH1F* ZJ_CMS_scale_tDown = (TH1F*)ZJ->Clone("ZJ_CMS_scale_tDown");
//   TH1F* ZJ_CMS_scale_jUp = (TH1F*)ZJ->Clone("ZJ_CMS_scale_jUp");
//   TH1F* ZJ_CMS_scale_jDown = (TH1F*)ZJ->Clone("ZJ_CMS_scale_jDown");

  TH1F* htmp = (TH1F*)(analysis.getSample("WJetsToLNu",histoname));
  TH1F* W =(TH1F*)(htmp->Clone("W"));//rename otherwise default name may conflict below
  W=changeBinning(W,10,300);
  delete htmp;
  C.Clear();
  W->SetTitle("W");
  W->Draw();
  C.Print("plots.ps");

//   TH1F* W_CMS_scale_tUp = (TH1F*)W->Clone("W_CMS_scale_tUp");
//   TH1F* W_CMS_scale_tDown = (TH1F*)W->Clone("W_CMS_scale_tDown");
//   TH1F* W_CMS_scale_jUp = (TH1F*)W->Clone("W_CMS_scale_jUp");
//   TH1F* W_CMS_scale_jDown = (TH1F*)W->Clone("W_CMS_scale_jDown");


  TH1F* htmp = (TH1F*)(analysis.getSample("TTJets",histoname));
  TH1F* TT =(TH1F*)(htmp->Clone("TT"));//rename otherwise default name may conflict below
  TT=changeBinning(TT,10,300);
  delete htmp;
  C.Clear();
  TT->SetTitle("TT");
  TT->Draw();
  C.Print("plots.ps");
  
//   TH1F* TT_CMS_scale_tUp = (TH1F*)TT->Clone("TT_CMS_scale_tUp");
//   TH1F* TT_CMS_scale_tDown = (TH1F*)TT->Clone("TT_CMS_scale_tDown");
//   TH1F* TT_CMS_scale_jUp = (TH1F*)TT->Clone("TT_CMS_scale_jUp");
//   TH1F* TT_CMS_scale_jDown = (TH1F*)TT->Clone("TT_CMS_scale_jDown");
    

  TH1F* htmp = (TH1F*)(analysis.getSample("WW",histoname));
  TH1F* WW =(TH1F*)(htmp->Clone("WW"));//rename otherwise default name may conflict below
  WW=changeBinning(WW,10,300);
  delete htmp;
  C.Clear();
  WW->SetTitle("WW");
  WW->Draw();
  C.Print("plots.ps");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("WZ",histoname));
  TH1F* WZ =(TH1F*)(htmp->Clone("WZ"));//rename otherwise default name may conflict below
  WZ=changeBinning(WZ,10,300);
  delete htmp;
  C.Clear();
  WZ->SetTitle("WZ");
  WZ->Draw();
  C.Print("plots.ps");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZZ",histoname));
  TH1F* ZZ =(TH1F*)(htmp->Clone("ZZ"));//rename otherwise default name may conflict below
  ZZ=changeBinning(ZZ,10,300);
  delete htmp;
  C.Clear();
  ZZ->SetTitle("ZZ");
  ZZ->Draw();
  C.Print("plots.ps");
  
  TH1F* VV =(TH1F*)(WW->Clone("VV"));
  VV->Add(WZ);
  VV->Add(ZZ);
  
  TH1F* htmp=analysis.getQCD(histoname);
  TH1F* QCD=(TH1F*)(htmp->Clone("QCD"));//rename otherwise default name may conflict below
  QCD=changeBinning(QCD,10,300);
  delete htmp;
  C.Clear();
  QCD->SetTitle("QCD");
  QCD->Draw();
  C.Print("plots.ps");

  
  TH1F* htmp=analysis.getTotalBackground(histoname);
  TH1F* bkg_obs = (TH1F*)(htmp->Clone("bkg_obs"));//rename otherwise default name may conflict below
  bkg_obs=changeBinning(bkg_obs,10,300);
  delete htmp;
  
  TH1F* htmp=analysis.getTotalData(histoname);
  TH1F* data_obs = (TH1F*)(htmp->Clone("data_obs"));//rename otherwise default name may conflict below
  data_obs=changeBinning(data_obs,10,300);
  delete htmp;

  C.Clear();
  data_obs->Draw("pe");
  bkg_obs->Draw("histsame");
  C.Print("plots.ps");
  cout<<" Data yield = "<<data_obs->Integral()<<"  Background yield = "<<bkg_obs->Integral()<<endl;


  TH1F* htmp=analysis.getTotalDataSS(histoname);
  TH1F* data_obsSS=(TH1F*)(htmp->Clone("data_obsSS"));//rename otherwise default name may conflict below
  data_obsSS=changeBinning(data_obsSS,10,300);
  delete htmp;
  C.Clear();
  data_obsSS->SetTitle("data_obsSS");
  data_obsSS->Draw();
  C.Print("plots.ps");

  C.Print("plots.ps]");


  ////////////////Histograms file

  char rootfilename[256];
  sprintf(rootfilename, "muTau.root");  
  TFile output(rootfilename,"recreate");
  TDirectory* dir = output.mkdir("muTau_SM0");
  
  dir->cd();
  SM115->Write();
//   SM115_CMS_scale_tUp   ->Write();
//   SM115_CMS_scale_tDown ->Write();
//   SM115_CMS_scale_jUp   ->Write();
//   SM115_CMS_scale_jDown ->Write();

  VBF115->Write();
//   VBF115_CMS_scale_tUp   ->Write();
//   VBF115_CMS_scale_tDown ->Write();
//   VBF115_CMS_scale_jUp   ->Write();
//   VBF115_CMS_scale_jDown ->Write();

  ZTT ->Write();
//   ZTT_CMS_scale_tUp   ->Write();
//   ZTT_CMS_scale_tDown ->Write();
//   ZTT_CMS_scale_jUp   ->Write();
//   ZTT_CMS_scale_jDown ->Write();

  ZL->Write();
//   ZL_CMS_scale_tUp   ->Write();
//   ZL_CMS_scale_tDown ->Write();
//   ZL_CMS_scale_jUp   ->Write();
//   ZL_CMS_scale_jDown ->Write();

  ZJ->Write();
//   ZJ_CMS_scale_tUp   ->Write();
//   ZJ_CMS_scale_tDown ->Write();
//   ZJ_CMS_scale_jUp   ->Write();
//   ZJ_CMS_scale_jDown ->Write();

  W->Write();
//   W_CMS_scale_tUp   ->Write();
//   W_CMS_scale_tDown ->Write();
//   W_CMS_scale_jUp   ->Write();
//   W_CMS_scale_jDown ->Write();

  TT->Write();
//   TT_CMS_scale_tUp   ->Write();
//   TT_CMS_scale_tDown ->Write();
//   TT_CMS_scale_jUp   ->Write();
//   TT_CMS_scale_jDown ->Write();

  VV->Write();

  QCD->Write();

  data_obs->Write();

  output.Close();


  
  //////////////////////////////////////Data Card
  ofstream file;
  file.open("muTau_SM0.txt");

  file << "imax 1" <<endl;
  file << "jmax *" <<endl;
  file << "kmax *" <<endl;
  file << "shapes *  *  "<< rootfilename << "  $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  file << endl;
  file << "observation " << data_obs->Integral()<<endl;
  file<<endl;


  file <<fillStringLong("bin")<<fillString(" ")
       <<fillString(" ")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")
       <<fillString("muTau_SM0")<<endl;
  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillString("SM115")
       <<fillString("VBF115")
       <<fillString("ZTT")
       <<fillString("QCD")
       <<fillString("W")
       <<fillString("ZJ")
       <<fillString("ZL")
       <<fillString("TT")
       <<fillString("VV")<<endl;
  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillInt(-1)
       <<fillInt(0)
       <<fillInt(1)
       <<fillInt(2)
       <<fillInt(3)
       <<fillInt(4)
       <<fillInt(5)
       <<fillInt(6)
       <<fillInt(7)<<endl;
  file <<fillStringLong("rate")<<fillString(" ")
       <<fillString(" ")
       <<fillFloat(SM115->Integral())
       <<fillFloat(VBF115->Integral())
       <<fillFloat(ZTT->Integral())
       <<fillFloat(QCD->Integral())
       <<fillFloat(W->Integral())
       <<fillFloat(ZJ->Integral())
       <<fillFloat(ZL->Integral())
       <<fillFloat(TT->Integral())
       <<fillFloat(VV->Integral())<<endl;

  file << endl;
  file <<"-------------------------------"<<endl;


  file <<fillStringLong("lumi")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.045)
       <<fillFloat(1.045)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"luminosity"<<endl;

  file <<fillStringLong("CMS_eff_m")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<"Muon ID/HLT"<<endl;

  file <<fillStringLong("CMS_eff_t")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<"Tau ID"<<endl;

  file <<fillStringLong("CMS_htt_zttNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.033)
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.033)
       <<fillFloat(1.033)
       <<fillString("-")
       <<fillString("-")
       <<"Norm Ztt"<<endl;

  file <<fillStringLong("CMS_htt_ttbarNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.075)
       <<fillString("-")
       <<"Norm ttbar"<<endl;

  file <<fillStringLong("CMS_htt_DiBosonNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.30)
       <<"Norm DiBoson"<<endl;

  file <<fillStringLong("CMS_htt_WNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.066)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.30)
       <<"Norm W+Jets"<<endl;

  file <<fillStringLong("CMS_htt_muTau_SM0_WNorm")<<fillString("gmN")
       <<fillInt(W->Integral()/analysis.getWJetsSignalToSBFraction())
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(analysis.getWJetsSignalToSBFraction())
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"W Background"<<endl;

  file <<fillStringLong("CMS_htt_muTau_SM0_QCDNorm")<<fillString("gmN")
       <<fillInt(QCD->Integral()/analysis.getQCDOStoSSRatio())
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(analysis.getQCDOStoSSRatio())
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD Background"<<endl;

  file <<fillStringLong("CMS_htt_ZJetFakeTau")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.10)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"Z(jet->tau) background"<<endl;

  file <<fillStringLong("CMS_htt_ZLeptonFakeTau")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.10)
       <<fillString("-")
       <<fillString("-")
       <<"Z(lepton->tau) background"<<endl;

  file <<fillStringLong("CMS_scale_j")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(0.99)
       <<fillFloat(0.92)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(0.97)
       <<fillFloat(0.94)
       <<"Jet scale"<<endl;

  file <<fillStringLong("CMS_scale_met")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.05)
       <<fillFloat(1.05)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.05)
       <<fillFloat(1.05)
       <<fillFloat(1.02)
       <<fillFloat(1.06)
       <<"Jet scale"<<endl;

  file <<fillStringLong("pdf_qqbar")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillFloat(1.03)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"PDF VBF"<<endl;

  file <<fillStringLong("pdf_gg")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.03)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"PDF VBF"<<endl;

  file <<fillStringLong("QCDscale_ggH")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.12)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD scale GGF"<<endl;

  file <<fillStringLong("QCDscale_qqH")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillFloat(1.035)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD scale VBF"<<endl;


  file <<fillStringLong("UEPS")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(0.96)
       <<fillFloat(0.96)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"Underlying events & PS"<<endl;


//  TString space="          ";
//   file << "bin                          "<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<<space<<"muTau_SM0"<< endl;
//   file << "process                      "<<space<<"SM115"<<space<<"VBF115"<<space<<"ZTT"<<space<<"QCD"<<space<<"W"<<space<<"ZJ"<<space<<"ZL"<<space<<"TT"<<space<<"VV"<< endl;
//   file << "process                      "<<space<<" -1  "<<space<<"   0  "<<space<<" 1 "<<space<<" 2 "<<space<<"3"<<space<<" 4"<<space<<" 5"<<space<<" 6"<<space<<" 7"<< endl;
//   file << "rate                         "<<space<<SM115->Integral()<<space<<VBF115->Integral()<<space<<ZTT->Integral()<<space<<QCD->Integral()<<space<<W->Integral()<<space<<ZJ->Integral()<<space<<ZL->Integral()<<space<<TT->Integral()<<space<<VV->Integral()  << endl;
//   file << endl;

//   file <<"-------------------------------------------------------"<<endl;
//   file <<"lumi                       lnN"<<space<<"1.045"<<space<<"1.045"<<space<<"-"<<space<<"-"<<space<<"1.045"<<space<<"1.045"<<space<<"1.045"<<space<<"1.045"<<space<<"1.045"<<space<<"luminosity"<<endl;
//   file <<"CMS_htt_zttNorm            lnN"<<space<<"-"<<space<<"-"<<space<<"1.033"<<space<<"-"<<space<<"-"<<space<<"1.033"<<space<<"1.033"<<space<<"-"<<space<<"-"<<space<<"ZTT"<<space<<"Scale"<<endl;

//   file <<"CMS_htt_muTau_SM0_QCDNorm  gmN"<<space<<(int)(QCD->Integral()/analysis.getQCDOStoSSRatio())<<"-"<<space<<"-"<<space<<"-"<<analysis.getQCDOStoSSRatio()<<"-"<<space<<"-"<<space<<"-"<<space<<"-"<<space<<"QCD Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_WNorm    gmN"<<(int)(W->Integral()/analysis.getWJetsSignalToSBFraction())<<"-"<<space<<"-"<<space<<"-"<<space<<"-"<<analysis.getWJetsSignalToSBFraction()<<"-"<<space<<"-"<<space<<"-"<<space<<"-"<<space<<"W Background"<<endl;


  //   file <<"bin                                   muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0" << endl;
  //   file <<"process                                SM115              ZTT                QCD                 W                  ZJ                 ZL                 TT" << endl;
  //   file <<"process                                  0                 1                  2                  3                  4                  5                  6" << endl;
  //   file <<"rate                                     " << SM115->Integral() << "              " << ZTT->Integral() << "             " << QCD->Integral() << "            " << W->Integral() << "             " << ZJ->Integral() << "             " << ZL->Integral() << "            " << TT->Integral() << endl;
  //   file << endl;
  //   file <<"--------------------------------------------------------------------------------------------------------------"<<endl;
  //   file <<"lumi                             lnN   1.045                     -                     -                     -                    -                     -               -               luminosity"<<endl;
//   file <<"CMS_eff_m                        lnN   1.020                   1.020                   -                     -                  1.020                 1.020                1.020             muon ID /HLT"<<endl;
//   file <<"CMS_eff_t                        lnN   1.060                   1.060                   -                     -                    -                     -                  1.060             Tau IDf"<<endl;
//   file <<"CMS_scale_t                    shape   1.000                   1.000                   -                   1.000                1.000                 1.000                1.000             shape"<<endl;
//   file <<"CMS_htt_zttNorm                  lnN     -                     1.033                   -                     -                  1.033                 1.033                  -               ZTT Scale"<<endl;
//   file <<"CMS_htt_ttbarNorm                lnN     -                        -                    -                     -                    -                     -                  1.12              TTbar background"<<endl;
//   file <<"CMS_htt_DiBosonNorm              lnN     -                        -                    -                     -                    -                     -                    -               DiBoson background"<<endl;
//  file <<"CMS_htt_muTau_SM0_QCDNorm  gmN  "<<QCD->Integral()/analysis.getQCDOStoSSRatio()<<"     -                        -                   1.11                   -                    -                     -                    -               QCD Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_QCDSyst        lnN     -                        -                   1.02                   -                    -                     -                    -               QCD Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_WNorm    gmN "<<W->Integral()/analysis.getWJetsSignalToSBFraction()<<"     -                        -                    -                   0.317277              -                     -                    -               W Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_WSyst          lnN     -                        -                    -                   1.06                   -                     -                    -               W Background" <<endl;
//   file <<"CMS_htt_muTau_ZJetFakeTau        lnN     -                        -                    -                     -                  1.12                    -                    -               Z(jet->tau) background"<<endl;
//   file <<"CMS_htt_muTau_ZLeptonFakeTau     lnN     -                        -                    -                     -                    -                   1.258                  -               Z(l->tau)   background"<<endl;
//   file <<"CMS_scale_j                    shape    1.0                     1.0                    -                   1.0                  1.0                   1.0                  1.0              shape and norm from jets"<<endl;
//   file <<"pdf_qqbar                        lnN     -                        -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"pdf_gg                           lnN   1.03                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"QCDscale_ggH                     lnN   1.12                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"QCDscale_qqH                     lnN      -                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"UEPS                             lnN   0.96                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;

  file.close();
  
}
