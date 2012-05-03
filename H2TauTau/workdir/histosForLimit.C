#include <TF1.h>
#include <TString.h>
#include <TH1F.h>
#include <TFile.h>

void histosForLimit(long sm=0, long mass=120){

  TString rootfilename=TString("muTau/muTau_SM")+(long)sm+"_mH"+(long)mass+".root";

  gROOT->ProcessLine(".L ./tauMuConfig.C");
  TauMuPlotter *analysis =tauMuConfig("analysis");
  analysis->plotvar_="svfitmass";
  analysis->nbins_=30;
  analysis->xmin_=0.;
  analysis->xmax_=300.;
  analysis->Isocat_=1;
  analysis->MTcat_=1;
  analysis->SMcat_=sm;
  if(!analysis->scaleSamplesLumi())return 0;
  if(!analysis->correctSamplesInc())return 0;

  
  TH1F* ZTT = analysis->getZToTauTau();
  ZTT->SetName("ZTT");//rename otherwise default name may conflict below

  TH1F* ZL = analysis->getZToMuMuInc();
  ZL->SetName("ZL");//rename otherwise default name may conflict below

  TH1F* ZJ = analysis->getZToLJetInc();
  ZJ->SetName("ZJ");//rename otherwise default name may conflict below

  TH1F* W = 0;
  if(sm==0) W = analysis->getWJetsInc();
  if(sm==1 || sm==2) W = analysis->getWJetsTauIsoSM();
  W->SetName("W");//rename otherwise default name may conflict below

  TH1F* TT = analysis->getTTJetsInc();
  TT->SetName("TT");//rename otherwise default name may conflict below

  TH1F* VV = analysis->getDiBoson();
  VV->SetName("VV");//rename otherwise default name may conflict below

  TH1F* QCD = 0;
  if(sm==0)QCD=analysis->getQCDInc();
  if(sm==1 || sm ==2)QCD=analysis->getQCDIsoSM();
  QCD->SetName("QCD");//rename otherwise default name may conflict below
  
  TH1F* VH = analysis->getSample(TString("HiggsVH")+mass);
  VH->SetName(TString("VH")+mass);//rename otherwise default name may conflict below

  TH1F* SM = analysis->getSample(TString("HiggsGG")+mass);
  SM->SetName(TString("SM")+mass);//rename otherwise default name may conflict below

  TH1F* VBF = analysis->getSample(TString("HiggsVBF")+mass);
  VBF->SetName(TString("VBF")+mass);//rename otherwise default name may conflict below
  
  TH1F* data_obs = analysis->getTotalData();;
  data_obs->SetName("data_obs");//rename otherwise default name may conflict below


  //create root file and data card 
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

//   ////Not sure the reprocessings used the same number of events ==> normalize to nominal for now
//   VH_CMS_scale_tUp->Scale(VH->Integral()/VH_CMS_scale_tUp->Integral());
//   SM_CMS_scale_tUp->Scale(SM->Integral()/SM_CMS_scale_tUp->Integral());
//   VBF_CMS_scale_tUp->Scale(VBF->Integral()/VBF_CMS_scale_tUp->Integral());
//   ZTT_CMS_scale_tUp->Scale(ZTT->Integral()/ZTT_CMS_scale_tUp->Integral());
//   ZL_CMS_scale_tUp->Scale(ZL->Integral()/ZL_CMS_scale_tUp->Integral());
//   ZJ_CMS_scale_tUp->Scale(ZJ->Integral()/ZJ_CMS_scale_tUp->Integral());
//   W_CMS_scale_tUp->Scale(W->Integral()/W_CMS_scale_tUp->Integral());
//   TT_CMS_scale_tUp->Scale(TT->Integral()/TT_CMS_scale_tUp->Integral());
//   VV_CMS_scale_tUp->Scale(VV->Integral()/VV_CMS_scale_tUp->Integral());

//   VH_CMS_scale_tDown->Scale(VH->Integral()/VH_CMS_scale_tDown->Integral());
//   SM_CMS_scale_tDown->Scale(SM->Integral()/SM_CMS_scale_tDown->Integral());
//   VBF_CMS_scale_tDown->Scale(VBF->Integral()/VBF_CMS_scale_tDown->Integral());
//   ZTT_CMS_scale_tDown->Scale(ZTT->Integral()/ZTT_CMS_scale_tDown->Integral());
//   ZL_CMS_scale_tDown->Scale(ZL->Integral()/ZL_CMS_scale_tDown->Integral());
//   ZJ_CMS_scale_tDown->Scale(ZJ->Integral()/ZJ_CMS_scale_tDown->Integral());
//   W_CMS_scale_tDown->Scale(W->Integral()/W_CMS_scale_tDown->Integral());
//   TT_CMS_scale_tDown->Scale(TT->Integral()/TT_CMS_scale_tDown->Integral());
//   VV_CMS_scale_tDown->Scale(VV->Integral()/VV_CMS_scale_tDown->Integral());

//   VH_CMS_scale_tUp->Write();
//   SM_CMS_scale_tUp->Write();
//   VBF_CMS_scale_tUp->Write();
//   ZTT_CMS_scale_tUp ->Write();
//   ZL_CMS_scale_tUp->Write();
//   ZJ_CMS_scale_tUp->Write();
//   W_CMS_scale_tUp->Write();
//   TT_CMS_scale_tUp->Write();
//   VV_CMS_scale_tUp->Write();

//   VH_CMS_scale_tDown->Write();
//   SM_CMS_scale_tDown->Write();
//   VBF_CMS_scale_tDown->Write();
//   ZTT_CMS_scale_tDown ->Write();
//   ZL_CMS_scale_tDown->Write();
//   ZJ_CMS_scale_tDown->Write();
//   W_CMS_scale_tDown->Write();
//   TT_CMS_scale_tDown->Write();
//   VV_CMS_scale_tDown->Write();

  output.Close();



//   delete analysis;
//   delete analysisUp;
//   delete analysisDown;


//   delete VH;
//   delete SM;
//   delete VBF;
//   delete ZTT ;
//   delete ZL;
//   delete ZJ;
//   delete W;
//   delete TT;
//   delete VV;
//   delete QCD;
//   delete data_obs;
//   delete totalbkg;

//   delete VH_CMS_scale_tUp;
//   delete SM_CMS_scale_tUp;
//   delete VBF_CMS_scale_tUp;
//   delete ZTT_CMS_scale_tUp ;
//   delete ZL_CMS_scale_tUp;
//   delete ZJ_CMS_scale_tUp;
//   delete W_CMS_scale_tUp;
//   delete TT_CMS_scale_tUp;
//   delete VV_CMS_scale_tUp;

//   delete VH_CMS_scale_tDown;
//   delete SM_CMS_scale_tDown;
//   delete VBF_CMS_scale_tDown;
//   delete ZTT_CMS_scale_tDown ;
//   delete ZL_CMS_scale_tDown;
//   delete ZJ_CMS_scale_tDown;
//   delete W_CMS_scale_tDown;
//   delete TT_CMS_scale_tDown;
//   delete VV_CMS_scale_tDown;
  
  gROOT->ProcessLine(".q");
}



//   //normalizes backgrounds for the data
//   TauMuAnalysis* analysis = TauMuConfig("");
//   analysis->init();
//   analysis->scaleHistos();
//   analysis->setSmearHistoRes(smearRes);

//   TauMuAnalysis* analysisUp = TauMuConfig("tUp");
//   analysisUp->setOutputPath("./output/V241ABFallTrigEfftUp");
//   analysisUp->init();
//   analysisUp->scaleHistos();
//   analysisUp->setSmearHistoRes(smearRes);

//   TauMuAnalysis* analysisDown = TauMuConfig("tDown");
//   analysisDown->setOutputPath("./output/V241ABFallTrigEfftDown");
//   analysisDown->init();
//   analysisDown->scaleHistos();
//   analysisDown->setSmearHistoRes(smearRes);
  
  
//   TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsVH")+mass,variable));
//   TH1F* VH =(TH1F*)(htmp->Clone(TString("VH")+mass));//rename otherwise default name may conflict below
//   VH=changeBinning(VH,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsGG")+mass,variable));
//   TH1F* SM =(TH1F*)(htmp->Clone(TString("SM")+mass));//rename otherwise default name may conflict below
//   SM=changeBinning(SM,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysis->getSample(TString("HiggsVBF")+mass,variable));
//   TH1F* VBF =(TH1F*)(htmp->Clone(TString("VBF")+mass));//rename otherwise default name may conflict below
//   VBF=changeBinning(VBF,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysis->getZToTauTau(variable));
//   TH1F* ZTT =(TH1F*)(htmp->Clone("ZTT"));//rename otherwise default name may conflict below
//   ZTT=changeBinning(ZTT,rebin,xmax);
//   delete htmp;
  
//   TH1F* htmp = (TH1F*)(analysis->getSample("ZToMuMu",variable));
//   TH1F* ZL =(TH1F*)(htmp->Clone("ZL"));//rename otherwise default name may conflict below
//   ZL=changeBinning(ZL,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysis->getSample("ZToLJet",variable));
//   TH1F* ZJ =(TH1F*)(htmp->Clone("ZJ"));//rename otherwise default name may conflict below
//   ZJ=changeBinning(ZJ,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = analysis->getWJets(variable,sm);
//   TH1F* W =(TH1F*)(htmp->Clone("W"));//rename otherwise default name may conflict below
//   W=changeBinning(W,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysis->getSample("TTJets",variable));
//   TH1F* TT =(TH1F*)(htmp->Clone("TT"));//rename otherwise default name may conflict below
//   TT=changeBinning(TT,rebin,xmax);
//   delete htmp;
  
//   TH1F* htmp = (TH1F*)(analysis->getDiBoson(variable));
//   TH1F* VV =(TH1F*)(htmp->Clone("VV"));//rename otherwise default name may conflict below
//   VV=changeBinning(VV,rebin,xmax);
//   delete htmp;

//   TH1F* htmp=analysis->getQCD(variable,sm);
//   TH1F* QCD=(TH1F*)(htmp->Clone("QCD"));//rename otherwise default name may conflict below
//   QCD=changeBinning(QCD,rebin,xmax);
//   delete htmp;
  
  
//   /////blind the result, get fake data from background below
//   //TH1F* htmp=analysis->getTotalData(variable);
//   //TH1F* data_obs = (TH1F*)(htmp->Clone("data_obs"));//rename otherwise default name may conflict below
//   //data_obs=changeBinning(data_obs,rebin,xmax);
//   //delete htmp;
  
//   TH1F* htmp=analysis->getTotalBackground(variable,sm);
//   TH1F* totalbkg = (TH1F*)(htmp->Clone("totalbkg"));//rename otherwise default name may conflict below
//   totalbkg=changeBinning(totalbkg,rebin,xmax);
//   delete htmp;

//   ///blind the result
//   TH1F* data_obs = new TH1F("data_obs","",totalbkg->GetNbinsX(),totalbkg->GetXaxis()->GetXmin(),totalbkg->GetXaxis()->GetXmax());
//   data_obs->Sumw2();
//   data_obs->FillRandom(totalbkg,(int)(totalbkg->Integral()+0.5));


  
//   ////////////////tau energy scaled up

//   TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsVH")+mass,variable));
//   TH1F* VH_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(VH->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   VH_CMS_scale_tUp=changeBinning(VH_CMS_scale_tUp,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsGG")+mass,variable));
//   TH1F* SM_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(SM->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   SM_CMS_scale_tUp=changeBinning(SM_CMS_scale_tUp,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getSample(TString("HiggsVBF")+mass,variable));
//   TH1F* VBF_CMS_scale_tUp =(TH1F*)(htmp->Clone(TString(VBF->GetName())+"_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   VBF_CMS_scale_tUp=changeBinning(VBF_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getZToTauTau(variable));
//   TH1F* ZTT_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZTT_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   ZTT_CMS_scale_tUp=changeBinning(ZTT_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getSample("ZToMuMu",variable));
//   TH1F* ZL_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZL_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   ZL_CMS_scale_tUp=changeBinning(ZL_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getSample("ZToLJet",variable));
//   TH1F* ZJ_CMS_scale_tUp =(TH1F*)(htmp->Clone("ZJ_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   ZJ_CMS_scale_tUp=changeBinning(ZJ_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getWJets(variable,sm));
//   TH1F* W_CMS_scale_tUp =(TH1F*)(htmp->Clone("W_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   W_CMS_scale_tUp=changeBinning(W_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getSample("TTJets",variable));
//   TH1F* TT_CMS_scale_tUp =(TH1F*)(htmp->Clone("TT_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   TT_CMS_scale_tUp=changeBinning(TT_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisUp->getDiBoson(variable));
//   TH1F* VV_CMS_scale_tUp =(TH1F*)(htmp->Clone("VV_CMS_scale_tUp"));//rename otherwise default name may conflict below
//   VV_CMS_scale_tUp=changeBinning(VV_CMS_scale_tUp,rebin,xmax);
//   delete htmp;

//   ////////////////tau energy scaled down

//   TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsVH")+mass,variable));
//   TH1F* VH_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(VH->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   VH_CMS_scale_tDown=changeBinning(VH_CMS_scale_tDown,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsGG")+mass,variable));
//   TH1F* SM_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(SM->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   SM_CMS_scale_tDown=changeBinning(SM_CMS_scale_tDown,rebin,xmax);//use 10GeV wide bins from 0-->300
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getSample(TString("HiggsVBF")+mass,variable));
//   TH1F* VBF_CMS_scale_tDown =(TH1F*)(htmp->Clone(TString(VBF->GetName())+"_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   VBF_CMS_scale_tDown=changeBinning(VBF_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getZToTauTau(variable));
//   TH1F* ZTT_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZTT_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   ZTT_CMS_scale_tDown=changeBinning(ZTT_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getSample("ZToMuMu",variable));
//   TH1F* ZL_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZL_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   ZL_CMS_scale_tDown=changeBinning(ZL_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getSample("ZToLJet",variable));
//   TH1F* ZJ_CMS_scale_tDown =(TH1F*)(htmp->Clone("ZJ_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   ZJ_CMS_scale_tDown=changeBinning(ZJ_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getWJets(variable,sm));
//   TH1F* W_CMS_scale_tDown =(TH1F*)(htmp->Clone("W_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   W_CMS_scale_tDown=changeBinning(W_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getSample("TTJets",variable));
//   TH1F* TT_CMS_scale_tDown =(TH1F*)(htmp->Clone("TT_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   TT_CMS_scale_tDown=changeBinning(TT_CMS_scale_tDown,rebin,xmax);
//   delete htmp;

//   TH1F* htmp = (TH1F*)(analysisDown->getDiBoson(variable));
//   TH1F* VV_CMS_scale_tDown =(TH1F*)(htmp->Clone("VV_CMS_scale_tDown"));//rename otherwise default name may conflict below
//   VV_CMS_scale_tDown=changeBinning(VV_CMS_scale_tDown,rebin,xmax);
//   delete htmp;
