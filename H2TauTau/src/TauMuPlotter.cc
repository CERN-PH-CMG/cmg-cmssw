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
  QCDOSTauIsoSideRatio_(0),
  QCDSSTauIsoSideRatio_(0),
  WJetsOSSideCorr_(1.),
  WJetsSSSideCorr_(1.),
  WJetsSMOSSideCorr_(1.),
  WJetsSMSSSideCorr_(1.),
  plotvar_("ditaumass"),
  nbins_(100),
  xmin_(0.),
  xmax_(1000.),
  selection_("eventweight"),
  Isocat_(-1),
  MTcat_(-1),
  SMcat_(-1),
  extrasel_("1")
{
 
  //qcdTauIsoRatio_="(0.190265-0.00241721*taupt)*(taupt<80)";
  //p0                        =     0.190265   +/-   0.0110291   
  //p1                        =  -0.00241721   +/-   0.000393394 

  //vs njet
  //qcdTauIsoRatio_="((njet==0)*0.142+(njet==1)*0.090+(njet==2)*0.075+(njet==3)*0.095)";
  //qcdTauIsoRatio_="((njet==0)*0.138+(njet==1)*0.086+(njet==2)*0.086+(njet==3)*0.046)";
  //Feb28
  //qcdTauIsoRatio_="((njet==0)*0.141179+(njet==1)*0.090182+(njet==2)*0.074195+(njet==3)*0.07)"; //last number is a guess
  //njet and pt dependent
  //n=0
  //qcdTauIsoRatio_="((njet==0)*(0.139577*(20<taupt&&taupt<=30) + 0.127993*(30<taupt&&taupt<=40) + 0.119574*(40<taupt&&taupt<=50) + 0.982777*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet==1)*(0.0988439*(20<taupt&&taupt<=30) + 0.0762868*(30<taupt&&taupt<=40) + 0.085495*(40<taupt&&taupt<=50) + 0.0186197*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet==2)*(0.0769998*(20<taupt&&taupt<=30) + 0.0700322*(30<taupt&&taupt<=40) + 0.0491831*(40<taupt&&taupt<=50) + 0.0*(50<taupt&&taupt<=60) + 0.*(60<taupt)) + (njet>2)*0.0)";


//   ///tau jet pt dependent inclusive ratio
//  qcdTauIsoRatio_="( 1.56883*(20<taujetpt&&taujetpt<=30) + 0.281955*(30<taujetpt&&taujetpt<=40) + 0.0985824*(40<taujetpt&&taujetpt<=50) + 0.0462377*(50<taujetpt&&taujetpt<=60) + 0.0288263*(60<taujetpt&&taujetpt<=70) + 0.0208332*(70<taujetpt&&taujetpt<=80) + 0.0145108*(80<taujetpt&&taujetpt<=90) + 0.0209432*(90<taujetpt&&taujetpt<=100) + 0.0*(100<taujetpt) )";

//   ///fit to the njet=0 sample [0]*exp([1]*x+[2]*x*x)
//   NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
//    1  p0           3.42541e+02   1.04396e+02   8.21319e-03  -5.28112e-07
//    2  p1          -2.67974e-01   1.43404e-02  -1.18736e-06  -7.39962e-03
//    3  p2           1.87351e-03   1.63759e-04   1.46712e-08  -3.64506e-01
//  qcdTauIsoRatio_="(20<taujetpt&&taujetpt<80)*(3.42541e+02*exp(-2.67974e-01*taujetpt+1.87351e-03*taujetpt*taujetpt))";
//--> problem with first bin  

  //fine binning for inclusive
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*10.14+(25<taujetpt&&taujetpt<=30)*1.37238+(30<taujetpt&&taujetpt<=35)*0.425582+(35<taujetpt&&taujetpt<=40)*0.201691+(40<taujetpt&&taujetpt<=45)*0.117622+(45<taujetpt&&taujetpt<=50)*0.0729338+(50<taujetpt&&taujetpt<=55)*0.0500108+(55<taujetpt&&taujetpt<=60)*0.0330328+(60<taujetpt&&taujetpt<=65)*0.0300232+(65<taujetpt&&taujetpt<=70)*0.0170739+(70<taujetpt&&taujetpt<=75)*0.020635+(75<taujetpt&&taujetpt<=80)*0.0148927+(80<taujetpt&&taujetpt<=85)*0.0127072+(85<taujetpt&&taujetpt<=90)*0.0139045+(90<taujetpt&&taujetpt<=95)*0.0185733+(95<taujetpt&&taujetpt<=100)*0.0157922)";

  //fine fit to njet=0
  //qcdTauIsoRatio_="((njet==0)*1.0+(njet==1||njet==2)*1.067)*((20<taujetpt&&taujetpt<=25)*11.5247+(25<taujetpt&&taujetpt<=30)*1.37424+(30<taujetpt&&taujetpt<=35)*0.41256+(35<taujetpt&&taujetpt<=40)*0.19751+(40<taujetpt&&taujetpt<=45)*0.115416+(45<taujetpt&&taujetpt<=50)*0.0759201+(50<taujetpt&&taujetpt<=55)*0.0424669+(55<taujetpt&&taujetpt<=60)*0.0328491+(60<taujetpt&&taujetpt<=65)*0.0328291+(65<taujetpt&&taujetpt<=70)*0.0152393+(70<taujetpt&&taujetpt<=75)*0.0153301+(75<taujetpt&&taujetpt<=80)*0.0128618+(80<taujetpt&&taujetpt<=85)*0+(85<taujetpt&&taujetpt<=90)*0+(90<taujetpt&&taujetpt<=95)*0+(95<taujetpt&&taujetpt<=100)*0)";

  //fine ratio to njet=1
  //qcdTauIsoRatio_="(njet==1||njet==2)*((20<taujetpt&&taujetpt<=25)*6.06837+(25<taujetpt&&taujetpt<=30)*1.21503+(30<taujetpt&&taujetpt<=35)*0.510201+(35<taujetpt&&taujetpt<=40)*0.223787+(40<taujetpt&&taujetpt<=45)*0.122981+(45<taujetpt&&taujetpt<=50)*0.0639785+(50<taujetpt&&taujetpt<=55)*0.0677511+(55<taujetpt&&taujetpt<=60)*0.038791+(60<taujetpt&&taujetpt<=65)*0.0251048+(65<taujetpt&&taujetpt<=70)*0.0181727+(70<taujetpt&&taujetpt<=75)*0.0206081+(75<taujetpt&&taujetpt<=80)*0.02035+(80<taujetpt&&taujetpt<=85)*0.0385446+(85<taujetpt&&taujetpt<=90)*0.0129727+(90<taujetpt&&taujetpt<=95)*0.0204044+(95<taujetpt&&taujetpt<=100)*0.0438523)";
 
  //ratio from Anti-Iso Muons inclusive
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*10.5064+(25<taujetpt&&taujetpt<=30)*1.31087+(30<taujetpt&&taujetpt<=35)*0.40539+(35<taujetpt&&taujetpt<=40)*0.184538+(40<taujetpt&&taujetpt<=45)*0.103179+(45<taujetpt&&taujetpt<=50)*0.062483+(50<taujetpt&&taujetpt<=55)*0.0418848+(55<taujetpt&&taujetpt<=60)*0.0308868+(60<taujetpt&&taujetpt<=65)*0.026565+(65<taujetpt&&taujetpt<=70)*0.0206908+(70<taujetpt&&taujetpt<=75)*0.0157965+(75<taujetpt&&taujetpt<=80)*0.0143084+(80<taujetpt&&taujetpt<=85)*0.0125011+(85<taujetpt&&taujetpt<=90)*0.0123864+(90<taujetpt&&taujetpt<=95)*0.0108087+(95<taujetpt&&taujetpt<=100)*0.010392)";

  //ratio from Anti-Iso Muons njet=0
  //qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*10.1928+(25<taujetpt&&taujetpt<=30)*1.29939+(30<taujetpt&&taujetpt<=35)*0.396667+(35<taujetpt&&taujetpt<=40)*0.180465+(40<taujetpt&&taujetpt<=45)*0.101745+(45<taujetpt&&taujetpt<=50)*0.0627062+(50<taujetpt&&taujetpt<=55)*0.0396773+(55<taujetpt&&taujetpt<=60)*0.0304729+(60<taujetpt&&taujetpt<=65)*0.0272851+(65<taujetpt&&taujetpt<=70)*0.0202714+(70<taujetpt&&taujetpt<=75)*0.0154489+(75<taujetpt&&taujetpt<=80)*0.0138551+(80<taujetpt&&taujetpt<=85)*0.00838189+(85<taujetpt&&taujetpt<=90)*0.00806126+(90<taujetpt&&taujetpt<=95)*0.00781547+(95<taujetpt&&taujetpt<=100)*0.0093825)";

  //ratio from Anti-Iso Muons njet=1
  qcdTauIsoRatio_="((20<taujetpt&&taujetpt<=25)*11.3082+(25<taujetpt&&taujetpt<=30)*1.28471+(30<taujetpt&&taujetpt<=35)*0.45327+(35<taujetpt&&taujetpt<=40)*0.197425+(40<taujetpt&&taujetpt<=45)*0.108273+(45<taujetpt&&taujetpt<=50)*0.0608861+(50<taujetpt&&taujetpt<=55)*0.0477466+(55<taujetpt&&taujetpt<=60)*0.0329757+(60<taujetpt&&taujetpt<=65)*0.0246043+(65<taujetpt&&taujetpt<=70)*0.0203719+(70<taujetpt&&taujetpt<=75)*0.0150402+(75<taujetpt&&taujetpt<=80)*0.0167879+(80<taujetpt&&taujetpt<=85)*0.0203463+(85<taujetpt&&taujetpt<=90)*0.0151795+(90<taujetpt&&taujetpt<=95)*0.00986912+(95<taujetpt&&taujetpt<=100)*0.0103063)";
  
}

TauMuPlotter::~TauMuPlotter(){
}

/////////////General methods start here

bool TauMuPlotter::scaleSamplesLumi(){

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
      //cout<<(*s)->GetName()<<" lumi scale "<<totalDataLumi/(*s)->getLumi()<<endl;
      (*s)->resetScale();
      (*s)->scale(totalDataLumi/(*s)->getLumi());
    }



  ///////////////////////////////////
  ////Scale the embedded samples:  Should there be separate scale factor for OS and SS ? --> should be Ok. ZTauTau is small in SS
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
      if((*s)->getDataType()=="Embedded"){
	(*s)->resetScale();
	(*s)->scale(ZToTauTauMC/EmbeddedOS);
      }
  }



  return 1;
}




TH1F* TauMuPlotter::getTotalData(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0)    sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0)     sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)    sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hData","Data",nbins_,xmin_,xmax_);
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


TH1F* TauMuPlotter::getTotalDataSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hDataSS","Data SS",nbins_,xmin_,xmax_);
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

TH1F* TauMuPlotter::getTotalMC(){
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0) sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hTotalMC","TotalMC",nbins_,xmin_,xmax_);
  h->Sumw2();

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
  TString sel="eventweight*(categoryCh==1)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hMC_SS","",nbins_,xmin_,xmax_);
  h->Sumw2();

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
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

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
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

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
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F(samplename,"",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
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
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F(samplename+"SS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){
      TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      h->Add(hd,(*s)->getNorm());
      delete hd;
    }  
  cout<<samplename<<" : "<<h->Integral()<<endl;

  return h;
}

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

TH1F* TauMuPlotter::getDiBosonSS(){

  TH1F* h=new TH1F("hVV","VV",nbins_,xmin_,xmax_);
  h->Sumw2();  

  TH1F* hWW=getSampleSS("WW");
  if(!hWW)return 0;
  h->Add(hWW);
  delete hWW;

  TH1F* hWZ=getSampleSS("WZ");
  if(!hWZ)return 0;
  h->Add(hWZ);
  delete hWZ;

  TH1F* hZZ=getSampleSS("ZZ");
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



///////////////////Methods for inclusive categories start here
bool TauMuPlotter::correctSamplesInc(){

  //determine these factors here as they may be used by SM methods too
  /////Correct WJets using m_T high side with inclusive level selections
  cout<<"Determining WJets  from sidebands"<<endl;
  TString WJetsOSCorrSel=TString("eventweight*(categoryCh==1&&categoryMT==3)");
  TString WJetsSSCorrSel=TString("eventweight*(categoryCh==2&&categoryMT==3)");
  if(Isocat_>0){//scale factors depend on the isolation
    WJetsOSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
    WJetsSSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
  }
  if(extrasel_.CompareTo("1")!=0){
    WJetsOSCorrSel+=TString("*")+extrasel_;
    WJetsSSCorrSel+=TString("*")+extrasel_;
  }

  WJetsOSSideCorr_=1.; Float_t WJetsOSSide=0; Float_t MCOSSide=0; Float_t DataOSSide=0;
  WJetsSSSideCorr_=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->getDataType())=="Data"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      DataOSSide+=hos->Integral();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      DataSSSide+=hss->Integral();
      delete hss;
    }
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      WJetsOSSide=hos->Integral()*(*s)->getNorm();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      WJetsSSSide=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
    if(TString((*s)->getDataType())=="MC" && TString((*s)->GetName())!="WJetsToLNu"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      MCOSSide+=hos->Integral()*(*s)->getNorm();
      delete hos;
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      MCSSSide+=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
  }  
  WJetsOSSideCorr_=(DataOSSide-MCOSSide)/WJetsOSSide;  
  WJetsSSSideCorr_=(DataSSSide-MCSSSide)/WJetsSSSide;
  cout<<"WJetsIncOS  Correction = "<<WJetsOSSideCorr_<<endl;
  cout<<"WJetsIncSS Correction = "<<WJetsSSSideCorr_<<endl;


  //QCD correction factor
  cout<<" QCD Inc OS/SS scale factor "<<QCDOStoSSRatio_<<endl;


  return 1;
}


TH1F* TauMuPlotter::getWJetsInc(){
  TH1F*h=getSample("WJetsToLNu");
  h->Scale(WJetsOSSideCorr_);
  return h;
}
TH1F* TauMuPlotter::getWJetsIncSS(){
  TH1F*h=getSampleSS("WJetsToLNu");
  h->Scale(WJetsSSSideCorr_);
  return h;
}


TH1F* TauMuPlotter::getTotalBackgroundIncSS(){
  TString sel="eventweight*(categoryCh==2)";
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
  if(SMcat_>=0)sel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  TH1F* h=new TH1F("hBackgroundSS","",nbins_,xmin_,xmax_);
  h->Sumw2();

  TH1F*hZToTauTauSS=getZToTauTauSS();//could be from Embedded
  if(!hZToTauTauSS)return 0;
  h->Add(hZToTauTauSS);
  delete hZToTauTauSS;

  TH1F*hWJetsSS=getWJetsIncSS();//WJets needs a correction factor
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


TH1F* TauMuPlotter::getQCDInc(){

  TH1F* h=new TH1F("hQCDInc","QCDInc",nbins_,xmin_,xmax_);
  h->Sumw2();
  
  TH1F* hDataSS=getTotalDataSS();
  h->Add(hDataSS);
  delete hDataSS;

  TH1F* hMCSS=getTotalBackgroundIncSS();
  h->Add(hMCSS,-1.);
  delete hMCSS;

  h->Scale(QCDOStoSSRatio_);

  return h;
}



bool TauMuPlotter::plotInc(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){

  if(SMcat>0)return 0; 

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;


  TString plotvarname=plotvar_+extrasel_;
  for(Int_t l=0;l<plotvarname.Sizeof();l++){//fix the variable name to work with the file name
    if(TString(plotvarname[l])=="(") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])==")") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])=="*") plotvarname.Replace(l,1,"X");
    if(TString(plotvarname[l])=="=") plotvarname.Replace(l,1,"Q");
    if(TString(plotvarname[l])=="<") plotvarname.Replace(l,1,"L");
    if(TString(plotvarname[l])==">") plotvarname.Replace(l,1,"M");
    if(TString(plotvarname[l])=="&") plotvarname.Replace(l,1,"A");
  }


  TString filename=outputpath_+"/TauMuPlotter_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  TCanvas C("C",filename);  
  C.Print(filename+"[");
    

  
  TH1F*hQCD=getQCDInc();
  if(!hQCD)return 0;
  hQCD->SetLineWidth(1);
  hQCD->SetLineColor(QCDColor_);
  hQCD->SetFillColor(QCDColor_);
  
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
 
  
  TH1F*hWJetsToLNu=getWJetsInc();
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

  
  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZToTauTau,"hist");



  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  ////Total Data
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
 

  //create legend key
  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,hData->GetTitle(),"p");
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","f");
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)  legend.AddEntry(hQCD,"QCD","f");

  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Bkg "<<hBkg->Integral(0,hBkg->GetNbinsX()+1)<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;

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

  TFile DiffFile(outputpath_+"/DiffFileInc_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  
  hDiff->Write();

  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  
  hRatio->Write();

  DiffFile.ls();
  DiffFile.Close();


  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  //clean up
  if(hQCD) delete hQCD;
  if(hEWK) delete hEWK;
  if(hTTJets) delete hTTJets;
  if(hZToTauTau) delete hZToTauTau;
  if(hBkg) delete hBkg;
  if(hData) delete hData;
  if(hDiff) delete hDiff;


  return 1;

}


bool TauMuPlotter::plotIncSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
 
  if(SMcat>0)return 0;


  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;



  TString plotvarname=plotvar_+extrasel_;
  for(Int_t l=0;l<plotvarname.Sizeof();l++){//fix the variable name to work with the file name
    if(TString(plotvarname[l])=="(") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])==")") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])=="*") plotvarname.Replace(l,1,"X");
    if(TString(plotvarname[l])=="=") plotvarname.Replace(l,1,"Q");
    if(TString(plotvarname[l])=="<") plotvarname.Replace(l,1,"L");
    if(TString(plotvarname[l])==">") plotvarname.Replace(l,1,"M");
    if(TString(plotvarname[l])=="&") plotvarname.Replace(l,1,"A");
  }


  TString filename=outputpath_+"/TauMuPlotterSS_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  TCanvas C("C",filename);  
  C.Print(filename+"[");
  
  
  ////Total Data
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
 
  //
  TH1F*hQCD=0;//QCD for SS inclusive not defined

  //
  TH1F*hTTJets=getSampleSS("TTJets");
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  //
  TH1F*hWJetsToLNu=getWJetsIncSS();
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  
  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBosonSS();
  if(!hVV)return 0;
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSampleSS("ZToLJet");
  if(!hZToLJet)return 0;
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;


  ///////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
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
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD)  cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Bkg "<<hBkg->Integral(0,hBkg->GetNbinsX()+1)<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;
  
  

  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  if(log) C.SetLogy(1);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");//bring Data points back to front
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

  TFile DiffFile(outputpath_+"/DiffFileIncSS_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");
  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  
  hDiff->Write();

  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  
  hRatio->Write();

  DiffFile.ls();
  DiffFile.Close();
    

 
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;


  return 1;

}



//////////////Methods for the SM categories start here 
bool TauMuPlotter::correctSamplesSM(){


  //determine these factors here as they may be used by SM methods too
  /////Correct WJets using m_T high side with inclusive level selections
  cout<<"Determining WJets  from sidebands"<<endl;
  TString Wsample="WJetsToLNu";
//   if(SMcat_==0)Wsample="WJetsToLNu";//test of QCD with SM0
//   else if(SMcat_==1)Wsample="WJetsToLNu";
//   else if(SMcat_==2)Wsample="WJetsToLNu";
//   else if(SMcat_==-1)Wsample="WJetsToLNu"; //cross-check of QCD estimation in VBF defined using extrasel_
//   else {cout<<" bad SMcat chosen"<<endl; return 0;}

  TString WJetsOSCorrSel=TString("eventweight*(categoryCh==1&&categoryMT==3)"); 
  if(Isocat_>0) WJetsOSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
  if(SMcat_>=0) WJetsOSCorrSel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0) WJetsOSCorrSel+=TString("*")+extrasel_;

  WJetsSMOSSideCorr_=1.; Float_t WJetsOSSide=0; Float_t MCOSSide=0; Float_t DataOSSide=0;  
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->getDataType())=="Data"){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      DataOSSide+=hos->Integral();
      delete hos;
    }
    if(TString((*s)->GetName())==Wsample.Data()){
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      WJetsOSSide=hos->Integral()*(*s)->getNorm();
      delete hos;
    }
    if(TString((*s)->getDataType())=="MC" && TString((*s)->GetName())!="WJetsToLNu"){//WJets here is not the same as above because W2Jets,W3Jets are declared as MCcat
      TH1F* hos=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsOSCorrSel.Data());
      MCOSSide+=hos->Integral()*(*s)->getNorm();
      delete hos;
    }
  }  
  if(WJetsOSSide==0){cout<<" WJetsOS MC yield at high MT is 0"<<endl; return 0;}
  WJetsSMOSSideCorr_=(DataOSSide-MCOSSide)/WJetsOSSide;
  cout<<"WJetsSMOS Correction = "<<WJetsSMOSSideCorr_<<endl;


  TString WJetsSSCorrSel=TString("eventweight*(categoryCh==2&&categoryMT==3)");
  if(Isocat_>0) WJetsSSCorrSel+=TString("*(categoryIso==")+(long)Isocat_+")";
  if(SMcat_>=0) WJetsSSCorrSel += TString("*(categorySM==")+(long)SMcat_+")";
  if(extrasel_.CompareTo("1")!=0)  WJetsSSCorrSel+=TString("*")+extrasel_;
  WJetsSMSSSideCorr_=1.; Float_t WJetsSSSide=0; Float_t MCSSSide=0; Float_t DataSSSide=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->getDataType())=="Data"){
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      DataSSSide+=hss->Integral();
      delete hss;
    }
    if(TString((*s)->GetName())==Wsample.Data()){
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      WJetsSSSide=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
    if(TString((*s)->getDataType())=="MC" && TString((*s)->GetName())!="WJetsToLNu"){//WJets here is not the same as above because W2Jets,W3Jets are declared as MCcat
      TH1F* hss=(*s)->getHistoNtpFile("ditaumass",100,0,1000,WJetsSSCorrSel.Data());
      MCSSSide+=hss->Integral()*(*s)->getNorm();
      delete hss;
    }
  }  
  if(WJetsSSSide==0){cout<<" WJetsSS MC yield at high MT is 0"<<endl; return 0;}
  WJetsSMSSSideCorr_=(DataSSSide-MCSSSide)/WJetsSSSide;
  cout<<"WJetsSMSS Correction = "<<WJetsSMSSSideCorr_<<endl;




  return 1;
}



TH1F* TauMuPlotter::getWJetsSM(){
  TH1F*h=0;
  if(SMcat_==-1 || SMcat_==0)  h=getWJetsInc();
  else if(SMcat_==1 && Isocat_==1){//check Isocat to cope with the anti-isolated sample which has enough stats for the sideband correction
    h=getSample("WJetsToLNu"); 
    h->Scale(WJetsOSSideCorr_);//use factor from inclusive level because of too low stats
  } else if(SMcat_==2 && Isocat_==1) {
    h=getSample("WJetsToLNu");
    h->Scale(WJetsOSSideCorr_);
  } else { 
    h=getSample("WJetsToLNu");
    h->Scale(WJetsSMOSSideCorr_);
  }
  cout<<" WJets : "<<h->Integral()<<endl;
  return h;
}

TH1F* TauMuPlotter::getWJetsSMSS(){
  TH1F*h=0;
  if(SMcat_==-1 || SMcat_==0)  h=getWJetsIncSS();
  else if(SMcat_==1 && Isocat_==1){
    h=getSampleSS("WJetsToLNu");
    h->Scale(WJetsSSSideCorr_);//use factor from inclusive level because of too low stats
  }else if(SMcat_==2 && Isocat_==1) {
    h=getSampleSS("WJetsToLNu");
    h->Scale(WJetsSSSideCorr_);
  }else { 
    h=getSampleSS("WJetsToLNu");
    h->Scale(WJetsSMSSSideCorr_);
  }
  cout<<" WJetsSS : "<<h->Integral()<<endl;
  return h;
}



// TH1F* TauMuPlotter::getQCDMuIsoSM(){
//   TString sel="eventweight*((categoryCh==1)&&categoryIso==3)";
//   if(MTcat_>0) sel += TString("*(categoryMT==")+(long)MTcat_+")";
//   if(SMcat_>=0)sel+=TString("*(categorySM==")+(long)SMcat_+")";
//   if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

//   TH1F* h=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
//   h->Sumw2();
 
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data"){
//       TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
//       h->Add(hd);
//       delete hd;
//     }
//   }

//   h->Scale(QCDMuIsoSideRatio_);

//   return h;
// }

TH1F* TauMuPlotter::getTotalMCSM(){
  TH1F* h=new TH1F("hTotalMCSM","total MC",nbins_,xmin_,xmax_);
  h->Sumw2();
   

  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;
  TH1F*hWJets=getWJetsSM();
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;
  TH1F*hDiBoson=getDiBoson();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson);
  delete hDiBoson;
  TH1F*hTTJets=getSample("TTJets");
  if(!hTTJets)return 0;
  h->Add(hTTJets);
  delete hTTJets;
  TH1F*hZLJet=getSample("ZToLJet");
  if(!hZLJet)return 0;
  h->Add(hZLJet);
  delete hZLJet;
  TH1F*hZToMuMu=getSample("ZToMuMu");
  if(!hZToMuMu)return 0;
  h->Add(hZToMuMu);
  delete hZToMuMu;

  cout<<" TotalMCSM : "<<h->Integral()<<endl;
  return h;
}


TH1F* TauMuPlotter::getTotalMCSMSS(){
  TH1F* h=new TH1F("hTotalMCSM","total MC",nbins_,xmin_,xmax_);
  h->Sumw2();
   

  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;
  TH1F*hWJets=getWJetsSMSS();
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;
  TH1F*hDiBoson=getDiBosonSS();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson);
  delete hDiBoson;
  TH1F*hTTJets=getSampleSS("TTJets");
  if(!hTTJets)return 0;
  h->Add(hTTJets);
  delete hTTJets;
  TH1F*hZLJet=getSampleSS("ZToLJet");
  if(!hZLJet)return 0;
  h->Add(hZLJet);
  delete hZLJet;
  TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  if(!hZToMuMu)return 0;
  h->Add(hZToMuMu);
  delete hZToMuMu;

  cout<<" TotalMCSMSS : "<<h->Integral()<<endl;

  return h;
}


TH1F* TauMuPlotter::getQCDTauIsoSM(){
  
  TH1F* h=new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
  h->Sumw2();
   
  Int_t TmpIsocat=Isocat_;  
  Isocat_=2;//switch temporarily while keeping all other categories the same  
  if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;

  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  TString tmpextrasel=extrasel_;
  extrasel_ += TString("*(nditau==1)*")+qcdTauIsoRatio_;//apply  ratio here 
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hData);
  delete hData;
  TH1F* hMC=getTotalMCSM();
  if(!hMC){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hMC,-1);
  delete hMC;
  extrasel_=tmpextrasel;

  Isocat_=TmpIsocat;//return to normal
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;

  return h;

}


TH1F* TauMuPlotter::getQCDTauIsoSMSS(){
  
  TH1F* h=new TH1F("hSMQCDSS","SMQCDSS",nbins_,xmin_,xmax_);
  h->Sumw2();
   
  Int_t TmpIsocat=Isocat_;  
  Isocat_=2;//switch temporarily while keeping all other categories the same
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;
  
  cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
  TString tmpextrasel=extrasel_;
  extrasel_ +=  TString("*(nditau==1)*")+qcdTauIsoRatio_;//apply pt dependent ratio here 
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hData);
  delete hData;
  TH1F* hMC=getTotalMCSMSS();
  if(!hMC){cout<<" Total Data not determined "<<endl; return 0;}
  h->Add(hMC,-1);
  delete hMC;
  extrasel_=tmpextrasel;

  Isocat_=TmpIsocat;
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;

  return h;

}




bool TauMuPlotter::plotSM(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
 
 
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;
  
  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;


  TString plotvarname=plotvar_+extrasel_;
  for(Int_t l=0;l<plotvarname.Sizeof();l++){//fix the variable name to work with the file name
    if(TString(plotvarname[l])=="(") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])==")") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])=="*") plotvarname.Replace(l,1,"X");
    if(TString(plotvarname[l])=="=") plotvarname.Replace(l,1,"Q");
    if(TString(plotvarname[l])=="<") plotvarname.Replace(l,1,"L");
    if(TString(plotvarname[l])==">") plotvarname.Replace(l,1,"M");
    if(TString(plotvarname[l])=="&") plotvarname.Replace(l,1,"A");
  }


  TString filename=outputpath_+"/TauMuPlotterSM_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  TCanvas C("C",filename);  
  C.Print(filename+"[");
   


  //Fakes
  TH1F*hQCD=0;
  if(Isocat_==1){
    hQCD=getQCDTauIsoSM();
    if(!hQCD)return 0;
    if(smearHistoRes_>0) hQCD=smearHisto(hQCD);
    hQCD->SetLineWidth(1);
    hQCD->SetLineColor(QCDColor_);
    hQCD->SetFillColor(QCDColor_);
  }


 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  if(smearHistoRes_>0) hZToTauTau=smearHisto(hZToTauTau);
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  
  //
  TH1F*hTTJets=getSample("TTJets");
  if(!hTTJets)return 0;
  if(smearHistoRes_>0) hTTJets=smearHisto(hTTJets);
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);

  
  TH1F*hWJetsToLNu=getWJetsSM();///Note that this should be in sync with QCD determination from TauIso
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

  if(smearHistoRes_>0) hEWK=smearHisto(hEWK);
  hEWK->SetLineWidth(1);
  hEWK->SetLineColor(WJetsColor_);
  hEWK->SetFillColor(WJetsColor_);

  
  ////Total Data
  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}


  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
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
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;



  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetYaxis()->SetRangeUser(log ? 1 : 0, hData->GetMaximum()*(log ? 10 : 1.3));
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


  TFile DiffFile(outputpath_+"/DiffFileSM_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+"_SM"+(long)SMcat_+".root","recreate");

  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  
  hDiff->Write();
  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  
  hRatio->Write();


  DiffFile.ls();
  DiffFile.Close();
    

  
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

  return 1;

}



bool TauMuPlotter::plotSMSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString xlabel, TString ylabel, Float_t* legendcoords, bool log){
   

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=Isocat;
  MTcat_=MTcat;
  SMcat_=SMcat;
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;

  if(!scaleSamplesLumi())return 0;
  if(!correctSamplesInc())return 0;
  if(!correctSamplesSM())return 0;

  TString plotvarname=plotvar_+extrasel_;
  for(Int_t l=0;l<plotvarname.Sizeof();l++){//fix the variable name to work with the file name
    if(TString(plotvarname[l])=="(") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])==")") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])=="*") plotvarname.Replace(l,1,"X");
    if(TString(plotvarname[l])=="=") plotvarname.Replace(l,1,"Q");
    if(TString(plotvarname[l])=="<") plotvarname.Replace(l,1,"L");
    if(TString(plotvarname[l])==">") plotvarname.Replace(l,1,"M");
    if(TString(plotvarname[l])=="&") plotvarname.Replace(l,1,"A");
  }

  
  TString filename=outputpath_+"/TauMuPlotterSMSS_"+plotvarname;
  if(Isocat_>0) filename=filename+"_Iso"+(long)Isocat_;
  if(MTcat_>0) filename=filename+"_MT"+(long)MTcat_;
  if(SMcat_>=0) filename=filename+"_SM"+(long)SMcat_;
  filename+=".ps";
  TCanvas C("C",filename);  
  C.Print(filename+"[");
   


  //Fakes
  TH1F*hQCD=0;
//   if(Isocat_==1){
//     hQCD=getQCDTauIsoSMSS();
//     if(!hQCD)return 0;
//     hQCD->SetLineWidth(1);
//     hQCD->SetLineColor(QCDColor_);
//     hQCD->SetFillColor(QCDColor_);
//   }

 
  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTauSS();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(ZTauTauColor_);
  hZToTauTau->SetFillColor(ZTauTauColor_);

  //
  TH1F*hTTJets=getSampleSS("TTJets");
  if(!hTTJets)return 0;
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(TTJetsColor_);
  hTTJets->SetFillColor(TTJetsColor_);
    
  
  TH1F*hWJetsToLNu=getWJetsSMSS();///Note that this should be in sync with QCD determination from TauIso
  if(!hWJetsToLNu)return 0;
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(WJetsColor_);
  hWJetsToLNu->SetFillColor(WJetsColor_);

  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");
  delete hWJetsToLNu;

  //combine Di-Bosons
  TH1F*hVV=getDiBosonSS();
  if(!hVV)return 0;
  hEWK->Add(hVV);
  delete hVV;

  //combine ZLJet
  TH1F*hZToLJet=getSampleSS("ZToLJet");
  if(!hZToLJet)return 0;
  hEWK->Add(hZToLJet);
  delete hZToLJet;  

  //combine Z->MuMu
  TH1F*hZToMuMu=getSampleSS("ZToMuMu");
  if(!hZToMuMu)return 0;
  hZToMuMu->SetLineWidth(1);
  hZToMuMu->SetLineColor(ZMuMuColor_);
  hZToMuMu->SetFillColor(ZMuMuColor_);
  hEWK->Add(hZToMuMu);
  delete hZToMuMu;

  
  ////Total Data
  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}


  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
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
  legend.AddEntry(hEWK,"EWK","f");
  if(hQCD)legend.AddEntry(hQCD,"QCD","f");


  //create total Background
  TH1F* hBkg=new TH1F("hBkg","Bkg",nbins_,xmin_,xmax_);
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }


  cout<<" Summary of yields : "<<endl;
  if(hQCD) cout<<"QCD "<<hQCD->Integral(0,hQCD->GetNbinsX()+1)<<endl;
  cout<<"ElectroWeak "<<hEWK->Integral(0,hEWK->GetNbinsX()+1)<<endl;
  cout<<"TTJets "<<hTTJets->Integral(0,hTTJets->GetNbinsX()+1)<<endl;
  cout<<"ZToTauTau "<<hZToTauTau->Integral(0,hZToTauTau->GetNbinsX()+1)<<endl;
  cout<<"Total Background "<<(int)(hBkg->Integral(0,hBkg->GetNbinsX()+1))<<endl;
  cout<<"Total Data "<<(int)(hData->Integral(0,hData->GetNbinsX()+1))<<endl;



  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetYaxis()->SetRangeUser(log ? 1 : 0, hData->GetMaximum()*(log ? 10 : 1.3));
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


  TFile DiffFile(outputpath_+"/DiffFileSMSS_"+plotvarname+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat+"_SM"+(long)SMcat_+".root","recreate");
  //make the difference plot  
  TH1F* hDiff=new TH1F("hDiff","Difference",nbins_,xmin_,xmax_);
  hDiff->GetXaxis()->SetTitle(plotvar_);
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  C.Clear();
  hDiff->Draw("hist pe");
  C.Print(filename);  
  hDiff->Write();

  //make the ratio plot
  TH1F* hRatio=new TH1F("hRatio","Ratio",nbins_,xmin_,xmax_);
  hRatio->GetXaxis()->SetTitle(plotvar_);
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  C.Clear();
  hRatio->Draw("hist pe");
  C.Print(filename);  
  hRatio->Write();

  DiffFile.ls();
  DiffFile.Close();
    

  
  C.Print(filename+"]");
  cout<<"closed ps file"<<endl;

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



bool TauMuPlotter::printRawYields(TString selection){
  
  Float_t totalData=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      totalData+=hmass->Integral(0,hmass->GetNbinsX()+1);
      delete hmass;
    }
  }
  cout<<"Total Data  = "<<(int)(totalData)<<endl;

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  } 

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MCCat"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }  

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Signal"){
      TH1F*hmass=(TH1F*)((*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data()));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
      delete hmass;
    }
  }
  

  return 1;
}



//tau fake rate
void TauMuPlotter::plotTauFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t njets, Float_t ymax,Bool_t log){
  
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
 
  MTcat_=-1;
  if(plotvar_.CompareTo("transversemass")!=0)MTcat_=1;//QCD is enhanced in the signal region
  SMcat_=-1;  
  extrasel_="(nditau==1)";
  if(njets>-1)extrasel_+=TString("*(njet==")+(long)njets+")";

  TString plotvarname=plotvar_+extrasel_;
  for(Int_t l=0;l<plotvarname.Sizeof();l++){//fix the variable name to work with the file name
    if(TString(plotvarname[l])=="(") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])==")") plotvarname.Replace(l,1,"_");
    if(TString(plotvarname[l])=="*") plotvarname.Replace(l,1,"X");
    if(TString(plotvarname[l])=="=") plotvarname.Replace(l,1,"Q");
    if(TString(plotvarname[l])=="<") plotvarname.Replace(l,1,"L");
    if(TString(plotvarname[l])==">") plotvarname.Replace(l,1,"M");
    if(TString(plotvarname[l])=="&") plotvarname.Replace(l,1,"A");
  }
 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauMuPlotterTauFakeRate_"+plotvarname;
  filename += ".ps";
  C.Print(filename+"[");


  /////////Anti-Isolated taus
  Isocat_=2;
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;

  TH1F* hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  TH1F* hMC=getTotalBackgroundIncSS();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}
  
  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename);


  TH1F* hA=new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
  hA->Add(hData);
  hA->Add(hMC,-1);
  delete hData; delete hMC;

  C.Clear();
  hA->Draw("pe");
  C.Print(filename);


  /////////////Isolated taus
  Isocat_=1;
  if(!scaleSamplesLumi())return;
  if(!correctSamplesInc())return;
  hData=getTotalDataSS();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  hMC=getTotalBackgroundIncSS();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}

  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename);

  TH1F* hI=new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
  hI->Add(hData);
  hI->Add(hMC,-1);
  delete hData; delete hMC;
  
  C.Clear();
  hI->Draw("pe");
  C.Print(filename);

  //average ratio
  float r=hI->Integral()/hA->Integral();
  float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
  cout<<" Ratio = "<<r<<" +- "<<re<<endl;


  /////////////////Calculate the ratio as a function of the variable
  TH1F* hR=new TH1F("hRatio","",nbins_,xmin_,xmax_);
  hR->Add(hI);
  hR->Divide(hA);
  delete hA; delete hI;

  C.Clear();
  C.SetLogy(log);
  delete hR->FindObject("stats");
  hR->GetYaxis()->SetRangeUser(log ? 0.001 : 0,ymax);
  hR->GetXaxis()->SetTitle(plotvar_);
  hR->GetYaxis()->SetTitle("ratio");
  hR->SetTitle("");
  hR->Draw("pe");
  C.Print(filename);

  C.Clear();
  C.SetLogy(log);
  delete hR->FindObject("stats");
  hR->Fit("pol1");
  C.Print(filename);
  

  //print values
  for(Int_t b=1;b<=hR->GetNbinsX();b++)
    cout<<"RValue["<<b-1<<"]="<<hR->GetBinContent(b)<<";"<<endl;
  for(Int_t b=1;b<=hR->GetNbinsX();b++)
    cout<<"RErr["<<b-1<<"]="<<hR->GetBinError(b)<<";"<<endl;

  //print weight function
  cout<<"(";
  for(Int_t b=1;b<=hR->GetNbinsX();b++)
    cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*"<<hR->GetBinContent(b)<<"+";
  cout<<")"<<endl;  

  delete hR;

  C.Print(filename+"]");

}





