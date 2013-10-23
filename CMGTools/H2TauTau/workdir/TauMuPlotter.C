#include "TauMuPlotter.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>
#include <TVectorD.h>
#include <TLatex.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGraphAsymmErrors.h>
//#include "CMGTools/H2TauTau/interface/QCDEstimate.h"

TauMuPlotter::TauMuPlotter():
  TNamed("","")
{
}

TauMuPlotter::TauMuPlotter(const char * name):
  TNamed(name,name),
  smearHistoRes_(0.),
  Isocat_(-1),
  MTcat_(-1),
  extrasel_("1"),
  blindsel_("1"),
  MSSMFlag_(0),
  QCDOStoSSRatio_(1.11),
  ZTTType_(1),
  WJetsType_(0),
  mTCut_(20),
  leadJetpTCut_(30),
  taupTCut_(40),
  taupTCut1_(30),
  taupTCut2_(40),
  higgspTCutLow_(0),
  higgspTCutMed_(0),
  higgspTCutHigh_(0),
  eventWeight_("eventweight"),
  plotvar_("ditaumass"),
  nbins_(100),
  xmin_(0.),
  xmax_(1000.),
  nbinsVariable_(0),
  xbinsVariable_(0),
  plotTitle_("")
{
 
  TTJetsCorrFactor[0]=1.;
  TTJetsCorrFactor[1]=1.;
  TTJetsCorrFactor[2]=1.;

  qcdTauIsoRatio_ = "((20<taujetpt&&taujetpt<=25)*2.85+(25<taujetpt&&taujetpt<=30)*1+(30<taujetpt&&taujetpt<=35)*0.411+(35<taujetpt&&taujetpt<=40)*0.196+(40<taujetpt&&taujetpt<=45)*0.109+(45<taujetpt&&taujetpt<=50)*0.0644+(50<taujetpt&&taujetpt<=55)*0.0458+(55<taujetpt&&taujetpt<=60)*0.0349+(60<taujetpt&&taujetpt<=65)*0.0338)";
  qcdMuIsoRatio_  = "((17<mujetpt&&mujetpt<=22)*1.51+(22<mujetpt&&mujetpt<=27)*0.462+(27<mujetpt&&mujetpt<=32)*0.2+(32<mujetpt&&mujetpt<=37)*0.12+(37<mujetpt&&mujetpt<=42)*0.101+(42<mujetpt&&mujetpt<=47)*0.0964+(47<mujetpt&&mujetpt<=52)*0.14+(52<mujetpt&&mujetpt<=57)*0.116)";

  wjetsTauIsoRatio_ = "((20<taujetpt&&taujetpt<=25)*3.82+(25<taujetpt&&taujetpt<=30)*1.3+(30<taujetpt&&taujetpt<=35)*0.502+(35<taujetpt&&taujetpt<=40)*0.256+(40<taujetpt&&taujetpt<=45)*0.144+(45<taujetpt&&taujetpt<=50)*0.0986+(50<taujetpt&&taujetpt<=55)*0.0736+(55<taujetpt&&taujetpt<=60)*0.0546+(60<taujetpt&&taujetpt<=65)*0.0477+(65<taujetpt&&taujetpt<=70)*0.0365+(70<taujetpt&&taujetpt<=75)*0.0341+(75<taujetpt&&taujetpt<=80)*0.0271+(80<taujetpt&&taujetpt<=85)*0.024+(85<taujetpt&&taujetpt<=90)*0.0212+(90<taujetpt&&taujetpt<=95)*0.0192+(95<taujetpt&&taujetpt<=100)*0.0163)";


}

TauMuPlotter::~TauMuPlotter(){
}

bool TauMuPlotter::scaleSamplesLumi(){

  cout<<"---------> Scaling all MC samples to Lumi "<<endl;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    (*s)->resetScale();
  float totalDataLumi=getTotalDataLumi();
  cout<<"totalDataLumi = "<<totalDataLumi<<endl;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC" 
       || (*s)->getDataType()=="MCCat" 
       || (*s)->getDataType()=="Signal" ){
      //cout<<(*s)->GetName()<<" lumi scale "<<totalDataLumi/(*s)->getLumi()<<endl;
      if((*s)->getLumi()>0.){
	cout<<"Scaling "<<(*s)->GetName()<<"  by "<<totalDataLumi/(*s)->getLumi()<<endl;
	(*s)->scale(totalDataLumi/(*s)->getLumi());
      } else (*s)->scale(0.);
    }



  ///////////////////////////////////
  ////Scale the embedded samples at inclusive level:  Should there be separate scale factor for OS and SS ? --> should be Ok. ZTauTau is small in SS
  /////////////////////////////////
  cout<<"---------> Scaling Embedded samples "<<endl;
  //TString sel=eventWeight_+"*(categoryIso==1&&abs(ditaucharge)==0&&1.<ditaumass&&ditaumass<1000.)";
  TString sel=eventWeight_+"*"+tauIsoCut_+"*(muiso<0.1&&abs(ditaucharge)==0&&1.<ditaumass&&ditaumass<1000.)";
  cout<<" using inclusive selection : "<<endl<<sel<<endl;

  Float_t ZToTauTauMC=0.;  
  TH1F*hZTTMC=findSample("ZToTauTau")->getHistoNtpFile("ditaumass",10,1,1001,sel);
  ZToTauTauMC=hZTTMC->Integral();
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


TString TauMuPlotter::getCurrentSelection(){

  TString sel=eventWeight_;
  if(Chcat_==1)  sel += "*(abs(ditaucharge)==0)";
  if(Chcat_==2)  sel += "*(abs(ditaucharge)==2)";
//   if(Isocat_==1)  sel += "*(categoryIso==1)";
//   if(Isocat_==2)  sel += "*(categoryIso==2)";

//   if(Isocat_==1)  sel += "*(muiso<0.1&&tauisomvaraw>0.795)";
//   if(Isocat_==2)  sel += "*(muiso>0.1||tauisomvaraw<0.795)";

//   if(Isocat_==1)  sel += "*(muiso<0.1&&tauisomva2raw>0.86)";
//   if(Isocat_==2)  sel += "*(muiso>0.1||tauisomva2raw<0.86)";

//   if(Isocat_==1)  sel += "*(muiso<0.1&&tauiso3hitraw<2.74)";
//   if(Isocat_==2)  sel += "*(muiso>0.1||tauiso3hitraw>2.74)";
  
  if(Isocat_==1)  sel += "*(muiso<0.1)*"+tauIsoCut_;
  if(Isocat_==2)  sel += "*(muiso>0.1||!"+tauIsoCut_+")";

  if(MTcat_==1)  sel += TString("*(transversemass<")+mTCut_+")";
  if(MTcat_==3)  sel += "*(70<transversemass&&transversemass<150)";
  if(MTcat_==13) sel += "*(60<transversemass&&transversemass<120)";
  if(extrasel_.CompareTo("1")!=0)sel += TString("*")+extrasel_;

  return sel;
}

TH1F* TauMuPlotter::getSample(TString samplename, bool removeLumiNorm){
  //cout<<"getSample "<<samplename<<endl;

  TString sel=getCurrentSelection();

  //cout<<sel<<endl;

  TH1F* h=getPlotHisto(samplename);//return a histo with the appropriate binning even if sample is not found
 
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if(TString((*s)->GetName())==samplename.Data()){

      /////reweight decay modes
      if( (*s)->getDataType()=="Signal"){
	sel = sel +"*(1-0.12*(taudecaymode==0))";
      }
      

      //cout<<sel<<endl;
      TH1F*hSample=0;
      if(nbins_>0) hSample=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hSample=(*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}

      if(removeLumiNorm){//needed when adding W+njet samples
	//cout<<" Removing Lumi normalization for sample "<<(*s)->GetName()<<" "<<(*s)->getLumi()<<" / "<<getTotalDataLumi()<<endl;
	hSample->Scale((*s)->getLumi()/getTotalDataLumi());
      }

      h->Add(hSample);
      delete hSample;
    }  

  //cout<<samplename<<" "<<sel<<" "<<h->Integral()<<endl;
  return h;
}




TH1F* TauMuPlotter::getTotalData(){
  TString sel=getCurrentSelection();
  if(blindsel_.CompareTo("1")!=0)sel += TString("*")+blindsel_;
  
  //cout<<sel<<endl;
  
  TH1F* h=getPlotHisto("hData");

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data"){
      //TH1F*hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      TH1F*hd=0;//
      if(nbins_>0) hd = (*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hd = (*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}
      h->Add(hd);
      delete hd;
    }

  return h;
}



TH1F* TauMuPlotter::getTotalMC(){
  TString sel=getCurrentSelection();

  TH1F* h=getPlotHisto("hTotalMC");

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC"){// && TString((*s)->GetName()) != "ZToTauTau" && TString((*s)->GetName()) != "WJetsToLNu"){
      TH1F*hd=0;
      if(nbins_>0)hd=(*s)->getHistoNtpFile(plotvar_,nbins_,xmin_,xmax_,sel);
      else if(nbinsVariable_>0) hd=(*s)->getHistoNtpFile(plotvar_,nbinsVariable_,xbinsVariable_,sel);
      else { cout<<"binning not recognized "<<endl; return 0;}
      h->Add(hd);  
      delete hd;
    }
 
  //cout<<"Total MC : "<<h->Integral()<<endl;

  return h;
}


TH1F* TauMuPlotter::getTotalEmbedded(){
  TString sel=getCurrentSelection();

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
 
  //cout<<"TotalEmbedded : "<<h->Integral()<<endl;

  return h;
}



TH1F* TauMuPlotter::getDiBoson(){
  //cout<<"calling getDiBoson"<<endl;

  TH1F* h=getPlotHisto("hVV");

  TH1F* hWW2L2Nu=getSample("WW2L2Nu");  if(!hWW2L2Nu)return 0;  h->Add(hWW2L2Nu);  delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSample("WZ3LNu");  if(!hWZ3LNu)return 0;  h->Add(hWZ3LNu);  delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSample("WZ2L2Q");  if(!hWZ2L2Q)return 0;  h->Add(hWZ2L2Q);  delete hWZ2L2Q;
  TH1F* hZZ4L=getSample("ZZ4L");  if(!hZZ4L)return 0;  h->Add(hZZ4L);  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSample("ZZ2L2Nu");  if(!hZZ2L2Nu)return 0;  h->Add(hZZ2L2Nu);  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSample("ZZ2L2Q");  if(!hZZ2L2Q)return 0;  h->Add(hZZ2L2Q);  delete hZZ2L2Q;

  TH1F* hTopTW=getSample("TopTW"); if(!hTopTW)return 0;  h->Add(hTopTW);  delete hTopTW;
  TH1F* hTopBTW=getSample("TopBTW");  if(!hTopBTW)return 0;  h->Add(hTopBTW);  delete hTopBTW;

  return h;
}

TH1F* TauMuPlotter::getDiBosonVBFHCP(){
  //cout<<"calling getDiBosonVBFHCP"<<endl;
  TH1F* h=getPlotHisto("hVV");

  TH1F* hWW2L2Nu=getSampleVBFHCP("WW2L2Nu"); if(!hWW2L2Nu)return 0;  h->Add(hWW2L2Nu);   delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSampleVBFHCP("WZ3LNu");   if(!hWZ3LNu)return 0;   h->Add(hWZ3LNu);   delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSampleVBFHCP("WZ2L2Q");   if(!hWZ2L2Q)return 0;   h->Add(hWZ2L2Q);  delete hWZ2L2Q;
  TH1F* hZZ4L=getSampleVBFHCP("ZZ4L");       if(!hZZ4L)return 0;     h->Add(hZZ4L);  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSampleVBFHCP("ZZ2L2Nu"); if(!hZZ2L2Nu)return 0;  h->Add(hZZ2L2Nu);  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSampleVBFHCP("ZZ2L2Q");   if(!hZZ2L2Q)return 0;   h->Add(hZZ2L2Q);  delete hZZ2L2Q;

  TH1F* hTopTW=getSampleVBFHCP("TopTW");  if(!hTopTW)return 0;  h->Add(hTopTW);  delete hTopTW;
  TH1F* hTopBTW=getSampleVBFHCP("TopBTW");  if(!hTopBTW)return 0;  h->Add(hTopBTW);  delete hTopBTW;

  cout<<h->Integral()<<endl;
  return h;

}

TH1F* TauMuPlotter::getDiBosonVBFTightHCP(){
  //cout<<"calling getDiBosonVBFHCP"<<endl;
  TH1F* h=getPlotHisto("hVV");

  TH1F* hWW2L2Nu=getSampleVBFTightHCP("WW2L2Nu"); if(!hWW2L2Nu)return 0;  h->Add(hWW2L2Nu);   delete hWW2L2Nu;
  TH1F* hWZ3LNu=getSampleVBFTightHCP("WZ3LNu");   if(!hWZ3LNu)return 0;   h->Add(hWZ3LNu);   delete hWZ3LNu;
  TH1F* hWZ2L2Q=getSampleVBFTightHCP("WZ2L2Q");   if(!hWZ2L2Q)return 0;   h->Add(hWZ2L2Q);  delete hWZ2L2Q;
  TH1F* hZZ4L=getSampleVBFTightHCP("ZZ4L");       if(!hZZ4L)return 0;     h->Add(hZZ4L);  delete hZZ4L;
  TH1F* hZZ2L2Nu=getSampleVBFTightHCP("ZZ2L2Nu"); if(!hZZ2L2Nu)return 0;  h->Add(hZZ2L2Nu);  delete hZZ2L2Nu;
  TH1F* hZZ2L2Q=getSampleVBFTightHCP("ZZ2L2Q");   if(!hZZ2L2Q)return 0;   h->Add(hZZ2L2Q);  delete hZZ2L2Q;

  TH1F* hTopTW=getSampleVBFTightHCP("TopTW");  if(!hTopTW)return 0;  h->Add(hTopTW);  delete hTopTW;
  TH1F* hTopBTW=getSampleVBFTightHCP("TopBTW");  if(!hTopBTW)return 0;  h->Add(hTopBTW);  delete hTopBTW;

  cout<<h->Integral()<<endl;
  return h;

}



TH1F* TauMuPlotter::getZToTauTau(){  
  TH1F*h=0;

  //fix the decay mode weights
  TString decaymodeCorrection="*(1-0.12*(taudecaymode==0))";  ///Summer13 SF for 1prong-no-pi0 from Phil
  TString tmpExtrasel=extrasel_;
  extrasel_ += decaymodeCorrection;
  //  cout<<"WARNING: Correcting for the ZTT tau decay modes weights: "<<decaymodeCorrection<<endl;

  if(ZTTType_==1)h=getSample("ZToTauTau");
  if(ZTTType_==2)h=getTotalEmbedded(); 
  extrasel_=tmpExtrasel;

  return h;
}


TH1F* TauMuPlotter::getZToTauTauVBF(){
  //cout<<"Calling method getZToTauTauVBF"<<endl;

  TH1F*h=getZToTauTau();
  h->SetName("getZToTauTauVBFNorm");

  TString shapesel="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getZToTauTau();
  hShape->SetName("getZToTauTauVBFShape");
  extrasel_=tmpextrasel;

  hShape->Scale(h->Integral()/hShape->Integral());
  return hShape;
}


//WJets
TH1F* TauMuPlotter::getWJetsInc(){

  TString sample="WJetsToLNu";
  TH1F*hShape=getSample(sample);
  hShape->SetName("getWJetsInc");

  //determine normalization
  Int_t tmpCategoryMT=MTcat_;//switch to high mT
  MTcat_=3;
  TH1F* HW=getSample(sample);
  TH1F* HData=getTotalData();
  TH1F* HMC=getZToTauTau();                  
  TH1F* HTT=getTTJetsInc();   HMC->Add(HTT); 
  TH1F* HVV=getDiBoson();     HMC->Add(HVV); 
  TH1F* HZL=getZLInc();       HMC->Add(HZL); 
  TH1F* HZJ=getZToLJetInc();  HMC->Add(HZJ); 
  delete HTT;
  delete HVV;
  delete HZL;
  delete HZJ;

  //return to Category
  MTcat_=tmpCategoryMT;

  float corr=0.;
  if(HW->Integral()>1.&&(HData->Integral()>HMC->Integral())){//avoid nan and negative values
    corr=(HData->Integral()-HMC->Integral())/HW->Integral();
    cout<<" WJets Inc correction factor : "<<corr<<endl;
  }else {
    cout<<"WARNING HW->Integral is 0"<<endl;
  }
  hShape->Scale(corr);
  delete HData;
  delete HMC;
  delete HW;

  //cout<<" getWJetsInc integral : "<<hShape->Integral()<<endl;
  return hShape;
}




TH1F* TauMuPlotter::getWNJetSum(){
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

  if(WIncLumi>0){
    hShape->Add(HW0ShapeInc,totalDataLumi/WIncLumi);

    hShape->Add(HW1ShapeExc,totalDataLumi/(WIncLumi + findSample("W1JetsToLNu")->getLumi()));
    hShape->Add(HW2ShapeExc,totalDataLumi/(WIncLumi + findSample("W2JetsToLNu")->getLumi()));
    hShape->Add(HW3ShapeExc,totalDataLumi/(WIncLumi + findSample("W3JetsToLNu")->getLumi()));
    hShape->Add(HW4ShapeExc,totalDataLumi/(WIncLumi + findSample("W4JetsToLNu")->getLumi()));

    hShape->Add(HW1ShapeInc,totalDataLumi/(WIncLumi + findSample("W1JetsToLNu")->getLumi()));
    hShape->Add(HW2ShapeInc,totalDataLumi/(WIncLumi + findSample("W2JetsToLNu")->getLumi()));
    hShape->Add(HW3ShapeInc,totalDataLumi/(WIncLumi + findSample("W3JetsToLNu")->getLumi()));
    hShape->Add(HW4ShapeInc,totalDataLumi/(WIncLumi + findSample("W4JetsToLNu")->getLumi()));
  }


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



TH1F* TauMuPlotter::getWNJetSumNoChCut(){
  Int_t tmpChcat=Chcat_;
  Chcat_=0;
  TH1F*hShape=getWNJetSum();
  hShape->SetName("getWNJetSumNoChCut");
  Chcat_=tmpChcat;

  return hShape;
}

TH1F* TauMuPlotter::getWNJetSumNoChNoMTCut(){
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

TH1F* TauMuPlotter::getWNJetSumLooseTau(){
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

TH1F* TauMuPlotter::getWJetsNJet(int shape){
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



TH1F* TauMuPlotter::getWJetsNJetVBFLoose(){
  //cout<<"====Calling getWJetsNJetVBFLoose===="<<endl;

  Int_t tmpCategoryMT=MTcat_;

  ///get W Yield at high mT without relaxed selection
  MTcat_=13;
  TH1F* HData=getTotalData(); HData->SetName("HData");  cout<<" Data high mT : "<<HData->Integral()<<endl;
  TH1F* HMC=getZToTauTau();  HMC->SetName("HMC");  cout<<" ZTT high mT : "<<HMC->Integral()<<endl;
  TH1F* HTT=getTTJetsInc();  HMC->Add(HTT); cout<<" TT high mT : "<<HTT->Integral()<<endl;
  delete HTT;
  TH1F* HVV=getDiBoson();    HMC->Add(HVV); cout<<" VV high mT : "<<HVV->Integral()<<endl;
  delete HVV;
  TH1F* HZL=getZLVBFHCP();      HMC->Add(HZL); cout<<" ZL high mT : "<<HZL->Integral()<<endl;
  delete HZL;
  TH1F* HZJ=getZToLJetInc(); HMC->Add(HZJ); cout<<" ZJ high mT : "<<HZJ->Integral()<<endl;
  delete HZJ;
  MTcat_=tmpCategoryMT;
  cout<<" Data - MC high mT : "<<HData->Integral()-HMC->Integral()<<endl;

  TString tmpextrasel=extrasel_;

  ///get the mass template with relaxed selection 
  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  extrasel_=shapesel;
  TH1F*hShape=getWNJetSumNoChCut();
  hShape->SetName("getWJetsNJetShape");
  extrasel_=tmpextrasel;

  ///Calculate the integral of the W template at high mT
  TString shaperatio="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  extrasel_=shaperatio;
  MTcat_=13;
  TH1F* HWH=getWNJetSumNoChCut();
  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;
  TH1F*HWL=getWNJetSumNoChCut();
  HWL->SetName("HWL");
  extrasel_=tmpextrasel;

  
  if(HWH->Integral()>0.&&hShape->Integral()>0.){
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



TH1F* TauMuPlotter::getWJetsNJetVBFTight(){
  //cout<<"====Calling getWJetsNJetVBFHCP===="<<endl;


  TString tmpextrasel=extrasel_;
  Int_t tmpIsocat=Isocat_;
  Isocat_=0;
  extrasel_+="*(muiso<0.1&&tauiso3hitraw<10.0)";
  TH1F*hShape=getWNJetSumNoChCut();
  hShape->SetName("getWJetsNJetVBFTightShape");
  Isocat_=tmpIsocat;
  extrasel_=tmpextrasel;

  Int_t tmpCategoryMT=MTcat_;

  ///get W Yield at high mT without relaxed selection
  MTcat_=13;
  TH1F* HData=getTotalData(); HData->SetName("HData"); 
  TH1F* HMC=getZToTauTau();   HMC->SetName("HMC"); 
  TH1F* HTT=getTTJetsInc();  
  TH1F* HVV=getDiBoson();     
  TH1F* HZL=getZLVBFHCP();    
  TH1F* HZJ=getZToLJetInc();  
  MTcat_=tmpCategoryMT;

  cout<<" Data high mT : "<<HData->Integral()<<endl; 
  cout<<" ZTT high mT : "<<HMC->Integral()<<endl; 
  cout<<" TT high mT : "<<HTT->Integral()<<endl;  HMC->Add(HTT);  delete HTT;
  cout<<" VV high mT : "<<HVV->Integral()<<endl;  HMC->Add(HVV);  delete HVV;
  cout<<" ZL high mT : "<<HZL->Integral()<<endl;  HMC->Add(HZL);  delete HZL;
  cout<<" ZJ high mT : "<<HZJ->Integral()<<endl;  HMC->Add(HZJ);  delete HZJ;
  cout<<" Data - MC high mT : "<<HData->Integral()-HMC->Integral()<<endl;




  ///Calculate the integral of the W template at high mT
  TString shaperatio="(njet>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  extrasel_=shaperatio;
  MTcat_=13;
  TH1F* HWH=getWNJetSumNoChCut();
  HWH->SetName("HWH");
  MTcat_=tmpCategoryMT;
  TH1F*HWL=getWNJetSumNoChCut();
  HWL->SetName("HWL");
  extrasel_=tmpextrasel;

  
  if(HWH->Integral()>0.&&hShape->Integral()>0.){
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


TH1F* TauMuPlotter::getSampleVBFHCPShape(TString sample){
  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getSample(sample);
  hShape->SetName(sample+"VBFHCPShape");
  extrasel_=tmpextrasel;
  return hShape;
}

TH1F* TauMuPlotter::getSampleVBFTightHCPShape(TString sample){
  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getSample(sample);
  hShape->SetName(sample+"VBFHCPShape");
  extrasel_=tmpextrasel;
  return hShape;
}

TH1F* TauMuPlotter::getSampleVBFHCPShape2(TString sample){
  TString shapesel="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TString tmpextrasel=extrasel_;
  extrasel_=shapesel;
  TH1F*hShape=getSample(sample);
  hShape->SetName(sample+"VBFHCPShape");
  extrasel_=tmpextrasel;
  return hShape;
}

TH1F* TauMuPlotter::getSampleVBFHCP(TString sample,int type){
  TH1F*h=getSample(sample);
  h->SetName(sample+"VBFHCP");
  TH1F*hShape=0;
  if(type==0)hShape=getSampleVBFHCPShape(sample);
  if(type==1)hShape=getSampleVBFHCPShape2(sample);

  if(hShape->Integral()>0.) hShape->Scale(h->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  return hShape;
}

TH1F* TauMuPlotter::getSampleVBFTightHCP(TString sample){
  TH1F*h=getSample(sample);
  h->SetName(sample+"VBFHCP");
  TH1F*hShape=0;
  hShape=getSampleVBFTightHCPShape(sample);

  if(hShape->Integral()>0.) hShape->Scale(h->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  return hShape;
}




//TTJets
TH1F* TauMuPlotter::getTTJetsInc(){
  return getSample("TTJets");
}
TH1F* TauMuPlotter::getTTJetsVBFHCP(){
  TH1F*h=getSampleVBFHCP("TTJets");
  h->Scale(TTJetsCorrFactor[2]);
  return h;
}


//ZLJet
TH1F* TauMuPlotter::getZToLJetInc(){
  //cout<<" Calling getZToLJetInc"<<endl;
  //return getSample("ZToLJet");
  return getZNJetSum("LJet");
}
TH1F* TauMuPlotter::getZToLJetVBFHCP(){
  //cout<<" Calling getZToLJetVBFHCP"<<endl;
  //return getSampleVBFHCP("ZToLJet",1);
  TH1F* h = getZNJetSum("LJet");
  h->SetName("getZNJetSumLJet");

  TString tmpextrasel=extrasel_;
  extrasel_="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TH1F* hShape = getZNJetSum("LJet");
  hShape->SetName("getZNJetSumLJetShape");
  extrasel_=tmpextrasel;
  
  if(hShape->Integral()>0)
    hShape->Scale(h->Integral()/hShape->Integral());
  delete h;
  return hShape;
}

TH1F* TauMuPlotter::getZToLJetVBFTightHCP(){
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


//ZMuMu
TH1F* TauMuPlotter::getZLInc(){
  //cout<<" Calling getZLInc"<<endl;
  //TH1F*h=getSample("ZToMuMu");
  TH1F*h=getZNJetSum("MuMu");
  return h;
}

TH1F* TauMuPlotter::getZNJetSum(TString decay){
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
TH1F* TauMuPlotter::getZLVBFHCP(){
  //cout<<" Calling getZLVBFHCP"<<endl;
  //return getSampleVBFHCP("ZToMuMu",1);


  //Norm from Embedded
  TString TmpExtrasel=extrasel_;  

  ////Get ZL Yield at Inclusive
  extrasel_="(njet>=2)"; 
  TH1F*hNorm=getZLInc();
  hNorm->SetName("getZLVBFHCP");
  TH1F*hEmbIncl=getTotalEmbedded(); 
  hEmbIncl->SetName("getZLVBFHCPEmbIncl");
  extrasel_=TmpExtrasel;

  //get Embedded yield at VBF
  TH1F*hEmbVBF=getTotalEmbedded(); 
  hEmbVBF->SetName("getZLVBFHCPEmbVBF");

  ////Shape
  //TH1F*hShape=getSampleVBFHCPShape2("ZToMuMu");
  TString tmpextrasel=extrasel_;
  extrasel_="(njet20>=2&&njetingap==0&&diJetMass>200&&abs(diJetDeltaEta)>2.0)";
  TH1F* hShape = getZNJetSum("MuMu");
  hShape->SetName("getZLVBFHCPShape");
  extrasel_=tmpextrasel;


  
  if(hShape->Integral()>0){
    hShape->Scale(hNorm->Integral()*(hEmbVBF->Integral()/hEmbIncl->Integral())/hShape->Integral());
  }else hShape->Scale(0.);

  delete hNorm;
  return hShape;

}

TH1F* TauMuPlotter::getZLLVBFHCP(){
  //cout<<" Calling getZLLVBFHCP"<<endl;
//   TH1F* hZL=getSample("ZToMuMu");
//   TH1F* hZJ=getSample("ZToLJet");

//   TH1F* hZLShape=getSampleVBFHCPShape("ZToMuMu");
//   TH1F* hZJShape=getSampleVBFHCPShape("ZToLJet");

//   hZLShape->Add(hZJShape);
//   hZLShape->Scale((hZL->Integral()+hZJ->Integral())/hZLShape->Integral());
//   hZLShape->SetName("getZLLVBFHCP");
//   return hZLShape;


  //////Embedded norm
  //Norm from Embedded
  TString TmpExtrasel=extrasel_;  
  extrasel_="1"; 

  ////Get ZL Yield at Inclusive
  TH1F*hZLNorm=getSample("ZToMuMu");
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
  TH1F* hZLLShape=getSampleVBFHCPShape("ZToMuMu");
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

// TH1F* TauMuPlotter::getWJetsIncShape(){
//   //cout<<" calling method getWJetsIncShape"<<endl;

//   TString tmpextrasel=extrasel_;
//   Int_t tmpMTcat=MTcat_;
//   Int_t tmpIsocat=Isocat_;

//   //get shape from relaxed iso 
//   Isocat_=-1;//relax isolation
//   extrasel_ += "*(muiso<0.2)";//loose tau iso biases mT shape
//   //extrasel_ += "*(mupt>20)*((nvtx==0)*0+(nvtx==1)*0.0245307+(nvtx==2)*0.048848+(nvtx==3)*0.0638493+(nvtx==4)*0.116917+(nvtx==5)*0.15697+(nvtx==6)*0.268479+(nvtx==7)*0.445571+(nvtx==8)*0.645714+(nvtx==9)*0.998239+(nvtx==10)*1.37402+(nvtx==11)*1.91361+(nvtx==12)*2.58661+(nvtx==13)*3.4313+(nvtx==14)*5.05569+(nvtx==15)*7.44155+(nvtx==16)*9.92943+(nvtx==17)*15.881+(nvtx==18)*24.6622+(nvtx==19)*38.3726+(nvtx==20)*57.9205+(nvtx==21)*99.3125+(nvtx==22)*150.007+(nvtx==23)*248.378+(nvtx==24)*357.212+(nvtx==25)*556.722+(nvtx==26)*688.842+(nvtx==27)*1295.89+(nvtx==28)*1578.77+(nvtx==29)*1500.43)";
//   //extrasel_ += "*(1+0.3*metpt*cos(metphi)/50.)*(1-0.06*metpt*sin(metphi)/50.)*(1-0.6*cos(metphi-muphi))";
//   cout<<" using relaxed iso selection :"<<extrasel_<<endl;
//   TH1F* hShape=getSample("WJetsToLNu");
//   hShape->SetName("hShape");

//   //go to high mT still with relaxed 
//   MTcat_=13;
//   if(MSSMFlag_)MTcat_=103;
//   TH1F* hShapeHighMT=getSample("WJetsToLNu");
//   hShapeHighMT->SetName("hShapeHighMT");

//   //normalize to at high mT
//   extrasel_=tmpextrasel;
//   Isocat_=tmpIsocat;
//   TH1F* hDataHighMT=getTotalData();
//   TH1F* hZToTauTau=getSample("ZToTauTau");
//   hDataHighMT->Add(hZToTauTau,-1); delete hZToTauTau;
//   TH1F* hTTJets=getSample("TTJets");
//   hDataHighMT->Add(hTTJets,-1); delete hTTJets;
//   TH1F* hZToLJet=getSample("ZToLJet");
//   hDataHighMT->Add(hZToLJet,-1); delete hZToLJet;
//   TH1F* hZToMuMu=getSample("ZToMuMu");
//   hDataHighMT->Add(hZToMuMu,-1); delete hZToMuMu;
//   MTcat_=tmpMTcat;

//   //
//   hShape->Scale(hDataHighMT->Integral()/hShapeHighMT->Integral());
//   delete hDataHighMT;
//   delete hShapeHighMT;
  
//   return hShape;

// }



TH1F* TauMuPlotter::getQCDInc(Int_t WType,bool cleanNegativeBins){
  //cout<<"Calling method getQCDInc"<<endl;

  Int_t ChcatTmp=Chcat_;
  Chcat_=2;

  TH1F* hDataSS=getTotalData();
  hDataSS->SetName("hgetQCDInc");
  cout<<" Data SS : "<<hDataSS->Integral()<<endl;

  //subtract MC
  TH1F*hWJets=0;
  if(WType==0)hWJets=getWJetsInc();   
  if(WType==1)hWJets=getWJetsNJet(); 
  cout<<"W "<<hWJets->Integral()<<endl;
  hDataSS->Add(hWJets,-1); delete hWJets;

   
  
  Int_t ZTTypeTmp=ZTTType_;
  ZTTType_=1; //always use MC in SS
  TH1F*hZToTauTau=getZToTauTau();   
  hDataSS->Add(hZToTauTau,-1);
  cout<<"ZTT "<<hZToTauTau->Integral()<<endl; 
  delete hZToTauTau;
  ZTTType_=ZTTypeTmp;

  TH1F*hDiBoson =getDiBoson();      hDataSS->Add(hDiBoson,-1);   cout<<"VV "<<hDiBoson->Integral()<<endl;
  delete hDiBoson;
  TH1F*hTTJets  =getTTJetsInc();    hDataSS->Add(hTTJets,-1);    cout<<"TT "<<hTTJets->Integral()<<endl;
  delete hTTJets;
  TH1F*hZL      =getZLInc();        hDataSS->Add(hZL,-1);        cout<<"ZL "<<hZL->Integral()<<endl; 
  delete hZL;
  TH1F*hZLJet   =getZToLJetInc();     hDataSS->Add(hZLJet,-1);     cout<<"ZJ "<<hZLJet->Integral()<<endl; 
  delete hZLJet;

  Chcat_=ChcatTmp;

  //keep norm before cleaning negative bins
  float norm=hDataSS->Integral();

  //clean up negative bins
  if(cleanNegativeBins)
    for(Int_t b=1;b<=hDataSS->GetNbinsX();b++)
      if(hDataSS->GetBinContent(b)<0.) hDataSS->SetBinContent(b,0.);

  if( hDataSS->Integral()>0.){
    hDataSS->Scale(norm/hDataSS->Integral());
  } else hDataSS->Scale(0.);

  cout<<"Scaling QCDInc by OS/SS factor "<<QCDOStoSSRatio_<<endl;
  hDataSS->Scale(QCDOStoSSRatio_);
  
  return hDataSS;
}

TH1F* TauMuPlotter::getQCDIncWNJet(){
  return getQCDInc(1,1);//do clean negative bins as the shape will be used
}


TH1F* TauMuPlotter::getQCDMuIso(){
  
  TH1F* hNorm=getQCDInc(1,0);//dont clean up the negative bins in QCD otherwise norm is biased

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;

  TString extraselTmp=extrasel_;
  extrasel_+="*(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)";

  // cout<<"calling getQCDMuIso Shape"<<endl;
  TH1F* hShape=getTotalData();

  Chcat_=ChcatTmp;
  Isocat_=IsocatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  
  return hShape;
}


TH1F* TauMuPlotter::getQCDMuIsoLooseTau(){
  //cout<<"Calling method getQCDMuIso"<<endl;

  TH1F* hNorm=getQCDInc(1,0);//dont clean up the negative bins in QCD otherwise norm is biased

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  extrasel_+="*(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10)";
  TH1F* hShape=getTotalData();
  Chcat_=ChcatTmp;
  Isocat_=IsocatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);
  
  return hShape;
}


TH1F* TauMuPlotter::getQCDIncLowPt(){
  //cout<<"Calling method getQCDLowPt"<<endl;

  TH1F*hNorm=getQCDIncWNJet();///Note is using the of exclusive W samples

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  extrasel_+="*(0.2<muiso&&muiso<1.)";
  
  TH1F* hShape=getTotalData();
  hShape->SetName("getQCDIncLooseShape");
//   TH1F*hMC=getTotalMCSM();
//   hShape->Add(hMC,-1);
//   delete hMC;

  for(Int_t b=1;b<=hShape->GetNbinsX();b++) 
    if(hShape->GetBinContent(b)<0.) hShape->SetBinContent(b,0.);

  Chcat_=ChcatTmp;
  Isocat_=IsocatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0.)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);

  ///Apply correction at low mass 
  if(plotvar_.CompareTo("ditaumass")==0 || plotvar_.CompareTo("svfitmass")==0)
    for(Int_t b=1;b<=hShape->GetNbinsX();b++)
      if(hShape->GetBinCenter(b)<70)hShape->SetBinContent(b,1.2*hShape->GetBinContent(b));


  return hShape;
}

///Method for 1-Jet High pT category
TH1F* TauMuPlotter::getQCDIncHighPt(){
  //cout<<"Calling method getQCDHighPt"<<endl;

  TH1F*hNorm=getQCDIncWNJet();

  int ChcatTmp=Chcat_;
  Chcat_=2;
  int IsocatTmp=Isocat_;
  Isocat_=0;
  TString extraselTmp=extrasel_;
  extrasel_+="*(0.1<muiso)";
  
  TH1F* hShape=getTotalData();
  hShape->SetName("getQCDIncLooseShape");
  TH1F*hMC=getTotalMCSM();
  hShape->Add(hMC,-1);
  delete hMC;

  for(Int_t b=1;b<=hShape->GetNbinsX();b++) 
    if(hShape->GetBinContent(b)<0.) hShape->SetBinContent(b,0.);

  Chcat_=ChcatTmp;
  Isocat_=IsocatTmp;
  extrasel_=extraselTmp;

  if(hShape->Integral()>0.)
    hShape->Scale(hNorm->Integral()/hShape->Integral());
  else hShape->Scale(0.);

  return hShape;
}


TH1F* TauMuPlotter::getQCDKetiHCP(){
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




TH1F* TauMuPlotter::getQCDVBFHCP(){
  //cout<<"=====Calling getQCDVBFHCP======"<<endl;

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
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10.0&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)");
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData();
  hShape->SetName("hShape");

  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&%s)",tauIsoCut_.Data());
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
    cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
    cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
    float vbfeff=hDataVBFLoose->Integral()/hDataIncLoose->Integral();
    cout<<" VBF efficiency "<<vbfeff<<endl;
    hShape->Scale(hQCDInc->Integral()*vbfeff/hShape->Integral());
  }else hShape->Scale(0.);

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}

TH1F* TauMuPlotter::getQCDVBFLoose(){
  //cout<<"=====Calling getQCDVBFLoose======"<<endl;

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
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10.0&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0)");
  extrasel_=isocuttxtshape;
  TH1F* hShape=getTotalData();
  hShape->SetName("hShape");

  char isocuttxt[100];
  sprintf(isocuttxt,"(0.2<muiso&&muiso<0.5&&%s)",tauIsoCut_.Data());
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
    cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
    cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
    float vbfeff=hDataVBFLoose->Integral()/hDataIncLoose->Integral();
    cout<<" VBF efficiency "<<vbfeff<<endl;
    hShape->Scale(hQCDInc->Integral()*vbfeff/hShape->Integral());
  }else hShape->Scale(0.);

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}



TH1F* TauMuPlotter::getQCDVBFTight(){
  //cout<<"=====Calling getQCDVBFTight======"<<endl;

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
  sprintf(isocuttxtshape,"(0.2<muiso&&muiso<0.5&&tauiso3hitraw<10.0&&njet20>=2&&diJetMass>200&&abs(diJetDeltaEta)>2.0&&ditaumetpt>100)");
  extrasel_=isocuttxtshape;
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
    cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
    cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
    float vbfeff=hDataVBFLoose->Integral()/hDataIncLoose->Integral();
    cout<<" VBF efficiency "<<vbfeff<<endl;
    hShape->Scale(hQCDInc->Integral()*vbfeff/hShape->Integral());
  }else hShape->Scale(0.);

  delete hDataIncLoose;
  delete hDataVBFLoose;
  delete hQCDInc;

  return hShape;
}



TH1F* TauMuPlotter::getQCDBoostTight(){
  ///cout<<"=====Calling getQCDBoostTight======"<<endl;

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



bool TauMuPlotter::plotInc(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t Chcat,  Int_t Isocat, Int_t MTcat,TString extrasel, TString blindsel, Int_t QCDType, Int_t WJetsType, TString xlabel, TString ylabel,Float_t* legendcoords, int higgs,TString filetag){

  

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Chcat_=Chcat;
  Isocat_=Isocat;
  MTcat_=MTcat;
  extrasel_="1";//reset for multiple plotting
  if(extrasel.CompareTo("")!=0) extrasel_ += TString("*")+extrasel;
  cout<<"eventweight = "<<eventWeight_<<endl;
  cout<<"extrasel = "<<extrasel_<<endl;

  if(!scaleSamplesLumi())return 0;
   

  TH1F*hQCD = 0;
  if(QCDType==0) hQCD=getQCDIncWNJet();
  else if(QCDType==1) hQCD=getQCDMuIso();
  else if(QCDType==2) hQCD=getQCDMuIsoLooseTau();
  else if(QCDType==5) hQCD=getQCDVBFLoose();//option # 5 has to be VBF always since logic below relies on that
  else if(QCDType==6) hQCD=getQCDKetiHCP();
  else if(QCDType==7) hQCD=getQCDIncLowPt();
  else if(QCDType==8) hQCD=getQCDBoostTight();
  else if(QCDType==9) hQCD=getQCDVBFTight();
  else {
    cout<<"no QCD determined "<<endl;
  }
  if(hQCD){
    hQCD->SetLineWidth(1);
    hQCD->SetLineColor(1);
    hQCD->SetFillColor(QCDColor_);
  }
  if(hQCD) hQCD->SetName("hQCDplotInc");
  if(hQCD) cout<<"QCD "<<hQCD->Integral()<<endl;
  if(nbins_==0) if(hQCD) makeDensityHisto(hQCD);


  
  TH1F* hWJetsToLNu=0;
  if(WJetsType==0) hWJetsToLNu = getWJetsNJet();
  else if(WJetsType==1) hWJetsToLNu = getWJetsNJet(1);
  else if(WJetsType==2) hWJetsToLNu = getWJetsNJet(2);
  else if(WJetsType==6) hWJetsToLNu = getWJetsNJetVBFLoose();
  else if(WJetsType==7) hWJetsToLNu = getWJetsNJetVBFTight();
  else  hWJetsToLNu = getWJetsInc(); 
  if(!hWJetsToLNu){
    cout<<"WJets Background is NULL"<<endl; 
    return 0;
  }
  hWJetsToLNu->SetName("hWJetsToLNuplotInc");
  hWJetsToLNu->SetLineWidth(1);
  hWJetsToLNu->SetLineColor(1);
  hWJetsToLNu->SetFillColor(WJetsColor_);
  cout<<"hWJetsToLNu "<<hWJetsToLNu->Integral()<<endl;
  if(nbins_==0) makeDensityHisto(hWJetsToLNu);  



  TH1F*hTTJets=0;
  if(QCDType==5|| QCDType==9)  hTTJets=getTTJetsVBFHCP();
  else hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  hTTJets->SetName("hTTJetsplotInc");
  hTTJets->SetLineWidth(1);
  hTTJets->SetLineColor(1);
  hTTJets->SetFillColor(TTJetsColor_); 
  cout<<"TTJets "<<hTTJets->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hTTJets);  

  ///Z->tau tau
  TH1F*hZToTauTau=getZToTauTau();
  //TH1F*hZToTauTau=getZToTauTauVBF();//makes large bias
  if(!hZToTauTau)return 0;
  hZToTauTau->SetName("hZToTauTauplotInc");
  hZToTauTau->SetLineWidth(1);
  hZToTauTau->SetLineColor(1);
  hZToTauTau->SetFillColor(ZTauTauColor_);
  cout<<"ZToTauTau "<<hZToTauTau->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZToTauTau);  

  
  TH1F*hEWK=(TH1F*)hWJetsToLNu->Clone("hElectroWeak");

  //combine Di-Bosons
  TH1F*hVV=0;
  if(QCDType==5 || QCDType==9)  hVV=getDiBosonVBFHCP();
  else hVV=getDiBoson();
  if(!hVV)return 0;
  hVV->SetName("hVVplotInc");
  cout<<hVV->GetName()<<"  "<<hVV->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hVV); 
  hEWK->Add(hVV);




  //combine Z->MuMu
  //TH1F*hZL=getSample("ZToMuMu");
  TH1F*hZL=0;
  //if(QCDType==5)  hZL=getZLVBFHCP();
  //else 
  hZL=getZLInc();
  if(!hZL)return 0;
  hZL->SetName("ZL");
  hZL->SetLineWidth(1);
  hZL->SetLineColor(1);
  hZL->SetFillColor(ZMuMuColor_);
  cout<<"ZL  "<<hZL->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZL); 
  TH1F* hZLL=(TH1F*)hZL->Clone("ZLL");

  //combine ZLJet
  //TH1F*hZJ=getSample("ZToLJet");
  TH1F*hZJ=0;
  //if(QCDType==5)  hZJ=getZToLJetVBFHCP();
  //else 
  hZJ=getZToLJetInc();
  if(!hZJ)return 0;
  hZJ->SetName("ZJ");
  cout<<"ZJ  "<<hZJ->Integral()<<endl;
  if(nbins_==0)makeDensityHisto(hZJ); 
  hZLL->Add(hZJ);


  if(QCDType==5|| QCDType==9){//Replace ZL by ZLL and set ZJ to 0, this is only for the plot
    hZLL->Scale(0.);
    TH1F* htmp=getZLLVBFHCP();//special estimation
    hZLL->Add(htmp);
    delete htmp;
    cout<<"ZLL "<<hZLL->Integral()<<endl;
    if(nbins_==0)makeDensityHisto(hZLL); 
  }

  //cout<<"ElectroWeak "<<hEWK->Integral()<<endl;


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
      TH1F*hHiggsGG=getSample("HiggsGG125");      hHiggs->Add(hHiggsGG);       delete hHiggsGG;
      TH1F*hHiggsVH=getSample("HiggsVH125");      hHiggs->Add(hHiggsVH);       delete hHiggsVH;
      cout<<"Total Higgs yield : "<<hHiggs->Integral()<<endl;
      hHiggs->Scale(higgs);
      if(nbins_==0)makeDensityHisto(hHiggs); 
      hHiggs->Add(hBkg);
      hHiggs->SetLineColor(1);
      hHiggs->SetLineStyle(2);
    }
  }

  ////Total Data
  if(blindsel.CompareTo("")!=0) blindsel_=blindsel;
  TH1F* hData=getTotalData();
  if(nbins_==0)makeDensityHisto(hData); 
  blindsel_="1";//blind only around this call otherwise may affect background estimations
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}

  cout<<"Total Data "<<(int)(hData->Integral())<<endl; 

  //create legend key
  TLegend legend;
  legend.SetFillStyle (0);
  legend.SetFillColor (0);
  legend.SetBorderSize(0);
  legend.AddEntry(hData,"Observed","P");  
  if(hHiggs){
    if(MSSMFlag_)legend.AddEntry(hHiggs,TString("Signal"),"L");
    else legend.AddEntry(hHiggs,TString("")+(long)higgs+" x SM H(125)","L");
  }
  legend.AddEntry(hZToTauTau,"Z#rightarrow#tau^{+}#tau^{-}","F");
  legend.AddEntry(hZLL,"Z#rightarrow#mu^{+}#mu^{-}","f");
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
//   TString filename=outputpath_+"/TauMuPlotter_"+tag;
  TString filename=outputpath_+"/"+filetag;

  TCanvas C("C",filename);  
  C.Print(filename+".ps[");


  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->SetTitle("");
  //hData->GetYaxis()->SetRangeUser(0,(hData->GetMaximum()+hData->GetBinError(hData->GetMaximumBin()))*1.15);
  float maxyData=(hData->GetMaximum()+hData->GetBinError(hData->GetMaximumBin()))*1.25;
  float maxyMC=(hBkg->GetMaximum()+hBkg->GetBinError(hBkg->GetMaximumBin()))*1.25;
  hData->GetYaxis()->SetRangeUser(0,maxyData>maxyMC ? maxyData : maxyMC);
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
  //C.Print(filename+".eps");  
  //C.Print(filename+".pdf");  
  //C.Print(filename+".C");  
  hData->GetYaxis()->SetRangeUser(0.1,hData->GetMaximum()*100);
  C.Update();
  C.SetLogy(1);
  C.Print(filename+".ps"); 
  C.Print(filename+"_log.png");   
  //C.Print(filename+"_log.eps");   
  //C.Print(filename+"_log.pdf");   
  //C.Print(filename+"_.C");  
  C.SetLogy(0);

  
  TLine line;




  //make the ratio plot
  TH1F* hRatio=getPlotHisto("hRatio");
  hRatio->Add(hData);
  hRatio->Divide(hBkg);


  TPad * pad1=new TPad("pad1","",0,0.2,1,1);
  TPad * pad2=new TPad("pad2","",0,0,1,0.2);
    
  pad1->cd();
  hData->GetYaxis()->SetRangeUser(0,maxyData>maxyMC ? maxyData : maxyMC);
  hData->Draw("hist pe");
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");
  hBkg->Draw("histsame");
  hMCStack.GetHistogram()->Draw("same");
  if(hHiggs)hHiggs->Draw("histsame");
  legend.Draw();
  title.Draw();

  pad2->cd();
  hRatio->SetStats(0);
  hRatio->SetTitle("");
  hRatio->GetYaxis()->SetTitle("Data/MC");
  hRatio->GetXaxis()->SetTitle("");
  hRatio->GetYaxis()->SetNdivisions(5);
  hRatio->GetYaxis()->SetLabelSize(0.2);
  hRatio->GetYaxis()->SetTitleSize(0.2);
  hRatio->GetYaxis()->SetTitleOffset(0.32);
  hRatio->GetXaxis()->SetNdivisions(-1);
  hRatio->GetXaxis()->SetLabelSize(0.0001);
  hRatio->GetYaxis()->SetRangeUser(0.8,1.2);
  hRatio->Draw("hist pe");
  line.DrawLine(xmin_,1,xmax_,1);

  C.Clear();
  pad1->Draw();
  pad2->Draw();
  C.Print(filename+".ps");  
  C.Print(filename+"_ratio.png");   



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




TH1F* TauMuPlotter::getTotalMCSM(){
  TH1F* h=getPlotHisto("hTotalsMCSM");

  TH1F*hZToTauTau=getZToTauTau(); 
  if(!hZToTauTau)return 0;
  h->Add(hZToTauTau); cout<<" ZTT : "<<hZToTauTau->Integral()<<endl;
  delete hZToTauTau;
  TH1F*hWJets=getWJetsInc();
  if(!hWJets)return 0;
  h->Add(hWJets); cout<<" W : "<<hWJets->Integral()<<endl;
  delete hWJets;
  TH1F*hDiBoson=getDiBoson();
  if(!hDiBoson)return 0;
  h->Add(hDiBoson); cout<<" VV : "<<hDiBoson->Integral()<<endl;
  delete hDiBoson;
  TH1F*hTTJets=getTTJetsInc();
  if(!hTTJets)return 0;
  h->Add(hTTJets); cout<<" TT : "<<hTTJets->Integral()<<endl;
  delete hTTJets;
  TH1F*hZToMuMu=getZLInc();
  if(!hZToMuMu)return 0;
  h->Add(hZToMuMu); cout<<" ZL : "<<hZToMuMu->Integral()<<endl;
  delete hZToMuMu;
  TH1F*hZLJet=getZToLJetInc();
  if(!hZLJet)return 0;
  h->Add(hZLJet); cout<<" ZJ : "<<hZLJet->Integral()<<endl;
  delete hZLJet;


  cout<<" Total MC : "<<h->Integral()<<endl;
  return h;
}



// TH1F* TauMuPlotter::getW3JetsVBF(){
//   //cout<<" calling getW3JetsVBF"<<endl;

//   //get templates from inclusive 2-jet sample and normalize at high mT in VBF
//   TString TmpExtrasel=extrasel_;
//   extrasel_="(njet>=2)"; 

//   TH1F* hW3JetsShape=getSample("W3JetsToLNu");
//   hW3JetsShape->SetName("hW3JetsShape");
//   cout<<"hW3JetsShape :"<<hW3JetsShape->Integral()<<endl;
//   //need to know integral of template at high mT for normalization
//   Int_t tmpMTcat=MTcat_;
//   MTcat_=13;//go to high mT with same selections as with plot, cannot relax iso according to Lorenzo
//   TH1F* hW3JetsShapeHighMT=getSample("W3JetsToLNu");
//   hW3JetsShapeHighMT->SetName("hW3JetsShapeHighMT");

//   extrasel_=TmpExtrasel;  //return to VBF catetory But stay at high mT

//   //determine normalization factor
//   //Get Data and TTJets at high mT in VBF
//   TH1F* hDataHighMT=getTotalData();
//   hDataHighMT->SetName("hDataHighMT");
//   cout<<"hDataHighMT :"<<hDataHighMT->Integral()<<endl;
//   TH1F* hTTJetsHighMT=getSample("TTJets");
//   hTTJetsHighMT->SetName("hTTJetsHighMT");
//   cout<<"hTTJetsHighMT :"<<hTTJetsHighMT->Integral()<<endl;
//   TH1F* hZTTHighMT=getSample("ZToTauTau");
//   hZTTHighMT->SetName("hZTTHighMT");
//   cout<<"hZTTHighMT :"<<hZTTHighMT->Integral()<<endl;
//   TH1F* hZJHighMT=getSample("ZToLJet");
//   hZJHighMT->SetName("hZJHighMT");
//   cout<<"hZJHighMT :"<<hZJHighMT->Integral()<<endl;

//   MTcat_=tmpMTcat;//Return signal mT region

//   //normalize the Template
//   Float_t WJetsIntegralFromhighMTData=(hDataHighMT->Integral()-hTTJetsHighMT->Integral()-hZTTHighMT->Integral()-hZJHighMT->Integral());
//   hW3JetsShape->Scale(WJetsIntegralFromhighMTData/hW3JetsShapeHighMT->Integral());
  
//   delete hW3JetsShapeHighMT;
//   delete hDataHighMT;
//   delete hTTJetsHighMT;
//   delete hZTTHighMT;
//   delete hZJHighMT;

//   //cout<<"----> getW3JetsVBFyield: "<<hW3JetsShape->Integral()<<endl;
 

//   return hW3JetsShape;
// }


// TH1F* TauMuPlotter::getW2JetsBJet(){
//   //cout<<" calling getW2JetsBJet"<<endl;

// // Keti:
// // Wjets normalization with p_zeta
// // *check normalization in p_zeta < -40
// // * apply the correction in signal region p_zeta > -20
// // Wjets shape
// // * using W2jet sample without bjet requirement (if needed) and without OS charge requirement.


//   TString TmpExtrasel=extrasel_;
//   Int_t tmpIsocat=Isocat_;
//   Isocat_=-1;
//   extrasel_=extrasel_+"*(muiso<0.1)";
//   TH1F* hWJetsShape=getSample("W2JetsToLNu");
//   hWJetsShape->SetName("hWJetsShape");
//   cout<<"hWJetsShape :"<<hWJetsShape->Integral()<<endl;

//   Int_t tmpMTcat=MTcat_;
//   MTcat_=103;  //need to know integral of template at high mT for normalization

//   TH1F* hWJetsShapeHighMT=getSample("W2JetsToLNu");
//   hWJetsShapeHighMT->SetName("hWJetsShapeHighMT");

//   Isocat_=tmpIsocat;
//   extrasel_=TmpExtrasel;  //return to category at but stay at high mT

//   //determine normalization factor
//   TH1F* hDataHighMT=getTotalData();
//   hDataHighMT->SetName("hDataHighMT");
//   cout<<"hDataHighMT :"<<hDataHighMT->Integral()<<endl;
//   TH1F* hTTJetsHighMT=getSample("TTJets");
//   hTTJetsHighMT->SetName("hTTJetsHighMT");
//   cout<<"hTTJetsHighMT :"<<hTTJetsHighMT->Integral()<<endl;
//   TH1F* hZTTHighMT=getZToTauTau();
//   hZTTHighMT->SetName("hZTTHighMT");
//   cout<<"hZTTHighMT :"<<hZTTHighMT->Integral()<<endl;
//   TH1F* hZJHighMT=getSample("ZToLJet");
//   hZJHighMT->SetName("hZJHighMT");
//   cout<<"hZJHighMT :"<<hZJHighMT->Integral()<<endl;
//   MTcat_=tmpMTcat;//Return signal  region

//   //normalize the Template
//   Float_t WJetsIntegralFromhighMTData=(hDataHighMT->Integral()-hTTJetsHighMT->Integral()-hZTTHighMT->Integral()-hZJHighMT->Integral());
//   float scale=WJetsIntegralFromhighMTData/hWJetsShapeHighMT->Integral();
//   cout<<" Scaling WJets by factor "<<scale<<endl;
//   hWJetsShape->Scale(scale);
  
//   delete hWJetsShapeHighMT;
//   delete hDataHighMT;
//   delete hTTJetsHighMT;
//   delete hZTTHighMT;
//   delete hZJHighMT;

//   //cout<<"----> getWJets yield: "<<hWJetsShape->Integral()<<endl;
 

//   return hWJetsShape;
// }



// TH1F* TauMuPlotter::getW3Jets(){
//   //cout<<" calling getW3Jets(): "<<endl;

//   //integral of WJets in signal region
//   TH1F* hW3JetsLowMT=getSample("W3JetsToLNu");
//   hW3JetsLowMT->SetName("hW3JetsLowMT");
//   cout<<"hW3JetsLowMT :"<<hW3JetsLowMT->Integral()<<endl;

//   //calculate normalization factor
//   Int_t tmpMTcat=MTcat_;
//   MTcat_=3;//go to high mT with same selections as with plot, cannot relax iso according to Lorenzo
//   TH1F* hDataHighMT=getTotalData();
//   hDataHighMT->SetName("hDataHighMT");
//   cout<<"hDataHighMT :"<<hDataHighMT->Integral()<<endl;
//   TH1F* hTTJetsHighMT=getSample("TTJets");
//   hTTJetsHighMT->SetName("hTTJetsHighMT");
//   cout<<"hTTJetsHighMT :"<<hTTJetsHighMT->Integral()<<endl;
//   TH1F* hW3JetsHighMT=getSample("W3JetsToLNu");
//   hW3JetsHighMT->SetName("hW3JetsHighMT");
//   cout<<"hW3JetsHighMT :"<<hW3JetsHighMT->Integral()<<endl;
//   MTcat_=tmpMTcat;

//   //for shape relax the isolation in low mT region
//   Int_t TmpIsocat=Isocat_;
//   Isocat_=-1;
//   TString TmpExtrasel=extrasel_; 
//   char isocuttxt[100];
//   //sprintf(isocuttxt,"(muiso<0.1&&tauisomvaraw>%.3f)",-0.75);
//   sprintf(isocuttxt,"(muiso<0.1)");
//   cout<<" using loose iso selection "<<isocuttxt<<endl;
//   extrasel_+=TString("*")+isocuttxt;
//   TH1F* hW3JetsShape=getSample("W3JetsToLNu");
//   hW3JetsShape->SetName("hW3JetsShape");
//   cout<<"hW3JetsShape :"<<hW3JetsShape->Integral()<<endl;

//   //return to normal
//   Isocat_=TmpIsocat;
//   extrasel_=TmpExtrasel;

//   //smear
//   TH1F * htmp  = smearHisto(hW3JetsShape);
//   delete hW3JetsShape;
//   hW3JetsShape = htmp;


//   if(hW3JetsHighMT->Integral()>0&&hW3JetsShape->Integral()>0){
//     float HighToLowRatio=hW3JetsLowMT->Integral()/hW3JetsHighMT->Integral();
//     cout<<"getW3Jets HighToLowRatio "<<HighToLowRatio<<endl;
//     hW3JetsShape->Scale((HighToLowRatio*(hDataHighMT->Integral()-hTTJetsHighMT->Integral()))/hW3JetsShape->Integral());
//   }  else {
//     cout<<" getW3Jets  Warning hW3JetsHighMT is 0 cannot divide"<<endl;
//     hW3JetsShape->Scale(0.);
//   }

//   delete hDataHighMT;
//   delete hTTJetsHighMT;
//   delete hW3JetsLowMT;
//   delete hW3JetsHighMT;

  
//   //cout<<"getW3Jets yield: "<<hW3JetsShape->Integral()<<endl;

//   return hW3JetsShape;
// }



// TH1F* TauMuPlotter::getQCDKeti(){
//   //cout<<" Calling getQCDKeti only for b-jet category "<<endl;

//   TH1F* hQCDInc=getPlotHisto("hQCDKeti");

//   Int_t ChcatTmp=Chcat_;
//   Chcat_=2;

//   TH1F* hDataSS=getTotalData();
//   hQCDInc->Add(hDataSS);
//   delete hDataSS;

//   TH1F*hZToTauTauSS=getZToTauTau();
//   if(!hZToTauTauSS)return 0;
//   hQCDInc->Add(hZToTauTauSS,-1);
//   delete hZToTauTauSS;

//   cout<<"Using getW2JetsBJetSS, should only be used for b-jet category"<<endl;
//   TH1F*hWJetsSS=getW2JetsBJet();
//   if(!hWJetsSS) return 0;
//   hQCDInc->Add(hWJetsSS,-1);
//   delete hWJetsSS;

//   TH1F*hTTJetsSS=getTTJetsInc();
//   if(!hTTJetsSS)return 0;
//   hQCDInc->Add(hTTJetsSS,-1);
//   delete hTTJetsSS;

//   TH1F*hZToLJetSS=getZToLJetInc();
//   if(!hZToLJetSS)return 0;
//   hQCDInc->Add(hZToLJetSS,-1);
//   delete hZToLJetSS;

//   TH1F*hZToMuMuSS=getZLInc();
//   if(!hZToMuMuSS)return 0;
//   hQCDInc->Add(hZToMuMuSS,-1);
//   delete hZToMuMuSS;

//   TH1F*hDiBosonSS=getDiBoson();
//   if(!hDiBosonSS)return 0;
//   hQCDInc->Add(hDiBosonSS,-1);
//   delete hDiBosonSS;

//   hQCDInc->Scale(QCDOStoSSRatio_);



//   //determine shape
//   char isocuttxtshape[100];
//   sprintf(isocuttxtshape,"(%.2f<muiso&&muiso<%.2f)",0.2,0.5);//for shape
//   cout<<"Calculating QCD Shape with Keti Method and loose iso cut: "<<isocuttxtshape<<endl;
  
//   Int_t TmpIsocat=Isocat_;
//   TString TmpExtrasel=extrasel_;  
//   Isocat_=-1;
//   extrasel_=extrasel_+"*"+isocuttxtshape;
//   TH1F* hQCDShape = getTotalData(); 

//   TH1F* hZToTauTau = getZToTauTau();
//   hQCDShape->Add(hZToTauTau,-1);
//   delete hZToTauTau;

//   TH1F*hTTJets=getTTJetsInc();
//   hQCDShape->Add(hTTJets,-1);
//   delete hTTJets;

//   TH1F*hZLJet=getZToLJetInc();
//   hQCDShape->Add(hZLJet,-1);
//   delete hZLJet;

//   //return selections
//   Isocat_=TmpIsocat;
//   extrasel_=TmpExtrasel;


//   if(hQCDShape->Integral()>0)
//     hQCDShape->Scale(hQCDInc->Integral()/hQCDShape->Integral());
//   else {
//     cout<<"hQCDShape integral is 0 cannot normalize shape "<<endl;
//     hQCDShape->Scale(0.);
//   }
//   delete hQCDInc;

  
//   Chcat_=ChcatTmp;

//   return hQCDShape;
// }




// TH1F* TauMuPlotter::getQCDMike(){

//   char isocuttxt[100];
//   //sprintf(isocuttxt,"(0.2<muiso&&muiso<%.3f&&tauisomvaraw>%.3f)",0.5,0.7);//for normalization
//   sprintf(isocuttxt,"(0.2<muiso&&muiso<%.3f)",0.5);//switch cut while selecting between mva and cut-based
//   cout<<"Calculating QCD Yield with Mike Method and loose iso cut: "<<isocuttxt<<endl;

//   char isocuttxtshape[100];
//   //sprintf(isocuttxtshape,"(0.2<muiso&&muiso<%.3f&&tauisomvaraw>%.3f)",0.5,-0.75);//for shape
//   sprintf(isocuttxtshape,"(0.2<muiso&&muiso<%.3f)",0.5);//
//   cout<<"Calculating QCD Shape with Mike Method and loose iso cut: "<<isocuttxtshape<<endl;
  
//   Int_t TmpIsocat=Isocat_;
//   TString TmpExtrasel=extrasel_;  
//   Int_t ChcatTmp=Chcat_;
//   Chcat_=2;

//   //SS incl QCD 
//   Isocat_=1;
//   extrasel_="1";
//   TH1F* hQCDInc = getQCDInc();  if(!hQCDInc){cout<<" QCDInc not determined "<<endl; return 0;}   hQCDInc->SetName("hQCDInc");
 
//   //SS Loose Incl QCD 
//   Isocat_=-1;
//   extrasel_=isocuttxt;
//   TH1F* hDataIncLoose = getTotalData();  if(!hDataIncLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataIncLoose->SetName("hDataIncLoose");
//   TH1F* hMCIncLoose = getTotalMCSM();  if(!hMCIncLoose){cout<<" Total MC not determined "<<endl; return 0;}   hMCIncLoose->SetName("hMCIncLoose");

//   //SS Loose VBF QCD 
//   Isocat_=-1;  
//   extrasel_=TmpExtrasel+"*"+isocuttxt;
//   TH1F* hDataVBFLoose=getTotalData();  if(!hDataVBFLoose){cout<<" Total Data not determined "<<endl; return 0;}   hDataVBFLoose->SetName("hDataVBFLoose");
//   TH1F* hMCVBFLoose=getTotalMCSM();   if(!hMCVBFLoose){cout<<" Total MC not determined "<<endl; return 0;}   hMCVBFLoose->SetName("hMCVBFLoose");
  

//   //SS Loose VBF QCD 
//   Isocat_=-1;  
//   extrasel_=TmpExtrasel+"*"+isocuttxtshape;
//   TH1F* hDataVBFLooseShape=getTotalData();  if(!hDataVBFLooseShape){cout<<" Total Data not determined "<<endl; return 0;}   hDataVBFLooseShape->SetName("hDataVBFLooseShape");
//   TH1F* hMCVBFLooseShape=getTotalMCSM();   if(!hMCVBFLooseShape){cout<<" Total MC not determined "<<endl; return 0;}   hMCVBFLooseShape->SetName("hMCVBFLooseShape");
  
//   TH1F* hShape=getPlotHisto("hSMQCDShape");//new TH1F("hSMQCDShape","QCD",nbins_,xmin_,xmax_);

//   hShape->Sumw2();
//   hShape->Add(hDataVBFLooseShape);
//   hShape->Add(hMCVBFLooseShape,-1.);


//   //return scale factors to normal
//   Isocat_=TmpIsocat;
//   extrasel_=TmpExtrasel;
//   Chcat_=ChcatTmp;


//   //OS VBF QCD = 1.11 * (SS VBF QCD with loose iso.) * [(SS incl QCD) / ( SS incl QCD with loose iso.)]
//   TH1F* h=getPlotHisto("hSMQCD");//new TH1F("hSMQCD","QCD",nbins_,xmin_,xmax_);
//   h->Sumw2();
//   h->Add(hDataVBFLoose);
//   h->Add(hMCVBFLoose,-1.);

  
//   //smear
//   TH1F * htmp  = smearHisto(h);
//   delete h;
//   h = htmp;

//   //scale to yield
//   cout<<" QCDInc "<<hQCDInc->Integral()<<endl;
//   cout<<" DataVBFLoose "<<hDataVBFLoose->Integral()<<endl;
//   cout<<" MCVBFLoose "<<hMCVBFLoose->Integral()<<endl;
//   cout<<" DataIncLoose "<<hDataIncLoose->Integral()<<endl;
//   cout<<" MCIncLoose "<<hMCIncLoose->Integral()<<endl;
//   float ratio=hQCDInc->Integral()/(hDataIncLoose->Integral()-hMCIncLoose->Integral());
//   //float ratio=(hQCDInc->Integral()/hDataIncLoose->Integral());
//   cout<<" Scaling Incl QCD by "<<ratio<<endl;
//   h->Scale(ratio);
//   cout<<"QCDMike "<<h->Integral()<<endl;
//   hShape->Scale(h->Integral()/hShape->Integral());


//   TString filetag=TString("FQCDMike_")+plotvar_+"_TauIso"+(long)Isocat_+"_MT"+(long)MTcat_+extrasel_+isocuttxt;
//   fixFileTag(&filetag);
//   TFile FQCD(outputpath_+"/"+filetag+".root","recreate");
//   h->Write();
//   hShape->Write();
//   hDataIncLoose->Write();
//   hMCIncLoose->Write();
//   hDataVBFLoose->Write();
//   hMCVBFLoose->Write();
//   FQCD.ls();
//   FQCD.Close();

//   delete hDataIncLoose;
//   delete hMCIncLoose;
//   delete hDataVBFLoose;
//   delete hMCVBFLoose;
//   delete h;

//   return hShape;
// }





// TH1F* TauMuPlotter::getQCDIsoSM(){
   

//   TH1F* h=getPlotHisto("hQCDSM");//new TH1F("hQCD","SMQCD",nbins_,xmin_,xmax_);

  
//   Int_t TmpIsocat=Isocat_;  
//   Isocat_=-1;  
//   TString tmpextrasel=extrasel_;
  
//   TString muisocut="(muiso<0.1)";
//   TString tauisocut=tauIsoCut_;
//   TString muantiisocut="(muiso>0.1)";
//   TString tauantiisocut="(!"+tauIsoCut_+")";



// //   //QCD From Region B
//   TH1F* hB=getPlotHisto("hSMQCDB");//new TH1F("hSMQCDB","SMQCDB",nbins_,xmin_,xmax_);
// //   hB->Sumw2();
// //   cout<<"Applying Tau Iso QCD ratio : "<<qcdTauIsoRatio_<<endl;
// //   extrasel_ = extrasel_+ "*"+muisocut+"*"+tauantiisocut+"*"+qcdTauIsoRatio_;
// //   if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
// //   if(!correctSamplesInc())return 0;
// //   TH1F* hDataB=getTotalData();  if(!hDataB){cout<<" Total Data not determined "<<endl; return 0;}   hDataB->SetName("hDataB");
// //   TH1F* hMCB=getTotalMCSM();   if(!hMCB){cout<<" Total Data not determined "<<endl; return 0;}   hMCB->SetName("hMCB");
// //   extrasel_ = tmpextrasel;
// //   hB->Add(hDataB,1.);
// //   hB->Add(hMCB,-1.);
// //   cout<<"QCD Region B "<<hDataB->Integral()<<" - "<<hMCB->Integral()<<endl;


// //   //QCD From Region C
//   TH1F* hC=getPlotHisto("hSMQCDC");//new TH1F("hSMQCC","SMQCC",nbins_,xmin_,xmax_);
// //   hC->Sumw2();
// //   cout<<"Applying Mu Iso QCD ratio : "<<qcdMuIsoRatio_<<endl;
// //   extrasel_ = extrasel_+"*"+muantiisocut+"*"+tauisocut+"*"+qcdMuIsoRatio_;
// //   if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
// //   if(!correctSamplesInc())return 0;
// //   TH1F* hDataC=getTotalData();  if(!hDataC){cout<<" Total Data not determined "<<endl; return 0;}   hDataC->SetName("hDataC");
// //   TH1F* hMCC=getTotalMCSM();   if(!hMCC){cout<<" Total Data not determined "<<endl; return 0;}   hMCC->SetName("hMCC");
// //   extrasel_ = tmpextrasel;
// //   hC->Add(hDataC,1.);
// //   hC->Add(hMCC,-1.);
// //   Float_t Cerr=0.;
// //   for(Int_t b=1;b<=nbins_;b++) Cerr+=hC->GetBinError(b)*hC->GetBinError(b);
// //   cout<<"QCD Region C "<<hDataC->Integral()<<" - "<<hMCC->Integral()<<endl;



//   //QCD From Region D
//   TH1F* hD=getPlotHisto("hSMQCDD");//new TH1F("hSMQCD","SMQCD",nbins_,xmin_,xmax_);
//   cout<<"Applying Tau-Mu Iso QCD ratio : "<<qcdTauIsoRatio_+"*"+qcdMuIsoRatio_<<endl;
//   extrasel_ = extrasel_+ "*"+muantiisocut+"*"+tauantiisocut+"*"+qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;
//   if(!scaleSamplesLumi())return 0;//need to recalculate WJets correction factors because isolation is different
//   TH1F* hDataD=getTotalData();  if(!hDataD){cout<<" Total Data not determined "<<endl; return 0;}   hDataD->SetName("hDataD");
//   TH1F* hMCD=getTotalMCSM();   if(!hMCD){cout<<" Total Data not determined "<<endl; return 0;}   hMCD->SetName("hMCD");
//   extrasel_ = tmpextrasel;
//   hD->Add(hDataD,1.);
//   hD->Add(hMCD,-1.);
//   cout<<"QCD Region D "<<hDataD->Integral()<<" - "<<hMCD->Integral()<<endl;


//   //////Determine yield for each region with its error
//   Float_t QCDYieldB=hB->Integral();
//   Float_t QCDerrB=0.;
//   Float_t QCDYieldC=hC->Integral();
//   Float_t QCDerrC=0.;
//   Float_t QCDYieldD=hD->Integral();
//   Float_t QCDerrD=0.;
//   for(Int_t b=1;b<=nbins_;b++){
//     QCDerrB+=hB->GetBinError(b)*hB->GetBinError(b);
//     QCDerrC+=hC->GetBinError(b)*hC->GetBinError(b);
//     QCDerrD+=hD->GetBinError(b)*hD->GetBinError(b);
//   }


//   cout<<"summary of QCD estimates:"<<endl;
//   cout<<"QCD Region B "<<QCDYieldB<<" +- "<<sqrt(QCDerrB)<<endl;
//   cout<<"QCD Region C "<<QCDYieldC<<" +- "<<sqrt(QCDerrC)<<endl;
//   cout<<"QCD Region D "<<QCDYieldD<<" +- "<<sqrt(QCDerrD)<<endl;

// //   //for the average make an error weighted average
// //   Float_t wsum=(1./QCDerrB+1./QCDerrC+1./QCDerrD);
// //   Float_t QCDYield=(QCDYieldB/QCDerrB+QCDYieldC/QCDerrC+QCDYieldD/QCDerrD)/wsum;
// //   //Float_t QCDerr=(1./sqrt(QCDerrB)+1./sqrt(QCDerrC)+1./sqrt(QCDerrD))/(1./QCDerrB+1./QCDerrC+1./QCDerrD);
// //   Float_t QCDerr=sqrt((QCDerrB*pow((1./QCDerrB)/wsum,2)+QCDerrC*pow((1./QCDerrC)/wsum,2)+QCDerrD*pow((1./QCDerrD)/wsum,2)));

//   //for the average make an error weighted average
//   Float_t QCDYield=QCDYieldD;
//   Float_t QCDerr=QCDerrD;
  
//   //   //find the maximum deviation from the mean for systematic estimate:
//   float SystErr=0.;
//   //   if(fabs(QCDYield-QCDYieldB)>SystErr)SystErr=fabs(QCDYield-QCDYieldB);
//   //   if(fabs(QCDYield-QCDYieldC)>SystErr)SystErr=fabs(QCDYield-QCDYieldC);
//   //   if(fabs(QCDYield-QCDYieldD)>SystErr)SystErr=fabs(QCDYield-QCDYieldD);
//   cout<<"QCDIso: "<<QCDYield<<" +- "<<QCDerr<<" +- "<<SystErr<<endl;

//   //////////////////////////////////Shape///////////////////////////////////

//   /////Shape taken from D region with fake rate applied
//   //   h->Add(hD);
//   //   //fix any negative bins
//   //   for(Int_t b=1;b<=nbins_;b++) 
//   //     if(h->GetBinContent(b)<0.){
//   //       h->SetBinContent(b,0); h->SetBinError(b,0);
//   //     }



//   //QCD Shape from Region D
//   cout<<" QCD shape from region D"<<endl;
//   extrasel_ = extrasel_+ "*"+muantiisocut+"*"+tauantiisocut;
//   if(!scaleSamplesLumi())return 0; //need to recalculate WJets correction factors because isolation is different
//   TH1F* hDataShape=getTotalData();  if(!hDataShape){cout<<" Total Data not determined "<<endl; return 0;}   hDataShape->SetName("hDataShape");
//   TH1F* hMCShape=getTotalMCSM(); if(!hMCShape){cout<<" Total Data not determined "<<endl; return 0;}   hMCShape->SetName("hMCShape");
//   h->Add(hDataShape); delete hDataShape;
//   h->Add(hMCShape,-1.); delete hMCShape;
//   extrasel_ = tmpextrasel;
//   //////////////////////////////////////////////////////////////////////////



//   //Apply smearing here, this may change the integral
//   cout<<"Applying smearing "<<endl;
//   TH1F * hsmeared = smearHisto(h);
//   delete h;
//   h=hsmeared;

//   //Normalize to the yield
//   h->Scale(QCDYield/h->Integral());
  
//   //Save the intermediate histograms in a root file
//   TString filetag = TString("FQCDIsoSM_")+plotvar_+"_MT"+(long)MTcat_+"_"+extrasel_;
//   fixFileTag(&filetag);
//   TFile FQCDIsoSM(outputpath_+"/"+filetag+".root","recreate");
// //   hDataB->Write(); delete hDataB;
// //   hMCB->Write();   delete hMCB;
// //   hDataC->Write(); delete hDataC;
// //   hMCC->Write();   delete hMCC;
//   hDataD->Write(); delete hDataD;
//   hMCD->Write();   delete hMCD;
//   h->Write();
//   FQCDIsoSM.ls();
//   FQCDIsoSM.Close();
  

//   delete hB;
//   delete hC;
//   delete hD;

//   /////////return to normal
//   Isocat_=TmpIsocat;
//   extrasel_=tmpextrasel;
//   if(!scaleSamplesLumi())return 0;

  
//   return h;

// }




// TH1F* TauMuPlotter::getWJetsTauIsoSM(){
  
//   //1.normalize WJets MC Anti-Iso sample at high MT 
//   //2.apply ratio to predict the yield in the Iso sample

//   TString tmpextrasel=extrasel_;
//   Int_t TmpIsocat=Isocat_;  

//   //switch temporarily while keeping all other categories the same  
//   Isocat_=2;
//   extrasel_ += "*(muiso<0.1)";//
//   if(!scaleSamplesLumi())return 0;

//   //get the shape from the anti-iso sample without the fake rate:
//   cout<<"WJets shape obtained from anti-iso sample without fake rate"<<endl;
//   TH1F* hShape=getWJetsInc();
//   TH1F * h = smearHisto(hShape);
//   delete hShape;

//   //get the yield by applying the fake rate
//   cout<<"Applying Tau Iso WJets ratio : "<<wjetsTauIsoRatio_<<endl;
//   extrasel_ = extrasel_+ "*"+wjetsTauIsoRatio_;//
//   TH1F* hW=getWJetsInc();
//   h->Scale(hW->Integral()/h->Integral());
//   delete hW;

//   //return to normal
//   extrasel_=tmpextrasel;
//   Isocat_=TmpIsocat;
//   if(!scaleSamplesLumi())return 0;

//   float yielder=0.;
//   for(Int_t b=1;b<=h->GetNbinsX();b++){
//     yielder+=h->GetBinError(b)*h->GetBinError(b);
//   }

//   //  cout<<"WJetsTauIso : "<<h->Integral()<<" +- "<<sqrt(yielder)<<endl;
//   return h;

// }

void TauMuPlotter::plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, TString IsoSel, Float_t ymax,Bool_t log){
  
  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Isocat_=-1;
  Chcat_=2;
  if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;//something like (njet==0)
  if(!scaleSamplesLumi())return;

  TString filetag=plotvar_+extrasel_;
  fixFileTag(&filetag);

 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauMuPlotterIsoFakeRate_"+filetag;
  C.Print(filename+".ps[");


  /////////Anti-Isolated taus
  TString tmpextrasel=extrasel_;
  extrasel_=extrasel_+"*(!"+IsoSel+")";

  TH1F* hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  TH1F* hMC=getTotalMCSM();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}  
  extrasel_=tmpextrasel;

  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hA=getPlotHisto("hDiffAnti");//new TH1F("hDiffAnti","",nbins_,xmin_,xmax_);
  hA->Add(hData);
  hA->Add(hMC,-1);
  delete hData; delete hMC;


  /////////////Isolated taus
  extrasel_=extrasel_+"*"+IsoSel;
  hData=getTotalData();
  if(!hData){cout<<" Total Data not determined "<<endl; return;}
  hMC=getTotalMCSM();
  if(!hMC){cout<<" Total MC not determined "<<endl; return;}

  C.Clear();
  hData->Draw("pe");
  hMC->Draw("hist same");
  C.Print(filename+".ps");

  TH1F* hI=getPlotHisto("hDiffIso");//new TH1F("hDiffIso","",nbins_,xmin_,xmax_);
  hI->Add(hData);
  hI->Add(hMC,-1);
  delete hData; delete hMC;
 
  C.Clear();
  C.SetLogy(1);
  hA->Draw("pe");
  hI->Draw("pesame");
  C.Print(filename+".ps");
  C.SetLogy(0);

  //average ratio
  float r=hI->Integral()/hA->Integral();
  float re=r*sqrt(1./hI->Integral() + 1./hA->Integral());
  cout<<" Ratio = "<<r<<" +- "<<re<<endl;


  ////Calculate the ratio as a function of the variable
  TH1F* hR=getPlotHisto("hRatio");//new TH1F("hRatio","",nbins_,xmin_,xmax_);
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
  C.Print(filename+".ps");


  cout.precision(3);
  cout<<"(";
  Int_t b=1;
  while(hR->GetBinContent(b)>0&&b<=hR->GetNbinsX()){
    cout<<"("<<hR->GetBinLowEdge(b)<<"<"<<plotvar_<<"&&"<<plotvar_<<"<="<<hR->GetBinLowEdge(b)+hR->GetBinWidth(b)<<")*";
    cout<<hR->GetBinContent(b)<<"+";
    b++;
  }
  cout<<")"<<endl;  


  ///save the ratio histo to a root file:
  TFile FRatio(filename+".root","recreate");
  hR->Write();
  FRatio.ls();
  FRatio.Close();
  

  delete hR;

  C.Print(filename+".ps]");

}


void TauMuPlotter::plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel, Float_t ymax,Bool_t log){
  
  //Ratio of Isolated/Anti-Isolated taus using the the high mT region
  //-Should be computed at inclusive level
  //-Should be computed with OS samples only (SS is different)

  plotvar_=variable;
  nbins_=nbins;
  xmin_=xmin;
  xmax_=xmax;
  Chcat_=1;
  MTcat_=-1;//dont use this because need to make sure it refers to the proper MET 
  if(extrasel!="")extrasel_=extrasel_+"*"+extrasel;

  TString filetag=plotvar_+extrasel_;
  fixFileTag(&filetag);

 
  TCanvas C("C");
  TString filename = outputpath_ + "/TauMuPlotterTauFakeRateWJets_"+filetag;
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


void TauMuPlotter::plotQCDSSOSRatio(){

  plotvar_="muiso";
  nbins_=7;
  xmin_=0;
  xmax_=0.7;
  Isocat_=-1;
  MTcat_=-1;
  Chcat_=2;
  extrasel_="(muiso>0.2)";//&&taudecaymode==0
  //extrasel_="(muiso>0.2&&tauisomvaraw>-1.0&&njet==1)";
  scaleSamplesLumi();

  TCanvas C("plotQCDSSOSRatio");

  C.Print("TauMuPlotter_plotQCDSSOSRatio.ps[");
  

  TH1F* hDataSS=getTotalData();
  hDataSS->SetTitle("");
  hDataSS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataSS->GetXaxis()->SetTitle("muon rel. isolation ");


  TH1F* HMCSS=getZToTauTau(); 
  HMCSS->SetTitle("HMCSS");
  TH1F* HTTSS=getTTJetsInc(); 
  HMCSS->Add(HTTSS); delete HTTSS;
  TH1F* HWSS=getWJetsNJet(); 
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
  hDataOS->GetXaxis()->SetTitle("muon rel. isolation ");
  
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
  hDataOS->GetXaxis()->SetTitle("muon rel. isolation ");
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



void TauMuPlotter::plotQCDSSOSRatioBTag(){

  plotvar_="muiso";
  nbins_=7;
  xmin_=0;
  xmax_=0.7;
  Isocat_=-1;
  MTcat_=1;
  Chcat_=2;
  extrasel_="(nbjetLoose>=1&&muiso>0.2)*"+tauIsoCut_;//&&tauisomvaraw>-1.0 
  //extrasel_="(muiso>0.2&&tauisomvaraw>-1.0&&njet==1)";
  scaleSamplesLumi();

  TCanvas C("plotQCDSSOSRatioBTag");

  C.Print("plotQCDSSOSRatioBTag.ps[");
  

  TH1F* hDataSS=getTotalData();
  hDataSS->SetTitle("");
  hDataSS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataSS->GetXaxis()->SetTitle("muon rel. isolation ");


  TH1F* HMCSS=getZToTauTau(); 
  HMCSS->SetTitle("HMCSS");
  cout<<"ZTT SS: "<<HMCSS->Integral()<<endl;
  TH1F* HTTSS=getTTJetsInc(); 
  HMCSS->Add(HTTSS);
  cout<<"TT SS: "<<HTTSS->Integral()<<endl;
  delete HTTSS;
  TH1F* HWSS=getWJetsInc(); 
  HMCSS->Add(HWSS);
  cout<<"W SS: "<<HWSS->Integral()<<endl;
  delete HWSS;
  
  C.Clear();
  hDataSS->Draw("histpe");
  HMCSS->Draw("histsame");
  C.Print("plotQCDSSOSRatioBTag.ps");
  
  hDataSS->Add(HMCSS,-1);
  delete HMCSS;

  Chcat_=1;
  ////////////////////////////
  TH1F* hDataOS=getTotalData();
  hDataOS->SetTitle("");
  hDataOS->GetYaxis()->SetTitle("Events / 0.1 ");
  hDataOS->GetXaxis()->SetTitle("muon rel. isolation ");
  
  TH1F* HMC=getZToTauTau(); 
  HMC->SetTitle("HMC");
  cout<<"ZTT OS: "<<HMC->Integral()<<endl;
  TH1F* HTT=getTTJetsInc(); 
  HMC->Add(HTT);
  cout<<"TT OS: "<<HTT->Integral()<<endl;
  delete HTT;
  TH1F* HW=getWJetsInc(); 
  HMC->Add(HW);
  cout<<"W OS: "<<HW->Integral()<<endl;
  delete HW;

  C.Clear();
  hDataOS->Draw("histpe");
  HMC->Draw("histsame");
  C.Print("plotQCDSSOSRatioBTag.ps");

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
  hDataOS->GetXaxis()->SetTitle("muon rel. isolation ");
  hDataOS->Draw("hist");
  hDataSS->Draw("histpesame");
  legend.Draw();
  C.Print("plotQCDSSOSRatioBTag.ps");

  //plot ratio
  hDataOS->Divide(hDataSS);
  C.Clear();
  hDataOS->GetYaxis()->SetRangeUser(0,1.2);
  hDataOS->GetYaxis()->SetTitle("OS / SS ");
  hDataOS->Draw("histpe");
  TLine line;
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print("plotQCDSSOSRatioBTag.ps");

  //Fit to ratio
  //TF1 F1("F1","[0]+[1]*x",xmin_,xmax_);
  TF1 F1("F1","[0]",xmin_,xmax_);
  F1.SetLineColor(2);
  hDataOS->Fit(&F1,"","",xmin_,xmax_);
  C.Clear();
  hDataOS->Draw("histpe");
  F1.Draw("same");
  line.DrawLine(xmin_,1,xmax_,1);
  char fitres[100];
//  sprintf(fitres,"(%.2f +- %.2f) - (%.2f +- %.2f)*x",F1.GetParameter(0),F1.GetParError(0),fabs(F1.GetParameter(1)),F1.GetParError(1));
  sprintf(fitres,"(%.3f +- %.3f)",F1.GetParameter(0),F1.GetParError(0));
  TLatex text; text.SetTextSize(0.05); text.SetTextColor(2);
  text.DrawTextNDC(.4,.5,fitres);
  C.Print("plotQCDSSOSRatioBTag.ps");

  C.Print("plotQCDSSOSRatioBTag.ps]");

  delete hDataSS;
  delete hDataOS;
}



void TauMuPlotter::compareZTTEmbedded(){

  //plotvar_="ditaumass";
  plotvar_="svfitmass";
  nbins_=30;
  xmin_=0;
  xmax_=150;
  Chcat_=1;
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
  //HMC->GetXaxis()->SetTitle("m(#mu#tau)");
  HMC->GetXaxis()->SetTitle("m(#tau#tau)");
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
    //HMCcat->GetXaxis()->SetTitle("m(#mu#tau)");
    HMCcat->GetXaxis()->SetTitle("m(#tau#tau)");
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

  extrasel_="(njet>=1)";
  ZTTType_=1;
  TH1F*HMCcat1Jet=getZToTauTau();
  ZTTType_=2;
  TH1F*HDatacat1Jet=getZToTauTau();
  C.Clear();
  HMCcat1Jet->SetTitle("");
  HMCcat1Jet->GetXaxis()->SetTitle("m(#tau#tau)");
  HMCcat1Jet->GetYaxis()->SetTitle("njet>=1");
  HMCcat1Jet->SetMarkerColor(4);
  HMCcat1Jet->SetLineColor(4);
  HMCcat1Jet->Draw("histpe");
  HDatacat1Jet->Draw("histpesame");
  title.SetTextColor(4);
  sprintf(text,"MC (histogram)        : integral = %.0f  mean = %.2f   rms = %.2f",HMCcat1Jet->Integral(),HMCcat1Jet->GetMean(),HMCcat1Jet->GetRMS());
  title.DrawTextNDC(0.2,0.96,text);
  title.SetTextColor(1);
  sprintf(text,"Embedded (points)  : integral = %.0f  mean = %.2f   rms = %.2f",HDatacat1Jet->Integral(),HDatacat1Jet->GetMean(),HDatacat1Jet->GetRMS());
  title.DrawTextNDC(0.2,0.93,text);
  C.Print("compareZTTEmbedded.pdf");
      
  delete HMCcat1Jet;
  delete HDatacat1Jet;


  extrasel_="(njet>=2)";
  ZTTType_=1;
  TH1F*HMCcat2Jet=getZToTauTau();
  ZTTType_=2;
  TH1F*HDatacat2Jet=getZToTauTau();
  C.Clear();
  HMCcat2Jet->SetTitle("");
  HMCcat2Jet->GetXaxis()->SetTitle("m(#tau#tau)");
  HMCcat2Jet->GetYaxis()->SetTitle("njet>=2");
  HMCcat2Jet->SetMarkerColor(4);
  HMCcat2Jet->SetLineColor(4);
  HMCcat2Jet->Draw("histpe");
  HDatacat2Jet->Draw("histpesame");
  title.SetTextColor(4);
  sprintf(text,"MC (histogram)        : integral = %.0f  mean = %.2f   rms = %.2f",HMCcat2Jet->Integral(),HMCcat2Jet->GetMean(),HMCcat2Jet->GetRMS());
  title.DrawTextNDC(0.2,0.96,text);
  title.SetTextColor(1);
  sprintf(text,"Embedded (points)  : integral = %.0f  mean = %.2f   rms = %.2f",HDatacat2Jet->Integral(),HDatacat2Jet->GetMean(),HDatacat2Jet->GetRMS());
  title.DrawTextNDC(0.2,0.93,text);
  C.Print("compareZTTEmbedded.pdf");
      
  delete HMCcat2Jet;
  delete HDatacat2Jet;



  C.Print("compareZTTEmbedded.pdf]");

}



void TauMuPlotter::plotZTTShape(){

  plotvar_="svfitmass";
  nbins_=50;
  xmin_=50;
  xmax_=200;
  Chcat_=1;
  Isocat_=1;
  MTcat_=1;
  ZTTType_=2;


  scaleSamplesLumi();

  TCanvas C("plotZTTShape");
  C.Print("plotZTTShape.ps[");
  
  extrasel_="(njet==0&&nbjet==0&&taupt>40)";
  TH1F*HZTT=getZToTauTau(); HZTT->SetName("HZTT");
  extrasel_="(njet==0&&nbjet==0&&taupt>40)*(1-0.2*(taudecaymode==0))";
  TH1F*HZTTReweight=getZToTauTau(); HZTTReweight->SetName("HZTTReweight");
  C.Clear();
  HZTTReweight->SetMarkerColor(4);
  HZTTReweight->SetLineColor(4);
  HZTTReweight->SetTitle("");
  HZTT->GetXaxis()->SetTitle("m(#tau#tau)");
  HZTT->SetTitle("ZTT Embedded");
  HZTT->Draw("hist");
  HZTTReweight->Draw("histpesame");
  C.Print("plotZTTShape.ps");

  ///////////////Ratio to Nominal
  TH1F*HRatio=(TH1F*)HZTT->Clone("HRatio");
  HRatio->Divide(HZTTReweight);
  C.Clear();
  HRatio->SetTitle("");
  HRatio->GetYaxis()->SetTitle("Uncorrected ZTT / Corrected ZTT ");
  HRatio->GetYaxis()->SetRangeUser(0.90,1.10);
  TF1 Fpol("Fpol","[0]",xmin_,xmax_);
  HRatio->Fit(&Fpol);
  HRatio->Draw("histp");
  Fpol.Draw("same");
  TLine line;
  line.DrawLine(xmin_,1,xmax_,1);
  C.Print("plotZTTShape.ps");


  /////////Diff to Nominal
  TH1F*HDiff=(TH1F*)HZTT->Clone("HDiff");
  HDiff->Add(HZTTReweight,-1);
//   C.Clear();
//   HDiff->Draw("histp");
//   C.Print("plotZTTShape.ps");



  ///Compare to the Higgs signal
  TH1F*HHiggs=getSample("HiggsVBF125"); HHiggs->SetName("HHiggs");
  TH1F*hgg=getSample("HiggsGG125");HHiggs->Add(hgg); delete hgg;
  TH1F*hvh=getSample("HiggsVB125");HHiggs->Add(hvh); delete hvh;
  C.Clear();
  HHiggs->SetMarkerColor(2);
  HHiggs->SetLineColor(2);
  HHiggs->SetTitle("");
  HHiggs->GetXaxis()->SetTitle("m(#tau#tau)");
  HDiff->Draw("hist");
  HHiggs->Draw("histsame");
  C.Print("plotZTTShape.ps");



  delete HZTT;  
  delete HZTTReweight;  
 //  delete HZTT1P0Pi0;
//   delete HZTT1P1Pi0;
//   delete HZTT3P0Pi0;
  delete HRatio;
  delete HDiff;
  C.Print("plotZTTShape.ps]");


//   extrasel_="(njet==0&&nbjet==0&&taupt>40&&taudecaymode==0)";
//   TH1F*HZTT1P0Pi0=getZToTauTau(); HZTT1P0Pi0->SetName("HZTT1P0Pi0");
//   extrasel_="(njet==0&&nbjet==0&&taupt>40&&taudecaymode==1)";
//   TH1F*HZTT1P1Pi0=getZToTauTau(); HZTT1P1Pi0->SetName("HZTT1P1Pi0");
//   extrasel_="(njet==0&&nbjet==0&&taupt>40&&taudecaymode==10)";
//   TH1F*HZTT3P0Pi0=getZToTauTau(); HZTT3P0Pi0->SetName("HZTT3P0Pi0");


//   //normalize to nominal weights 
//   TH1F*HNominal=(TH1F*)HZTT1P0Pi0->Clone("HNominal");
//   HNominal->Clear();
//   HNominal->Add(HZTT1P0Pi0);
//   HNominal->Add(HZTT1P1Pi0);
//   HNominal->Add(HZTT3P0Pi0);

//   //reweight the components
//   TH1F*HReweighted=(TH1F*)HZTT1P0Pi0->Clone("HReweighted");
//   HReweighted->Clear();
//   HReweighted->Add(HZTT1P0Pi0,0.8);
//   HReweighted->Add(HZTT1P1Pi0,1.0);
//   HReweighted->Add(HZTT3P0Pi0,1.0);
//   C.Clear();
//   HNominal->Draw("hist");
//   HReweighted->Draw("histsame");
//   C.Print("plotZTTShape.ps");


}




void TauMuPlotter::compareZTTEmbeddedUnfolding(){


  scaleSamplesLumi();

  TCanvas C("compareZTTEmbeddedUnfolding");
  C.Print("compareZTTEmbeddedUnfolding.ps[");

  TLine line;

  ///plot the difference in eta
  plotvar_="mueta";
  nbins_=20;
  xmin_=-2.1;
  xmax_=2.1;
  Chcat_=1;
  Isocat_=1;
  MTcat_=1;
  extrasel_="1";
  ZTTType_=1;
  TH1F*HMCEta=getZToTauTau();
  ZTTType_=2;
  TH1F*HDataEta=getZToTauTau();
  C.Clear();
  HMCEta->SetMarkerColor(4);
  HMCEta->SetLineColor(4);
  HMCEta->SetTitle("");
  HMCEta->GetXaxis()->SetTitle("muon eta");
  HMCEta->GetYaxis()->SetTitle("");
  HMCEta->Draw("histpe");
  HDataEta->Draw("histpesame");
  C.Print("compareZTTEmbeddedUnfolding.ps");

  C.Clear();
  HDataEta->Divide(HMCEta);
  HDataEta->GetYaxis()->SetRangeUser(0,1.2);
  HDataEta->GetYaxis()->SetTitle("MC/Embedded");
  HDataEta->SetTitle("");
  HDataEta->Draw("histpe");
  line.DrawLine(-2.1,1,2.1,1);
  C.Print("compareZTTEmbeddedUnfolding.ps");

  //print out the reweighting string:
  for(Int_t b=1;b<=HDataEta->GetNbinsX()/2;b++){
    cout<<"("<<fabs(HDataEta->GetBinLowEdge(b))<<">mueta&&mueta>"<<fabs(HDataEta->GetBinLowEdge(b)+HDataEta->GetBinWidth(b))<<")*"<<(HDataEta->GetBinContent(b)+HDataEta->GetBinContent(HDataEta->GetNbinsX()+1-b))/2<<"+";
  }
  cout<<endl;

  delete HMCEta;
  delete HDataEta;

  ///plot the difference in pT
  plotvar_="mupt";
  nbins_=15;
  xmin_=20;
  xmax_=50;
  Chcat_=1;
  Isocat_=1;
  MTcat_=1;
  extrasel_="1";
  ZTTType_=1;
  TH1F*HMCPt=getZToTauTau();
  ZTTType_=2;
  TH1F*HDataPt=getZToTauTau();
  C.Clear();
  HMCPt->SetMarkerColor(4);
  HMCPt->SetLineColor(4);
  HMCPt->SetTitle("");
  HMCPt->GetXaxis()->SetTitle("muon pT");
  HMCPt->GetYaxis()->SetTitle("");
  HMCPt->SetTitle("");
  HMCPt->Draw("histpe");
  HDataPt->Draw("histpesame");
  C.Print("compareZTTEmbeddedUnfolding.ps");

  C.Clear();
  HDataPt->Divide(HMCPt);
  HDataPt->GetYaxis()->SetRangeUser(0.,1.2);
  HDataPt->GetYaxis()->SetTitle("MC/Embedded");
  HDataPt->Draw("histpe");
  line.DrawLine(20,1,50,1);
  C.Print("compareZTTEmbeddedUnfolding.ps");
  delete HMCPt;
  delete HDataPt;


  ////////////Now apply the reweighting to the embedded and check the difference inthe mass
  TString weight="((2.1>abs(mueta)&&abs(mueta)>1.89)*1.14967+(1.89>abs(mueta)&&abs(mueta)>1.68)*1.00897+(1.68>abs(mueta)&&abs(mueta)>1.47)*0.956634+(1.47>abs(mueta)&&abs(mueta)>1.26)*0.933614+(1.26>abs(mueta)&&abs(mueta)>1.05)*1.06881+(1.05>abs(mueta)&&abs(mueta)>0.84)*1.01936+(0.84>abs(mueta)&&abs(mueta)>0.63)*0.98277+(0.63>abs(mueta)&&abs(mueta)>0.42)*0.990675+(0.42>abs(mueta)&&abs(mueta)>0.21)*0.935568+(0.21>abs(mueta)&&abs(mueta)>0)*0.953869)";

  ///plot the difference in mass
  plotvar_="svfitmass";
  nbins_=25;
  xmin_=0;
  xmax_=250;
  Chcat_=1;
  Isocat_=1;
  MTcat_=1;
  ZTTType_=2;
  extrasel_="1";
  TH1F*HData=getZToTauTau();
  extrasel_=weight;
  TH1F*HDataWeight=getZToTauTau();
  C.Clear();
  HData->SetMarkerColor(4);
  HData->SetLineColor(4);
  HData->SetTitle("");
  HData->GetXaxis()->SetTitle("m(#tau#tau)");
  HData->GetYaxis()->SetTitle("");
  HData->Draw("hist");
  HDataWeight->Scale(HData->Integral()/HDataWeight->Integral());
  HDataWeight->Draw("histpesame");
  C.Print("compareZTTEmbeddedUnfolding.ps");

  HDataWeight->Divide(HData);
  C.Clear();
  HDataWeight->SetTitle("");
  HDataWeight->GetYaxis()->SetTitle("");
  HDataWeight->GetYaxis()->SetRangeUser(0.9,1.1);
  HDataWeight->Draw("histp");
  line.DrawLine(0,1,250,1);
  C.Print("compareZTTEmbeddedUnfolding.ps");

  delete HDataWeight;
  delete HData;
  

  C.Print("compareZTTEmbeddedUnfolding.ps]");

}

TH1F* TauMuPlotter::computeTrigEff(TH1F* HPass, TH1F* HFail){
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

void  TauMuPlotter::plotTauTrigger(Int_t Region, TString tag){
  scaleSamplesLumi();

  ///plot the difference in eta
  plotvar_="taupt";
  nbins_=30;
  xmin_=0;
  xmax_=60;
  MTcat_=1;
  Chcat_=1;
  Isocat_=1;
  extrasel_="1";

  Float_t xbinsValues[11]={18,20,22,24,26,30,34,38,42,50,60};
  setVariableBinning(10,xbinsValues);
//   Float_t xbinsValues[15]={18,20,22,24,26,30,34,38,42,50,60,80,100,150,200};//Simones high Tau pT request
//   setVariableBinning(14,xbinsValues);
  nbins_=0;


  
  TString selectionTrigPass="(trigTest1==1||trigTest2==1||trigTest3==1||trigTest4==1||trigTest5==1||trigTest6==1||trigTest7==1||trigTest8==1||trigTest9==1)";
  TString selectionTrigFail="(!"+selectionTrigPass+")";

  TString selection="(abs(taueta)<2.3)";//combined
  if(Region==1) selection="(abs(taueta)<1.5)";
  if(Region==2) selection="(abs(taueta)>1.5)";

  TString region;
  if(Region==1) region="Barrel";
  if(Region==2) region="EndCap";


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
  
  extrasel_=selection+"*"+selectionTrigPass;
  TH1F*HTAUPTTrigPass=getTotalData();  HTAUPTTrigPass->SetName("HTAUPTTrigPass");
  TH1F*HMCTAUPTTrigPass=getPlotHisto("HMCTAUPTPass");
  TH1F*HMCZTTTrigPass=getZToTauTau();          HMCTAUPTTrigPass->Add(HMCZTTTrigPass);                delete HMCZTTTrigPass; 
  TH1F*HMCWTrigPass=getWNJetSumNoChNoMTCut();  
  HMCTAUPTTrigPass->Add(HMCWTrigPass,FakesScaleFactor*HMCW->Integral()/HMCWRaw->Integral());         delete HMCWTrigPass; 
  
  extrasel_=selection+"*"+selectionTrigFail;
  TH1F*HTAUPTTrigFail=getTotalData();  HTAUPTTrigFail->SetName("HTAUPTTrigFail");
  TH1F*HMCTAUPTTrigFail=getPlotHisto("HMCTAUPTFail");
  TH1F*HMCZTTTrigFail=getZToTauTau();         HMCTAUPTTrigFail->Add(HMCZTTTrigFail);                delete HMCZTTTrigFail; 
  TH1F*HMCWTrigFail=getWNJetSumNoChNoMTCut(); 
  HMCTAUPTTrigFail->Add(HMCWTrigFail,FakesScaleFactor*HMCW->Integral()/HMCWRaw->Integral());        delete HMCWTrigFail; 

 
 
  TCanvas C;
  TString plotFileName=TString("TauTriggerEfficiency_muTau_")+region+"_"+tag;
  C.Print(plotFileName+".ps[");

  ////Compare the Pass distribution to the total
  C.Clear();
  HTAUPT->SetTitle("Data");
  HTAUPT->GetXaxis()->SetTitle("Tau p_{T}");
  HTAUPT->GetYaxis()->SetRangeUser(0,HTAUPT->GetMaximum()*1.3);
  HTAUPT->Draw("hist");
  HTAUPTTrigPass->Draw("histpesame");
  C.Print(plotFileName+".ps");

  C.Clear();
  HMCTAUPT->SetTitle("MC");
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
  HTAUPTTrigEff->GetYaxis()->SetRangeUser(0.5,1);
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
//   for(Int_t p=0;p<100;p++){
//     //float x=17+p;
//     //HTAUPTTrigEff_Josh.SetPoint(p,x,triggerEff_.effTau_muTau_Data_2012ABCD(x,(Region==1)*0.0+(Region==2)*2.0));
//     //HMCTAUPTTrigEff_Josh.SetPoint(p,x,triggerEff_.effTau_muTau_MC_2012ABCD(x,(Region==1)*0.0+(Region==2)*2.0));
//   }

//   C.Clear();
//   HTAUPTTrigEff->Draw("histpe");
//   HMCTAUPTTrigEff->Draw("histpesame");
//   //HTAUPTTrigEff_Josh.SetLineColor(1);
//   //HTAUPTTrigEff_Josh.Draw("lsame");
//   //HMCTAUPTTrigEff_Josh.SetLineColor(4);
//   //HMCTAUPTTrigEff_Josh.Draw("lsame");
//   C.Print(plotFileName+".ps");


//   ///////////////Compare Real and Fake Taus
//   //Low mT ZTT  
//   MTcat_=1;
//   extrasel_=selection+"*"+selectionTrigPass;
//   TH1F*HTAUPTZTTTrigPass=getZToTauTau();  HTAUPTZTTTrigPass->SetName("HTAUPTZTTTrigPass");
//   extrasel_=selection+"*"+selectionTrigFail;
//   TH1F*HTAUPTZTTTrigFail=getZToTauTau();  HTAUPTZTTTrigFail->SetName("HTAUPTZTTTrigFail");
//   TH1F*HMCTAUPTZTTTrigEff = computeTrigEff(HTAUPTZTTTrigPass,HTAUPTZTTTrigFail); 

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
//   HMCTAUPTZTTTrigEff->SetMarkerColor(ZTauTauColor_);
//   HMCTAUPTZTTTrigEff->SetLineColor(ZTauTauColor_);
//   HMCTAUPTZTTTrigEff->Draw("histpesame");
//   TLegend legend;
//   legend.SetFillStyle (0);
//   legend.SetFillColor (0);
//   legend.SetBorderSize(0);
//   legend.SetTextSize(0.04);
//   legend.AddEntry(HMCTAUPTZTTTrigEff,TString("ZTT MC (mT<")+mTCut_+")","pe");
//   legend.AddEntry(HMCTAUPTWTrigEff,"W+jets (mT>70)","pe");
//   legend.AddEntry(HTAUPTWTrigEff,"Data (mT>70)","pe");
//   legend.SetX1NDC(.50);
//   legend.SetX2NDC(.85);
//   legend.SetY1NDC(.22);
//   legend.SetY2NDC(.57);
//   legend.Draw();
//   C.Print(plotFileName+".ps");



  C.Print(plotFileName+".ps]");
}


void  TauMuPlotter::plotTauTriggerReal(Int_t Region, TString tag){
  scaleSamplesLumi();

  ///plot the difference in eta
  plotvar_="taupt";
  nbins_=30;
  xmin_=0;
  xmax_=60;
  MTcat_=1;
  Chcat_=1;
  Isocat_=1;
  extrasel_="1";

  Float_t xbinsValues[15]={17,18,19,20,21,22,24,26,28,30,34,38,42,46,50};
  setVariableBinning(14,xbinsValues);
  nbins_=0;

  
  TString selectionTrigPass="(trigTest1==1||trigTest2==1||trigTest3==1||trigTest4==1||trigTest5==1||trigTest6==1||trigTest7==1||trigTest8==1||trigTest9==1)";
  TString selectionTrigFail="(!"+selectionTrigPass+")";

  TString selection;
  if(Region==1) selection="(30<ditaumass&&ditaumass<100&&abs(taueta)<1.5)";
  if(Region==2) selection="(30<ditaumass&&ditaumass<100&&abs(taueta)>1.5)";

  TString region;
  if(Region==1) region="Barrel";
  if(Region==2) region="EndCap";


  extrasel_=selection;
  TH1F*HMCTAUPT=getZToTauTau();           HMCTAUPT->SetName("HMCTAUPT"); 
  TH1F*HTAUPT=getTotalData();             HTAUPT->SetName("HTAUPT");
  TH1F*HMCW=getWJetsNJet(1);              HTAUPT->Add(HMCW,-1);  delete HMCW;
  TH1F*HMCQ=getQCDInc();                  HTAUPT->Add(HMCQ,-1);  delete HMCQ;
  TH1F*HMCT=getTTJetsInc();               HTAUPT->Add(HMCT,-1);  delete HMCT;
  
  extrasel_=selection+"*"+selectionTrigPass;
  TH1F*HMCTAUPTTrigPass=getZToTauTau();   HMCTAUPTTrigPass->SetName("HMCTAUPTTrigPass");
  TH1F*HTAUPTTrigPass=getTotalData();     HTAUPTTrigPass->SetName("HTAUPTTrigPass");
  HMCW=getWJetsNJet(1);          HTAUPTTrigPass->Add(HMCW,-1);  delete HMCW;
  HMCQ=getQCDInc();                       HTAUPTTrigPass->Add(HMCQ,-1);  delete HMCQ;
  HMCT=getTTJetsInc();                    HTAUPTTrigPass->Add(HMCT,-1);  delete HMCT;

  extrasel_=selection+"*"+selectionTrigFail;
  TH1F*HMCTAUPTTrigFail=getZToTauTau();   HMCTAUPTTrigFail->SetName("HMCTAUPTTrigFail");
  TH1F*HTAUPTTrigFail=getTotalData();     HTAUPTTrigFail->SetName("HTAUPTTrigFail");
  HMCW=getWJetsNJet(1);          HTAUPTTrigFail->Add(HMCW,-1);  delete HMCW;
  HMCQ=getQCDInc();                       HTAUPTTrigFail->Add(HMCQ,-1);  delete HMCQ;
  HMCT=getTTJetsInc();                    HTAUPTTrigFail->Add(HMCT,-1);  delete HMCT;

 
  TCanvas C;
  TString plotFileName=TString("TauTriggerEfficiency_muTau_")+region+"_"+tag;
  C.Print(plotFileName+".ps[");

  ////Compare the Pass distribution to the total
  C.Clear();
  HTAUPT->SetTitle("Data");
  HTAUPT->GetXaxis()->SetTitle("Tau p_{T}");
  HTAUPT->GetYaxis()->SetRangeUser(0,HTAUPT->GetMaximum()*1.3);
  HTAUPT->Draw("hist");
  HTAUPTTrigPass->Draw("histpesame");
  C.Print(plotFileName+".ps");

  C.Clear();
  HMCTAUPT->SetTitle("MC");
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
  HTAUPTTrigEff->GetYaxis()->SetRangeUser(0.5,1);
  HTAUPTTrigEff->Draw("histpe");
  HMCTAUPTTrigEff->SetMarkerColor(4);
  HMCTAUPTTrigEff->SetLineColor(4);
  HMCTAUPTTrigEff->Draw("histpesame");
  C.Print(plotFileName+".ps");


  ///save the histograms for fitting later
  TFile FData(plotFileName+"_Data.root","recreate");
  HTAUPTTrigEff->SetName("efficiency");     HTAUPTTrigEff->SetTitle("");
  HTAUPTTrigEff->Write();
  FData.ls(); FData.Close();
  TFile FMC(plotFileName+"_MC.root","recreate");
  HMCTAUPTTrigEff->SetName("efficiency");   HMCTAUPTTrigEff->SetTitle("");
  HMCTAUPTTrigEff->Write();
  FMC.ls(); FMC.Close();



  C.Print(plotFileName+".ps]");
}


TH1F* TauMuPlotter::getSignal(long mass){
  TH1F* H=getSample(TString("HiggsGG")+mass);
  TH1F* Hqq=getSample(TString("HiggsVBF")+mass);
  TH1F* HVH=getSample(TString("HiggsVH")+mass);
  H->Add(Hqq);
  H->Add(HVH);
  
  delete Hqq; delete HVH;

  return H;
}

void TauMuPlotter::compareSignalShapes(){


  ///plot the difference in eta
  plotvar_="svfitmass";
  nbins_=40;
  xmin_=0;
  xmax_=200;
  MTcat_=1;
  Chcat_=1;
  Isocat_=1;
  //extrasel_="1";
  //extrasel_=getSMcut(3);
  extrasel_=getSMcut(4);

  TH1F * HZ=getZToTauTau();
  HZ->Scale(1./HZ->Integral());
  
  TH1F * H[8];
  for(Int_t m=0;m<3;m++){
    H[m]=getSignal(115+m*10);
  }

  TCanvas C;
  HZ->SetFillColor(kOrange-4);
  HZ->SetTitle("");
  HZ->GetXaxis()->SetTitle("m(#tau#tau)   [GeV]");
  HZ->GetYaxis()->SetTitle("Unit Norm");
  HZ->Draw("hist");
  for(Int_t m=0;m<3;m++){
    H[m]->Scale(1./H[m]->Integral());
    if(m==0)H[m]->SetLineColor(1);
    if(m==1)H[m]->SetLineColor(2);
    if(m==2)H[m]->SetLineColor(4);
    H[m]->Draw("histsame");
  }
  C.Print("compareSignalShapes.eps");
  C.Print("compareSignalShapes.png");
}


bool TauMuPlotter::printRawYields(TString selection){
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    TH1F*hmass=(*s)->getHistoNtpFile("ditaumass",100,0,1000,selection.Data());
    cout<<hmass->GetName()<<" "<<(int)(hmass->Integral(0,hmass->GetNbinsX()+1))<<endl;
    delete hmass;
  }

  return 1;
}


TH1F* TauMuPlotter::smearHisto(TH1F* h){
  if(!h){
    cout<<" smearHisto : histo is NULL"<<endl;
    return 0;
  }

 
  TH1F* hs=0;
 
  if(smearHistoRes_==0.){//just copy
    hs=(TH1F*)h->Clone(TString(h->GetName())+"smeared");
    cout<<"smearHistoRes is 0 returning the same histo"<<endl;
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
    
    hs->Scale(h->Integral()/hs->Integral());//make sure the output histo has the same integral
  }
  
  return hs;
}


void TauMuPlotter::fixFileTag(TString * filetag){

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



