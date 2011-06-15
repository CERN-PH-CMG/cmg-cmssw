#ifndef TESTCMGTAUS_H_
#define TESTCMGTAUS_H_

#include <iomanip>

#include <TString.h>

class TestCMGTaus {
public:
  
  TestCMGTaus():
    filename_(""){
  };
  TestCMGTaus(TString filename):
    filename_(filename){
  };
  ~TestCMGTaus(){};

  void testTau(TString inputtag="cmgTauSel");
  void testElectron(TString inputtag="cmgElectronSel");
  void testMuon(TString inputtag="cmgMuonSel");
  void testMET(TString inputtag="cmgPFMET");

private:
  
  TString filename_;
  
};



#endif /*TESTCMGTAUS_H_*/
