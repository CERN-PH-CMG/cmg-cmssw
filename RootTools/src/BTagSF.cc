#include "CMGTools/RootTools/interface/BTagSF.h"

BTagSF::BTagSF( int seed ) {
  randm = new TRandom3(seed);
}

BTagSF::~BTagSF() {
  delete randm;
}

Bool_t BTagSF::isbtagged(Float_t pt, Float_t eta, Float_t csv, Int_t jetflavor, 
			 Bool_t isdata, UInt_t btagsys, UInt_t mistagsys, Bool_t is2012){
  Bool_t btagged = kFALSE;
  
  if(isdata) {
    if(csv>0.679) btagged = kTRUE;
    else          btagged = kFALSE;
    return btagged;
  }
  
  Double_t SFb = 0.0;
  Double_t eff_b = 0.719;
  
  if(is2012) {
    // scale factor from ttbar measured by BTV in 2011 = 0.97 /pm 0.03, inflate uncertainties by 1.5x
    if(btagsys == kNo) SFb = 0.97;
    if(btagsys == kDown) SFb = 0.925;
    if(btagsys == kUp) SFb = 1.015;
  } else {
    SFb = getSFb(pt, btagsys);
  }
  
  Double_t promoteProb_btag=0; // ~probability to promote to tagged
  Double_t demoteProb_btag=0; // ~probability to demote from tagged
  
  if(SFb < 1) {
    demoteProb_btag = TMath::Abs(1.0 - SFb);
  } else {
    promoteProb_btag = TMath::Abs(SFb - 1.0)/((SFb/eff_b) - 1.0);
  }
  
  if(TMath::Abs(jetflavor) == 5) {                // real b-jet
    if(csv > 0.679) {                       // if tagged
      btagged = kTRUE;
      if(demoteProb_btag > 0 && randm->Uniform() > demoteProb_btag) btagged = kTRUE;  // leave it tagged
      else                                                          btagged = kFALSE; // demote it to untagged
    } else {
      btagged = kFALSE;
      if(promoteProb_btag > 0 && randm->Uniform() < promoteProb_btag) btagged = kTRUE;  // promote it to tagged
      else                                                            btagged = kFALSE; // leave it untagged
    }
    return btagged;
  }
  
  // not a real b-jet, apply mistagsys
  
  Double_t SFl = getSFl(pt, eta, mistagsys, is2012);
  Double_t eff_l = getMistag(pt, eta);
  
  Double_t promoteProb_mistag=0; // ~probability to promote to tagged
  Double_t demoteProb_mistag=0; // ~probability to demote from tagged
  
  if(SFl > 1) {
    promoteProb_mistag = TMath::Abs(SFl - 1.0)/((SFl/eff_l) - 1.0);
  }
  else {
    demoteProb_mistag = SFl;
  }
  
  if(csv > 0.679) {         // if tagged
    btagged = kTRUE;
    if(demoteProb_mistag > 0 && randm->Uniform() > demoteProb_mistag) btagged = kFALSE; // demote it to untagged
    else                                                              btagged = kTRUE;  // leave it tagged
  }
  else {                    // not tagged
    btagged = kFALSE;
    if(promoteProb_mistag > 0 && randm->Uniform() < promoteProb_mistag) btagged = kTRUE;  // promote it to tagged
    else                                                                btagged = kFALSE; // leave it untagged
  }
  
  return btagged;
}

Double_t BTagSF::getSFb(Float_t pt, UInt_t btagsys){
  
  // pT dependent scale factors
  // Tagger: CSVM within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
  // SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
  // for pt > 670 GeV: use the SFb value at 670 GeV with twice the quoted uncertainty
  // for pt < 30 GeV: use the SFb value at 30 GeV with a +/-0.12 absolute uncertainty
  // i.e SFb(pt<30) = SFb(pt=30) +/- 0.12, so the relative uncertainty is 0.12/SFb(pt=30) 
  
  Float_t x = pt;
  if(pt >= 670.0) x = 669.9;
  if(pt < 30.0) x = 30.0;
  
  Double_t SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
  if(btagsys == kNo)        return SFb;
  
  Double_t SFb_error[14] = {0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062, 0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432};
  Float_t ptmin[14] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
  Float_t ptmax[14] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
  Double_t SFb_error_x = 0.0;
  for (UInt_t ibin=0; ibin<14; ibin++) {
    if(x >= ptmin[ibin] && x < ptmax[ibin]) SFb_error_x = SFb_error[ibin];
  }
  if(pt >= 670.0) SFb_error_x = 2.0*0.0655432;
  if(pt <   30.0) SFb_error_x = 0.12;
  
  Double_t scalefactor = SFb;
  
  if(btagsys == kDown) scalefactor = (SFb - SFb_error_x);
  if(btagsys == kUp)   scalefactor = (SFb + SFb_error_x);

  return scalefactor;
  
}

Double_t BTagSF::getSFl(Float_t pt, Float_t eta, UInt_t mistagsys, Bool_t is2012){
  
  Float_t x = TMath::Min(double(pt), 670.0);
  
  Double_t corr_2012 = 1.10422 + -0.000523856*x + 1.14251e-06*x*x;                           // 2012/2011 correction factor
  Double_t SFl = 0;
  
  if(TMath::Abs(eta) >= 0.0 && TMath::Abs(eta) < 0.8) {
    if(mistagsys == kNo)        SFl = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
    else if(mistagsys == kDown) SFl = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));
    else if(mistagsys == kUp)   SFl = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
  } else if(TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.6) {
    if(mistagsys == kNo)        SFl = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
    else if(mistagsys == kDown) SFl = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
    else if(mistagsys == kUp)   SFl = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
  } else if(TMath::Abs(eta) >= 1.6 && TMath::Abs(eta) < 2.4) {
    if(mistagsys == kNo)        SFl = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
    else if(mistagsys == kDown) SFl = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
    else if(mistagsys == kUp)   SFl = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
  }
  if(is2012) {
    SFl *= corr_2012;
  }
  
  return SFl;
  
}

Double_t BTagSF::getMistag(Float_t pt, Float_t eta){
  
  Float_t x = TMath::Min(double(pt), 670.0);
  
  Double_t eff_l = 0.0;
  
  if(TMath::Abs(eta) >= 0.0 && TMath::Abs(eta) < 0.8) {
    eff_l = ((0.00967751+(2.54564e-05*x))+(-6.92256e-10*(x*x)));
  } else if(TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.6) {
    eff_l = ((0.00974141+(5.09503e-05*x))+(2.0641e-08*(x*x)));
  } else if(TMath::Abs(eta) >= 1.6 && TMath::Abs(eta) < 2.4) {
    eff_l = ((0.013595+(0.000104538*x))+(-1.36087e-08*(x*x)));
  }

  return eff_l;

}
