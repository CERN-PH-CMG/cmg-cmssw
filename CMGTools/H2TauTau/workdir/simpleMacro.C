void simpleMacro(){
  
  
  TString path="/data/benitezj/Samples/TauMuV541June2_TrigEff";

  Sample TauPlusXMay("TauPlusXMay",path);
  Sample TauPlusXv4("TauPlusXv4",path);
  Sample TauPlusXAug("TauPlusXAug",path);
  Sample TauPlusXOct3("TauPlusXOct3",path);
  Sample TauPlusX2011B("TauPlusX2011B",path);

  float lumi=4934;

  Sample ZTT("ZToTauTau",path);
  ZTT.setCrossection(3048);
  ZTT.setSampleGenEvents(36209629);
  ZTT.scale(0.95*lumi/ZTT.getLumi());

  Sample W("WJetsToLNu",path);
  W.setCrossection(31314);
  W.setSampleGenEvents(81345381);
  W.scale(lumi/W.getLumi());

  Sample TT("TTJets",path);
  TT.setCrossection(165.8);
  TT.setSampleGenEvents(59613991);
  TT.scale(0.95*lumi/TT.getLumi());


  TString selectionOS="eventweight*(categoryCh==1&&categoryIso==1&&(pZeta-1.5*pZetaVis)>-20)";
  TString selectionSS="eventweight*(categoryCh==2&&categoryIso==1&&(pZeta-1.5*pZetaVis)>-20)";
  //- all seletions on objects have already been applied except isolation 
  //- eventweiht has trigger efficiency scale factors and pileup weight


  /////Data
  TH1F* HData=TauPlusXMay.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HData->SetName("HData");
  TH1F* Htmp=TauPlusXv4.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HData->Add(Htmp); delete Htmp;
  Htmp=TauPlusXAug.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HData->Add(Htmp); delete Htmp;
  Htmp=TauPlusXOct3.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HData->Add(Htmp); delete Htmp;
  Htmp=TauPlusX2011B.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HData->Add(Htmp); delete Htmp;

  ///MC
  TH1F* HBkg=new TH1F("HBkg","",100,0,300);
  TH1F*HZTT=ZTT.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HBkg->Add(HZTT); delete HZTT;
  TH1F* HW=W.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HBkg->Add(HW); delete HW;
  TH1F* HTT=TT.getHistoNtpFile("ditaumass",100,0,300,selectionOS);
  HBkg->Add(HTT); delete HTT;

  //QCD
  TH1F* HDataSS=TauPlusXMay.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HDataSS->SetName("HDataSS");
  Htmp=TauPlusXv4.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HDataSS->Add(Htmp); delete Htmp;
  Htmp=TauPlusXAug.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HDataSS->Add(Htmp); delete Htmp;
  Htmp=TauPlusXOct3.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HDataSS->Add(Htmp); delete Htmp;
  Htmp=TauPlusX2011B.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HDataSS->Add(Htmp); delete Htmp; 

  TH1F* HMCSS=new TH1F("HMCSS","",100,0,300);
  TH1F* HZTTSS=ZTT.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HMCSS->Add(HZTTSS); delete HZTTSS;
  TH1F* HWSS=W.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HMCSS->Add(HWSS); delete HWSS;
  TH1F* HTTSS=TT.getHistoNtpFile("ditaumass",100,0,300,selectionSS);
  HMCSS->Add(HTTSS); delete HTTSS;

  HDataSS->Add(HMCSS,-1);
  HDataSS->Scale(1.11);
  HBkg->Add(HDataSS);
  delete HDataSS;


  ///Plot
  TCanvas C;
  C.Print("file.ps[");

  C.Clear();
  HData->Draw("histpe");
  HBkg->Draw("histsame");
  C.Print("file.ps");

  C.Print("file.ps]");

  delete HData;
  delete HBkg;
}
