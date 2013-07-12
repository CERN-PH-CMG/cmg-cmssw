#include "TauElePlotter.h"
#include <THStack.h>
#include <TGraph.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>
#include <TVectorD.h>
#include <TLatex.h>
#include <TText.h>
#include <TPaveText.h>
//#include "CMGTools/H2TauTau/interface/QCDEstimate.h"
//#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"


// #include "RooDataHist.h"
// #include "RooHistPdf.h"
// #include "RooHistPdf.h"
// #include "RooRealVar.h"
// #include "RooAddPdf.h"
// #include "RooDataSet.h"
// #include "RooChi2Var.h"
// #include "RooMinuit.h"
//using namespace RooFit;



TauElePlotter::TauElePlotter():
  TNamed("","")
{
}

TauElePlotter::TauElePlotter(const char * name):
  TNamed(name,name),
  smearHistoRes_(0.),
  Chcat_(-1),
  Isocat_(-1),
  MTcat_(-1),
  SMcat_(-1),
  extrasel_("1"),
  blindsel_("1"),
  mTCut_(30),
  eventWeight_("eventweight"),
  plotvar_("ditaumass"),
  nbins_(100),
  xmin_(0.),
  xmax_(1000.),
  ZTTType_(1),
  WJetsType_(0),
  MSSMFlag_(0),
  nbinsVariable_(0),
  xbinsVariable_(0)
{
 
  QCDOStoSSRatio_=1.06;
  TTJetsCorrFactor[0]=1.;
  TTJetsCorrFactor[1]=1.;
  TTJetsCorrFactor[2]=1.;

}

TauElePlotter::~TauElePlotter(){
}

/////////////General methods start here

bool TauElePlotter::scaleSamplesLumi(){
  
  //reset all scales
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    (*s)->resetScale();

  /////////determine total lumi and scale MCs
  float totalDataLumi=getTotalDataLumi();
  cout<<"totalDataLumi = "<<totalDataLumi<<endl;
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" 
       || (*s)->getDataType()=="MCCat" 
       || (*s)->getDataType()=="Signal" ){
      //cout<<(*s)->GetName()<<" lumi scale "<<totalDataLumi/(*s)->getLumi()<<endl;
      if((*s)->getLumi()>0.)
	(*s)->scale(totalDataLumi/(*s)->getLumi());
      else (*s)->scale(0.);
    }



  ///////////////////////////////////
  ////Scale the embedded samples at inclusive level:  Should there be separate scale factor for OS and SS ? --> should be Ok. ZTauTau is small in SS
  /////////////////////////////////
  cout<<"---------> Scaling Embedded samples "<<endl;
  TString sel=eventWeight_+"*(categoryIso==1&&abs(ditaucharge)==0&&1.<ditaumass&&ditaumass<1000.)";
  TH1F*hZTTMC=findSample("ZToTauTau")->getHistoNtpFile("ditaumass",10,1,1001,sel);
  Float_t ZToTauTauMC=hZTTMC->Integral();
  delete hZTTMC;
  Float_t EmbeddedOS=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Embedded"){
      TH1F*hEmbedded=findSample((*s)->GetName())->getHistoNtpFile("ditaumass",10,1,1001,sel);
      EmbeddedOS+=hEmbedded->Integral();
      delete hEmbedded;
    }
  }
  cout<<"ZToTauTauMC= "<<ZToTauTauMC<<" EmbeddedOS="<<EmbeddedOS<<endl;
  if(ZToTauTauMC==0. || EmbeddedOS==0. || ZToTauTauMC!=ZToTauTauMC || EmbeddedOS!=EmbeddedOS){
    cout<<"ERROR---------------->Bad ZToTauTauMC or EmbeddedOS yields: ZToTauTauMC="<<ZToTauTauMC<<" EmbeddedOS="<<EmbeddedOS<<endl;
  } else {
    cout<<"Rescaling Embedded samples by factor : "<<ZToTauTauMC/EmbeddedOS<<endl;
    for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if((*s)->getDataType()=="Embedded") (*s)->scale(ZToTauTauMC/EmbeddedOS);
  }



  return 1;
}


TH1F* TauElePlotter::getSample(TString samplename, bool removeLumiNorm){

  TString sel=eventWeight_;
  if(Chcat_==1) sel += TString("*(abs(ditaucharge)==0)");
  if(Chcat_==2) sel += TString("*(abs(ditaucharge)==2)");
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_==1)  sel += TString("*(transversemass<")+mTCut_+")";
  //if(MTcat_==3)  sel += "*(transversemass>70)";//this doesnt work for 2-jet
  if(MTcat_==3)  sel += "*(80<transversemass&&transversemass<150)";
  if(MTcat_==13)  sel += "*(60<transversemass&&transversemass<120)";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;


  TH1F* h=getPlotHisto(samplename);

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename){

      if( (*s)->getDataType()=="Signal"){
	sel = sel +"*(1-0.12*(taudecaymode==0))";
      }
      
      TH1F*hd=0;
      if(nbins_>0) hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hd=(*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}


      if(removeLumiNorm){//needed when adding W+njet samples
        //cout<<" Removing Lumi normalization for sample "<<(*s)->GetName()<<" "<<(*s)->getLumi()<<" / "<<getTotalDataLumi()<<endl;
        hd->Scale((*s)->getLumi()/getTotalDataLumi());
      }


      h->Add(hd);
      delete hd;
    }  

  //cout<<" getSample: "<<samplename<<" : "<<h->Integral()<<endl;
  return h;
}



TH1F* TauElePlotter::getTotalData(){
  TString sel=eventWeight_;
  if(Chcat_==1)  sel += TString("*(abs(ditaucharge)==0)");
  if(Chcat_==2)  sel += TString("*(abs(ditaucharge)==2)");
  if(Isocat_>0)  sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_==1)  sel += TString("*(transversemass<")+mTCut_+")";
  //if(MTcat_==3)  sel += "*(transversemass>70)";
  if(MTcat_==3)  sel += "*(80<transversemass&&transversemass<150)";
  if(MTcat_==13) sel += "*(60<transversemass&&transversemass<120)";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;
  if(blindsel_.CompareTo("1")!=0)sel += TString("*")+blindsel_;
  //cout<<"selection : "<<sel<<endl;
  
  TH1F* h=getPlotHisto("hData");

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      TH1F*hd=0;
      if(nbins_>0) hd = (*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hd = (*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}
      h->Add(hd);
      delete hd;
    }
 
  return h;
}



TH1F* TauElePlotter::getTotalEmbedded(){
  TString sel=eventWeight_;
  if(Chcat_==1) sel += TString("*(abs(ditaucharge)==0)");
  if(Chcat_==2) sel += TString("*(abs(ditaucharge)==2)");
  if(Isocat_>0) sel += TString("*(categoryIso==")+(long)Isocat_+")";
  if(MTcat_==1)  sel += TString("*(transversemass<")+mTCut_+")";
  //if(MTcat_==3)  sel += "*(transversemass>70)";
  if(MTcat_==3)  sel += "*(80<transversemass&&transversemass<150)";
  if(MTcat_==13)  sel += "*(60<transversemass&&transversemass<120)";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;


  TH1F* h=getPlotHisto("hEmbedded");

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Embedded"){
      TH1F*hd=0;
      if(nbins_>0)hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hd=(*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}
      h->Add(hd);  
      delete hd;
    }
 
  cout<<"TotalEmbedded : "<<h->Integral()<<endl;

  return h;
}


TH1F* TauElePlotter::getDiBoson(){

  TH1F* h=getPlotHisto("hVV");


  TH1F* hWW2L2Nu=getSample("WW2L2Nu");
  if(!hWW2L2Nu)return 0;
  h->Add(hWW2L2Nu);
  delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSample("WZ3LNu");
  if(!hWZ3LNu)return 0;
  h->Add(hWZ3LNu);
  delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSample("WZ2L2Q");
  if(!hWZ2L2Q)return 0;
  h->Add(hWZ2L2Q);
  delete hWZ2L2Q;
  TH1F* hZZ4L=getSample("ZZ4L");
  if(!hZZ4L)return 0;
  h->Add(hZZ4L);
  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSample("ZZ2L2Nu");
  if(!hZZ2L2Nu)return 0;
  h->Add(hZZ2L2Nu);
  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSample("ZZ2L2Q");
  if(!hZZ2L2Q)return 0;
  h->Add(hZZ2L2Q);
  delete hZZ2L2Q;

  TH1F* hTopTW=getSample("TopTW");
  if(!hTopTW)return 0;
  h->Add(hTopTW);
  delete hTopTW;

  TH1F* hTopBTW=getSample("TopBTW");
  if(!hTopBTW)return 0;
  h->Add(hTopBTW);
  delete hTopBTW;
 
  return h;
}


TH1F* TauElePlotter::getZToTauTau(){  
  TH1F*h=0;

  //fix the decay mode weights
  TString decaymodeCorrection="*(1-0.12*(taudecaymode==0))";  ///Summer13 SF for 1prong-no-pi0 from Phil
  TString tmpExtrasel=extrasel_;
  extrasel_ += decaymodeCorrection;
  cout<<"WARNING: Correcting for the ZTT tau decay modes weights: "<<decaymodeCorrection<<endl;

  if(ZTTType_==1)h=getSample("ZToTauTau");
  if(ZTTType_==2)h=getTotalEmbedded(); 

  extrasel_=tmpExtrasel;


  return h;
}



//TTJets
TH1F* TauElePlotter::getTTJetsInc(){
  TH1F*h=getSample("TTJets");
  return h;
}

TH1F* TauElePlotter::getZNJetSum(TString decay){
  //cout<<" Calling getZNJetSum"<<endl;

  TString tmpExtrasel=extrasel_;
  
  TH1F* hShape=getPlotHisto(TString("getZNJetSum")+decay);

  TH1F* H1ShapeExc=getSample(TString("Z1To")+decay,1);
  TH1F* H2ShapeExc=getSample(TString("Z2To")+decay,1);
  TH1F* H3ShapeExc=getSample(TString("Z3To")+decay,1);
  TH1F* H4ShapeExc=getSample(TString("Z4To")+decay,1);

  extrasel_+="*(!(lhenup==6||lhenup==7||lhenup==8||lhenup==9))";
  TH1F*H0ShapeInc=getSample(TString("ZTo")+decay,1); H0ShapeInc->SetName("H0ShapeInc");  //cout<<"H0ShapeInc "<<H0ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==6)";
  TH1F*H1ShapeInc=getSample(TString("ZTo")+decay,1); H1ShapeInc->SetName("H1ShapeInc");  //cout<<"H1ShapeInc "<<H1ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==7)";
  TH1F*H2ShapeInc=getSample(TString("ZTo")+decay,1); H2ShapeInc->SetName("H2ShapeInc");  //cout<<"H2ShapeInc "<<H2ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==8)";
  TH1F*H3ShapeInc=getSample(TString("ZTo")+decay,1); H3ShapeInc->SetName("H3ShapeInc");  //cout<<"H3ShapeInc "<<H3ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==9)";
  TH1F*H4ShapeInc=getSample(TString("ZTo")+decay,1); H4ShapeInc->SetName("H4ShapeInc");  //cout<<"H4ShapeInc "<<H4ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;


  float totalDataLumi=getTotalDataLumi();
  float IncLumi=findSample(TString("ZTo")+decay)->getLumi();

  if(IncLumi>0){
    hShape->Add(H0ShapeInc,totalDataLumi/IncLumi);
    hShape->Add(H1ShapeExc,totalDataLumi/(IncLumi + findSample(TString("Z1To")+decay)->getLumi()));
    hShape->Add(H2ShapeExc,totalDataLumi/(IncLumi + findSample(TString("Z2To")+decay)->getLumi()));
    hShape->Add(H3ShapeExc,totalDataLumi/(IncLumi + findSample(TString("Z3To")+decay)->getLumi()));
    hShape->Add(H4ShapeExc,totalDataLumi/(IncLumi + findSample(TString("Z4To")+decay)->getLumi()));
    hShape->Add(H1ShapeInc,totalDataLumi/(IncLumi + findSample(TString("Z1To")+decay)->getLumi()));
    hShape->Add(H2ShapeInc,totalDataLumi/(IncLumi + findSample(TString("Z2To")+decay)->getLumi()));
    hShape->Add(H3ShapeInc,totalDataLumi/(IncLumi + findSample(TString("Z3To")+decay)->getLumi()));
    hShape->Add(H4ShapeInc,totalDataLumi/(IncLumi + findSample(TString("Z4To")+decay)->getLumi()));
  }


  delete H0ShapeInc;
  delete H1ShapeInc;
  delete H2ShapeInc;
  delete H3ShapeInc;
  delete H4ShapeInc;

  delete H1ShapeExc;
  delete H2ShapeExc;
  delete H3ShapeExc;
  delete H4ShapeExc;

  return hShape;
}

//ZLJet
TH1F* TauElePlotter::getZToLJetInc(){
  //TH1F*h=getSample("ZToLJet");
  TH1F*h=getZNJetSum("LJet");
  return h;
}

//ZEE
TH1F* TauElePlotter::getZLInc(){
  //TH1F*h=getSample("ZToEE");
  //TH1F*h=getZNJetSum("EE");
  TH1F*h=getZL2012();
  return h;
}


//ZEE with corrections
TH1F* TauElePlotter::getZL2012(){
  //cout<<" WARNING ---------------------> Applying corrections to Z--> ee MC "<<endl;
  TString tmpextrasel=extrasel_;
  extrasel_+="*(taudecaymode!=0&&taudecaymode!=1)";
  TH1F*h=getZNJetSum("EE");
  h->SetName("getZL2012");
  extrasel_=tmpextrasel;

  //1Prong 0pi0
  //extrasel_+="*(taudecaymode==0)*((abs(taueta)<1.5)*(0.85)+(abs(taueta)>=1.5)*(1.85))";//Moriond
  extrasel_+="*(taudecaymode==0)*((abs(taueta)<1.5)*(1.37)+(abs(taueta)>=1.5)*(2.07))";//For Summer13
  TH1F*h1P0Pi0=getZNJetSum("EE");
  h->Add(h1P0Pi0);
  delete h1P0Pi0;
  extrasel_=tmpextrasel;


  ///now get the 1Prong-1Pi0: 
  //extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*(1.64)+(abs(taueta)>=1.5)*(0.28))"; //Moriond
  extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*(2.18)+(abs(taueta)>=1.5)*(0.55))"; //Summer13
  TString tmpplotvar=plotvar_;
  if(plotvar_.CompareTo("svfitmass")==0) plotvar_="svfitmass*1.000"; 
  if(plotvar_.CompareTo("ditaumass")==0) plotvar_="ditaumass*1.000"; 
  TH1F*h1P1Pi0=getZNJetSum("EE");
  h->Add(h1P1Pi0);
  delete h1P1Pi0;
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;

  return h;
}

TH1F* TauElePlotter::getZLVBFHCP(){

  /////////////////////////////////////////
  //Norm from Embedded
  TString TmpExtrasel=extrasel_;  
  //extrasel_="1"; 
  //extrasel_="(njet>=1)"; 
  extrasel_="(njet>=2)"; 
  //extrasel_="(njet>=2&&njetingap==0)"; 


  ////Get ZL Yield at Inclusive
  TH1F*hNorm=getZLInc(); 
  hNorm->SetName("getZLVBFHCP");
  //get Embedded yield at inclusive
  TH1F*hEmbIncl=getTotalEmbedded(); 
  hEmbIncl->SetName("getZLVBFHCPhEmbIncl");

  extrasel_=TmpExtrasel;
  //get Embedded yield at VBF
  TH1F*hEmbVBF=getTotalEmbedded(); 
  hEmbVBF->SetName("getZLVBFHCPhEmbVBF");

  //////////////////////////////////////////////////////



  //TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  //TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString shapesel="(njet20>=2)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getZLInc();//getZL2012();//getZL2012Type2();
  hShape->SetName("getZLVBFHCPShape");
  extrasel_=tmpextrasel;
  
  if(hShape->Integral()>0){
    //hShape->Scale(hNorm->Integral()/hShape->Integral());
    hShape->Scale(hNorm->Integral()*(hEmbVBF->Integral()/hEmbIncl->Integral())/hShape->Integral());
  }else hShape->Scale(0.);
  delete hEmbIncl;
  delete hEmbVBF;

  delete hNorm;
  return hShape;
}


//ZEE with corrections
TH1F* TauElePlotter::getZL2012Type1(){
  //cout<<"Calling getZL2012Type1 "<<endl;

  TString tmpextrasel=extrasel_;
  TString tmpplotvar=plotvar_;
  cout<<" WARNING ---------------------> Applying corrections to Z--> ee MC "<<endl;

  extrasel_+="*(taudecaymode!=0&&taudecaymode!=1)";
  TH1F*h=getSample("ZToEE"); 
  h->SetName("hZToEE");
  extrasel_=tmpextrasel;

  //get Z-->ee excluding the 1Prong-1Pi0 and apply scale factors to 1prong-0pi0
  extrasel_+="*(taudecaymode==0)*(((abs(taueta)<1.5)*((taupt<40)*(0.74)+(taupt>=40)*(0.96))+(abs(taueta)>=1.5)*(1.85)))";
  if(plotvar_.CompareTo("svfitmass")==0) plotvar_="svfitmass"; //shift mass
  if(plotvar_.CompareTo("ditaumass")==0) plotvar_="ditaumass"; //shift mass
  TH1F*h1P0Pi0=getSample("ZToEE");
  smearHistoRes_=0.0;
  TH1F*h1P0Pi0Smeared=smearHisto(h1P0Pi0);
  delete h1P0Pi0;
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P0Pi0Smeared);
  delete h1P0Pi0Smeared;

  ///now get the 1Prong-1Pi0: apply scale factors and mass corrections if its the mass variable
  extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*(1.5)+(abs(taueta)>=1.5)*(0.28))"; //scale 1prong-1pi0
  if(plotvar_.CompareTo("svfitmass")==0) plotvar_="svfitmass*(0.95210+0.00108*taupt)"; //shift mass
  if(plotvar_.CompareTo("ditaumass")==0) plotvar_="ditaumass*(0.95210+0.00108*taupt)"; //shift mass
  TH1F*h1P1Pi0=getSample("ZToEE");
  smearHistoRes_=1.6;
  TH1F*h1P1Pi0Smeared=smearHisto(h1P1Pi0);
  delete h1P1Pi0;
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P1Pi0Smeared);
  delete h1P1Pi0Smeared;

  return h;
}


TH1F* TauElePlotter::getZL2012Type1LowHigh(){
  //cout<<"Calling getZL2012Type1LowHigh "<<endl;

  TString tmpextrasel=extrasel_;
  TString tmpplotvar=plotvar_;
  cout<<" WARNING ---------------------> Applying corrections to Z--> ee MC "<<endl;

  extrasel_+="*(taudecaymode!=0&&taudecaymode!=1)";
  TH1F*h=getSample("ZToEE"); 
  h->SetName("hZToEE");
  extrasel_=tmpextrasel;

  //get the 1Prong-0Pi0 and apply scale factors
  extrasel_+="*(taudecaymode==0)*(((abs(taueta)<1.5)*((taupt<40)*(0.69)+(taupt>=40)*(0.98))+(abs(taueta)>=1.5)*(1.85)))";
  TH1F*h1P0Pi0Smeared=0;
  if( plotvar_.CompareTo("ditaumass")==0){
    plotvar_+="*((taupt<40)*1.000+(taupt>=40)*1.006)"; //shift computed from fit
    TH1F*h1P0Pi0=getSample("ZToEE");
    smearHistoRes_=2.7;//note this was computed for high tau pT
    h1P0Pi0Smeared=smearHisto(h1P0Pi0);//smear
    delete h1P0Pi0;
  } else h1P0Pi0Smeared=getSample("ZToEE");
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P0Pi0Smeared);
  delete h1P0Pi0Smeared;

  ///now get the 1Prong-1Pi0: apply scale factors and mass corrections if its the mass variable
  extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*((taupt<40)*(1.97)+(taupt>=40)*(1.95))+(abs(taueta)>=1.5)*(0.28))"; 
  TH1F*h1P1Pi0Smeared=0;
  if( plotvar_.CompareTo("ditaumass")==0){
    plotvar_+="*((taupt<40)*0.963+(taupt>=40)*1.007)"; //shift computed from fit
    TH1F*h1P1Pi0=getSample("ZToEE");
    smearHistoRes_=3.2;//computed for high tau pT
    h1P1Pi0Smeared=smearHisto(h1P1Pi0);//smear
    delete h1P1Pi0;  
  } else h1P1Pi0Smeared=getSample("ZToEE");
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P1Pi0Smeared);
  delete h1P1Pi0Smeared;

  return h;
}



TH1F* TauElePlotter::getZL2012Type2(){
  //cout<<"Calling getZL2012Type2 "<<endl;

  TString tmpextrasel=extrasel_;
  TString tmpplotvar=plotvar_;
  cout<<" WARNING ---------------------> Applying corrections to Z--> ee MC "<<endl;

  extrasel_+="*(taudecaymode!=0&&taudecaymode!=1)";
  TH1F*h=getSample("ZToEE"); 
  h->SetName("hZToEE");
  extrasel_=tmpextrasel;

  //get the 1Prong-0Pi0 and apply scale factors
  //extrasel_+="*(taudecaymode==0)*(((abs(taueta)<1.5)*((taupt<40)*(0.69)+(taupt>=40)*(0.98))+(abs(taueta)>=1.5)*(1.85)))";
  //extrasel_+="*(taudecaymode==0)*(((abs(taueta)<1.5)*((taupt<40)*(0.74)+(taupt>=40)*(0.96))+(abs(taueta)>=1.5)*(1.85)))";//from fixed signal template fit
  extrasel_+="*(taudecaymode==0)*(((abs(taueta)<1.5)*((taupt<40)*(0.80)+(taupt>=40)*(1.02))+(abs(taueta)>=1.5)*(1.85)))";//by eye
  TH1F*h1P0Pi0Smeared=0;
  if( plotvar_.CompareTo("ditaumass")==0){
    plotvar_+="*((taupt<40)*1.000+(taupt>=40)*1.004)"; 
    TH1F*h1P0Pi0=getSample("ZToEE");
    smearHistoRes_=2.8;//by eye
    //smearHistoRes_=0.0;//assumes smearing has already been done
    h1P0Pi0Smeared=smearHisto(h1P0Pi0);//smear
    delete h1P0Pi0;
  } else   if(plotvar_.CompareTo("svfitmass")==0 ){
    plotvar_+="*((taupt<40)*1.000+(taupt>=40)*1.007)"; 
    TH1F*h1P0Pi0=getSample("ZToEE");
    smearHistoRes_=3.5;//by eye
    //smearHistoRes_=0.0;//assumes smearing has already been done
    h1P0Pi0Smeared=smearHisto(h1P0Pi0);//smear
    delete h1P0Pi0;
  } else h1P0Pi0Smeared=getSample("ZToEE");
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P0Pi0Smeared);
  delete h1P0Pi0Smeared;

  ///now get the 1Prong-1Pi0: apply scale factors and mass corrections if its the mass variable
  //extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*((taupt<40)*(1.97)+(taupt>=40)*(1.95))+(abs(taueta)>=1.5)*(0.28))"; 
  //extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*((taupt<40)*(1.5)+(taupt>=40)*(1.5))+(abs(taueta)>=1.5)*(0.28))"; //from fixed signal template fit
  extrasel_+="*(taudecaymode==1)*((abs(taueta)<1.5)*((taupt<40)*(1.8)+(taupt>=40)*(1.85))+(abs(taueta)>=1.5)*(0.28))"; //by eye
  TH1F*h1P1Pi0Smeared=0;
  if( plotvar_.CompareTo("ditaumass")==0){
    plotvar_+="*((taupt<40)*1.000+(taupt>=40)*1.002)";
    TH1F*h1P1Pi0=getSample("ZToEE");
    smearHistoRes_=3.2;//by eye
    //smearHistoRes_=0.0;//assumes smearing has already been done
    h1P1Pi0Smeared=smearHisto(h1P1Pi0);//smear
    delete h1P1Pi0;  
  } else if(plotvar_.CompareTo("svfitmass")==0 ){
    plotvar_+="*((taupt<40)*1.000+(taupt>=40)*1.002)";
    TH1F*h1P1Pi0=getSample("ZToEE");
    smearHistoRes_=3.2;//by eye
    //smearHistoRes_=0.0;//assumes smearing has already been done
    h1P1Pi0Smeared=smearHisto(h1P1Pi0);//smear
    delete h1P1Pi0;  
  } else h1P1Pi0Smeared=getSample("ZToEE");
  plotvar_=tmpplotvar;
  extrasel_=tmpextrasel;
  h->Add(h1P1Pi0Smeared);
  delete h1P1Pi0Smeared;

  return h;
}



TH1F* TauElePlotter::getTotalMCSM(Int_t WType){
  TH1F* h=getPlotHisto("hTotalsMCSM");

  TH1F*hZToTauTau=getZToTauTau();
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau);
  delete hZToTauTau;
  TH1F*hWJets=0;
  if(WType==0)hWJets=getWJetsInc();
  if(WType==1)hWJets=getWJetsNJet();
  if(!hWJets)return 0;
  h->Add(hWJets);
  delete hWJets;
  TH1F*hDiBoson=getDiBoson();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson);
  delete hDiBoson;
  TH1F*hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  h->Add(hTTJets);
  delete hTTJets;
  TH1F*hZLJet=getZToLJetInc();
  if(!hZLJet)return 0;
  h->Add(hZLJet);
  delete hZLJet;
  TH1F*hZToEE=getZLInc();
  if(!hZToEE)return 0;
  h->Add(hZToEE);
  delete hZToEE;


  cout<<" TotalMCSM : "<<h->Integral()<<endl;
  return h;
}



//////////W+jets estimations////////////////////////////
TH1F* TauElePlotter::getWJetsInc(){
  //cout<<"Calling getWJetsInc "<<endl;
  TString sname="WJetsToLNu";
  
  //shape of W+jets
  TH1F*hShape=getSample(sname);
  hShape->SetName("getWJetsInc");
  
  //determine normalization
  Int_t tmpCategoryMT=MTcat_;
  MTcat_=3;
  TH1F* HW=getSample(sname);
  TH1F* HData=getTotalData();
  TH1F* HMC=getZToTauTau();
  TH1F* HTT=getTTJetsInc();   HMC->Add(HTT); delete HTT;
  TH1F* HZJ=getZToLJetInc();  HMC->Add(HZJ); delete HZJ;
  TH1F* HZL=getZLInc();       HMC->Add(HZL); delete HZL;
  MTcat_=tmpCategoryMT;
  
  if(HW->Integral()>0.) {
    float corrFactor = (HData->Integral() - HMC->Integral()) / HW->Integral();
    cout<<"  WJets corrFactor : "<<corrFactor<<endl;
    hShape->Scale(corrFactor);
  }else {
    cout<<"WARNING HW->Integral is 0"<<endl;
    hShape->Scale(0.);
  }

  delete HData;
  delete HMC;
  delete HW;

  return hShape;
}


TH1F* TauElePlotter::getWNJetSum(){

  //cout<<"Calling getWNJetSum"<<endl;
  TString tmpExtrasel=extrasel_;
  
  TH1F* hShape=getPlotHisto("getWNJetSum");

  TH1F* HW1ShapeExc=getSample("W1JetsToLNu",1); //cout<<"HW1ShapeExc "<<HW1ShapeExc->Integral()<<endl; 
  TH1F* HW2ShapeExc=getSample("W2JetsToLNu",1); //cout<<"HW2ShapeExc "<<HW2ShapeExc->Integral()<<endl; 
  TH1F* HW3ShapeExc=getSample("W3JetsToLNu",1); //cout<<"HW3ShapeExc "<<HW3ShapeExc->Integral()<<endl; 
  TH1F* HW4ShapeExc=getSample("W4JetsToLNu",1); //cout<<"HW4ShapeExc "<<HW4ShapeExc->Integral()<<endl; 

  extrasel_+="*(!(lhenup==6||lhenup==7||lhenup==8||lhenup==9))";
  TH1F*HW0ShapeInc=getSample("WJetsToLNu",1); HW0ShapeInc->SetName("HW0ShapeInc");  //cout<<"HW0ShapeInc "<<HW0ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==6)";
  TH1F*HW1ShapeInc=getSample("WJetsToLNu",1); HW1ShapeInc->SetName("HW1ShapeInc");  //cout<<"HW1ShapeInc "<<HW1ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==7)";
  TH1F*HW2ShapeInc=getSample("WJetsToLNu",1); HW2ShapeInc->SetName("HW2ShapeInc");  //cout<<"HW2ShapeInc "<<HW2ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==8)";
  TH1F*HW3ShapeInc=getSample("WJetsToLNu",1); HW3ShapeInc->SetName("HW3ShapeInc");  //cout<<"HW3ShapeInc "<<HW3ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;
  extrasel_+="*(lhenup==9)";
  TH1F*HW4ShapeInc=getSample("WJetsToLNu",1); HW4ShapeInc->SetName("HW4ShapeInc");  //cout<<"HW4ShapeInc "<<HW4ShapeInc->Integral()<<endl; 
  extrasel_= tmpExtrasel;


  float totalDataLumi=getTotalDataLumi();
  float WIncLumi=findSample("WJetsToLNu")->getLumi();

  hShape->Add(HW0ShapeInc,totalDataLumi/WIncLumi);
  hShape->Add(HW1ShapeExc,totalDataLumi/(WIncLumi + findSample("W1JetsToLNu")->getLumi()));
  hShape->Add(HW2ShapeExc,totalDataLumi/(WIncLumi + findSample("W2JetsToLNu")->getLumi()));
  hShape->Add(HW3ShapeExc,totalDataLumi/(WIncLumi + findSample("W3JetsToLNu")->getLumi()));
  hShape->Add(HW4ShapeExc,totalDataLumi/(WIncLumi + findSample("W4JetsToLNu")->getLumi()));
  hShape->Add(HW1ShapeInc,totalDataLumi/(WIncLumi + findSample("W1JetsToLNu")->getLumi()));
  hShape->Add(HW2ShapeInc,totalDataLumi/(WIncLumi + findSample("W2JetsToLNu")->getLumi()));
  hShape->Add(HW3ShapeInc,totalDataLumi/(WIncLumi + findSample("W3JetsToLNu")->getLumi()));
  hShape->Add(HW4ShapeInc,totalDataLumi/(WIncLumi + findSample("W4JetsToLNu")->getLumi()));


  delete HW0ShapeInc;
  delete HW1ShapeInc;
  delete HW2ShapeInc;
  delete HW3ShapeInc;
  delete HW4ShapeInc;

  delete HW1ShapeExc;
  delete HW2ShapeExc;
  delete HW3ShapeExc;
  delete HW4ShapeExc;


  return hShape;
}


TH1F* TauElePlotter::getWNJetSumNoChCut(){
  Int_t tmpChcat=Chcat_;
  Chcat_=0;
  TH1F*hShape=getWNJetSum();
  hShape->SetName("getWNJetSumNoChCut");
  Chcat_=tmpChcat;

  return hShape;
}

TH1F* TauElePlotter::getWNJetSumNoChNoMTCut(){
  Int_t tmpChcat=Chcat_;
  Chcat_=0;
  Int_t tmpCategoryMT=MTcat_;
  MTcat_=-1;
  TH1F*hShape=getWNJetSum();
  hShape->SetName("getWNJetSumNoChNoMTCut");
  Chcat_=tmpChcat;
  MTcat_=tmpCategoryMT;
  return hShape;
}

TH1F* TauElePlotter::getWNJetSumLooseTau(){
  int tmpIsocat=Isocat_;
  TString tmpExtrasel=extrasel_;
  Isocat_=0;
  extrasel_+="*(muiso<0.1&&tauiso3hitraw<10.0)";

  TH1F*hShape=getWNJetSum();
  hShape->SetName("getWNJetSumLooseTau");

  Isocat_=tmpIsocat;
  extrasel_=tmpExtrasel;

  return hShape;
}


TH1F* TauElePlotter::getWJetsNJet(int shape){
  //cout<<"Calling method getWJetsNJet"<<endl;

  TH1F*hShape=0;
  if(shape==1) hShape=getWNJetSumNoChCut();
  else if(shape==2) hShape=getWNJetSumLooseTau();
  else if(shape==3) hShape=getWNJetSumNoChNoMTCut();
  else hShape=getWNJetSum();
  hShape->SetName("getWJetsNJet");

  
  Int_t tmpCategoryMT=MTcat_;
  MTcat_=3;


  TH1F* HData=getTotalData();
  TH1F* HMC=getZToTauTau(); 
  TH1F* HTT=getTTJetsInc(); 
  TH1F* HVV=getDiBoson();  
  TH1F* HZL=getZLInc();      
  TH1F* HZJ=getZToLJetInc(); 
  MTcat_=tmpCategoryMT;

  cout<<"W+Jets estimation from high mT :"<<endl;
  cout<<"Data "<<HData->Integral()<<endl;
  cout<<"ZTT "<<HMC->Integral()<<endl;
  cout<<"TT "<<HTT->Integral()<<endl;    HMC->Add(HTT);    delete HTT;
  cout<<"VV "<<HVV->Integral()<<endl;    HMC->Add(HVV);    delete HVV;
  cout<<"ZL "<<HZL->Integral()<<endl;    HMC->Add(HZL);    delete HZL;
  cout<<"ZJ "<<HZJ->Integral()<<endl;    HMC->Add(HZJ);    delete HZJ;
  cout<<"Data - MC at high mT = "<<HData->Integral()-HMC->Integral()<<endl;
    
  
  MTcat_=3;
  TH1F* HWH=getWNJetSum();  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;  
  TH1F*HWL=getWNJetSum();   HWL->SetName("HWL");


  Float_t ratio=0.;
  if(HWH->Integral()>0.){
    double NWLerr=0.;
    double NWHerr=0.;
    Float_t NWL=HWL->IntegralAndError(1,HWL->GetNbinsX(),NWLerr);
    Float_t NWH=HWH->IntegralAndError(1,HWH->GetNbinsX(),NWHerr);
    ratio=NWL/NWH;
    Float_t ratioerr=ratio*sqrt((NWLerr*NWLerr)/(NWL*NWL)+(NWHerr*NWHerr)/(NWH*NWH));

    cout<<" W high mT : "<<NWH<<" "<<NWHerr<<endl;
    cout<<" W low mT : "<<NWL<<" "<<NWLerr<<endl;
    cout<<" extrapolation factor : "<<ratio<<" +- "<<ratioerr<<endl;
  } else ratio=0.;


  /////////////////////////Normalize the Shape
  if(hShape->Integral()>0.){
    float corrFactor=((HData->Integral()-HMC->Integral())*ratio)/hShape->Integral();
    cout<<"Wjets correction factor: "<<corrFactor<<endl;
    hShape->Scale(corrFactor);
  }else     hShape->Scale(0.);

  delete HData;
  delete HMC;
  delete HWH;
  delete HWL;

  return hShape;
}





TH1F* TauElePlotter::getWJetsNJetVBFHCP(){
  //cout<<" Calling getWJetsNJetVBFHCP"<<endl;


  Int_t tmpCategoryMT=MTcat_;


  ///get W Yield at high mT without relaxed selection
  MTcat_=13;
  TH1F* HData=getTotalData(); HData->SetName("HData");
  //cout<<" Data high mT : "<<HData->Integral()<<endl;
  TH1F* HMC=getZToTauTau();  HMC->SetName("HMC");  //cout<<" ZTT high mT : "<<HMC->Integral()<<endl;
  TH1F* HTT=getTTJetsInc();  HMC->Add(HTT); //cout<<" TT high mT : "<<HTT->Integral()<<endl;
  delete HTT;
  TH1F* HVV=getDiBoson();    HMC->Add(HVV); //cout<<" VV high mT : "<<HVV->Integral()<<endl;
  delete HVV;
  TH1F* HZL=getZToLJetVBFHCP();     HMC->Add(HZL); //cout<<" ZL high mT : "<<HZL->Integral()<<endl;
  delete HZL;
  TH1F* HZJ=getZToLJetInc(); HMC->Add(HZJ); //cout<<" ZJ high mT : "<<HZJ->Integral()<<endl;
  delete HZJ;
  MTcat_=tmpCategoryMT;
  //cout<<" Data - MC high mT : "<<HData->Integral()-HMC->Integral()<<endl;


  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString tmpextrasel=extrasel_;

  ///get the mass template with relaxed selection but at low mT
  extrasel_=shapesel;
  TH1F*hShape=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  hShape->SetName("getWJetsNJetVBFHCPShape");
  extrasel_=tmpextrasel;



  ///Calculate mT extrapolation factor with relaxed selections
  TString shaperatio="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  extrasel_=shaperatio;
  MTcat_=13;
  TH1F* HWH=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;
  TH1F*HWL=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  HWL->SetName("HWL");
  extrasel_=tmpextrasel;
  

  
  if(HWH->Integral()>0.){
    double NWLerr=0.;
    double NWHerr=0.;
    Float_t NWL=HWL->IntegralAndError(1,HWL->GetNbinsX(),NWLerr);
    Float_t NWH=HWH->IntegralAndError(1,HWH->GetNbinsX(),NWHerr);
    Float_t ratio=NWL/NWH;
    Float_t ratioerr=ratio*sqrt((NWLerr*NWLerr)/(NWL*NWL)+(NWHerr*NWHerr)/(NWH*NWH));

    cout<<" W high mT : "<<NWH<<" "<<NWHerr<<endl;
    cout<<" W low mT : "<<NWL<<" "<<NWLerr<<endl;
    cout<<" extrapolation factor : "<<ratio<<" +- "<<ratioerr<<endl;

    hShape->Scale(((HData->Integral()-HMC->Integral())*ratio)/hShape->Integral());
  }else     hShape->Scale(0.);

  delete HData;
  delete HMC;
  delete HWH;
  delete HWL;

  return hShape;
}


TH1F* TauElePlotter::getWJetsNJetVBFLoose(){
  //cout<<" Calling getWJetsNJetVBFLoose"<<endl;


  Int_t tmpCategoryMT=MTcat_;


  ///get W Yield at high mT without relaxed selection
  MTcat_=13;
  TH1F* HData=getTotalData(); HData->SetName("HData");
  //cout<<" Data high mT : "<<HData->Integral()<<endl;
  TH1F* HMC=getZToTauTau();  HMC->SetName("HMC");  //cout<<" ZTT high mT : "<<HMC->Integral()<<endl;
  TH1F* HTT=getTTJetsInc();  HMC->Add(HTT); //cout<<" TT high mT : "<<HTT->Integral()<<endl;
  delete HTT;
  TH1F* HVV=getDiBoson();    HMC->Add(HVV); //cout<<" VV high mT : "<<HVV->Integral()<<endl;
  delete HVV;
  TH1F* HZL=getZToLJetVBFHCP();     HMC->Add(HZL); //cout<<" ZL high mT : "<<HZL->Integral()<<endl;
  delete HZL;
  TH1F* HZJ=getZToLJetInc(); HMC->Add(HZJ); //cout<<" ZJ high mT : "<<HZJ->Integral()<<endl;
  delete HZJ;
  MTcat_=tmpCategoryMT;
  //cout<<" Data - MC high mT : "<<HData->Integral()-HMC->Integral()<<endl;


  Int_t IsocatTmp=Isocat_;
  Isocat_=0;
  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&muiso<0.1&&tauiso3hitraw<1.5)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  hShape->SetName("getWJetsNJetVBFHCPShape");
  extrasel_=tmpextrasel;
  Isocat_=IsocatTmp;


  ///Calculate mT extrapolation factor with relaxed selections
  TString shaperatio="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  extrasel_=shaperatio;
  MTcat_=13;
  TH1F* HWH=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;
  TH1F*HWL=getWNJetSumNoChCut();//getSample("WJetsToLNu");//
  HWL->SetName("HWL");
  extrasel_=tmpextrasel;
  

  
  if(HWH->Integral()>0.){
    double NWLerr=0.;
    double NWHerr=0.;
    Float_t NWL=HWL->IntegralAndError(1,HWL->GetNbinsX(),NWLerr);
    Float_t NWH=HWH->IntegralAndError(1,HWH->GetNbinsX(),NWHerr);
    Float_t ratio=NWL/NWH;
    Float_t ratioerr=ratio*sqrt((NWLerr*NWLerr)/(NWL*NWL)+(NWHerr*NWHerr)/(NWH*NWH));

    cout<<" W high mT : "<<NWH<<" "<<NWHerr<<endl;
    cout<<" W low mT : "<<NWL<<" "<<NWLerr<<endl;
    cout<<" extrapolation factor : "<<ratio<<" +- "<<ratioerr<<endl;

    hShape->Scale(((HData->Integral()-HMC->Integral())*ratio)/hShape->Integral());
  }else     hShape->Scale(0.);

  delete HData;
  delete HMC;
  delete HWH;
  delete HWL;

  return hShape;
}


TH1F* TauElePlotter::getWJetsNJetVBFTight(){
  ///cout<<" Calling getWJetsNJetVBFTight"<<endl;


  Int_t tmpCategoryMT=MTcat_;


  ///get W Yield at high mT without relaxed selection
  MTcat_=13;
  TH1F* HData=getTotalData(); HData->SetName("HData");
  //cout<<" Data high mT : "<<HData->Integral()<<endl;
  TH1F* HMC=getZToTauTau();  HMC->SetName("HMC");  //cout<<" ZTT high mT : "<<HMC->Integral()<<endl;
  TH1F* HTT=getTTJetsInc();  HMC->Add(HTT); //cout<<" TT high mT : "<<HTT->Integral()<<endl;
  delete HTT;
  TH1F* HVV=getDiBoson();    HMC->Add(HVV); //cout<<" VV high mT : "<<HVV->Integral()<<endl;
  delete HVV;
  TH1F* HZL=getZToLJetVBFHCP();     HMC->Add(HZL); //cout<<" ZL high mT : "<<HZL->Integral()<<endl;
  delete HZL;
  TH1F* HZJ=getZToLJetInc(); HMC->Add(HZJ); //cout<<" ZJ high mT : "<<HZJ->Integral()<<endl;
  delete HZJ;
  MTcat_=tmpCategoryMT;
  //cout<<" Data - MC high mT : "<<HData->Integral()-HMC->Integral()<<endl;


  Int_t IsocatTmp=Isocat_;
  Isocat_=0;
  //TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&muiso<0.1&&tauiso3hitraw<10&&ditaumetpt>100)";
  TString tmpextrasel=extrasel_;
  //extrasel_=shapesel;
  extrasel_+="*(muiso<0.1&&tauiso3hitraw<10)";
  TH1F*hShape=getWNJetSumNoChCut();
  hShape->SetName("getWJetsNJetVBFHCPShape");
  extrasel_=tmpextrasel;
  Isocat_=IsocatTmp;


  ///Calculate mT extrapolation factor with relaxed selections
  TString shaperatio="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  extrasel_=shaperatio;
  MTcat_=13;
  TH1F* HWH=getWNJetSumNoChCut();
  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;
  TH1F*HWL=getWNJetSumNoChCut();
  HWL->SetName("HWL");
  extrasel_=tmpextrasel;
  

  
  if(HWH->Integral()>0.){
    double NWLerr=0.;
    double NWHerr=0.;
    Float_t NWL=HWL->IntegralAndError(1,HWL->GetNbinsX(),NWLerr);
    Float_t NWH=HWH->IntegralAndError(1,HWH->GetNbinsX(),NWHerr);
    Float_t ratio=NWL/NWH;
    Float_t ratioerr=ratio*sqrt((NWLerr*NWLerr)/(NWL*NWL)+(NWHerr*NWHerr)/(NWH*NWH));

    cout<<" W high mT : "<<NWH<<" "<<NWHerr<<endl;
    cout<<" W low mT : "<<NWL<<" "<<NWLerr<<endl;
    cout<<" extrapolation factor : "<<ratio<<" +- "<<ratioerr<<endl;

    hShape->Scale(((HData->Integral()-HMC->Integral())*ratio)/hShape->Integral());
  }else     hShape->Scale(0.);

  delete HData;
  delete HMC;
  delete HWH;
  delete HWL;

  return hShape;
}



TH1F* TauElePlotter::getWJetsIncShape(){
  //cout<<" calling method getWJetsIncShape"<<endl;

  TString tmpextrasel=extrasel_;
  Int_t tmpMTcat=MTcat_;
  Int_t tmpIsocat=Isocat_;

  //get shape from relaxed iso 
  Isocat_=-1;//relax isolation
  extrasel_ = extrasel_ + "*(muiso<0.15&&tauiso3hitraw<5.0)";
  cout<<" using relaxed iso selection :"<<extrasel_<<endl;
  TH1F* hShape=getSample("WJetsToLNu");
  hShape->SetName("hShape");

  //go to high mT still with relaxed 
  MTcat_=13;
  if(MSSMFlag_)MTcat_=103;
  TH1F* hShapeHighMT=getSample("WJetsToLNu");
  hShapeHighMT->SetName("hShapeHighMT");

  //normalize to at high mT
  extrasel_=tmpextrasel;
  Isocat_=tmpIsocat;
  TH1F* hDataHighMT=getTotalData();
  TH1F* hZToTauTau=getZToTauTau();
  hDataHighMT->Add(hZToTauTau,-1); delete hZToTauTau;
  TH1F* hTTJets=getSample("TTJets");
  hDataHighMT->Add(hTTJets,-1); delete hTTJets;
  TH1F* hZToLJet=getSample("ZToLJet");
  hDataHighMT->Add(hZToLJet,-1); delete hZToLJet;
  TH1F* hZToEE=getSample("ZToEE");
  hDataHighMT->Add(hZToEE,-1); delete hZToEE;
  MTcat_=tmpMTcat;

  //
  hShape->Scale(hDataHighMT->Integral()/hShapeHighMT->Integral());
  delete hDataHighMT;
  delete hShapeHighMT;
  
  return hShape;
}


TH1F* TauElePlotter::getW3JetsVBF(){
  //cout<<" calling getW3JetsVBF"<<endl;

  //
  TString TmpExtrasel=extrasel_;
  Int_t tmpMTcat=MTcat_;
  Int_t tmpIsocat=Isocat_;
  
  //relax selections
  Isocat_=-1;
  extrasel_="(njet>=2&&muiso<0.1&&tauiso3hitraw<5.0)"; 
  cout<<"Determining WJets template with relaxed selection: "<<extrasel_<<endl;
  TH1F* hW3JetsShape=getSample("W3JetsToLNu");
  hW3JetsShape->SetName("hW3JetsShape");

  //go to high mT with same selections as shape
  MTcat_=13;
  TH1F* hW3JetsShapeHighMT=getSample("W3JetsToLNu");
  hW3JetsShapeHighMT->SetName("hW3JetsShapeHighMT");

  //return to VBF catetory But stay at high mT
  Isocat_=tmpIsocat;
  extrasel_=TmpExtrasel;  

  //determine normalization factor
  //Get Data and TTJets at high mT in VBF
  TH1F* hDataHighMT=getTotalData();
  hDataHighMT->SetName("hDataHighMT");
  TH1F* hMCHighMT=getSample("TTJets");
  hMCHighMT->SetName("hMC");
  TH1F* hZTTHighMT=getZToTauTau();
  hMCHighMT->Add(hZTTHighMT); delete hZTTHighMT;
  TH1F* hZJHighMT=getSample("ZToLJet");
  hMCHighMT->Add(hZJHighMT); delete hZJHighMT;
  TH1F* hZEEHighMT=getSample("ZToEE");
  hMCHighMT->Add(hZEEHighMT); delete hZEEHighMT;


  //Return signal mT region
  MTcat_=tmpMTcat;

  //normalize the Template
  hW3JetsShape->Scale((hDataHighMT->Integral()-hMCHighMT->Integral())/hW3JetsShapeHighMT->Integral());
  
  delete hW3JetsShapeHighMT;
  delete hDataHighMT;
  delete hMCHighMT;

  return hW3JetsShape;
}


TH1F* TauElePlotter::getW2JetsBJet(){
  //cout<<" calling getW2JetsBJet"<<endl;

// Keti:
// Wjets normalization with p_zeta
// *check normalization in p_zeta < -40
// * apply the correction in signal region p_zeta > -20
// Wjets shape
// * using W2jet sample without bjet requirement (if needed) and without OS charge requirement.


  TString TmpExtrasel=extrasel_;
  Int_t tmpIsocat=Isocat_;
  Isocat_=-1;
  extrasel_=extrasel_+"*(muiso<0.1&&tauiso3hitraw<5.0)";
  TH1F* hWJetsShape=getSample("W2JetsToLNu");
  hWJetsShape->SetName("hWJetsShape");
  cout<<"hWJetsShape :"<<hWJetsShape->Integral()<<endl;

  Int_t tmpMTcat=MTcat_;
  MTcat_=103;  //need to know integral of template at high mT for normalization

  TH1F* hWJetsShapeHighMT=getSample("W2JetsToLNu");
  hWJetsShapeHighMT->SetName("hWJetsShapeHighMT");
  Isocat_=tmpIsocat;
  extrasel_=TmpExtrasel;  //return to category at but stay at high mT

  //determine normalization factor
  TH1F* hDataHighMT=getTotalData();
  hDataHighMT->SetName("hDataHighMT");
  cout<<"hDataHighMT :"<<hDataHighMT->Integral()<<endl;
  TH1F* hTTJetsHighMT=getSample("TTJets");
  hTTJetsHighMT->SetName("hTTJetsHighMT");
  cout<<"hTTJetsHighMT :"<<hTTJetsHighMT->Integral()<<endl;
  TH1F* hZTTHighMT=getZToTauTau();
  hZTTHighMT->SetName("hZTTHighMT");
  cout<<"hZTTHighMT :"<<hZTTHighMT->Integral()<<endl;
  TH1F* hZJHighMT=getSample("ZToLJet");
  hZJHighMT->SetName("hZJHighMT");
  cout<<"hZJHighMT :"<<hZJHighMT->Integral()<<endl;
  MTcat_=tmpMTcat;//Return signal  region

  //normalize the Template
  Float_t WJetsIntegralFromhighMTData=(hDataHighMT->Integral()-hTTJetsHighMT->Integral()-hZTTHighMT->Integral()-hZJHighMT->Integral());
  float scale=WJetsIntegralFromhighMTData/hWJetsShapeHighMT->Integral();
  cout<<" Scaling WJets by factor "<<scale<<endl;
  hWJetsShape->Scale(scale);
  
  delete hWJetsShapeHighMT;
  delete hDataHighMT;
  delete hTTJetsHighMT;
  delete hZTTHighMT;
  delete hZJHighMT;

  cout<<"----> getWJets yield: "<<hWJetsShape->Integral()<<endl;
 

  return hWJetsShape;
}




////////QCD estimations/////////////////////////////////

TH1F* TauElePlotter::getQCDInc(Int_t WType,bool cleanNegativeBins){
  //cout<<"Calling method getQCDInc"<<endl;

  TH1F* h=getPlotHisto("hQCDInc");
  
  int ChcatTmp=Chcat_;
  Chcat_=2;

  TH1F* hDataSS=getTotalData();
  cout<<"    Data "<<hDataSS->Integral()<<endl;
  h->Add(hDataSS);
  delete hDataSS;


  //subtract MC
  TH1F*hWJets=0;
  if(WType==0)hWJets=getWJetsInc();   
  if(WType==1)hWJets=getWJetsNJet();   
  cout<<"   W "<<hWJets->Integral()<<endl;
  h->Add(hWJets,-1);
  delete hWJets;

  Int_t ZTTypeTmp=ZTTType_;
  ZTTType_=1; //always use MC in SS
  TH1F*hZToTauTau=getZToTauTau();   
  cout<<"   ZTT "<<hZToTauTau->Integral()<<endl; 
  h->Add(hZToTauTau,-1); 
  delete hZToTauTau;
  ZTTType_=ZTTypeTmp;


  TH1F*hDiBoson=getDiBoson();      h->Add(hDiBoson,-1);  cout<<"   VV "<<hDiBoson->Integral()<<endl;
  delete hDiBoson;
  TH1F*hTTJets=getTTJetsInc();     h->Add(hTTJets,-1);  cout<<"    TT "<<hTTJets->Integral()<<endl;
  delete hTTJets;
  TH1F*hZToEE=getZLInc();          h->Add(hZToEE,-1);   cout<<"    ZL "<<hZToEE->Integral()<<endl;
  delete hZToEE;
  TH1F*hZLJet=getZToLJetInc();     h->Add(hZLJet,-1);   cout<<"    ZJ "<<hZLJet->Integral()<<endl;
  delete hZLJet;

  Chcat_=ChcatTmp;

  ///if integral of histo is <=0 scale to 0
  if(h->Integral()<0.)h->Scale(0.);

  ///clean any negative bins
  if(cleanNegativeBins)
    for(Int_t b=1;b<=h->GetNbinsX();b++)
      if(h->GetBinContent(b)<0.)h->SetBinContent(b,0.);
  
  ////Scale to go to OS
  h->Scale(QCDOStoSSRatio_);

  cout<<" getQCDInc yield ="<<h->Integral()<<endl;
  return h;
}


TH1F* TauElePlotter::getQCDIncWNJet(){
  return getQCDInc(1,0);
}


TH1F* TauElePlotter::getQCDMuIso(){
  //cout<<"Calling method getQCDMuIsoSM"<<endl;

  TH1F* hNorm=getQCDInc(1,0);
  
  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;

  TString extraselTmp=extrasel_;
  extrasel_+="*(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)";  
  TH1F* hShape=getTotalData();

  Isocat_=IsocatTmp;
  Chcat_=ChcatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  
  delete hNorm;
  return hShape;
}

TH1F* TauElePlotter::getQCDMuIsoLooseTau(){
  //cout<<"Calling method getQCDMuIsoSM"<<endl;

  TH1F* hNorm=getQCDInc();
  
  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;

  TString extraselTmp=extrasel_;
  extrasel_+="*(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10)";  
  TH1F* hShape=getTotalData();

  Isocat_=IsocatTmp;
  Chcat_=ChcatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  
  delete hNorm;
  return hShape;
}


///Method for 1-Jet Low pT category
TH1F* TauElePlotter::getQCDIncLowPt(){
  //cout<<"Calling method getQCDIncLowPt"<<endl;

  TH1F*hNorm=getQCDIncWNJet();

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  //extrasel_="(njet20>=1&&tauisomva>0.7&&0.2<muiso&&muiso<0.5&&taupt<40&&metpt>30)";  
  extrasel_="(njet20>=1&&tauiso3hitraw<5.0&&0.2<muiso&&muiso<0.5&&taupt<40&&metpt>30)";  
  //extrasel_="(njet20>=1&&tauisomva>0.&&muiso<0.1&&taupt<40)";  
  
  TH1F* hShape=getTotalData();

  Isocat_=IsocatTmp;
  Chcat_=ChcatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0.)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  
  delete hNorm;
  return hShape;
}

///Method for 1-Jet High pT category
TH1F* TauElePlotter::getQCDIncHighPt(){
  //cout<<"Calling method getQCDHighPt"<<endl;

  TH1F*hNorm=getQCDIncWNJet();///Note is using the of exclusive W samples

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  extrasel_="(0.2<muiso&&tauiso3hitraw<10.0&&taupt>40&&metpt>20)";  

  TH1F* hShape=getTotalData();
  hShape->SetName("getQCDIncHighPtShape");

  Isocat_=IsocatTmp;
  Chcat_=ChcatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0.)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);

  delete hNorm;  
  return hShape;
}


TH1F* TauElePlotter::getQCDIncLooseShape(){
  //cout<<"Calling method getQCDIncLooseShape"<<endl;

  //integral from SS after subtracting MC
  TH1F* hNorm=getQCDInc();

  //Shape from sample with loose isolation
  TH1F* h=getPlotHisto("hQCDShape");
  
  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  extrasel_+="*(muiso>0.2&&tauiso3hitraw<5.0)";
  
  TH1F* hDataSS=getTotalData();
  h->Add(hDataSS);
  delete hDataSS;
//   TH1F*hMC=getTotalMCSM();
//   h->Add(hMC,-1);
//   delete hMC;

  //remove negative bins
  for(Int_t b=1;b<=h->GetNbinsX();b++)
    if(h->GetBinContent(b)<0.)h->SetBinContent(b,0);

  Chcat_=ChcatTmp;
  Isocat_=IsocatTmp;
  extrasel_=extraselTmp;

  if(h->Integral()>0)
    h->Scale(TMath::Max(hNorm->Integral(),1.)/h->Integral());
  
  delete hNorm;
  return h;
}


TH1F* TauElePlotter::getQCDIncWJetsShape(){
  //cout<<"Calling method getQCDIncWJetsShape"<<endl;

  //yield
  TH1F* h=getPlotHisto("hQCDInc");

  int ChcatTmp=Chcat_;
  Chcat_=2;


  TH1F* hDataSS=getTotalData();
  h->Add(hDataSS); delete hDataSS;
  TH1F*hZToTauTau=getZToTauTau();
  h->Add(hZToTauTau,-1);  delete hZToTauTau;
  TH1F*hWJets=getWJetsIncShape();
  h->Add(hWJets,-1);  delete hWJets;
  TH1F*hDiBoson=getDiBoson();
  h->Add(hDiBoson,-1);  delete hDiBoson;
  TH1F*hTTJets=getTTJetsInc();
  h->Add(hTTJets,-1);  delete hTTJets;
  TH1F*hZLJet=getZToLJetInc();
  h->Add(hZLJet,-1);  delete hZLJet;
  TH1F*hZToEE=getZLInc();
  h->Add(hZToEE,-1);  delete hZToEE;  

  cout<<"Scaling QCDInc by OS/SS factor "<<QCDOStoSSRatio_<<endl;
  h->Scale(QCDOStoSSRatio_);

  //Shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<%.3f&&tauiso3hitraw>%.3f)",0.5,5.0);//for shape
  cout<<"QCD Shape with loose iso cut: "<<isocuttxtshape<<endl;
  Int_t tmpIsocat=Isocat_;
  TString tmpExtrasel=extrasel_;
  Isocat_=-1;
  extrasel_=extrasel_+"*"+isocuttxtshape;
  TH1F* hShape=getTotalData(); 
  TH1F*hZToTauTauShape=getZToTauTau();
  hShape->Add(hZToTauTauShape,-1);  delete hZToTauTauShape;
  TH1F*hWJetsShape=getWJetsIncShape();
  hShape->Add(hWJetsShape,-1);  delete hWJetsShape;
  TH1F*hDiBosonShape=getDiBoson();
  hShape->Add(hDiBosonShape,-1);  delete hDiBosonShape;
  TH1F*hTTJetsShape=getTTJetsInc();
  hShape->Add(hTTJetsShape,-1);  delete hTTJetsShape;
  TH1F*hZLJetShape=getZToLJetInc();
  hShape->Add(hZLJetShape,-1);  delete hZLJetShape;
  TH1F*hZToEEShape=getZLInc();
  hShape->Add(hZToEEShape,-1);  delete hZToEEShape;  
  hShape->Scale(h->Integral()/hShape->Integral()); delete h;
  Isocat_=tmpIsocat;
  extrasel_=tmpExtrasel;


  Chcat_=ChcatTmp;

  return hShape;
}


TH1F* TauElePlotter::getQCDIncFit(){
  //cout<<"Calling method getQCDIncFit"<<endl;
  
  /////////////////fit to the transversemass
  //-relax mT cut to be able to make fit
  //-subtract ZTT+ZEE+ZJ+TT backgrounds
  //-get WJets template
  //-get QCD template
  //-fit the remaing SS distribution with these templates
  //get QCD in varible distribution with full selections
  //get QCD in variable distribution withou mT cut
  //rescale first QCD 

  
  int ChcatTmp=Chcat_;
  Chcat_=2;

  Int_t tmpIsocat=Isocat_;
  Int_t tmpMTcat=MTcat_;
  TString tmpExtrasel=extrasel_;
  TString tmpPlotVar=plotvar_;
  float tmpXmin=xmin_;
  float tmpXmax=xmax_;
  Int_t tmpnbins=nbins_;

  /// get mT distribution in SS Data
  MTcat_=0;
  plotvar_="transversemass";
  xmin_=0;
  xmax_=200;
  nbins_=20;

  TH1F* hQDCPlusW=getPlotHisto("hQDCPlusW");
  TH1F* hDataSS=getTotalData();
  hQDCPlusW->Add(hDataSS);       delete hDataSS;
  TH1F*hZToTauTau=getZToTauTau();
  hQDCPlusW->Add(hZToTauTau,-1); delete hZToTauTau;
  TH1F*hDiBoson=getDiBoson();
  hQDCPlusW->Add(hDiBoson,-1);   delete hDiBoson;
  TH1F*hTTJets=getTTJetsInc();
  hQDCPlusW->Add(hTTJets,-1);    delete hTTJets;
  TH1F*hZLJet=getZToLJetInc();
  hQDCPlusW->Add(hZLJet,-1);     delete hZLJet;
  TH1F*hZToEE=getZLInc();
  hQDCPlusW->Add(hZToEE,-1);     delete hZToEE;


  Isocat_=-1;
  
  //Get QCD template
  char isocuttxtshapeQCD[100];
  sprintf(isocuttxtshapeQCD,"(0.1<muiso&&muiso<0.2&&tauiso3hitraw<5.0)");//for shape
  cout<<"QCD template iso cut: "<<isocuttxtshapeQCD<<endl;
  extrasel_=extrasel_+"*"+isocuttxtshapeQCD;
  TH1F*hQCDTemplate=getTotalData();
  hQCDTemplate->SetName("hQCDTemplate");
  TH1F*hZToTauTauQCDTemplate=getZToTauTau();
  hQCDTemplate->Add(hZToTauTauQCDTemplate,-1); delete hZToTauTauQCDTemplate;
  TH1F*hDiBosonQCDTemplate=getDiBoson();
  hQCDTemplate->Add(hDiBosonQCDTemplate,-1);   delete hDiBosonQCDTemplate;
  TH1F*hTTJetsQCDTemplate=getTTJetsInc();
  hQCDTemplate->Add(hTTJetsQCDTemplate,-1);    delete hTTJetsQCDTemplate;
  TH1F*hZLJetQCDTemplate=getZToLJetInc();
  hQCDTemplate->Add(hZLJetQCDTemplate,-1);     delete hZLJetQCDTemplate;
  TH1F*hZToEEQCDTemplate=getZLInc();
  hQCDTemplate->Add(hZToEEQCDTemplate,-1);     delete hZToEEQCDTemplate;
  if(hQCDTemplate->Integral()>0.){
    hQCDTemplate->Scale(1./hQCDTemplate->Integral());
  }else return 0;
  //There is some W contribution with this isolation cut but dont know how to normalize it so just ignored
  extrasel_=tmpExtrasel;


  //Get WJets template
  extrasel_=extrasel_+"*(muiso<0.15&&tauiso3hitraw<5.0)";
  TH1F*hWTemplate=getSample("WJetsToLNu");
  hWTemplate->Scale(1./hWTemplate->Integral());
  extrasel_=tmpExtrasel;


  Isocat_=tmpIsocat;


  ///
  //  RooRealVar x("x","x",xmin_,xmax_);
  //  RooDataHist QCDDataHist("QCDDataHist","",RooArgList(x),hQCDTemplate);
//   RooDataHist WDataHist("WDataHist","",RooArgList(x),hWTemplate);
//   RooHistPdf QCDPdf("QCDPdf","",RooArgSet(x),QCDDataHist);
//   RooHistPdf WPdf("WPdf","",RooArgSet(x),WDataHist);
//  RooRealVar coef("coef","coef",0,1);
//  RooAddPdf QCDPlusWPdf("QCDPlusWPdf","",WPdf,QCDPdf,coef);
//   RooDataHist QCDPlusWData("QCDPlusWData","",RooArgList(x),hQDCPlusW);
//   RooChi2Var chi2("chi2","chi2",QCDPlusWPdf,QCDPlusWData);
//   RooMinuit min(chi2);
//   min.migrad();

  //normalize template to fitted yield
  //hQCDTemplate->Scale(coef.getVal()*hQDCPlusW->Integral());

  //show the fit
  TCanvas cTemp;
  //hWTemplate->Scale((1-coef.getVal())*hQDCPlusW->Integral());
  hWTemplate->Add(hQCDTemplate);
  hWTemplate->SetFillColor(kRed+2);
  hWTemplate->Draw("hist");
  hQCDTemplate->SetFillColor(kMagenta-10);
  hQCDTemplate->Draw("histsame");
  hQDCPlusW->Draw("histpesame");
  cTemp.Print("TauElePlotter_getQCDIncFit.ps");
  delete hQDCPlusW;
  delete hWTemplate;



  //return to normal
  Isocat_=tmpIsocat;
  extrasel_=tmpExtrasel;
  MTcat_=tmpMTcat;
  plotvar_=tmpPlotVar;
  xmin_=tmpXmin;
  xmax_=tmpXmax;
  nbins_=tmpnbins;

  ///now get the distribution for plot 
  Isocat_=-1;
  extrasel_=extrasel_+"*"+isocuttxtshapeQCD;
  TH1F*hQCDIncFit=getTotalData();
  hQCDIncFit->SetName("hQCDIncFit");
  TH1F*hZToTauTauPlotVar=getZToTauTau();
  hQCDIncFit->Add(hZToTauTauPlotVar,-1); delete hZToTauTauPlotVar;
  TH1F*hDiBosonPlotVar=getDiBoson();
  hQCDIncFit->Add(hDiBosonPlotVar,-1);   delete hDiBosonPlotVar;
  TH1F*hTTJetsPlotVar=getTTJetsInc();
  hQCDIncFit->Add(hTTJetsPlotVar,-1);    delete hTTJetsPlotVar;
  TH1F*hZLJetPlotVar=getZToLJetInc();
  hQCDIncFit->Add(hZLJetPlotVar,-1);     delete hZLJetPlotVar;
  TH1F*hZToEEPlotVar=getZLInc();
  hQCDIncFit->Add(hZToEEPlotVar,-1);     delete hZToEEPlotVar;
  //normalize according to QCD yield from fit above
  MTcat_=0;
  TH1F*hQCDforNorm=getTotalData();
  hQCDIncFit->Scale(hQCDTemplate->Integral()/hQCDforNorm->Integral());
  delete hQCDTemplate;
  delete hQCDforNorm;
  Isocat_=tmpIsocat;
  extrasel_=tmpExtrasel;
  MTcat_=tmpMTcat;

  cout<<"Scaling QCDInc by OS/SS factor "<<QCDOStoSSRatio_<<endl;
  hQCDIncFit->Scale(QCDOStoSSRatio_);


  Chcat_=ChcatTmp;
  return hQCDIncFit;
}


  
TH1F* TauElePlotter::getQCDKeti(){
  //cout<<" Calling getQCDKeti only for b-jet category "<<endl;


// QCD normalization (Keti)
// * in SS data events in p_zeta>-20 remove contribution from SM processes, multiply by 1.11
// ---> When I was doing before, I was using normalization for SS Wjets obtained from inclusive selection, i.e. not b-jet requirement.
// QCD shape
// * in SS data events with inverted isolation on muon (0.15 < iso < 0.4). The rest of the selection is the same as category.


  
  int ChcatTmp=Chcat_;
  Chcat_=2;

  //determine yield
  cout<<"QCD yield from SSx1.11 method"<<endl;

  TH1F* hQCDInc=getPlotHisto("hQCDKeti");

  TH1F* hDataSS=getTotalData();
  hQCDInc->Add(hDataSS);
  delete hDataSS;

  TH1F*hZToTauTauSS=getZToTauTau();
  if(!hZToTauTauSS)return 0;
  hQCDInc->Add(hZToTauTauSS,-1);
  delete hZToTauTauSS;

  cout<<"Using getW2JetsBJetSS, should only be used for b-jet category"<<endl;
  TH1F*hWJetsSS=getW2JetsBJet();
  if(!hWJetsSS) return 0;
  hQCDInc->Add(hWJetsSS,-1);
  delete hWJetsSS;

  TH1F*hTTJetsSS=getTTJetsInc();
  if(!hTTJetsSS)return 0;
  hQCDInc->Add(hTTJetsSS,-1);
  delete hTTJetsSS;

  TH1F*hZToLJetSS=getZToLJetInc();
  if(!hZToLJetSS)return 0;
  hQCDInc->Add(hZToLJetSS,-1);
  delete hZToLJetSS;

  TH1F*hZToEESS=getZLInc();
  if(!hZToEESS)return 0;
  hQCDInc->Add(hZToEESS,-1);
  delete hZToEESS;

  TH1F*hDiBosonSS=getDiBoson();
  if(!hDiBosonSS)return 0;
  hQCDInc->Add(hDiBosonSS,-1);
  delete hDiBosonSS;

  hQCDInc->Scale(QCDOStoSSRatio_);



  //determine shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(%.2f<muiso&&muiso<%.2f&&tauiso3hitraw<%.2f)",0.2,0.5,5.0);//for shape
  cout<<"Calculating QCD Shape with Keti Method and loose iso cut: "<<isocuttxtshape<<endl;
  
  Int_t TmpIsocat=Isocat_;
  TString TmpExtrasel=extrasel_;  
  Isocat_=-1;
  extrasel_=extrasel_+"*"+isocuttxtshape;
  TH1F* hQCDShape = getTotalData(); 

  TH1F* hZToTauTau = getZToTauTau();
  hQCDShape->Add(hZToTauTau,-1);
  delete hZToTauTau;

  TH1F*hTTJets=getTTJetsInc();
  hQCDShape->Add(hTTJets,-1);
  delete hTTJets;

  TH1F*hZLJet=getZToLJetInc();
  hQCDShape->Add(hZLJet,-1);
  delete hZLJet;

  //return selections
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;


  if(hQCDShape->Integral()>0)
    hQCDShape->Scale(hQCDInc->Integral()/hQCDShape->Integral());
  else {
    cout<<"hQCDShape integral is 0 cannot normalize shape "<<endl;
    hQCDShape->Scale(0.);
  }
  delete hQCDInc;


  Chcat_=ChcatTmp;
  return hQCDShape;
}

TH1F* TauElePlotter::getQCDKetiHCP(){
  cout<<"getQCDKetiHCP"<<endl;

  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5)");//for normalization
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5)");//for shape 
  
  Int_t TmpIsocat=Isocat_;
  TString TmpExtrasel=extrasel_;  
  Int_t ChcatTmp=Chcat_;
  Chcat_=2;

  //SS incl QCD 
  Isocat_=1;
  extrasel_="1";
  TH1F* hQCDInc = getQCDInc();  if(!hQCDInc){cout<<" QCDInc not determined "<<endl; return 0;}   hQCDInc->SetName("hQCDInc");
 
  //SS Loose Incl QCD 
  Isocat_=-1;
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();  if(!hDataIncLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataIncLoose->SetName("hDataIncLoose");

  //SS Loose VBF QCD 
  Isocat_=-1;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();  if(!hDataVBFLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataVBFLoose->SetName("hDataVBFLoose");

  //QCD Shape
  Isocat_=-1;  
  extrasel_=TmpExtrasel+"*"+isocuttxtshape;
  TH1F* hShape=getTotalData();  if(!hShape){cout<<" hShape not made "<<endl; return 0;}   hShape->SetName("hShape");

  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  hShape->Scale(hQCDInc->Integral()*(hDataVBFLoose->Integral()/hDataIncLoose->Integral())/hShape->Integral());



  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}



TH1F* TauElePlotter::getQCDMike(){

  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<%.3f&&tauiso3hitraw<%.3f)",0.5,5.0);//for normalization
  cout<<"Calculating QCD Yield with Mike Method and loose iso cut: "<<isocuttxt<<endl;
  
  int ChcatTmp=Chcat_;
  Chcat_=2;

  Int_t TmpIsocat=Isocat_;
  TString TmpExtrasel=extrasel_;  
   
  //SS incl QCD 
  Isocat_=1;
  extrasel_="1";
  //if(!scaleSamplesLumi())return 0;
  TH1F* hQCDInc = getQCDInc();
  hQCDInc->SetName("hQCDInc");
 
  //SS Loose Incl QCD 
  Isocat_=-1;
  extrasel_=isocuttxt;
  //if(!scaleSamplesLumi())return 0;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");
  TH1F* hMCIncLoose = getTotalMCSM(1);
  hMCIncLoose->SetName("hMCIncLoose");

  //SS Loose VBF QCD 
  Isocat_=-1;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  //if(!scaleSamplesLumi())return 0;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");
  TH1F* hMCVBFLoose=getTotalMCSM(1);
  hMCVBFLoose->SetName("hMCVBFLoose");
  

  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<%.3f&&tauiso3hitraw<%.3f)",0.5,5.0);//for shape
  cout<<"Calculating QCD Shape with Mike Method and loose iso cut: "<<isocuttxtshape<<endl;

  //SS Loose VBF QCD 
  Isocat_=-1;  
  extrasel_=TString("(njet>=2)*")+isocuttxtshape;
  //if(!scaleSamplesLumi())return 0;
  TH1F* hDataVBFLooseShape=getTotalData();
  hDataVBFLooseShape->SetName("hDataVBFLooseShape");
  TH1F* hMCVBFLooseShape=getTotalMCSM(1);
  hMCVBFLooseShape->SetName("hMCVBFLooseShape");
  
  TH1F* hShape=getPlotHisto("hSMQCDShape");

  hShape->Sumw2();
  hShape->Add(hDataVBFLooseShape);
  hShape->Add(hMCVBFLooseShape,-1.);


  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  if(!scaleSamplesLumi())return 0;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  TH1F* h=getPlotHisto("hSMQCD");//new TH1F("hSMQCD","QCD",nbins_,xmin_,xmax_);
  h->Sumw2();
  h->Add(hDataVBFLoose);
  h->Add(hMCVBFLoose,-1.);
  cout<<"QCD sideband integral for datacard : "<< h->Integral()<<endl;

  
  //smear
  TH1F * htmp  = smearHisto(h);
  delete h;
  h = htmp;

  //scale to yield
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  cout<<" MCVBFLoose "<<hMCVBFLoose->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  cout<<" MCIncLoose "<<hMCIncLoose->Integral()<<endl;
  h->Scale(hQCDInc->Integral()/(hDataIncLoose->Integral()-hMCIncLoose->Integral()));
  hShape->Scale(h->Integral()/hShape->Integral());
  delete hDataIncLoose;
  delete hMCIncLoose;
  delete hDataVBFLoose;
  delete hMCVBFLoose;
  delete h;

  Chcat_=ChcatTmp;
  return hShape;
}


TH1F* TauElePlotter::getQCDVBFHCP(){

  char isocuttxt[100];
  //sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauisomva>0.7)");
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)");
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)");
  
  Int_t TmpIsocat=Isocat_;
  TString TmpExtrasel=extrasel_;  
  Int_t ChcatTmp=Chcat_;


  //SS incl QCD 
  Isocat_=1;
  extrasel_="1";
  TH1F* hQCDInc = getQCDIncWNJet(); //getQCDInc();
  if(!hQCDInc){cout<<" QCDInc not determined "<<endl; return 0;} 
  hQCDInc->SetName("getQCDVBFHCPIncl");
 

  Chcat_=2;

  //SS Loose Incl QCD 
  Isocat_=0;
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");

  //SS Loose VBF QCD 
  Isocat_=0;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");

  //QCD Shape
  Isocat_=0;  
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData(); 
  hShape->SetName("getQCDVBFHCPhShape");

  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  hShape->Scale(hQCDInc->Integral()*(hDataVBFLoose->Integral()/hDataIncLoose->Integral())/hShape->Integral());

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}


TH1F* TauElePlotter::getQCDVBFHCP2(){
  //cout<<" Calling getQCDVBFHCP2"<<endl;
 
  TString TmpExtrasel=extrasel_;  
  Int_t TmpIsocat=Isocat_;
  Int_t ChcatTmp=Chcat_;
  Chcat_=2;


  //get the OS inclusive QCD yield
  extrasel_="1";
  TH1F* hQCDInc = getQCDInc(1,0);
  hQCDInc->SetName("getQCDVBFHCP2Incl");


  //QCD Shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)");
  Isocat_=0;  
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData(); 
  hShape->SetName("getQCDVBFHCP2hShape");


  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)");
  //SS Loose Incl QCD 
  Isocat_=0;
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");

  //SS Loose VBF QCD 
  Isocat_=0;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");

  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  float ratio=(hDataVBFLoose->Integral()/hDataIncLoose->Integral());
  hShape->Scale(hQCDInc->Integral()*ratio/hShape->Integral());
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  cout<<" VBF ratio "<<ratio<<endl;
  cout<<" QCD yield = "<<hShape->Integral()<<endl;

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}

TH1F* TauElePlotter::getQCDVBFLoose(){
  //cout<<" Calling getQCDVBFLooseIso"<<endl;
 
  TString TmpExtrasel=extrasel_;  
  Int_t TmpIsocat=Isocat_;
  Int_t ChcatTmp=Chcat_;
  Chcat_=2;


  //get the OS inclusive QCD yield
  extrasel_="1";
  TH1F* hQCDInc = getQCDInc(1,0);
  hQCDInc->SetName("getQCDVBFHCP2Incl");


  //QCD Shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)");
  Isocat_=0;  
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData(); 
  hShape->SetName("getQCDVBFHCP2hShape");


  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)");
  //SS Loose Incl QCD 
  Isocat_=0;
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");

  //SS Loose VBF QCD 
  Isocat_=0;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");

  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  float ratio=(hDataVBFLoose->Integral()/hDataIncLoose->Integral());
  hShape->Scale(hQCDInc->Integral()*ratio/hShape->Integral());
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  cout<<" VBF ratio "<<ratio<<endl;
  cout<<" QCD yield = "<<hShape->Integral()<<endl;

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}



TH1F* TauElePlotter::getQCDVBFTight(){
  //cout<<" Calling getQCDVBFTight"<<endl;
 
  TString TmpExtrasel=extrasel_;  
  Int_t TmpIsocat=Isocat_;
  Int_t ChcatTmp=Chcat_;
  Chcat_=2;


  //get the OS inclusive QCD yield
  extrasel_="1";
  TH1F* hQCDInc = getQCDInc(1,0);
  hQCDInc->SetName("getQCDVBFHCP2Incl");


  //QCD Shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)");
  Isocat_=0;  
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData(); 
  hShape->SetName("getQCDVBFHCP2hShape");


  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10.0)");
  //SS Loose Incl QCD 
  Isocat_=0;
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");

  //SS Loose VBF QCD 
  Isocat_=0;  
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");

  //return scale factors to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  float ratio=(hDataVBFLoose->Integral()/hDataIncLoose->Integral());
  hShape->Scale(hQCDInc->Integral()*ratio/hShape->Integral());
  cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
  cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
  cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
  cout<<" VBF ratio "<<ratio<<endl;
  cout<<" QCD yield = "<<hShape->Integral()<<endl;

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}



TH1F* TauElePlotter::getQCDBoostTight(){
  //cout<<"=====Calling getQCDBoostTight======"<<endl;

  //incl QCD 
  TString TmpExtrasel=extrasel_; 
  extrasel_="1";
  TH1F* hQCDInc = getQCDIncWNJet();
  hQCDInc->SetName("hQCDInc");

  Int_t TmpIsocat=Isocat_;
  Isocat_=-1;
  Int_t ChcatTmp=Chcat_;
  Chcat_=2;
  
  //QCD Shape
  char isocuttxtshape[100];
  sprintf(isocuttxtshape,"(0.2<muiso&&tauiso3hitraw<10.0)");
  extrasel_=TmpExtrasel+"*"+isocuttxtshape;
  TH1F* hShape=getTotalData();
  hShape->SetName("hShape");

  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10.0)");
  //SS Loose Incl QCD 
  extrasel_=isocuttxt;
  TH1F* hDataIncLoose = getTotalData();
  hDataIncLoose->SetName("hDataIncLoose");
  
  //SS Loose VBF QCD 
  extrasel_=TmpExtrasel+"*"+isocuttxt;
  TH1F* hDataVBFLoose=getTotalData();
  hDataVBFLoose->SetName("hDataVBFLoose");

  //return selections to normal
  Isocat_=TmpIsocat;
  extrasel_=TmpExtrasel;
  Chcat_=ChcatTmp;

  //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
  if(hDataIncLoose->Integral()>0.){
    cout<<" inclusive QCD  : "<<hQCDInc->Integral()<<endl;
    cout<<" Data Category Loose "<<hDataVBFLoose->Integral()<<endl;
    cout<<" Data Incl. Loose "<<hDataIncLoose->Integral()<<endl;
    float vbfeff=hDataVBFLoose->Integral()/hDataIncLoose->Integral();
    cout<<" VBF efficiency "<<vbfeff<<endl;
    hShape->Scale(hQCDInc->Integral()*vbfeff/hShape->Integral());
  }else hShape->Scale(0.);

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}




///HCP 2012 VBF methods
TH1F* TauElePlotter::getSampleVBFHCPShape(TString sample){

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  //TString shapesel="(njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  //TString shapesel="(njet20>=2)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getSample(sample);
  hShape->SetName(sample+"VBFHCPShape");
  extrasel_=tmpextrasel;

  return hShape;
}

TH1F* TauElePlotter::getSampleVBFTightHCPShape(TString sample){

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  //TString shapesel="(njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  //TString shapesel="(njet20>=2)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getSample(sample);
  hShape->SetName(sample+"VBFHCPShape");
  extrasel_=tmpextrasel;

  return hShape;
}


TH1F* TauElePlotter::getSampleVBFHCP(TString sample){
  TH1F*h=getSample(sample);
  h->SetName(sample+"VBFHCP");
  TH1F*hShape=getSampleVBFHCPShape(sample);
  if(hShape->Integral()>0){
    hShape->Scale(h->Integral()/hShape->Integral());
    delete h;
    return hShape;
  }
  delete hShape;
  return h;
}

TH1F* TauElePlotter::getSampleVBFTightHCP(TString sample){
  TH1F*h=getSample(sample);
  h->SetName(sample+"VBFHCP");
  TH1F*hShape=getSampleVBFTightHCPShape(sample);
  if(hShape->Integral()>0){
    hShape->Scale(h->Integral()/hShape->Integral());
    delete h;
    return hShape;
  }
  delete hShape;
  return h;
}


TH1F* TauElePlotter::getDiBosonVBFHCP(){
  //cout<<"Calling getDiBosonVBFHCP"<<endl;
  TH1F* h=getPlotHisto("hVV");

  TH1F* hWW2L2Nu=getSampleVBFHCP("WW2L2Nu"); if(!hWW2L2Nu)return 0;  h->Add(hWW2L2Nu);   delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSampleVBFHCP("WZ3LNu");   if(!hWZ3LNu)return 0;   h->Add(hWZ3LNu);   delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSampleVBFHCP("WZ2L2Q");   if(!hWZ2L2Q)return 0;   h->Add(hWZ2L2Q);  delete hWZ2L2Q;
  TH1F* hZZ4L=getSampleVBFHCP("ZZ4L");       if(!hZZ4L)return 0;     h->Add(hZZ4L);  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSampleVBFHCP("ZZ2L2Nu"); if(!hZZ2L2Nu)return 0;  h->Add(hZZ2L2Nu);  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSampleVBFHCP("ZZ2L2Q");   if(!hZZ2L2Q)return 0;   h->Add(hZZ2L2Q);  delete hZZ2L2Q;

  TH1F* hTopTW=getSampleVBFHCP("TopTW");  if(!hTopTW)return 0;  h->Add(hTopTW);  delete hTopTW;
  TH1F* hTopBTW=getSampleVBFHCP("TopBTW");  if(!hTopBTW)return 0;  h->Add(hTopBTW);  delete hTopBTW;

  h->SetName("getDiBosonVBFHCP");
  return h;

}

TH1F* TauElePlotter::getDiBosonVBFTightHCP(){
  //cout<<"Calling getDiBosonVBFHCP"<<endl;
  TH1F* h=getPlotHisto("hVV");

  TH1F* hWW2L2Nu=getSampleVBFTightHCP("WW2L2Nu"); if(!hWW2L2Nu)return 0;  h->Add(hWW2L2Nu);   delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSampleVBFTightHCP("WZ3LNu");   if(!hWZ3LNu)return 0;   h->Add(hWZ3LNu);   delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSampleVBFTightHCP("WZ2L2Q");   if(!hWZ2L2Q)return 0;   h->Add(hWZ2L2Q);  delete hWZ2L2Q;
  TH1F* hZZ4L=getSampleVBFTightHCP("ZZ4L");       if(!hZZ4L)return 0;     h->Add(hZZ4L);  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSampleVBFTightHCP("ZZ2L2Nu"); if(!hZZ2L2Nu)return 0;  h->Add(hZZ2L2Nu);  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSampleVBFTightHCP("ZZ2L2Q");   if(!hZZ2L2Q)return 0;   h->Add(hZZ2L2Q);  delete hZZ2L2Q;

  TH1F* hTopTW=getSampleVBFTightHCP("TopTW");  if(!hTopTW)return 0;  h->Add(hTopTW);  delete hTopTW;
  TH1F* hTopBTW=getSampleVBFTightHCP("TopBTW");  if(!hTopBTW)return 0;  h->Add(hTopBTW);  delete hTopBTW;

  h->SetName("getDiBosonVBFTightHCP");
  return h;

}

TH1F* TauElePlotter::getTTJetsVBFHCP(){
  // cout<<"Calling getTTJetsVBFHCP"<<endl;
  TH1F*h=getSampleVBFHCP("TTJets");
  h->Scale(TTJetsCorrFactor[2]);
  h->SetName("getTTJetsVBFHCP");
  return h;
}
TH1F* TauElePlotter::getZToLJetVBFHCP(){
  //cout<<"Calling getZToLJetVBFHCP"<<endl;
  //TH1F*h=getSampleVBFHCP("ZToLJet");
  

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*h=getZNJetSum("LJet");
  extrasel_=tmpextrasel;

  h->SetName("getZToLJetVBFHCP");
  return h;
}


TH1F* TauElePlotter::getZToLJetVBFTightHCP(){
  //cout<<"Calling getZToLJetVBFHCP"<<endl;
  //TH1F*h=getSampleVBFHCP("ZToLJet");
  

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*h=getZNJetSum("LJet");
  extrasel_=tmpextrasel;

  h->SetName("getZToLJetVBFHCP");
  return h;
}


TH1F* TauElePlotter::getZLLVBFHCP(){
  //cout<<" Calling getZLLVBFHCP"<<endl;

  //////////MC Norm
//   TH1F* hZL=getSample("ZToEE");
//   TH1F* hZJ=getSample("ZToLJet");


  //////Embedded norm
  //Norm from Embedded
  TString TmpExtrasel=extrasel_;  
  extrasel_="1"; 

  ////Get ZL Yield at Inclusive
  TH1F*hZLNorm=getZL2012();//getZL2012Type2();
  hZLNorm->SetName("getZLLVBFHCPZLNorm");
  TH1F* hZJNorm=getSample("ZToLJet");
  hZJNorm->SetName("getZLLVBFHCPZJNorm");
  //get Embedded yield at inclusive
  TH1F*hEmbIncl=getTotalEmbedded(); 
  hEmbIncl->SetName("getZLVBFHCPhEmbIncl");

  extrasel_=TmpExtrasel;
  //get Embedded yield at VBF
  TH1F*hEmbVBF=getTotalEmbedded(); 
  hEmbVBF->SetName("getZLVBFHCPhEmbVBF");


  ////////////Shape
  TH1F* hZLLShape=getSampleVBFHCPShape("ZToEE");
  TH1F* hZJShape=getSampleVBFHCPShape("ZToLJet");
  hZLLShape->Add(hZJShape);
  delete hZJShape;

  //
  float vbfeff=hEmbVBF->Integral()/hEmbIncl->Integral();
  cout<<"ZLL incl. yield = "<<hZLNorm->Integral()<<" ZL + "<<hZJNorm->Integral()<<" ZJ "<<endl;
  cout<<"ZLL VBF eff: "<<hEmbVBF->Integral()<<" / "<<hEmbIncl->Integral()<<"  =  "<<vbfeff<<endl;
  hZLLShape->Scale((hZLNorm->Integral()+hZJNorm->Integral())*vbfeff/hZLLShape->Integral());
  hZLLShape->SetName("getZLLVBFHCP");
  delete hZLNorm;
  delete hZJNorm;
  delete hEmbIncl;
  delete hEmbVBF;

  return hZLLShape;
}


TH1F* TauElePlotter::getZLBoost(){
  // cout<<"Calling getZLBoost"<<endl;

  TH1F*hNorm=getZLInc();
  //TH1F*hNorm=getZL2012Type2();
  //TH1F*hNorm=getZL2012();
  hNorm->SetName("getZLBoost");

  Int_t TmpIsocat=Isocat_;
  Int_t MTcatTmp=MTcat_;
  Isocat_=0;
  MTcat_=0;
  TString shapesel="(njet20>=1&&taupt>40&&metpt>20&&transversemass<30&&muiso<0.2&&tauiso3hitraw<2.0)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getZLInc();
  //TH1F*hShape=getZL2012Type2();
  //TH1F*hShape=getZL2012();
  hShape->SetName("getZLBoostShape");
 
  Isocat_=TmpIsocat;
  MTcat_=MTcatTmp;
  extrasel_=tmpextrasel;
  
  if(hShape->Integral()>0){
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  }else hShape->Scale(0.);

  delete hNorm;
  return hShape;
}

TH1F* TauElePlotter::getZToTauTauVBF(){  
  //cout<<"Calling getZToTauTauVBF"<<endl;

  TH1F*hnorm=getZToTauTau();
  hnorm->SetName("ZTTVBFHCPNorm");

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200.&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hshape=getZToTauTau();
  hshape->SetName("ZTTVBFHCPShape");
  extrasel_=tmpextrasel;

  //cout<<" hshape : "<<hshape->Integral()<<endl;
  if(hshape->Integral()>0.){
    hshape->Scale(hnorm->Integral()/hshape->Integral());
    delete hnorm;
    return hshape;
  }
  delete hshape;
  return hnorm;
}



//////////////////////////////Plot

bool TauElePlotter::plotInc(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t Chcat, Int_t Isocat, Int_t MTcat,TString extrasel, TString blindsel, Int_t QCDType, Int_t WJetsType, TString xlabel, TString ylabel,Float_t* legendcoords, int higgs,TString filetag){

  int rebin=0;

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Chcat_=Chcat;
  Isocat_=Isocat;
  MTcat_=MTcat;
  extrasel_="1";//reset for multiple plotting
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;


  if(!scaleSamplesLumi())return 0;
   
  
  TH1F*hQCD = 0;
  if(Chcat_==1){
    if(QCDType==0) hQCD=getQCDIncWNJet();
    if(QCDType==1) hQCD=getQCDMuIso();
    if(QCDType==2) hQCD=getQCDMuIsoLooseTau();
    if(QCDType==3) hQCD=getQCDIncWNJet();
    if(QCDType==4) hQCD=getQCDKeti();
    if(QCDType==5) hQCD=getQCDVBFLoose();
    if(QCDType==6) hQCD=getQCDIncFit();
    if(QCDType==7) hQCD=getQCDIncLowPt();
    if(QCDType==8) hQCD=getQCDBoostTight();
    if(QCDType==9) hQCD=getQCDVBFTight();
  }
  if(hQCD){
    hQCD->SetLineWidth(1);   hQCD->SetLineColor(1);    hQCD->SetFillColor(QCDColor_);
  } else     cout<<"no QCD determined "<<endl;
  if(hQCD)hQCD->SetName("hQCDplotInc");
  if(hQCD) cout<<"QCD "<<hQCD->Integral()<<endl;
  if(nbins_==0) if(hQCD) makeDensityHisto(hQCD);


  TH1F* hWJetsToLNu=0;
  if(WJetsType==0) hWJetsToLNu = getWJetsInc(); 
  if(WJetsType==1) hWJetsToLNu = getSample("WJetsToLNu"); 
  if(WJetsType==2) hWJetsToLNu = getWJetsNJet(); 
  if(WJetsType==3) hWJetsToLNu = getWJetsNJetVBFLoose();
  if(WJetsType==4) hWJetsToLNu = getWJetsNJet(2);
  if(WJetsType==5) hWJetsToLNu = getWJetsNJet(1);
  if(WJetsType==6) hWJetsToLNu = getWJetsNJet(3);
  if(WJetsType==7) hWJetsToLNu = getWJetsNJetVBFTight();
  if(!hWJetsToLNu){
    cout<<"WJets Background is NULL"<<endl; 
    return 0;
  }
  hWJetsToLNu->SetName("hWJetsToLNuplotInc");
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(1);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  cout<<"W "<<hWJetsToLNu->Integral()<<endl;
  if(nbins_==0) makeDensityHisto(hWJetsToLNu);


  TH1F*hTTJets=0;
  if(QCDType==5 || QCDType==9)  hTTJets=getTTJetsVBFHCP();
  else hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  hTTJets->SetName("hTTJetsplotInc");
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(1);
  hTTJets->SetFillColor(TTJetsColor_); 
  cout<<"TT "<<hTTJets->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hTTJets);  
 

  ///Z->tau tau
  TH1F*hZToTauTau=0;
  if(QCDType==9)hZToTauTau= getZToTauTauVBF();//for VBF loose use full selection
  else hZToTauTau= getZToTauTau();
  if(!hZToTauTau)return 0;
  hZToTauTau->SetName("hZToTauTauplotInc");
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(1);
  hZToTauTau->SetFillColor(ZTauTauColor_);
  cout<<"ZTT "<<hZToTauTau->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZToTauTau);  


  
  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");

  //combine Di-Bosons
  TH1F*hVV=0;
  if(QCDType==5 || QCDType==9)  hVV=getDiBosonVBFHCP();
  else hVV=getDiBoson();
  if(!hVV)return 0;
  hVV->SetName("hVVplotInc");
  cout<<"VV "<<hVV->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hVV); 
  hEWK->Add(hVV);



  //keep ZEE separate
  TH1F*hZL=0;
  if(QCDType==5 || QCDType==9)  hZL=getZLVBFHCP();
  //else if(QCDType==8)  hZL=getZLBoost();
  //else hZL=getZLInc();
  else hZL=getZL2012();//this is type0
  //else hZL=getZL2012Type1();
  //else hZL=getZL2012Type1LowHigh();
  //else hZL=getZL2012Type2();
  if(!hZL)return 0;
  hZL->SetName("ZL");
  hZL->SetLineWidth(1);
  hZL->SetLineColor(1);
  hZL->SetFillColor(ZMuMuColor_);
  cout<<"ZL "<<hZL->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZL); 
  TH1F* hZLL=(TH1F*)hZL->Clone("ZLL");

  //combine ZLJet
  TH1F*hZJ=0;
  if(QCDType==5|| QCDType==9)  hZJ=getZToLJetVBFHCP();
  else hZJ=getZToLJetInc();
  if(!hZJ)return 0;
  hZJ->SetName("ZJ");
  cout<<"ZJ "<<hZJ->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZJ); 
  hZLL->Add(hZJ);


  if(QCDType==5){
    hZLL->Scale(0.);
    TH1F* htmp=getZLLVBFHCP();
    hZLL->Add(htmp);
    delete htmp;
    cout<<"ZLL "<<hZLL->Integral()<<endl;
    if(nbins_==0)makeDensityHisto(hZLL); 
  }

  ////////////////////////
  cout<<"Creating Bkg Stack:"<<endl;
  THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
  if(hQCD)hMCStack.Add(hQCD,"hist");
  hMCStack.Add(hEWK,"hist");
  hMCStack.Add(hTTJets,"hist");
  hMCStack.Add(hZLL,"hist");
  hMCStack.Add(hZToTauTau,"hist");


  //create total Background
  TH1F* hBkg=getPlotHisto("hBkg");
  TList* histList= hMCStack.GetHists();
  for(Int_t i=0;i<histList->GetSize();i++){
    hBkg->Add((TH1F*)(histList->At(i)));
  }
  cout<<"Total Bkg "<<hBkg->Integral()<<endl;

  
  //the higgs
  TH1F*hHiggs=0;
  if(higgs>0){
    if(MSSMFlag_){
      hHiggs=getSample("SUSYBB200");
      hHiggs->SetName("hHiggs");
      TH1F*hHiggsGG=getSample("SUSYGG200");
      hHiggs->Add(hHiggsGG);
      hHiggs->Scale(higgs);
      if(nbins_==0)makeDensityHisto(hHiggs); 
      hHiggs->Add(hBkg);
      hHiggs->SetLineColor(1);
      hHiggs->SetLineStyle(2);
      delete hHiggsGG;
    }else {
      hHiggs=getSample("HiggsVBF125");
      hHiggs->SetName("hHiggs");
      TH1F*hHiggsGG=getSample("HiggsGG125");
      hHiggs->Add(hHiggsGG);
      TH1F*hHiggsVH=getSample("HiggsVH125");
      hHiggs->Add(hHiggsVH);
      cout<<"Total Higgs yield : VBF="<<hHiggs->Integral()<<" GG="<<hHiggsGG->Integral()<<" VH="<<hHiggsVH->Integral()<<endl;
      hHiggs->Scale(higgs);
      if(nbins_==0)makeDensityHisto(hHiggs); 
      hHiggs->Add(hBkg);
      hHiggs->SetLineColor(1);
      hHiggs->SetLineStyle(2);
      delete hHiggsGG;
      delete hHiggsVH;
    }
  }

  ////Total Data
  if(blindsel.CompareTo("")!=0) blindsel_=blindsel;
  TH1F* hData=getTotalData();
  if(nbins_==0)makeDensityHisto(hData); 
  blindsel_="1";//blind only around this call otherwise may affect background estimations
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}

  cout<<"Total Data "<<(int)(hData->Integral())<<endl; 

  if(rebin >1) if(hQCD) hQCD->Rebin(rebin);  
  if(rebin >1)  hEWK->Rebin(rebin);
  if(rebin >1)  hTTJets->Rebin(rebin);
  if(rebin >1)  hZLL->Rebin(rebin);
  if(rebin >1)  hZToTauTau->Rebin(rebin);
  if(rebin >1)  hData->Rebin(rebin);
  if(rebin >1)  hBkg->Rebin(rebin); 
  if(rebin >1) if(hHiggs) hHiggs->Rebin(rebin);  

  //create legend key
  TLegend legend;
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetLineColor(0);
  legend.SetLineWidth(0);
  legend.SetBorderSize(1);
  legend.SetTextAlign(12);
  legend.AddEntry(hData,"Observed","PL");  
  if(hHiggs){
    if(MSSMFlag_)legend.AddEntry(hHiggs,TString("Signal"),"L");
    else legend.AddEntry(hHiggs,TString("")+(long)higgs+" x SM H(125)","L");
  }
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","F");
  legend.AddEntry(hZLL,"Z #rightarrow e^{+}e^{-}","f");
  legend.AddEntry(hTTJets,"t#bar{t}","F");
  legend.AddEntry(hEWK,"Electroweak","F");
  if(hQCD)  legend.AddEntry(hQCD,"QCD","F");



//   TString tag;
//   if(filetag.CompareTo("")!=0) tag=plotvar_+"_"+filetag;
//   else{
//     tag=plotvar_;
//     if(extrasel_.CompareTo("1")!=0) tag = tag + extrasel_;
//     if(Isocat_>0) tag=tag+"_Iso"+(long)Isocat_;
//     if(MTcat_>0) tag=tag+"_MT"+(long)MTcat_;
//   }
//   fixFileTag(&tag);  
//   TString filename=outputpath_+"/TauElePlotter_"+tag;

  TString filename=outputpath_+"/"+filetag;
  TCanvas C("C",filename);  
  C.Print(filename+".ps[");


  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hMCStack.Draw("hist");  
  hData->SetTitle("");
  if(hMCStack.GetHistogram()->GetMaximum()<hData->GetMaximum())
    hData->GetYaxis()->SetRangeUser(0,hData->GetMaximum()*1.2);
  if(hMCStack.GetHistogram()->GetMaximum()>hData->GetMaximum())
    hData->GetYaxis()->SetRangeUser(0,hMCStack.GetHistogram()->GetMaximum()*1.2);
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  hData->Draw("hist pe");
  hMCStack.Draw("hist same");  
  //////////////////////////
  hData->Draw("hist pe same");//bring Data points back to front
  hBkg->SetFillColor(0);
  hBkg->SetLineColor(1);
  hBkg->Draw("histsame");//draw outline of MC        
  hMCStack.GetHistogram()->Draw("same");//bring axes back to front 
  if(hHiggs)hHiggs->Draw("histsame");
  if(legendcoords){
    legend.SetX1NDC(legendcoords[0]);
    legend.SetX2NDC(legendcoords[1]);
    legend.SetY1NDC(legendcoords[2]);
    legend.SetY2NDC(legendcoords[3]);
  }
  legend.SetTextSize(.04);
  legend.Draw();


  TPaveText title(.155,.91,.9,.999, "NDC");
  title.SetBorderSize(   0 );
  title.SetFillStyle(    0 );
  title.SetTextAlign(   12 );
  title.SetTextSize ( 0.034 ); //do not increase size power on fb will get separated
  title.SetTextColor(    1 );
  title.SetTextFont (   62 );
  title.AddText(plotTitle_);
  title.Draw();



  C.RedrawAxis();
  C.Update();
  C.Print(filename+".ps");  
  C.Print(filename+".png");  
  C.Print(filename+".pdf");  
  C.Print(filename+".C");  
  hData->GetYaxis()->SetRangeUser(0.1,hData->GetMaximum()*100);
  C.Update();
  C.SetLogy(1);
  C.Print(filename+".ps"); 
  C.Print(filename+"_log.png");   
  C.Print(filename+"_log.pdf");   
  C.Print(filename+"_log.C");   
  C.SetLogy(0);

  
  TLine line;


  //make the difference plot  
  TH1F* hDiff=getPlotHisto("hDiff");

  //hDiff->SetStats(0);
  hDiff->GetXaxis()->SetTitle(hData->GetXaxis()->GetTitle());
  hDiff->GetYaxis()->SetTitle("Data - MC");
  hDiff->Add(hData);
  hDiff->Add(hBkg,-1);
  float max=fabs(hDiff->GetMaximum());
  if(fabs(max)<fabs(hDiff->GetMinimum())) max = fabs(hDiff->GetMinimum());
  hDiff->GetYaxis()->SetRangeUser(-max,max);
  C.Clear();
  hDiff->Draw("hist pe");
  line.DrawLine(xmin_,0,xmax_,0);
  C.Print(filename+".ps");  


  //make the ratio plot
  TH1F* hRatio=getPlotHisto("hRatio");

  hRatio->SetStats(0);
  hRatio->GetXaxis()->SetTitle(hData->GetXaxis()->GetTitle());
  hRatio->GetYaxis()->SetTitle("Data/MC");
  hRatio->Add(hData);
  hRatio->Divide(hBkg);
  hRatio->GetYaxis()->SetRangeUser(0,2);
  C.Clear();
  hRatio->Draw("hist pe");
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print(filename+".ps");  

  C.Print(filename+".ps]");
  cout<<"closed ps file"<<endl;


  TFile DiffFile(filename+".root","recreate");

  if(hQCD)hQCD->Write();
  hTTJets->Write();
  hZToTauTau->Write();
  hZJ->Write();
  hZL->Write();
  hZLL->Write();
  hWJetsToLNu->Write();
  hVV->Write();
  hEWK->Write();
  hData->Write();
  hBkg->Write();
  if(hHiggs)hHiggs->Write();
  hDiff->Write();
  hRatio->Write();

  DiffFile.ls();
  DiffFile.Close();

  if(hQCD) delete hQCD;
  delete hTTJets;
  delete hZToTauTau;
  delete hZJ;  
  delete hZL;
  delete hZLL;
  delete hWJetsToLNu;
  delete hVV;
  delete hEWK;
  delete hData;
  if(hHiggs) delete hHiggs;
  delete hBkg;
  delete hDiff;
  delete hRatio;


  return 1;

}



// ///////////////////////////////////Fake Rate Estimations//////////////////////////////////////////
// void TauElePlotter::plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString IsoSel, Float_t ymax,Bool_t log){
  
//   plotvar_=variable;
//   nbins_=nbins;
//   xmin_=xmin;
//   xmax_=xmax;
//   Isocat_=-1;
//   if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;//something like (njet==0)

//   TString filetag=plotvar_+extrasel_;
//   fixFileTag(&filetag);

 
//   TCanvas C("C");
//   TString filename = outputpath_ + "/TauElePlotterIsoFakeRate_"+filetag;
//   C.Print(filename+".ps[");


//   /////////Anti-Isolated taus
//   TString tmpextrasel=extrasel_;
//   extrasel_=extrasel_+"*(!"+IsoSel+")";
//   if(!scaleSamplesLumi())return;
//   TH1F* hData=getTotalDataSS();
//   if(!hData){cout<<" Total Data not determined "<<endl; return;}
//   TH1F* hMC=getTotalMCSMSS();
//   if(!hMC){cout<<" Total MC not determined "<<endl; return;}  
//   extrasel_=tmpextrasel;

//   C.Clear();
//   hData->Draw("pe");
//   hMC->Draw("hist same");
//   C.Print(filename+".ps");

//   TH1F* hA=getPlotHisto("hDiffAnti");//new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
//   hA->Add(hData);
//   hA->Add(hMC,-1);
//   delete hData; delete hMC;


//   /////////////Isolated taus
//   extrasel_=extrasel_+"*"+IsoSel;
//   if(!scaleSamplesLumi())return;
//   hData=getTotalDataSS();
//   if(!hData){cout<<" Total Data not determined "<<endl; return;}
//   hMC=getTotalMCSMSS();
//   if(!hMC){cout<<" Total MC not determined "<<endl; return;}

//   C.Clear();
//   hData->Draw("pe");
//   hMC->Draw("hist same");
//   C.Print(filename+".ps");

//   TH1F* hI=getPlotHisto("hDiffIso");//new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
//   hI->Add(hData);
//   hI->Add(hMC,-1);
//   delete hData; delete hMC;
 
//   C.Clear();
//   C.SetLogy(1);
//   hA->Draw("pe");
//   hI->Draw("pesame");
//   C.Print(filename+".ps");
//   C.SetLogy(0);

//   //average ratio
//   float r=hI->Integral()/hA->Integral();
//   float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
//   cout<<" Ratio = "<<r<<" +- "<<re<<endl;


//   ////Calculate the ratio as a function of the variable
//   TH1F* hR=getPlotHisto("hRatio");//new TH1F("hRatio","",nbins_,xmin_,xmax_);
//   hR->Add(hI);
//   hR->Divide(hA);
//   delete hA; delete hI;

//   C.Clear();
//   C.SetLogy(log);
//   delete hR->FindObject("stats");
//   hR->GetYaxis()->SetRangeUser(log ? 0.001 : 0,ymax);
//   hR->GetXaxis()->SetTitle(plotvar_);
//   hR->GetYaxis()->SetTitle("ratio");
//   hR->SetTitle("");
//   hR->Draw("pe");
//   C.Print(filename+".ps");


//   cout.precision(3);
//   cout<<"(";
//   Int_t b=1;
//   while(hR->GetBinContent(b)>0&&b<=hR->GetNbinsX()){
//     cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*";
//     cout<<hR->GetBinContent(b)<<"+";
//     b++;
//   }
//   cout<<")"<<endl;  


//   ///save the ratio histo to a root file:
//   TFile FRatio(filename+".root","recreate");
//   hR->Write();
//   FRatio.ls();
//   FRatio.Close();
  

//   delete hR;

//   C.Print(filename+".ps]");

// }


void TauElePlotter::plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, Float_t ymax,Bool_t log){
  
  //Ratio of Isolated/Anti-Isolated taus using the the high mT region
  //-Should be computed at inclusive level
  //-Should be computed with OS samples only (SS is different)

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
 
  MTcat_=-1;//dont use this because need to make sure it refers to the proper MET 
  if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;

  TString filetag=plotvar_+extrasel_;
  fixFileTag(&filetag);

 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauElePlotterTauFakeRateWJets_"+filetag;
  C.Print(filename+".ps[");


  /////////Anti-Isolated taus
  Isocat_=2;
  if(!scaleSamplesLumi())return;

  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}

  TH1F* hMC=getPlotHisto("hMCAnti");//new TH1F("hMCAnti","",nbins_,xmin_,xmax_);
  TH1F* hZ=getZToTauTau(); hMC->Add(hZ); delete hZ;
  TH1F* hTT=getSample("TTJets"); hMC->Add(hTT); delete hTT;
 
  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hA=getPlotHisto("hDiffAnti");//new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
  hA->Add(hData); delete hData; 
  hA->Add(hMC,-1); delete hMC;

  C.Clear();
  hA->Draw("pe");
  C.Print(filename+".ps");


  /////////////Isolated taus
  Isocat_=1;
  if(!scaleSamplesLumi())return;
 
  hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}

  hMC=getPlotHisto("hMCIso");//new TH1F("hMCIso","",nbins_,xmin_,xmax_);
  hZ=getZToTauTau(); hMC->Add(hZ); delete hZ;
  hTT=getSample("TTJets"); hMC->Add(hTT); delete hTT;


  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hI=getPlotHisto("hDiffIso");//new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
  hI->Add(hData);delete hData;
  hI->Add(hMC,-1);delete hMC;
  
  C.Clear();
  hI->Draw("pe");
  C.Print(filename+".ps");

  //average ratio
  float r=hI->Integral()/hA->Integral();
  float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
  cout<<" Ratio = "<<r<<" +- "<<re<<endl;


  ////Calculate the ratio as a function of the variable
  TH1F* hR=getPlotHisto("hRatio");//new TH1F("hRatio","",nbins_,xmin_,xmax_);
  hR->Add(hI); 
  hR->Divide(hA); 

  C.Clear();
  C.SetLogy(log);
  delete hR->FindObject("stats");
  hR->GetYaxis()->SetRangeUser(log ? 0.001 : 0,ymax);
  hR->GetXaxis()->SetTitle(plotvar_);
  hR->GetYaxis()->SetTitle("ratio");
  hR->SetTitle("");
  hR->Draw("pe");
  C.Print(filename+".ps");


  cout.precision(3);
  cout<<"(";
  Int_t b=1;
  while(hR->GetBinContent(b)>0&&b<=hR->GetNbinsX()){
    cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*"<<hR->GetBinContent(b)<<"+";
    b++;
  }
  cout<<")"<<endl;  



  ///save the ratio histo to a root file:
  TFile FRatio(filename+".root","recreate");
  hI->Write();
  hA->Write();
  hR->Write();
  FRatio.ls();
  FRatio.Close();
  
  delete hI;
  delete hA; 
  delete hR;

  C.Print(filename+".ps]");

}



void TauElePlotter::plotQCDSSOSRatio(){

  plotvar_="muiso";
  nbins_=7;
  xmin_=0;
  xmax_=0.7;
  Isocat_=-1;
  MTcat_=1;
  Chcat_=2;
  extrasel_="(muiso>0.2&&tauiso3hitraw<5.0)";
  scaleSamplesLumi();

  TCanvas C("plotQCDSSOSRatio");

  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps[");
  

  TH1F* hDataSS=getTotalData();
  hDataSS->SetTitle("");
  hDataSS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataSS->GetXaxis()->SetTitle("electron rel. isolation ");


  TH1F* HMCSS=getZToTauTau(); 
  HMCSS->SetTitle("HMCSS");
  TH1F* HTTSS=getTTJetsInc(); 
  HMCSS->Add(HTTSS); delete HTTSS;
  TH1F* HWSS=getWJetsInc(); 
  HMCSS->Add(HWSS); delete HWSS;
  
  C.Clear();
  hDataSS->Draw("histpe");
  HMCSS->Draw("histsame");
  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps");
  
  hDataSS->Add(HMCSS,-1);
  delete HMCSS;

  Chcat_=1;
  ////////////////////////////
  TH1F* hDataOS=getTotalData();
  hDataOS->SetTitle("");
  hDataOS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataOS->GetXaxis()->SetTitle("electron rel. isolation ");
  
  TH1F* HMC=getZToTauTau(); 
  HMC->SetTitle("HMC");
  TH1F* HTT=getTTJetsInc(); 
  HMC->Add(HTT); delete HTT;
  TH1F* HW=getWJetsInc(); 
  HMC->Add(HW); delete HW;

  C.Clear();
  hDataOS->Draw("histpe");
  HMC->Draw("histsame");
  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps");

  hDataOS->Add(HMC,-1);
  delete HMC;

  


  TLegend legend;
  legend.SetFillStyle (0);
  legend.SetFillColor (0);
  legend.SetBorderSize(0);
  legend.AddEntry(hDataOS,"OS QCD","L");
  legend.AddEntry(hDataSS,"SS QCD","P");  
  legend.SetX1NDC(.5);
  legend.SetX2NDC(.9);
  legend.SetY1NDC(.2);
  legend.SetY2NDC(.4);


  C.Clear();
  hDataOS->SetTitle("");
  hDataOS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataOS->GetXaxis()->SetTitle("electron rel. isolation ");
  hDataOS->Draw("hist");
  hDataSS->Draw("histpesame");
  legend.Draw();
  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps");

  //plot ratio
  hDataOS->Divide(hDataSS);
  C.Clear();
  hDataOS->GetYaxis()->SetRangeUser(0,1.2);
  hDataOS->GetYaxis()->SetTitle("OS / SS ");
  hDataOS->Draw("histpe");
  TLine line;
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps");

  //Fit to ratio
  //TF1 F1("F1","[0]+[1]*x",xmin_,xmax_);
  TF1 F1("F1","[0]",xmin_,xmax_);
  F1.SetLineColor(2);
  hDataOS->Fit(&F1);
  C.Clear();
  hDataOS->Draw("histpe");
  F1.Draw("same");
  line.DrawLine(xmin_,1,xmax_,1);
  char fitres[100];
//  sprintf(fitres,"(%.2f +- %.2f) - (%.2f +- %.2f)*x",F1.GetParameter(0),F1.GetParError(0),fabs(F1.GetParameter(1)),F1.GetParError(1));
  sprintf(fitres,"(%.3f +- %.3f)",F1.GetParameter(0),F1.GetParError(0));
  TLatex text; text.SetTextSize(0.05); text.SetTextColor(2);
  text.DrawTextNDC(.4,.5,fitres);
  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps");

  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps]");

  delete hDataSS;
  delete hDataOS;
}



bool TauElePlotter::printRawYields(TString selection){
  
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





TH1F* TauElePlotter::smearHisto(TH1F* h){
  if(!h){
    cout<<" smearHisto : histo is NULL"<<endl;
    return 0;
  }

 
  TH1F* hs=0;
 
  if(smearHistoRes_<=0.||h->Integral()<=0.){//just copy
    hs=(TH1F*)(h->Clone(TString(h->GetName())+"smeared"));
    cout<<"returning the same histo : "<<smearHistoRes_<<" "<<h->Integral()<<endl;
  }else {
    hs=(TH1F*)(h->Clone(TString(h->GetName())+"smeared"));//copy original binning
    hs->Scale(0.);//clean 

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
      hs->SetBinError(bs,h->GetBinError(bs));//copy errors from input histogram
    }
    
    
    hs->Scale(h->Integral()/hs->Integral());//make sure the output histo has the same integral
  }
  
  return hs;
}


void TauElePlotter::fixFileTag(TString * filetag){

  for(Int_t l=0;l<filetag->Sizeof();l++){//fix the variable name to work with the file name
    if(TString((*filetag)[l])=="(") filetag->Replace(l,1,"_");
    if(TString((*filetag)[l])==")") filetag->Replace(l,1,"_");
    if(TString((*filetag)[l])=="*") filetag->Replace(l,1,"X");
    if(TString((*filetag)[l])=="=") filetag->Replace(l,1,"Q");
    if(TString((*filetag)[l])=="<") filetag->Replace(l,1,"L");
    if(TString((*filetag)[l])==">") filetag->Replace(l,1,"M");
    if(TString((*filetag)[l])=="&") filetag->Replace(l,1,"A");
    if(TString((*filetag)[l])=="+") filetag->Replace(l,1,"P");
    if(TString((*filetag)[l])=="-") filetag->Replace(l,1,"N");
    if(TString((*filetag)[l])=="!") filetag->Replace(l,1,"n");
    if(TString((*filetag)[l])=="%") filetag->Replace(l,1,"p");
    if(TString((*filetag)[l])=="^") filetag->Replace(l,1,"t");
  }

}


void TauElePlotter::compareZTTEmbedded(){

  plotvar_="ditaumass";
  //plotvar_="svfitmass";
  nbins_=30;
  xmin_=0;
  xmax_=150;
  Chcat_=0;
  Isocat_=1;
  MTcat_=1;
  extrasel_="1";//reset for multiple plotting

  scaleSamplesLumi();

  TCanvas C("compareZTTEmbedded");
  C.Print("compareZTTEmbedded.pdf[");
  char text[100];
  TLatex title;
  
  ZTTType_=1;
  TH1F*HMC=getZToTauTau();
  ZTTType_=2;
  TH1F*HData=getZToTauTau();
  C.Clear();
  HMC->SetMarkerColor(4);
  HMC->SetLineColor(4);
  HMC->SetTitle("");
  HMC->GetXaxis()->SetTitle("m(e#tau)");
  //HMC->GetXaxis()->SetTitle("m(#tau#tau)");
  HMC->GetYaxis()->SetTitle("Inclusive");
  HMC->Draw("histpe");
  HData->Draw("histpesame");
  title.SetTextSize ( 0.025 ); 
  title.SetTextFont (   62 );
  title.SetTextColor(4);
  sprintf(text,"MC (histogram)        : integral = %.0f  mean = %.2f   rms = %.2f",HMC->Integral(),HMC->GetMean(),HMC->GetRMS());
  title.DrawTextNDC(0.2,0.96,text);
  title.SetTextColor(1);
  sprintf(text,"Embedded (points)  : integral = %.0f  mean = %.2f   rms = %.2f",HData->Integral(),HData->GetMean(),HData->GetRMS());
  title.DrawTextNDC(0.2,0.93,text);
  C.Print("compareZTTEmbedded.pdf");
  
  delete HMC;
  delete HData;

  for(Int_t c=0;c<5;c++){
    extrasel_=getSMcut(c);
    
    ZTTType_=1;
    TH1F*HMCcat=getZToTauTau();
    ZTTType_=2;
    TH1F*HDatacat=getZToTauTau();
    C.Clear();
    HMCcat->SetTitle("");
    HMCcat->GetXaxis()->SetTitle("m(e#tau)");
    //HMCcat->GetXaxis()->SetTitle("m(#tau#tau)");
    if(c==0)HMCcat->GetYaxis()->SetTitle("0-Jet low");
    if(c==1)HMCcat->GetYaxis()->SetTitle("0-Jet high");
    if(c==2)HMCcat->GetYaxis()->SetTitle("1-Jet low");
    if(c==3)HMCcat->GetYaxis()->SetTitle("1-Jet high");
    if(c==4)HMCcat->GetYaxis()->SetTitle("VBF");
    HMCcat->SetMarkerColor(4);
    HMCcat->SetLineColor(4);
    HMCcat->Draw("histpe");
    HDatacat->Draw("histpesame");
    title.SetTextColor(4);
    sprintf(text,"MC (histogram)        : integral = %.0f  mean = %.2f   rms = %.2f",HMCcat->Integral(),HMCcat->GetMean(),HMCcat->GetRMS());
    title.DrawTextNDC(0.2,0.96,text);
    title.SetTextColor(1);
    sprintf(text,"Embedded (points)  : integral = %.0f  mean = %.2f   rms = %.2f",HDatacat->Integral(),HDatacat->GetMean(),HDatacat->GetRMS());
    title.DrawTextNDC(0.2,0.93,text);
    C.Print("compareZTTEmbedded.pdf");
      
    delete HMCcat;
    delete HDatacat;
  }


  C.Print("compareZTTEmbedded.pdf]");

}



TH1F* TauElePlotter::computeTrigEff(TH1F* HPass, TH1F* HFail){
  //Ratio = NPass/NTot = NPass/(NPass+NFail) =  1/(1+NFail/NPass)
  //RatioErr= Ratio^2 * d(NFail/NPass)
  TH1F* HRatio =(TH1F*)HFail->Clone(TString(HPass->GetName())+"Ratio");
  HRatio->Divide(HPass);
  TH1F* HEff =(TH1F*)HRatio->Clone(TString(HPass->GetName())+"Eff");
  for(Int_t b=1;b<=HEff->GetNbinsX();b++){
    HEff->SetBinContent(b,0);
    HEff->SetBinError(b,0);
    if(HPass->GetBinContent(b)>0 && HPass->GetBinError(b)/HPass->GetBinContent(b)<0.3){
      HEff->SetBinContent(b,HPass->GetBinContent(b)/(HPass->GetBinContent(b)+HFail->GetBinContent(b)));
      HEff->SetBinError(b,HEff->GetBinContent(b)*HEff->GetBinContent(b)*HRatio->GetBinError(b));
    }
  }
  delete HRatio;
  return HEff;
}

void  TauElePlotter::plotTauTrigger(Int_t Region, TString tag){
  scaleSamplesLumi();

  ///plot the difference in eta
  plotvar_="taupt";
  nbins_=30;
  xmin_=0;
  xmax_=60;
  Chcat_=1;
  Isocat_=1;
  MTcat_=1;
  extrasel_="1";

//   Float_t xbinsValues[15]={17,18,19,20,21,22,24,26,28,30,34,38,42,46,50};
//   setVariableBinning(14,xbinsValues);
  Float_t xbinsValues[11]={18,20,22,24,26,30,34,38,42,50,60};
  setVariableBinning(10,xbinsValues);
  nbins_=0;

  
  TString selectionTrigPass="(trigTest1==1||trigTest2==1||trigTest3==1||trigTest4==1||trigTest5==1||trigTest6==1||trigTest7==1||trigTest8==1||trigTest9==1)";
  TString selectionTrigFail="(!"+selectionTrigPass+")";

  TString selection;
  if(Region==1) selection="(abs(taueta)<1.5)";
  if(Region==2) selection="(abs(taueta)>1.5)";

  TString region;
  if(Region==1) region="Barrel";
  if(Region==2) region="EndCap";


  ///Calculate the Fakes Scale factor
  ///Changes between Barrel and EndCap
  extrasel_ = selection;
  TH1F*HW   = getWJetsNJet(3); 
  TH1F*HQCD = getQCDInc();
  float FakesScaleFactor=(HW->Integral()+HQCD->Integral())/HW->Integral();
  cout<<"W Fakes scale factor: "<<FakesScaleFactor<<endl;


  extrasel_=selection;
  TH1F*HTAUPT=getTotalData(); HTAUPT->SetName("HTAUPT");
  TH1F*HMCTAUPT=getPlotHisto("HMCTAUPT");
  TH1F*HMCZTT=getZToTauTau();         HMCTAUPT->Add(HMCZTT);                delete HMCZTT; 
  TH1F*HMCW=getWJetsNJet(3);
  HMCTAUPT->Add(HMCW,FakesScaleFactor); 
  TH1F*HMCWRaw=getWNJetSumNoChNoMTCut(); //uncorrected yield needed for scaling later 
  TH1F*HMCZL=getZLInc();              HMCTAUPT->Add(HMCZL);                 delete HMCZL; //add ZL 
  
  extrasel_=selection+"*"+selectionTrigPass;
  TH1F*HTAUPTTrigPass=getTotalData();  HTAUPTTrigPass->SetName("HTAUPTTrigPass");
  TH1F*HMCTAUPTTrigPass=getPlotHisto("HMCTAUPTPass");
  TH1F*HMCZTTTrigPass=getZToTauTau();          HMCTAUPTTrigPass->Add(HMCZTTTrigPass);                delete HMCZTTTrigPass; 
  TH1F*HMCWTrigPass=getWNJetSumNoChNoMTCut();  
  HMCTAUPTTrigPass->Add(HMCWTrigPass,FakesScaleFactor*HMCW->Integral()/HMCWRaw->Integral());         delete HMCWTrigPass; 
  TH1F*HMCZLTrigPass=getZLInc();               HMCTAUPTTrigPass->Add(HMCZLTrigPass);                 delete HMCZLTrigPass;
  
  extrasel_=selection+"*"+selectionTrigFail;
  TH1F*HTAUPTTrigFail=getTotalData();  HTAUPTTrigFail->SetName("HTAUPTTrigFail");
  TH1F*HMCTAUPTTrigFail=getPlotHisto("HMCTAUPTFail");
  TH1F*HMCZTTTrigFail=getZToTauTau();         HMCTAUPTTrigFail->Add(HMCZTTTrigFail);                delete HMCZTTTrigFail; 
  TH1F*HMCWTrigFail=getWNJetSumNoChNoMTCut(); 
  HMCTAUPTTrigFail->Add(HMCWTrigFail,FakesScaleFactor*HMCW->Integral()/HMCWRaw->Integral());        delete HMCWTrigFail; 
  TH1F*HMCZLTrigFail=getZLInc();              HMCTAUPTTrigFail->Add(HMCZLTrigFail);                 delete HMCZLTrigFail;

 
  TCanvas C;
  TString plotFileName=TString("TauTriggerEfficiency_eTau_")+region+"_"+tag;
  C.Print(plotFileName+".ps[");

  ////Compare the Pass distribution to the total
  C.Clear();
  HTAUPT->SetTitle("Data ");
  HTAUPT->GetXaxis()->SetTitle("Tau p_{T}");
  HTAUPT->GetYaxis()->SetRangeUser(0,HTAUPT->GetMaximum()*1.3);
  HTAUPT->Draw("hist");
  HTAUPTTrigPass->Draw("histpesame");
  C.Print(plotFileName+".ps");

  C.Clear();
  HMCTAUPT->SetTitle("MC ");
  HMCTAUPT->GetXaxis()->SetTitle("Tau p_{T}");
  HMCTAUPT->GetYaxis()->SetRangeUser(0,HMCTAUPT->GetMaximum()*1.3);
  HMCTAUPT->Draw("hist");
  HMCTAUPTTrigPass->Draw("histpesame");
  C.Print(plotFileName+".ps");

  /////////Compute the efficiency 
  TH1F* HTAUPTTrigEff = computeTrigEff(HTAUPTTrigPass,HTAUPTTrigFail); 
  TH1F* HMCTAUPTTrigEff = computeTrigEff(HMCTAUPTTrigPass,HMCTAUPTTrigFail); 


  C.Clear();
  HTAUPTTrigEff->SetTitle("");
  HTAUPTTrigEff->GetXaxis()->SetTitle("Tau p_{T}");
  HTAUPTTrigEff->GetYaxis()->SetTitle("Efficiency");
  HTAUPTTrigEff->GetYaxis()->SetRangeUser(0.,1);
  HTAUPTTrigEff->Draw("histpe");
  HMCTAUPTTrigEff->SetMarkerColor(4);
  HMCTAUPTTrigEff->SetLineColor(4);
  HMCTAUPTTrigEff->Draw("histpesame");
  C.Print(plotFileName+".ps");


  ///save the histograms for fitting later
  TFile FData(plotFileName+"_Data.root","recreate");
  HTAUPTTrigEff->SetName("efficiency");  HTAUPTTrigEff->SetTitle("");
  HTAUPTTrigEff->Write();
  FData.ls(); FData.Close();
  TFile FMC(plotFileName+"_MC.root","recreate");
  HMCTAUPTTrigEff->SetName("efficiency");   HMCTAUPTTrigEff->SetTitle("");
  HMCTAUPTTrigEff->Write();
  FMC.ls(); FMC.Close();


//   /////////////////compare to the curve from Josh
//   //TriggerEfficiency triggerEff_;
//   TGraph HTAUPTTrigEff_Josh;
//   TGraph HMCTAUPTTrigEff_Josh;
// //   for(Int_t p=0;p<100;p++){
// //     float x=17+p;
// //     //HTAUPTTrigEff_Josh.SetPoint(p,x,triggerEff_.effTau2012ABC_TauEle(x,(Region==1)*0.0+(Region==2)*2.0));
// //     //HMCTAUPTTrigEff_Josh.SetPoint(p,x,triggerEff_.eff2012Tau20MC53X_TauEle(x,(Region==1)*0.0+(Region==2)*2.0));
// //   }

//   C.Clear();
//   HTAUPTTrigEff->Draw("histpe");
//   HMCTAUPTTrigEff->Draw("histpesame");
//   HTAUPTTrigEff_Josh.SetLineColor(1);
//   HTAUPTTrigEff_Josh.Draw("lsame");
//   HMCTAUPTTrigEff_Josh.SetLineColor(4);
//   HMCTAUPTTrigEff_Josh.Draw("lsame");
//   C.Print(plotFileName+".ps");




//   ///////////////Compare Real and Fake Taus
//   //Low mT ZTT  
//   MTcat_=1;
//   extrasel_=selection+"*"+selectionTrigPass;
//   TH1F*HTAUPTZTTTrigPass=getZToTauTau();  HTAUPTZTTTrigPass->SetName("HTAUPTZTTTrigPass");
//   extrasel_=selection+"*"+selectionTrigFail;
//   TH1F*HTAUPTZTTTrigFail=getZToTauTau();  HTAUPTZTTTrigFail->SetName("HTAUPTZTTTrigFail");
//   TH1F*HMCTAUPTZTTTrigEff = computeTrigEff(HTAUPTZTTTrigPass,HTAUPTZTTTrigFail); 

//   //Low mT ZEE
//   MTcat_=1;
//   extrasel_=selection+"*"+selectionTrigPass;
//   TH1F*HTAUPTZLTrigPass=getZLInc();  HTAUPTZLTrigPass->SetName("HTAUPTZLTrigPass");
//   extrasel_=selection+"*"+selectionTrigFail;
//   TH1F*HTAUPTZLTrigFail=getZLInc();  HTAUPTZLTrigFail->SetName("HTAUPTZLTrigFail");
//   TH1F*HMCTAUPTZLTrigEff = computeTrigEff(HTAUPTZLTrigPass,HTAUPTZLTrigFail);

//   //high MT W : Do not normalize to Data otherwise efficiency is set to Data efficiency
//   MTcat_=3;
//   extrasel_=selection+"*"+selectionTrigPass;
//   TH1F*HMCTAUPTWTrigPass=getWNJetSumNoChCut(); HMCTAUPTWTrigPass->SetName("HMCTAUPTWTrigPass"); 
//   extrasel_=selection+"*"+selectionTrigFail;
//   TH1F*HMCTAUPTWTrigFail=getWNJetSumNoChCut(); HMCTAUPTWTrigFail->SetName("HMCTAUPTWTrigFail"); 
//   TH1F*HMCTAUPTWTrigEff= computeTrigEff(HMCTAUPTWTrigPass,HMCTAUPTWTrigFail);

//   //high MT Data 
//   MTcat_=3;
//   extrasel_=selection+"*"+selectionTrigPass;
//   TH1F*HTAUPTWTrigPass=getTotalData();  HTAUPTWTrigPass->SetName("HTAUPTWTrigPass");
//   extrasel_=selection+"*"+selectionTrigFail;
//   TH1F*HTAUPTWTrigFail=getTotalData();  HTAUPTWTrigFail->SetName("HTAUPTWTrigFail");
//   TH1F*HTAUPTWTrigEff= computeTrigEff(HTAUPTWTrigPass,HTAUPTWTrigFail);



//   C.Clear();
//   HTAUPTWTrigEff->SetTitle("");
//   HTAUPTWTrigEff->GetXaxis()->SetTitle("Tau p_{T}");
//   HTAUPTWTrigEff->GetYaxis()->SetTitle("Efficiency");
//   HTAUPTWTrigEff->GetYaxis()->SetRangeUser(.0,1);
//   HTAUPTWTrigEff->Draw("histpe");
//   HMCTAUPTWTrigEff->SetMarkerColor(WJetsColor_);
//   HMCTAUPTWTrigEff->SetLineColor(WJetsColor_);
//   HMCTAUPTWTrigEff->Draw("histpesame");
//   TLegend legend;
//   legend.SetFillStyle (0);
//   legend.SetFillColor (0);
//   legend.SetBorderSize(0);
//   legend.SetTextSize(0.04);
//   legend.AddEntry(HMCTAUPTWTrigEff,"W+jets (mT>70)","pe");
//   legend.AddEntry(HTAUPTWTrigEff,"Data (mT>70)","pe");
//   legend.SetX1NDC(.50);
//   legend.SetX2NDC(.85);
//   legend.SetY1NDC(.22);
//   legend.SetY2NDC(.57);
//   legend.Draw();
//   C.Print(plotFileName+".ps");



//   C.Clear();
//   HMCTAUPTZTTTrigEff->SetTitle("");
//   HMCTAUPTZTTTrigEff->GetXaxis()->SetTitle("Tau p_{T}");
//   HMCTAUPTZTTTrigEff->GetYaxis()->SetTitle("Efficiency");
//   HMCTAUPTZTTTrigEff->SetMarkerColor(ZTauTauColor_);
//   HMCTAUPTZTTTrigEff->SetLineColor(ZTauTauColor_);
//   HMCTAUPTZTTTrigEff->GetYaxis()->SetRangeUser(.0,1);
//   HMCTAUPTZTTTrigEff->Draw("histpe");
//   HMCTAUPTZLTrigEff->SetMarkerColor(ZMuMuColor_);
//   HMCTAUPTZLTrigEff->SetLineColor(ZMuMuColor_);
//   HMCTAUPTZLTrigEff->Draw("histpesame");
//   TLegend legend2;
//   legend2.SetFillStyle (0);
//   legend2.SetFillColor (0);
//   legend2.SetBorderSize(0);
//   legend2.SetTextSize(0.04);
//   legend2.AddEntry(HMCTAUPTZTTTrigEff,TString("ZTT MC (mT<")+mTCut_+")","pe");
//   legend2.AddEntry(HMCTAUPTZLTrigEff,TString("Z->ee MC (mT<")+mTCut_+")","pe");
//   legend2.SetX1NDC(.50);
//   legend2.SetX2NDC(.85);
//   legend2.SetY1NDC(.22);
//   legend2.SetY2NDC(.57);
//   legend2.Draw();
//   C.Print(plotFileName+".ps");




  C.Print(plotFileName+".ps]");
}

