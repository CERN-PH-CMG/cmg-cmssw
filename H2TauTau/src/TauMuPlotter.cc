#include "CMGTools/H2TauTau/interface/TauMuPlotter.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>
#include <TVectorD.h>

TauMuPlotter::TauMuPlotter():
  TNamed("","")
{
}

TauMuPlotter::TauMuPlotter(const char * name):
  TNamed(name,name),
  smearHistoRes_(0.),
  QCDOStoSSRatio_(0),
  QCDMuIsoSideRatio_(0.),
  WJetsOSSideCorr_(1.),
  WJetsSSSideCorr_(1.),
  plotvar_("ditaumass"),
  nbins_(100),
  xmin_(0.),
  xmax_(1000.),
  selection_("eventweight"),
  Isocat_(-1),
  MTcat_(-1),
  SMcat_(-1)
{
 
}

TauMuPlotter::~TauMuPlotter(){
}


bool TauMuPlotter::printRawYields(){
  
  Float_t totalData=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      totalData+=hmass->Integral(0,hmass->GetNbinsX()+1);
      delete hmass;
    }
  }
  cout<<"Total Data  = "<<(int)(totalData)<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  } 

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MCCat"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }  

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Signal"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }
  

  return 1;
}

Float_t TauMuPlotter::getWJetsScaleFactor(){
  float ratio=0.;

  if(SMcat_<=0) ratio = getWJetsSignalToSBFraction();
  
  if(SMcat_==1){
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if(TString((*s)->GetName())=="W2JetsToLNu")
	ratio=(*s)->getNorm()*getWJetsOSCorrFactor();
  }
  
  if(SMcat_==2){
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if(TString((*s)->GetName())=="W3JetsToLNu")
	ratio=(*s)->getNorm()*getWJetsOSCorrFactor();
  }
  
  cout<<"WJets Scale Factor : "<<ratio<<endl;
  
  return ratio;
}

Float_t TauMuPlotter::getWJetsSignalToSBFraction(){
  
  float ratio=1.;

  if(SMcat_==1||SMcat_==2)return ratio;

  //////needed by the data-card for limits before any rescalings
  Float_t WJetsSignal=0.;  Float_t WJetsSideBand=0.;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1F* hsig=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1&&categoryMT==1)"));
      WJetsSignal=hsig->Integral();
      delete hsig;

      TH1F* hsid=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1&&categoryMT==3)"));
      WJetsSideBand=hsid->Integral();
      delete hsid;
    }
  }
  if(WJetsSideBand==0.)return ratio=1.;
  else ratio=WJetsSignal/WJetsSideBand;

  cout<<" Ratio of WJets signal to sideband:  "<<ratio<<endl;
 
  return ratio;
}


bool TauMuPlotter::scaleHistos(){

  /////////determine total lumi and scale MCs
  float totalDataLumi=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data")
      totalDataLumi+=(*s)->getLumi();
  cout<<"Total Data lumi = "<<totalDataLumi<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" 
       || (*s)->getDataType()=="MCCat" 
       || (*s)->getDataType()=="Signal" ){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
    }


  /////Determine correction factor for WJets from massT sideband, this is done at Inclusive level
  cout<<"Determining WJets  from sidebands"<<endl;
  WJetsOSSideCorr_=1.; Float_t WJetsOSSide=0; Float_t MCOSSide=0; Float_t DataOSSide=0;
  WJetsSSSideCorr_=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->getDataType())=="Data"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1&&categoryMT==3)");
      DataOSSide+=hos->Integral();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryIso==1&&categoryMT==3)");
      DataSSSide+=hss->Integral();
      delete hss;
    }
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1&&categoryMT==3)");
      WJetsOSSide=hos->Integral()*(*s)->getNorm();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryIso==1&&categoryMT==3)");
      WJetsSSSide=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
    if(TString((*s)->getDataType())=="MC" && TString((*s)->GetName())!="WJetsToLNu"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1&&categoryMT==3)");
      MCOSSide+=hos->Integral()*(*s)->getNorm();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryIso==1&&categoryMT==3)");
      MCSSSide+=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
  }  
  WJetsOSSideCorr_=(DataOSSide-MCOSSide)/WJetsOSSide;  
  WJetsSSSideCorr_=(DataSSSide-MCSSSide)/WJetsSSSide;
  cout<<"WJetsOS Correction = "<<WJetsOSSideCorr_<<endl;
  cout<<"WJetsSS Correction = "<<WJetsSSSideCorr_<<endl;

  ///////////////////////////////////
  ////Scale the embedded samples:
  ///Should there be separate scale factor for OS and SS ?
  /////////////////////////////////
  Float_t ZToTauTauMC=0.;
  Float_t EmbeddedOS=0.;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*h=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1)"));
      EmbeddedOS+=h->Integral();
      delete h;
    }
    if(TString((*s)->GetName())=="ZToTauTau"){
      TH1F*h=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==1&&categoryIso==1)"));
      ZToTauTauMC=h->Integral()*(*s)->getNorm();
      delete h;
    }
  }
  if(ZToTauTauMC==0. || EmbeddedOS==0.){
    cout<<"Warning!!!: bad ZToTauTauMC or EmbeddedOS yields: ZToTauTauMC="<<ZToTauTauMC<<" EmbeddedOS="<<EmbeddedOS<<endl;
  } else {
    cout<<"Rescaling Embedded samples by factor : "<<ZToTauTauMC/EmbeddedOS<<endl;
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if((*s)->getDataType()=="Embedded")
	(*s)->scale(ZToTauTauMC/EmbeddedOS);
  }


  //extrapolation factor for Boosted/VBF QCD is determined at the inclusive level
  //from the QCD distribution obtained from the SS samples
  Float_t QCDDataSSSig=0.; Float_t QCDDataSSSide=0.;
  Float_t QCDMCSSSig=0.; Float_t QCDMCSSSide=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hsig=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryMT==1&&categoryIso==1)");
      QCDDataSSSig+=hsig->Integral();
      delete hsig;
      TH1F*hsid=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryMT==1&&categoryIso==2)");
      QCDDataSSSide+=hsid->Integral();
      delete hsid;
    }
    if((*s)->getDataType()=="MC"){
      TH1F*hsig=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryMT==1&&categoryIso==1)");
      QCDMCSSSig+=hsig->Integral()*(*s)->getNorm();
      delete hsig;
      TH1F*hsid=(*s)->getHistoNtpFile("ditaumass",100,0,1000,"eventweight*(categoryCh==2&&categoryMT==1&&categoryIso==2)");
      QCDMCSSSide+=hsid->Integral()*(*s)->getNorm();
      delete hsid;
    }  
  }   
  QCDMuIsoSideRatio_=(QCDDataSSSig-QCDMCSSSig)/(QCDDataSSSide-QCDMCSSSide);
  cout<<" QCDMuIsoSideRatio : "<<QCDMuIsoSideRatio_<<endl;

  return 1;
}


TH1F* TauMuPlotter::getTotalDataSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F(plotvar_+"hData_SS","Data SS",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
  cout<<"TotalDataSS : "<<h->Integral()<<endl;

  return h;
}

TH1F* TauMuPlotter::getTotalData(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F(plotvar_+"hData","Data",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
 
  cout<<"TotalData : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getTotalMC(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0) sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F(plotvar_+"hTotalMC","TotalMC",nbins_,xmin_,xmax_);
  h->Sumw2();

//   TH1F*hZToTauTau=getZToTauTau();
//   if(!hZToTauTau)return 0;
//   h->Add(hZToTauTau);
//   delete hZToTauTau;

//   TH1F*hWJets=getWJets();
//   if(!hWJets)return 0;
//   h->Add(hWJets);
//   delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"){// && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"Total MC : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getTotalMCSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hMC_SS","",nbins_,xmin_,xmax_);
  h->Sumw2();

//   TH1F*hZToTauTau=getZToTauTauSS();
//   if(!hZToTauTau)return 0;
//   h->Add(hZToTauTau);
//   delete hZToTauTau;

//   TH1F*hWJets=getWJetsSS();
//   if(!hWJets)return 0;
//   h->Add(hWJets);
//   delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"){// && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu" ){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"MCSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalEmbedded(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hEmbedded","",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"TotalEmbedded : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalEmbeddedSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hEmbeddedSS","",nbins_,xmin_,xmax_);
  h->Sumw2();
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());  
      delete hd;
    }
 
  cout<<"TotalEmbeddedSS : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}

TH1F* TauMuPlotter::getSample(TString samplename){

  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F(samplename+plotvar_,"",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
      h->SetTitle((*s)->getPlotLabel());
      delete hd;
    }  
  cout<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}

TH1F* TauMuPlotter::getSampleSS(TString samplename){

  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F(samplename+"SS"+plotvar_,"",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
      h->SetTitle((*s)->getPlotLabel());
      delete hd;
    }  
  cout<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}

TH1F* TauMuPlotter::getQCD(){

  TH1F* h=0;
  if(SMcat_<=0)h=getQCDSS();
  if(SMcat_==1){
    //NOTE that changing the method has to be done in sync with the getQCDScaleFactor() method below
    TH1F* hs=getQCDSide();
    //TH1F* hs=getQCDFullSide();
    
    h=smearHisto(hs);
    delete hs;
  }

  if(SMcat_==2){
    //NOTE that changing the method has to be done in sync with the getQCDScaleFactor() method below
    TH1F* hs=getQCDSide();
    //TH1F* hs=getQCDFullSide(plotvar_,SMcat_);
    h=smearHisto(hs);
    delete hs;
  }

  double factor=getQCDScaleFactor();   
  cout<<"QCD scale factor: "<<factor<<endl;

  //qcd integral before scaling here must be integer for datacards
  int qcdint=(int)(h->Integral()+0.5);
  h->Scale((qcdint/h->Integral())*factor);

  cout<<"QCD : "<<h->Integral()<<endl;

  return h;
}


//////////Different QCD determinations

Float_t TauMuPlotter::getQCDScaleFactor(){
  float ratio=0.; 
  if(SMcat_<=0)ratio=QCDOStoSSRatio_;
  if(SMcat_==1 || SMcat_==2) ratio=QCDMuIsoSideRatio_;

  return ratio;
}


TH1F* TauMuPlotter::getQCDSS(){

  TH1F* h=new TH1F("hQCDSS","QCDSS",nbins_,xmin_,xmax_);
  h->Sumw2();
  
  TH1F* hDataSS=getTotalDataSS();
  h->Add(hDataSS);
  delete hDataSS;

  TH1F* hMCSS=getTotalBackgroundSS();
  h->Add(hMCSS,-1.);
  delete hMCSS;

  return h;
}

TH1F* TauMuPlotter::getQCDSide(){
  TString sel="eventweight*(categoryCh==1&&categoryIso==3)";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
 
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd);
      delete hd;
    }
  }

  return h;
}


// TH1F* TauMuPlotter::getQCDFullSide(){

//   TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(plotvar_+"QCD2"));
//   if(!href){cout<<" plotvar_ not found in first sample"<<endl; return 0;}
//   TH1F* h=new TH1F("hSMQCD2","SMQCD2",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   h->Sumw2();

//   TH1F* hData=getTotalData(plotvar_+"QCD2");
//   h->Add(hData);
//   delete hData;

//   TH1F* hMC=getTotalMC(plotvar_+"QCD2",SMcat);
//   h->Add(hMC,-1);
//   delete hMC;

//   return h;
// }

TH1F* TauMuPlotter::getDiBoson(){

  TH1F* h=new TH1F("hVV","VV",nbins_,xmin_,xmax_);
  h->Sumw2();  

  TH1F* hWW=getSample("WW");
  if(!hWW)return 0;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSample("WZ");
  if(!hWZ)return 0;
  h->Add(hWZ);
  delete hWZ;

  TH1F* hZZ=getSample("ZZ");
  if(!hZZ)return 0;
  h->Add(hZZ);
  delete hZZ;

  cout<<"DiBoson : "<<h->Integral(0,h->GetNbinsX()+1)<<endl;

  return h;
}


TH1F* TauMuPlotter::getZToTauTau(){  
  //TH1F*h=getSample("ZToTauTau");
  TH1F*h=getTotalEmbedded(); 
  return h;
}

TH1F* TauMuPlotter::getZToTauTauSS(){  
  //TH1F*h=getSampleSS("ZToTauTau");
  TH1F*h=getTotalEmbeddedSS();  
  return h;
}

TH1F* TauMuPlotter::getWJets(){

  TH1F*h=0;
  if(SMcat_<=0){
    h=getSample("WJetsToLNu");
  }
  if(SMcat_==1){
    TH1F*hs=getSample("W2JetsToLNu");
    h=smearHisto(hs);
    delete hs;
  }
  if(SMcat_==2){
    TH1F*hs=getSample("W3JetsToLNu");
    h=smearHisto(hs);
    delete hs;
  }
  h->Scale(getWJetsOSCorrFactor());

  return h;
}

TH1F* TauMuPlotter::getWJetsSS(){

  TH1F*h=0;
  if(SMcat_<=0)h=getSampleSS("WJetsToLNu");
  if(SMcat_==1)h=getSampleSS("W2JetsToLNu");
  if(SMcat_==2)h=getSampleSS("W3JetsToLNu");
  h->Scale(getWJetsSSCorrFactor());

  return h;
}


TH1F* TauMuPlotter::getTotalBackground(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hBackground","",nbins_,xmin_,xmax_);
  h->Sumw2();

  TH1F*hQCD=getQCD();
  if(!hQCD)return 0;
  h->Add(hQCD);
  delete hQCD;

  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;

  TH1F*hWJets=getWJets();
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm()); 
      delete hd;
    }

  
  return h;
}



TH1F* TauMuPlotter::getTotalBackgroundSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";

  TH1F* h=new TH1F("hBackgroundSS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  //NOTE that QCD SS cannot be accounted because QCD is determined from SS samples
//   TH1F*hQCD=getQCD();
//   if(!hQCD)return 0;
//   h->Add(hQCD);
//   delete hQCD;

  TH1F*hZToTauTauSS=getZToTauTauSS();
  if(!hZToTauTauSS)return 0;
  h->Add(hZToTauTauSS);
  delete hZToTauTauSS;

  TH1F*hWJetsSS=getWJetsSS();
  if(!hWJetsSS)return 0;
  h->Add(hWJetsSS);
  delete hWJetsSS;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm()); 
      delete hd;
    }

  
  return h;
}



bool TauMuPlotter::plot(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
  
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;

  if(!scaleHistos())return 0;

  TCanvas C("C",plotvar_);
  TString filename=outputpath_+"/TauMuPlotter_"+plotvar_;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  C.Print(filename+"[");
  
  
  ////Show Data SS and MC SS
  TH1F* hQCDData=0;
  TH1F* hQCDMC=0;
  if(SMcat_<=0){
    hQCDData=getTotalDataSS();
    hQCDMC=getTotalBackgroundSS();
  }
  if(SMcat_==1 || SMcat_==2) hQCDData=getQCDSide();
  C.Clear();
  hQCDData->Draw("pe");
  if(hQCDMC){
    hQCDMC->Draw("hist same");
  }
  C.Print(filename); 
  delete hQCDData;
  if(hQCDMC)delete hQCDMC;
  

  ////Total Data
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
 
  ////Total MC
  ///must get here before getting individual components below because same histo object name is used for each component inside
  TH1F* hBkg=getTotalBackground();
  if(!hBkg){cout<<" Total Background not determined "<<endl; return 0;}
  hBkg->SetLineWidth(1);

  
  //
  TH1F*hQCD=getQCD();
  if(!hQCD)return 0;
  hQCD->SetLineWidth(1);
  hQCD->SetLineColor(QCDColor_);
  hQCD->SetFillColor(QCDColor_);

  TH1F*hWJetsToLNu=getWJets();
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);

  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBoson();
  if(!hVV)return 0;
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSample("ZToLJet");
  if(!hZToLJet)return 0;
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSample("ZToMuMu");
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;

  //
  TH1F*hTTJets=getSample("TTJets");
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
 

  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);


  cout<<" Summary of yields : "<<endl;
  cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;

  
  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","f");
  legend.AddEntry(hEWK,"ElectroWeak","f");
  legend.AddEntry(hQCD,"QCD","f");


  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  //hData->GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(filename);  
  if(log) C.SetLogy(0);

  
  TH1F*hRatio=(TH1F*)hData->Clone("hRatio");//get here before make stats 
  TH1F*hResid=(TH1F*)hData->Clone("hResid");//get here before make stats 
  TLine line;

  ///////////////////////////////////
  //show the histogram stats
  C.Clear();
  hData->SetStats(1);
  hData->Draw();
  C.Print(filename); 
  C.Clear();
  hBkg->GetXaxis()->SetRange(hData->GetXaxis()->GetFirst(),hData->GetXaxis()->GetLast());
  hBkg->GetYaxis()->SetRange(hData->GetYaxis()->GetFirst(),hData->GetYaxis()->GetLast());
  hBkg->SetStats(1);
  hBkg->Draw();
  C.Print(filename); 


  ////////////////////////////////////
  cout<<"Making ratio plot"<<endl;
  C.Clear();
  hRatio->Divide(hBkg);
  hRatio->GetYaxis()->SetTitle("Data/Background");
  hRatio->SetStats(0);
  hRatio->GetYaxis()->SetRangeUser(.5,1.5);
  hRatio->Draw("hist pe");
  line.DrawLine(hRatio->GetXaxis()->GetXmin(),1,hRatio->GetXaxis()->GetXmax(),1);
  C.Print(filename);  
  delete hRatio;

  cout<<"Making residual plot"<<endl;
  C.Clear();
  hResid->Add(hBkg,-1);
  hResid->GetYaxis()->SetTitle("Data - Background");
  hResid->SetStats(0);
  hResid->Draw("hist pe");
  line.DrawLine(hResid->GetXaxis()->GetXmin(),0,hResid->GetXaxis()->GetXmax(),0);
  C.Print(filename);  
  delete hResid;


 
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  //clean up
  delete hQCD;
  delete hEWK;
  delete hTTJets;
  delete hZToTauTau;
  delete hBkg;
  delete hData;


  return 1;

}



TH1F* TauMuPlotter::smearHisto(TH1F* h){
  if(!h){
    cout<<"BaseAnalysis::smearHisto : histo is NULL"<<endl;
    return 0;
  }

 
  TH1F* hs=0;
 
  if(smearHistoRes_==0.){//just copy
    hs=(TH1F*)h->Clone(TString(h->GetName())+"smeared");
  }else {
    hs=new TH1F(TString(h->GetName())+"smeared",h->GetTitle(),h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    TF1 gaus("gauss","[0]*exp(-0.5*(x-[1])**2/[2]**2)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    gaus.SetParameter(2,smearHistoRes_);

    for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
      gaus.SetParameter(0,h->GetBinContent(b));
      gaus.SetParameter(1,h->GetBinCenter(b));
      for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
        hs->AddBinContent(bs,gaus.Eval(hs->GetBinCenter(bs)));
      }
    }
    for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
      hs->SetBinError(bs,0.);//not sure this is necessary
    }
    hs->Scale(h->Integral()/hs->Integral());
  }
  
  return hs;
}
