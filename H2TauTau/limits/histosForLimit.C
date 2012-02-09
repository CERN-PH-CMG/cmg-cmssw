#include <TF1.h>
#include <TString.h>
#include <TH1F.h>

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

void histosForLimit(long sm, long mass){
  //defines samples
  gROOT->ProcessLine(".L ../workdir/TauMuConfig.C");
  

  //histogram used for fit in limit calcualtion
  TString histoname=TString("diTauMassHisto_SM")+sm;//"diTauMassHisto_SM1" "diTauMassHisto_SM2"
  Int_t rebin=30;
  Int_t xmax=240;  
  float smearRes=25.0;//should be changed based on which variable is being used

  //normalizes backgrounds for the data
  TauMuAnalysis* analysis = TauMuConfig("");
  analysis->init();
  analysis->scaleHistos();
  analysis->setSmearHistoRes(smearRes);

  TauMuAnalysis* analysisUp = TauMuConfig("tUp");
  analysisUp->setOutputPath("./output/V241ABFallTrigEfftUp");
  analysisUp->init();
  analysisUp->scaleHistos();
  analysisUp->setSmearHistoRes(smearRes);

  TauMuAnalysis* analysisDown = TauMuConfig("tDown");
  analysisDown->setOutputPath("./output/V241ABFallTrigEfftDown");
  analysisDown->init();
  analysisDown->scaleHistos();
  analysisDown->setSmearHistoRes(smearRes);
  

  /* ***************************************
    -access backgrounds like this (updated Dec 2)
    -everything should be obtained from class interface/TauMuAnalysis.h
    -individual backgrounds obtained using getSample(), except for ZtoTauTau and QCD 
    -methods return ownership of object --> delete objects
    ****************************************** 
    analysis->getTotalDataSS(TString histoname);//sum of SS Data samples 
    analysis->getTotalData(TString histoname);
    analysis->getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
    analysis->getQCD(TString histoname);
    analysis->getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
    analysis->getTotalBackground(TString histoname);
  */

  
  TCanvas C("Canvas");
  TString filename=TString("muTau_SM")+sm+"_mH"+mass+".ps";
  C.Print(filename+"[");
  
  
  TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsVH")+mass,histoname));
  TH1F* VH =(TH1F*)(htmp->Clone(TString("VH")+mass));//rename otherwise default name may conflict below
  VH=changeBinning(VH,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;
  C.Clear();
  VH->SetTitle(TString("VH")+mass);
  VH->Draw();
  C.Print(filename);
 
  TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsGG")+mass,histoname));
  TH1F* SM =(TH1F*)(htmp->Clone(TString("SM")+mass));//rename otherwise default name may conflict below
  SM=changeBinning(SM,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;
  C.Clear();
  SM->SetTitle(TString("SM")+mass);
  SM->Draw();
  C.Print(filename);

  TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsVBF")+mass,histoname));
  TH1F* VBF =(TH1F*)(htmp->Clone(TString("VBF")+mass));//rename otherwise default name may conflict below
  VBF=changeBinning(VBF,rebin,xmax);
  delete htmp;
  C.Clear();
  VBF->SetTitle(TString("VBF")+mass);
  VBF->Draw();
  C.Print(filename);
  
  TH1F* htmp = (TH1F*)(analysis->getZToTauTau(histoname));
  TH1F* ZTT =(TH1F*)(htmp->Clone("ZTT"));//rename otherwise default name may conflict below
  ZTT=changeBinning(ZTT,rebin,xmax);
  delete htmp;
  C.Clear();
  ZTT->SetTitle("ZTT");
  ZTT->Draw();
  C.Print(filename);
  
  TH1F* htmp = (TH1F*)(analysis->getSample("ZToMuMu",histoname));
  TH1F* ZL =(TH1F*)(htmp->Clone("ZL"));//rename otherwise default name may conflict below
  ZL=changeBinning(ZL,rebin,xmax);
  delete htmp;
  C.Clear();
  ZL->SetTitle("ZL");
  ZL->Draw();
  C.Print(filename);

  TH1F* htmp = (TH1F*)(analysis->getSample("ZToLJet",histoname));
  TH1F* ZJ =(TH1F*)(htmp->Clone("ZJ"));//rename otherwise default name may conflict below
  ZJ=changeBinning(ZJ,rebin,xmax);
  delete htmp;
  C.Clear();
  ZJ->SetTitle("ZJ");
  ZJ->Draw();
  C.Print(filename);

  TH1F* htmp = analysis->getWJets(histoname,sm);
  TH1F* W =(TH1F*)(htmp->Clone("W"));//rename otherwise default name may conflict below
  W=changeBinning(W,rebin,xmax);
  delete htmp;
  C.Clear();
  W->SetTitle("W");
  W->Draw();
  C.Print(filename);

  TH1F* htmp = (TH1F*)(analysis->getSample("TTJets",histoname));
  TH1F* TT =(TH1F*)(htmp->Clone("TT"));//rename otherwise default name may conflict below
  TT=changeBinning(TT,rebin,xmax);
  delete htmp;
  C.Clear();
  TT->SetTitle("TT");
  TT->Draw();
  C.Print(filename);
  
  TH1F* htmp = (TH1F*)(analysis->getDiBoson(histoname));
  TH1F* VV =(TH1F*)(htmp->Clone("VV"));//rename otherwise default name may conflict below
  VV=changeBinning(VV,rebin,xmax);
  delete htmp;
  C.Clear();
  VV->SetTitle("VV");
  VV->Draw();
  C.Print(filename);
  
  TH1F* htmp=analysis->getQCD(histoname,sm);
  TH1F* QCD=(TH1F*)(htmp->Clone("QCD"));//rename otherwise default name may conflict below
  QCD=changeBinning(QCD,rebin,xmax);
  delete htmp;
  C.Clear();
  QCD->SetTitle("QCD");
  QCD->Draw();
  C.Print(filename);
  
  
  /////blind the result, get fake data from background below
  //TH1F* htmp=analysis->getTotalData(histoname);
  //TH1F* data_obs = (TH1F*)(htmp->Clone("data_obs"));//rename otherwise default name may conflict below
  //data_obs=changeBinning(data_obs,rebin,xmax);
  //delete htmp;
  
  TH1F* htmp=analysis->getTotalBackground(histoname,sm);
  TH1F* totalbkg = (TH1F*)(htmp->Clone("totalbkg"));//rename otherwise default name may conflict below
  totalbkg=changeBinning(totalbkg,rebin,xmax);
  delete htmp;

  ///blind the result
  TH1F* data_obs = new TH1F("data_obs","",totalbkg->GetNbinsX(),totalbkg->GetXaxis()->GetXmin(),totalbkg->GetXaxis()->GetXmax());
  data_obs->Sumw2();
  data_obs->FillRandom(totalbkg,(int)(totalbkg->Integral()+0.5));


  C.Clear();
  totalbkg->GetYaxis()->SetRangeUser(0,totalbkg->GetMaximum()*1.4);
  totalbkg->Draw("hist");
  data_obs->Draw("pesame");
  C.Print(filename);

  TH1F* hratio = new TH1F("hratio","",totalbkg->GetNbinsX(),totalbkg->GetXaxis()->GetXmin(),totalbkg->GetXaxis()->GetXmax());
  hratio->Sumw2();
  hratio->Add(data_obs);
  hratio->Divide(totalbkg);
  C.Clear();
  hratio->Draw("hist pe");
  C.Print(filename);
  delete hratio;


  C.Print(filename+"]");

  
  ////////////////tau energy scaled up

  TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsVH")+mass,histoname));
  TH1F* VH_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(VH->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
  VH_CMS_scale_tUp=changeBinning(VH_CMS_scale_tUp,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsGG")+mass,histoname));
  TH1F* SM_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(SM->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
  SM_CMS_scale_tUp=changeBinning(SM_CMS_scale_tUp,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsVBF")+mass,histoname));
  TH1F* VBF_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(VBF->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
  VBF_CMS_scale_tUp=changeBinning(VBF_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getZToTauTau(histoname));
  TH1F* ZTT_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZTT_CMS_scale_tUp"));//rename otherwise default name may conflict below
  ZTT_CMS_scale_tUp=changeBinning(ZTT_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getSample("ZToMuMu",histoname));
  TH1F* ZL_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZL_CMS_scale_tUp"));//rename otherwise default name may conflict below
  ZL_CMS_scale_tUp=changeBinning(ZL_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getSample("ZToLJet",histoname));
  TH1F* ZJ_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZJ_CMS_scale_tUp"));//rename otherwise default name may conflict below
  ZJ_CMS_scale_tUp=changeBinning(ZJ_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getWJets(histoname,sm));
  TH1F* W_CMS_scale_tUp =(TH1F*)(htmp->Clone("W_CMS_scale_tUp"));//rename otherwise default name may conflict below
  W_CMS_scale_tUp=changeBinning(W_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getSample("TTJets",histoname));
  TH1F* TT_CMS_scale_tUp =(TH1F*)(htmp->Clone("TT_CMS_scale_tUp"));//rename otherwise default name may conflict below
  TT_CMS_scale_tUp=changeBinning(TT_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisUp->getDiBoson(histoname));
  TH1F* VV_CMS_scale_tUp =(TH1F*)(htmp->Clone("VV_CMS_scale_tUp"));//rename otherwise default name may conflict below
  VV_CMS_scale_tUp=changeBinning(VV_CMS_scale_tUp,rebin,xmax);
  delete htmp;

  ////////////////tau energy scaled down

  TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsVH")+mass,histoname));
  TH1F* VH_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(VH->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
  VH_CMS_scale_tDown=changeBinning(VH_CMS_scale_tDown,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsGG")+mass,histoname));
  TH1F* SM_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(SM->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
  SM_CMS_scale_tDown=changeBinning(SM_CMS_scale_tDown,rebin,xmax);//use 10GeV wide bins from 0-->300
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsVBF")+mass,histoname));
  TH1F* VBF_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(VBF->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
  VBF_CMS_scale_tDown=changeBinning(VBF_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getZToTauTau(histoname));
  TH1F* ZTT_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZTT_CMS_scale_tDown"));//rename otherwise default name may conflict below
  ZTT_CMS_scale_tDown=changeBinning(ZTT_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getSample("ZToMuMu",histoname));
  TH1F* ZL_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZL_CMS_scale_tDown"));//rename otherwise default name may conflict below
  ZL_CMS_scale_tDown=changeBinning(ZL_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getSample("ZToLJet",histoname));
  TH1F* ZJ_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZJ_CMS_scale_tDown"));//rename otherwise default name may conflict below
  ZJ_CMS_scale_tDown=changeBinning(ZJ_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getWJets(histoname,sm));
  TH1F* W_CMS_scale_tDown =(TH1F*)(htmp->Clone("W_CMS_scale_tDown"));//rename otherwise default name may conflict below
  W_CMS_scale_tDown=changeBinning(W_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getSample("TTJets",histoname));
  TH1F* TT_CMS_scale_tDown =(TH1F*)(htmp->Clone("TT_CMS_scale_tDown"));//rename otherwise default name may conflict below
  TT_CMS_scale_tDown=changeBinning(TT_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  TH1F* htmp = (TH1F*)(analysisDown->getDiBoson(histoname));
  TH1F* VV_CMS_scale_tDown =(TH1F*)(htmp->Clone("VV_CMS_scale_tDown"));//rename otherwise default name may conflict below
  VV_CMS_scale_tDown=changeBinning(VV_CMS_scale_tDown,rebin,xmax);
  delete htmp;

  ///////////////////////create root file and data card
  char rootfilename[256];
  sprintf(rootfilename,"muTau_SM%d_mH%d.root",sm,mass);  
  TFile output(rootfilename,"recreate");
  TDirectory* dir = output.mkdir((TString("muTau_SM")+sm).Data());  
  dir->cd();

  VH->Write();
  SM->Write();
  VBF->Write();
  ZTT ->Write();
  ZL->Write();
  ZJ->Write();
  W->Write();
  TT->Write();
  VV->Write();
  QCD->Write();
  data_obs->Write();

  ////Not sure the reprocessings used the same number of events ==> normalize to nominal for now
  VH_CMS_scale_tUp->Scale(VH->Integral()/VH_CMS_scale_tUp->Integral());
  SM_CMS_scale_tUp->Scale(SM->Integral()/SM_CMS_scale_tUp->Integral());
  VBF_CMS_scale_tUp->Scale(VBF->Integral()/VBF_CMS_scale_tUp->Integral());
  ZTT_CMS_scale_tUp->Scale(ZTT->Integral()/ZTT_CMS_scale_tUp->Integral());
  ZL_CMS_scale_tUp->Scale(ZL->Integral()/ZL_CMS_scale_tUp->Integral());
  ZJ_CMS_scale_tUp->Scale(ZJ->Integral()/ZJ_CMS_scale_tUp->Integral());
  W_CMS_scale_tUp->Scale(W->Integral()/W_CMS_scale_tUp->Integral());
  TT_CMS_scale_tUp->Scale(TT->Integral()/TT_CMS_scale_tUp->Integral());
  VV_CMS_scale_tUp->Scale(VV->Integral()/VV_CMS_scale_tUp->Integral());

  VH_CMS_scale_tDown->Scale(VH->Integral()/VH_CMS_scale_tDown->Integral());
  SM_CMS_scale_tDown->Scale(SM->Integral()/SM_CMS_scale_tDown->Integral());
  VBF_CMS_scale_tDown->Scale(VBF->Integral()/VBF_CMS_scale_tDown->Integral());
  ZTT_CMS_scale_tDown->Scale(ZTT->Integral()/ZTT_CMS_scale_tDown->Integral());
  ZL_CMS_scale_tDown->Scale(ZL->Integral()/ZL_CMS_scale_tDown->Integral());
  ZJ_CMS_scale_tDown->Scale(ZJ->Integral()/ZJ_CMS_scale_tDown->Integral());
  W_CMS_scale_tDown->Scale(W->Integral()/W_CMS_scale_tDown->Integral());
  TT_CMS_scale_tDown->Scale(TT->Integral()/TT_CMS_scale_tDown->Integral());
  VV_CMS_scale_tDown->Scale(VV->Integral()/VV_CMS_scale_tDown->Integral());

  VH_CMS_scale_tUp->Write();
  SM_CMS_scale_tUp->Write();
  VBF_CMS_scale_tUp->Write();
  ZTT_CMS_scale_tUp ->Write();
  ZL_CMS_scale_tUp->Write();
  ZJ_CMS_scale_tUp->Write();
  W_CMS_scale_tUp->Write();
  TT_CMS_scale_tUp->Write();
  VV_CMS_scale_tUp->Write();

  VH_CMS_scale_tDown->Write();
  SM_CMS_scale_tDown->Write();
  VBF_CMS_scale_tDown->Write();
  ZTT_CMS_scale_tDown ->Write();
  ZL_CMS_scale_tDown->Write();
  ZJ_CMS_scale_tDown->Write();
  W_CMS_scale_tDown->Write();
  TT_CMS_scale_tDown->Write();
  VV_CMS_scale_tDown->Write();

  output.Close();


  
  //////////////////////////////////////Data Card
  ofstream file;
  file.open((TString("muTau_SM")+sm+"_mH"+mass+".txt").Data());

  file << "imax 1" <<endl;
  file << "jmax *" <<endl;
  file << "kmax *" <<endl;
  file << "shapes *  *  "<< rootfilename << "  $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  file << endl;
  file << "observation " <<data_obs->Integral()<<endl;
  file<<endl;


  file <<fillStringLong("bin")<<fillString(" ")
       <<fillString(" ")
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))
       <<fillString((const char*)(TString("muTau_SM")+sm))<<endl;

  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillString((const char*)(TString("VH")+mass))
       <<fillString((const char*)(TString("SM")+mass))
       <<fillString((const char*)(TString("VBF")+mass))
       <<fillString("ZTT")
       <<fillString("QCD")
       <<fillString("W")
       <<fillString("ZJ")
       <<fillString("ZL")
       <<fillString("TT")
       <<fillString("VV")<<endl;

  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillInt(-2)
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
       <<fillFloat(VH->Integral())
       <<fillFloat(SM->Integral())
       <<fillFloat(VBF->Integral())
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
       <<fillFloat(1.045)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"luminosity"<<endl;

  //turn off the shape systematics on the small backgrounds due to instabilities
  file <<fillStringLong("CMS_scale_t")<<fillString("shape")
       <<fillString(" ")
       <<fillInt(1)
       <<fillInt(1)
       <<fillInt(1)
       <<fillInt(1)
       <<fillString("-")
       <<fillInt(1)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"shape"<<endl;

  file <<fillStringLong("CMS_eff_m")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.020)
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
       <<fillFloat(1.06)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<"Tau ID"<<endl;

  file <<fillStringLong("CMS_scale_j")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(0.96)
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

  std::string wsys=(TString("CMS_htt_muTau_SM")+sm+"_WNorm").Data();
  file <<fillStringLong(wsys)<<fillString("gmN")
       <<fillInt(W->Integral()/analysis->getWJetsScaleFactor(sm))
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(analysis->getWJetsScaleFactor(sm))
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"W Background"<<endl;
  
  std::string qcdsys=(TString("CMS_htt_muTau_SM")+sm+"_QCDNorm").Data();
  file <<fillStringLong(qcdsys)<<fillString("gmN")
       <<fillInt((QCD->Integral()/analysis->getQCDScaleFactor(sm))+0.5)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(analysis->getQCDScaleFactor(sm))
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD Background"<<endl;

  file <<fillStringLong("CMS_htt_zttNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
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
       <<fillString("-")
       <<fillFloat(1.30)
       <<"Norm DiBoson"<<endl;

  file <<fillStringLong("CMS_htt_ZJetFakeTau")<<fillString("lnN")
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
       <<fillString("-")
       <<fillFloat(1.10)
       <<fillString("-")
       <<fillString("-")
       <<"Z(lepton->tau) background"<<endl;

  file <<fillStringLong("CMS_scale_met")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.05)
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

  file <<fillStringLong("pdf_vh")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.08)
       <<fillString("-")
       <<fillString("-")
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
       <<fillString("-")
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

  file <<fillStringLong("pdf_qqbar")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
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


  file <<fillStringLong("QCDscale_ggH")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
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
       <<fillFloat(0.96)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"Underlying events & PS"<<endl;

  file.close();

  analysis->deleteSamples();  
  analysisUp->deleteSamples();  
  analysisDown->deleteSamples();  
  delete analysis;
  delete analysisUp;
  delete analysisDown;


  delete VH;
  delete SM;
  delete VBF;
  delete ZTT ;
  delete ZL;
  delete ZJ;
  delete W;
  delete TT;
  delete VV;
  delete QCD;
  delete data_obs;
  delete totalbkg;

  delete VH_CMS_scale_tUp;
  delete SM_CMS_scale_tUp;
  delete VBF_CMS_scale_tUp;
  delete ZTT_CMS_scale_tUp ;
  delete ZL_CMS_scale_tUp;
  delete ZJ_CMS_scale_tUp;
  delete W_CMS_scale_tUp;
  delete TT_CMS_scale_tUp;
  delete VV_CMS_scale_tUp;

  delete VH_CMS_scale_tDown;
  delete SM_CMS_scale_tDown;
  delete VBF_CMS_scale_tDown;
  delete ZTT_CMS_scale_tDown ;
  delete ZL_CMS_scale_tDown;
  delete ZJ_CMS_scale_tDown;
  delete W_CMS_scale_tDown;
  delete TT_CMS_scale_tDown;
  delete VV_CMS_scale_tDown;
  
  gROOT->ProcessLine(".q");
}

