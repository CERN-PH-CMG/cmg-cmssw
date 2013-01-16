#include "CMGTools/RootTools/interface/BTagSF.h"

BTagSF::BTagSF( int seed ) {

  randm = new TRandom3(seed);

}

BTagSF::~BTagSF() {

  delete randm;

}


Bool_t BTagSF::isbtagged(Float_t pt, Float_t eta, Float_t csv, Int_t jetflavor, Bool_t isdata, UInt_t btagsys, UInt_t mistagsys, Bool_t is2012)
{
  Bool_t btagged = kFALSE;

  if(isdata) {
    if(csv>0.679) btagged = kTRUE;
    else          btagged = kFALSE;
    return btagged;
  }

  Double_t SFb = 0.0;
  Double_t eff_b = 0.719;

  SFb = getSFb(pt, btagsys, is2012);

  Double_t promoteProb_btag=0; // ~probability to promote to tagged
  Double_t demoteProb_btag=0; // ~probability to demote from tagged

  if(SFb < 1) {
    demoteProb_btag = fabs(1.0 - SFb);
  } else {
    promoteProb_btag = fabs(SFb - 1.0)/((SFb/eff_b) - 1.0);
  }

  if(fabs(jetflavor) == 5) {                // real b-jet
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

  // not a real b-jet, apply mistag

  Double_t SFl = 0, eff_l = 0;

  // c or light
  if(fabs(jetflavor) == 4) {
    // SFc = SFb with twice the quoted uncertainty
    SFl = getSFc(pt, btagsys, is2012);
    eff_l = 0.192*SFl; // eff_c in MC for CSVM = (-1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724), x = 0.679
  } else {
    SFl = getSFl(pt, eta, mistagsys, is2012);
    eff_l = getMistag(pt, eta);
  }

  Double_t promoteProb_mistag=0; // ~probability to promote to tagged
  Double_t demoteProb_mistag=0; // ~probability to demote from tagged

  if(SFl > 1) {
    promoteProb_mistag = fabs(SFl - 1.0)/((SFl/eff_l) - 1.0);
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

Double_t BTagSF::getSFb(Float_t pt, UInt_t btagsys, Bool_t is2012)
{

  // pT dependent scale factors
  // Tagger: CSVM within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
  // SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x))); (2011)
  // SFb = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x))); (2012)
  // for pt > 670 (800) GeV: use the SFb value at 670 (800) GeV with twice the quoted uncertainty for 2011 (2012)
  // for pt < 30 (20) GeV: use the SFb value at 30 (20) GeV with a +/-0.12 absolute uncertainty (twice the quoted uncertainty) for 2011 (2012)
  // i.e SFb(pt<30) = SFb(pt=30) +/- 0.12, so the relative uncertainty is 0.12/SFb(pt=30) for 2011

  Float_t x = pt;
  Double_t SFb = 1.0; 
  if(!is2012) {
    if(pt >= 670.0) x = 669.9;
    if(pt < 30.0) x = 30.0;
    SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
  } else {
    if(pt >= 800.0) x = 799.9;
    if(pt < 20.0) x = 20.0;
    SFb = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));
  }

  if(btagsys == kNo)        return SFb;

  Double_t SFb_error_2011[] = {0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062, 0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432};
  Float_t ptmin_2011[] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
  Float_t ptmax_2011[] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
  Double_t SFb_error_2012[] = {0.0554504, 0.0209663, 0.0207019, 0.0230073, 0.0208719, 0.0200453, 0.0264232, 0.0240102, 0.0229375, 0.0184615, 0.0216242, 0.0248119, 0.0465748, 0.0474666, 0.0718173, 0.0717567};
  Float_t ptmin_2012[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  Float_t ptmax_2012[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  Double_t SFb_error_x = 0.0;

  UInt_t nbins = is2012 ? 16 : 14;

  for (UInt_t ibin=0; ibin<nbins; ibin++) {
    if(!is2012) {
      if(x >= ptmin_2011[ibin] && x < ptmax_2011[ibin]) SFb_error_x = SFb_error_2011[ibin];
    } else {
      if(x >= ptmin_2012[ibin] && x < ptmax_2012[ibin]) SFb_error_x = SFb_error_2012[ibin];
    }
  }
  if(!is2012) {
    if(pt >= 670.0) SFb_error_x = 2.0*0.0655432;
    if(pt <   30.0) SFb_error_x = 0.12;
  } else {
    if(pt >= 800.0) SFb_error_x = 2.0*0.0717567;
    if(pt <   20.0) SFb_error_x = 2.0*0.0554504;
  }

  Double_t scalefactor = SFb;

  if(btagsys == kDown) scalefactor = (SFb - SFb_error_x);
  if(btagsys == kUp)   scalefactor = (SFb + SFb_error_x);

  return scalefactor;

}

Double_t BTagSF::getSFc(Float_t pt, UInt_t btagsys, Bool_t is2012)
{

  // SFc = SFb with twice the quoted uncertainty

  Float_t x = pt;
  Double_t SFc = 1.0;
  if(!is2012) {
    if(pt >= 670.0) x = 669.9;
    if(pt < 30.0) x = 30.0;
    SFc = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
  } else {
    if(pt >= 800.0) x = 799.9;
    if(pt < 20.0) x = 20.0;
    SFc = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));
  }

  if(btagsys == kNo)        return SFc;

  Double_t SFb_error_2011[] = {0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062, 0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432};
  Float_t ptmin_2011[] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
  Float_t ptmax_2011[] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
  Double_t SFb_error_2012[] = {0.0554504, 0.0209663, 0.0207019, 0.0230073, 0.0208719, 0.0200453, 0.0264232, 0.0240102, 0.0229375, 0.0184615, 0.0216242, 0.0248119, 0.0465748, 0.0474666, 0.0718173, 0.0717567};
  Float_t ptmin_2012[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  Float_t ptmax_2012[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  Double_t SFc_error_x = 0.0;

  UInt_t nbins = is2012 ? 16 : 14;

  for (UInt_t ibin=0; ibin<nbins; ibin++) {
    if(!is2012) {
      if(x >= ptmin_2011[ibin] && x < ptmax_2011[ibin]) SFc_error_x = 2.0*SFb_error_2011[ibin];
    } else {
      if(x >= ptmin_2012[ibin] && x < ptmax_2012[ibin]) SFc_error_x = 2.0*SFb_error_2012[ibin];
    }

  }
  if(!is2012) {
    if(pt >= 670.0) SFc_error_x = 2.0*2.0*0.0655432;
    if(pt <   30.0) SFc_error_x = 2.0*0.12;
  } else {
    if(pt >= 800.0) SFc_error_x = 2.0*2.0*0.0717567;
    if(pt <   20.0) SFc_error_x = 2.0*2.0*0.0554504;
  }

  Double_t scalefactor = SFc;

  if(btagsys == kDown) scalefactor = (SFc - SFc_error_x);
  if(btagsys == kUp)   scalefactor = (SFc + SFc_error_x);

  return scalefactor;

}

Double_t BTagSF::getSFl(Float_t pt, Float_t eta, UInt_t mistagsys, Bool_t is2012)
{

  Float_t x = TMath::Min(double(pt), is2012 ? 670.0 : 800.0);

  Double_t SFl = 0;

  if(!is2012) {
    if(fabs(eta) >= 0.0 && fabs(eta) < 0.8) {
      if(mistagsys == kNo)        SFl = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
    } else if(fabs(eta) >= 0.8 && fabs(eta) < 1.6) {
      if(mistagsys == kNo)        SFl = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
    } else if(fabs(eta) >= 1.6 && fabs(eta) < 2.4) {
      if(mistagsys == kNo)        SFl = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
    }
  } else {
    if(fabs(eta) >= 0.0 && fabs(eta) < 0.8) {
      if(mistagsys == kNo)        SFl = ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x))); 
      else if(mistagsys == kDown) SFl = ((0.972746+(0.00104424*x))+(-2.36081e-06*(x*x)))+(1.53438e-09*(x*(x*x))); 
      else if(mistagsys == kUp)   SFl = ((1.15201+(0.00292575*x))+(-7.41497e-06*(x*x)))+(5.0512e-09*(x*(x*x))); 
    } else if(fabs(eta) >= 0.8 && fabs(eta) < 1.6) {
      if(mistagsys == kNo)        SFl = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x))); 
      else if(mistagsys == kDown) SFl = ((0.9836+(0.000649761*x))+(-1.59773e-06*(x*x)))+(1.14324e-09*(x*(x*x))); 
      else if(mistagsys == kUp)   SFl = ((1.17735+(0.00156533*x))+(-4.32257e-06*(x*x)))+(3.18197e-09*(x*(x*x))); 
    } else if(fabs(eta) >= 1.6 && fabs(eta) < 2.4) {
      if(mistagsys == kNo)        SFl = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((1.00616+(0.000358884*x))+(-1.23768e-06*(x*x)))+(6.86678e-10*(x*(x*x))); 
      else if(mistagsys == kUp)   SFl = ((1.17671+(0.0010147*x))+(-3.66269e-06*(x*x)))+(2.88425e-09*(x*(x*x)));
    }
  }

  return SFl;

}

Double_t BTagSF::getMistag(Float_t pt, Float_t eta)
{

  Float_t x = TMath::Min(double(pt), 670.0);

  Double_t eff_l = 0.0;

  if(fabs(eta) >= 0.0 && fabs(eta) < 0.8) {
    eff_l = ((0.00967751+(2.54564e-05*x))+(-6.92256e-10*(x*x)));
  } else if(fabs(eta) >= 0.8 && fabs(eta) < 1.6) {
    eff_l = ((0.00974141+(5.09503e-05*x))+(2.0641e-08*(x*x)));
  } else if(fabs(eta) >= 1.6 && fabs(eta) < 2.4) {
    eff_l = ((0.013595+(0.000104538*x))+(-1.36087e-08*(x*x)));
  }

  return eff_l;

}
