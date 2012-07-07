#ifndef CMGTools_H2TauTau_BTagEfficiency_H
#define CMGTools_H2TauTau_BTagEfficiency_H

#include <math.h> 
#include "TMath.h" 
#include <limits>

class BTagEfficiency {
 public:
  BTagEfficiency(){} ;


  // Obtain btag EFF
  double btagEFF(double x, int isb){
    
    double effb = 1.;
    double effc = 1.;
    
    //Functional form for b/c-tagging efficiency on MC:  
    //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/eff_b_c-ttbar_payload.txt
    /*
    //TCHEL
    effb = 3.90732786802e-06*x*x*x*x +  -0.000239934437355*x*x*x +  0.00664986827287*x*x +  -0.112578996016*x +  1.00775721404;
    effc = 0.343760640168*exp(-0.00315525164823*x*x*x + 0.0805427315196*x*x + -0.867625139194*x + 1.44815935164 );
    */

    //CSVM
    effb = -1.73338329789*x*x*x*x +  1.26161794785*x*x*x +  0.784721653518*x*x +  -1.03328577451*x +  1.04305075822;
    effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;

    double eff = 1.;
    if (isb==1)
      eff = effb;
    else
      eff = effc;
    
    return eff;
  }

  // Obtain btag EFF SF
  double btagSF(double x, int isb){
    
//     float ptmin[14] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
//     float ptmax[14] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
    
    if (x<30) 
      x=30.1;
    if (x>670)
      x=669.1;

    //Tagger: TCHEL within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt    
    /*
    double SFb = 0.603913*((1.+(0.286361*x))/(1.+(0.170474*x)));
    double SFb_error[14] =
      {0.0244956,
       0.0237293,
       0.0180131,
       0.0182411,
       0.0184592,
       0.0106444,
       0.011073,
       0.0106296,
       0.0175259,
       0.0161566,
       0.0158973,
       0.0186782,
       0.0371113,
       0.0289788 };
    */

    //    Tagger: CSVM within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
    double SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
//     double SFb_error[14] = {
//       0.0295675,
//       0.0295095,
//       0.0210867,
//       0.0219349,
//       0.0227033,
//       0.0204062,
//       0.0185857,
//       0.0256242,
//       0.0383341,
//       0.0409675,
//       0.0420284,
//       0.0541299,
//       0.0578761,
//       0.0655432 };

//     double SFunc = 0.;
//     for (int i=0; i<13; ++i){
//       if (x > ptmin[i] && x < ptmax[i+1])
// 	SFunc = SFb_error[i];
//     }
    
//     double unc = 0;
    
//     if (isb)
//       unc = SFunc;
//     else
//       unc = 2*SFunc;
  
    return SFb;
    //  return SFb-unc;
  }
  
  
  double mistagEFF(double x, double y) {
    
    //Values are available only for jets with pT<670  
    // https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C
    if (x>670) x=669;
    double tmpMistag = 1.;

    /*
    Float_t Etamin;
    Float_t Etamax;
    if (TMath::Abs(y)<0.5){
      Etamin = 0.0;
      Etamax = 0.5;
    }else if(TMath::Abs(y)>0.5&&TMath::Abs(y)<1.0){
      Etamin = 0.5;
      Etamax = 1.0;
    }else if(TMath::Abs(y)>1.0&&TMath::Abs(y)<1.5){
      Etamin = 1.0;
      Etamax = 1.5;
    }else if(TMath::Abs(y)>1.5&&TMath::Abs(y)<2.4){
      Etamin = 1.5;
      Etamax = 2.4;
    }else{
      //Do some initialization
      Etamin = 1.5;
      Etamax = 2.4;
      std::cout << "Warning from BTagEfficiency.h: The eta range of jet is not applicable for b-jets" << std::endl;
    }

    TString Atagger = "TCHEL";
    TString meanminmax = "mean";
    TString sEtamin = Form("%1.1f",Etamin);
    TString sEtamax = Form("%1.1f",Etamax);
    
    TString Atagger = "TCHEL";
    if( Atagger == "TCHEL" && sEtamin == "0.0" && sEtamax == "0.5") {
      if( meanminmax == "mean" ) tmpMistag = (((-0.0235318+(0.00268868*x))+(-6.47688e-06*(x*x)))+(7.92087e-09*(x*(x*x))))+(-4.06519e-12*(x*(x*(x*x))));
    }
    if( Atagger == "TCHEL" && sEtamin == "0.5" && sEtamax == "1.0") {
      if( meanminmax == "mean" ) tmpMistag = (((-0.0257274+(0.00289337*x))+(-7.48879e-06*(x*x)))+(9.84928e-09*(x*(x*x))))+(-5.40844e-12*(x*(x*(x*x))));
    }
    if( Atagger == "TCHEL" && sEtamin == "1.0" && sEtamax == "1.5") {
      if( meanminmax == "mean" ) tmpMistag = (((-0.0310046+(0.00307803*x))+(-7.94145e-06*(x*x)))+(1.06889e-08*(x*(x*x))))+(-6.08971e-12*(x*(x*(x*x))));
    }
    if( Atagger == "TCHEL" && sEtamin == "1.5" && sEtamax == "2.4"){
      if( meanminmax == "mean" ) tmpMistag = (((-0.0274561+(0.00301096*x))+(-8.89588e-06*(x*x)))+(1.40142e-08*(x*(x*x))))+(-8.95723e-12*(x*(x*(x*x))));
    }
    */
    
    Float_t Etamin; 
    Float_t Etamax; 
    
    if (TMath::Abs(y)<0.8){ 
      Etamin = 0.0;                               
      Etamax = 0.8;                                                                                                                      
    }else if(TMath::Abs(y)>0.8&&TMath::Abs(y)<1.6){                                                                                      
      Etamin = 0.8;                                                                                                                     
      Etamax = 1.6;                                                                                                                     
    }else if(TMath::Abs(y)>1.6&&TMath::Abs(y)<2.4){                                                                                     
      Etamin = 1.6;                                                                                                                      
      Etamax = 2.4;                                                                                                                           }else{ 
      //Do some initialization                                                                                                           
      Etamin = 1.6;
      Etamax = 2.4; 
      std::cout << "Warning from BTagEfficiency.h: The eta range of jet is not applicable for b-jets" << std::endl;     
    }                                      

    TString Atagger = "CSVM"; 
    TString meanminmax = "mean"; 
    TString sEtamin = Form("%1.1f",Etamin); 
    TString sEtamax = Form("%1.1f",Etamax); 
             
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8") {
      if( meanminmax == "mean" ) tmpMistag = (0.00967751+(2.54564e-05*x))+(-6.92256e-10*(x*x));
    }
    /*
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "2.4") {
      if( meanminmax == "mean" ) tmpMistag = (0.0113428+(5.18983e-05*x))+(-2.59881e-08*(x*x));
    }
    */
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6") {
      if( meanminmax == "mean" ) tmpMistag = (0.00974141+(5.09503e-05*x))+(2.0641e-08*(x*x));
    }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4") {
      if( meanminmax == "mean" ) tmpMistag = (0.013595+(0.000104538*x))+(-1.36087e-08*(x*x));
    }

    return tmpMistag;    
  }
  
  
  double mistagSF(double x, double y){
    //x= pT   
    //y= eta    
    //This function is only for TCHEL tagger  
    //Does not take into account syst+stat uncertainties on SF   
    //TF1* GetSFLight(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax)  
    //{                                              
    
    //Values are available only for jets with pT<670   
    if (x>670) x=669;
    
    Float_t Etamin;
    Float_t Etamax;
    if (TMath::Abs(y)<0.8){
      Etamin = 0.0;
      Etamax = 0.8;
    }else if(TMath::Abs(y)>0.8&&TMath::Abs(y)<1.6){
      Etamin = 0.8;
      Etamax = 1.6;
    }else if(TMath::Abs(y)>1.6&&TMath::Abs(y)<2.4){
      Etamin = 1.6;
      Etamax = 2.4;
    }else{
      Etamin = 1.6;
      Etamax = 2.4;
      std::cout << "Warning from BTagEfficiency.h: The eta range of jet is not applicable for b-jets" << std::endl;
    }
    TString meanminmax = "mean";
    //  TF1 *tmpSFl = NULL;     
    //    TString Atagger = "TCHEL"; //tagger+TaggerStrength;
    TString Atagger = "CSVM";
    TString sEtamin = Form("%1.1f",Etamin);
    TString sEtamax = Form("%1.1f",Etamax);
    //  cout << sEtamin << endl;               
    //  cout << sEtamax << endl;                                                                                                         
    
    double tmpSFl = 1.;
    
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
      }
    /*
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.04318+(0.000848162*x))+(-2.5795e-06*(x*x)))+(1.64156e-09*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.962627+(0.000448344*x))+(-1.25579e-06*(x*x)))+(4.82283e-10*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.12368+(0.00124806*x))+(-3.9032e-06*(x*x)))+(2.80083e-09*(x*(x*x)));
      }
    */
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
      }


    /*
    if( Atagger == "TCHEL" && sEtamin == "0.0" && sEtamax == "0.5"){
      if( meanminmax == "mean" ) tmpSFl = (1.13615*((1+(-0.00119852*x))+(1.17888e-05*(x*x))))+(-9.8581e-08*(x*(x*(x/(1+(0.00689317*x))))));
      if( meanminmax == "min" ) tmpSFl = (1.0369*((1+(-0.000945578*x))+(7.73273e-06*(x*x))))+(-4.47791e-08*(x*(x*(x/(1+(0.00499343*x))))));
      if( meanminmax == "max" ) tmpSFl = (1.22179*((1+(-0.000946228*x))+(7.37821e-06*(x*x))))+(-4.8451e-08*(x*(x*(x/(1+(0.0047976*x))))));
    }
    
    if( Atagger == "TCHEL" && sEtamin == "0.5" && sEtamax == "1.0"){
      if( meanminmax == "mean" ) tmpSFl = (1.13277*((1+(-0.00084146*x))+(3.80313e-06*(x*x))))+(-8.75061e-09*(x*(x*(x/(1+(0.00118695*x))))));
      if( meanminmax == "min" ) tmpSFl = (0.983748*((1+(7.13613e-05*x))+(-1.08648e-05*(x*x))))+(2.96162e-06*(x*(x*(x/(1+(0.282104*x))))));
      if( meanminmax == "max" ) tmpSFl = (1.22714*((1+(-0.00085562*x))+(3.74425e-06*(x*x))))+(-8.91028e-09*(x*(x*(x/(1+(0.00109346*x))))));
    }
    
    if( Atagger == "TCHEL" && sEtamin == "1.0" && sEtamax == "1.5"){
      if( meanminmax == "mean" ) tmpSFl = (1.17163*((1+(-0.000828475*x))+(3.0769e-06*(x*x))))+(-4.692e-09*(x*(x*(x/(1+(0.000337759*x))))));
      if( meanminmax == "min" ) tmpSFl = (1.0698*((1+(-0.000731877*x))+(2.56922e-06*(x*x))))+(-3.0318e-09*(x*(x*(x/(1+(5.04118e-05*x))))));
      if( meanminmax == "max" ) tmpSFl = (1.27351*((1+(-0.000911891*x))+(3.5465e-06*(x*x))))+(-6.69625e-09*(x*(x*(x/(1+(0.000590847*x))))));
    }
    if( Atagger == "TCHEL" && sEtamin == "1.5" && sEtamax == "2.4"){
      if( meanminmax == "mean" ) tmpSFl = (1.14554*((1+(-0.000128043*x))+(4.10899e-07*(x*x))))+(-2.07565e-10*(x*(x*(x/(1+(-0.00118618*x))))));
      if( meanminmax == "min" ) tmpSFl = (1.04766*((1+(-6.87499e-05*x))+(2.2454e-07*(x*x))))+(-1.18395e-10*(x*(x*(x/(1+(-0.00128734*x))))));
      if( meanminmax == "max" ) tmpSFl = (1.24367*((1+(-0.000182494*x))+(5.92637e-07*(x*x))))+(-3.3745e-10*(x*(x*(x/(1+(-0.00107694*x))))));
    }
    */

    return tmpSFl;
  }

  double mistagSF2012(double x, double y){
    //x= pT   
    //y= eta    
    //This function is only for TCHEL tagger  
    //Does not take into account syst+stat uncertainties on SF   
    //TF1* GetSFLight(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax)  
    //{                                              
    
    //Values are available only for jets with pT<670   
    if (x>670) x=669;
    
    
    double corr = 1.10422 + -0.000523856*x + 1.14251e-06*x*x;

    Float_t Etamin;
    Float_t Etamax;
    if (TMath::Abs(y)<0.8){
      Etamin = 0.0;
      Etamax = 0.8;
    }else if(TMath::Abs(y)>0.8&&TMath::Abs(y)<1.6){
      Etamin = 0.8;
      Etamax = 1.6;
    }else if(TMath::Abs(y)>1.6&&TMath::Abs(y)<2.4){
      Etamin = 1.6;
      Etamax = 2.4;
    }else{
      Etamin = 1.6;
      Etamax = 2.4;
      std::cout << "Warning from BTagEfficiency.h: The eta range of jet is not applicable for b-jets" << std::endl;
    }
    TString meanminmax = "mean";
    //  TF1 *tmpSFl = NULL;     
    //    TString Atagger = "TCHEL"; //tagger+TaggerStrength;
    TString Atagger = "CSVM";
    TString sEtamin = Form("%1.1f",Etamin);
    TString sEtamax = Form("%1.1f",Etamax);
    //  cout << sEtamin << endl;               
    //  cout << sEtamax << endl;                                                                                                         
    
    double tmpSFl = 1.;
    
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
      }
    /*
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.04318+(0.000848162*x))+(-2.5795e-06*(x*x)))+(1.64156e-09*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.962627+(0.000448344*x))+(-1.25579e-06*(x*x)))+(4.82283e-10*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.12368+(0.00124806*x))+(-3.9032e-06*(x*x)))+(2.80083e-09*(x*(x*x)));
      }
    */
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
	if( meanminmax == "min" ) tmpSFl = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
	if( meanminmax == "max" ) tmpSFl = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
      }

    
    return tmpSFl*corr;
  }

};
#endif


