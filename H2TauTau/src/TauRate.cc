
#include "CMGTools/H2TauTau/interface/TauRate.h"

float TauRate::getScale(TauType iType,double iPt,double iEta) { 
  TH2F* lH = 0;
  switch(iType) { 
  case Inclusive : 
    lH = fRAllProng;   break;
  case OneProng : 
    lH = fR1Prong;    break;
  case OneProngEM :
    lH = fR1ProngEM;  break;
  case ThreeProng :
    lH = fR3Prong;    break;
  }    
  if(lH == 0) return 1;
  return 1./lH->GetBinContent(lH->GetXaxis()->FindBin(iPt),lH->GetYaxis()->FindBin(fabs(iEta)));
}
float TauRate::getScaleError(TauType iType,double iPt,double iEta) { 
  TH2F* lH = 0;
  switch(iType) { 
  case Inclusive : 
    lH = fRAllProng;   break;
  case OneProng : 
    lH = fR1Prong;    break;
  case OneProngEM :
    lH = fR1ProngEM;  break;
  case ThreeProng :
    lH = fR3Prong;    break;
  }    
  if(lH == 0) return 1;
  double lE = lH->GetBinError  (lH->GetXaxis()->FindBin(iPt),lH->GetYaxis()->FindBin(fabs(iEta)));
  double lV = lH->GetBinContent(lH->GetXaxis()->FindBin(iPt),lH->GetYaxis()->FindBin(fabs(iEta)));
  return lE;
}
