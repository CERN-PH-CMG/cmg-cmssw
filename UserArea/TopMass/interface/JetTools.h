#ifndef _secvtx_jettools_h_
#define _secvtx_jettools_h_

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "Math/LorentzVector.h"
#include "TString.h"
#include "TRandom.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

/*
// Usage
JetTools jetTool;

//retrieve a jet with the JER correction applied
LorentzVector newJetP4 = jetTool.correctJetResolution(origPt, origEta, origPhi, genPt);

//apply shift energy scale up according to uncertainty
LorentzVector shiftedUpJetP4 =jetTool.applyShift(newJetP4.pt(), newJetP4.eta(), newJetP4.phi(),JetTools::JES_UP);

//list of possible shifts: JetTools::JES_UP / JetTools::JES_DOWN / JetTools::JER_UP / JetTools::JER_DOWN

*/

class JetTools
{

 public:
  JetTools()
    {
      TString uncFile="${CMSSW_BASE}/src/UserCode/SecVtxAnalysis/data/GR_R_42_V23_AK5PFchs_Uncertainty.txt";
      gSystem->ExpandPathName(uncFile);
      jecUnc = new JetCorrectionUncertainty(uncFile.Data());
    }

  enum ShiftType { JES_UP, JES_DOWN, JER_UP, JER_DOWN, JER_STD};

  //
  inline LorentzVector correctJetResolution(float pt, float eta, float phi, float genPt, int mode=JER_STD)
    {
      //smearing factors are described in https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
      double ptSF(1.0), ptSF_err(0.06);
      if(fabs(eta)<0.5)                        { ptSF=1.066; ptSF_err=sqrt(pow(0.007,2)+pow(0.5*(0.07+0.072),2)); }
      else if(fabs(eta)>=0.5 && fabs(eta)<1.7) { ptSF=1.191; ptSF_err=sqrt(pow(0.019,2)+pow(0.5*(0.06+0.062),2)); }
      else if(fabs(eta)>=1.7 && fabs(eta)<2.3) { ptSF=1.096; ptSF_err=sqrt(pow(0.030,2)+pow(0.5*(0.08+0.085),2)); }
      else if(fabs(eta)>=2.3 && fabs(eta)<5.0) { ptSF=1.166; ptSF_err=sqrt(pow(0.050,2)+pow(0.5*(0.19+0.199),2)); }
      if(mode==JER_UP)   ptSF += ptSF_err;
      if(mode==JER_DOWN) ptSF -= ptSF_err;
      ptSF=(genPt+gRandom->Gaus(ptSF,ptSF_err)*(pt-genPt))/pt;  
      if(ptSF<0) ptSF=1.0;
      if(genPt>0 && ptSF>0) pt *= ptSF;
      
      //new jet
      double px(pt*cos(phi)), py(pt*sin(phi)), pz( pt*sinh(eta) ), en( sqrt(px*px+py*py+pz*pz) );
      LorentzVector newJetP4(px,py,pz,en);
      return newJetP4;
    }


  //
  inline LorentzVector applyShift(float pt, float eta, float phi,float genPt,int shiftType)
    {
      double enSF(1.0);
      double px(pt*cos(phi)), py(pt*sin(phi)), pz( pt*sinh(eta) ), en( sqrt(px*px+py*py+pz*pz) );
      LorentzVector newJetP4(px,py,pz,en);

      if(shiftType==JER_STD || shiftType==JER_UP || shiftType==JER_DOWN) newJetP4=correctJetResolution(pt,eta,phi,genPt,shiftType);
      if(shiftType==JES_UP || shiftType==JES_DOWN)
	{
	  try{
	    double varSign=(shiftType==JES_UP ? 1.0 : -1.0 );
	    jecUnc->setJetEta(fabs(eta));
	    jecUnc->setJetPt(pt);
	    enSF = 1.0 + varSign*fabs(jecUnc->getUncertainty(true));
	  }catch(std::exception&e){
	  }
	  newJetP4 *= enSF;
	}
      
      return newJetP4;
    }
  
  //
  ~JetTools()
    {
    }

 private:

  //map of JES uncertainties
  JetCorrectionUncertainty *jecUnc;

};

#endif
