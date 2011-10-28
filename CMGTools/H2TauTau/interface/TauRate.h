#ifndef CMGTools_H2TauTau_TauRate_H
#define CMGTools_H2TauTau_TauRate_H

#include <iostream>
#include <math.h>
#include "TFile.h"
#include "TH2F.h"


class TauRate { 
protected:

public: 
  enum TauType { 
    Inclusive,
    OneProng,
    OneProngEM,
    ThreeProng
  };

  TauRate():
    lFile("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/TauDataMCScale.root","read")
  { 
    fRAllProng   = (TH2F*) lFile.FindObjectAny("AllProng");   fRAllProng->SetDirectory(0);
    fR1Prong     = (TH2F*) lFile.FindObjectAny("OneProng");   fR1Prong  ->SetDirectory(0);
    fR1ProngEM   = (TH2F*) lFile.FindObjectAny("OneProngEM"); fR1ProngEM->SetDirectory(0);
    fR3Prong     = (TH2F*) lFile.FindObjectAny("ThreeProng"); fR3Prong  ->SetDirectory(0);
  }
  
  ~TauRate() {}

  float  getScale     (TauType iType,double iPt,double iEta);
  float  getScaleError(TauType iType,double iPt,double iEta);

private:
  TH2F* fRAllProng;
  TH2F* fR1Prong;
  TH2F* fR1ProngEM;
  TH2F* fR3Prong;
  TFile lFile;
};


#endif 

