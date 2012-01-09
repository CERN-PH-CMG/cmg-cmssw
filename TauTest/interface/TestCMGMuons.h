#ifndef TESTCMGMUONS_H_
#define TESTCMGMUONS_H_

#include <iomanip>

#include <TString.h>

class TestCMGMuons {
public:
  
  TestCMGMuons():
    dirname_(""),
    firstfile_(1),
    lastfile_(1){
  };
  TestCMGMuons(TString filename, Int_t firstfile, Int_t lastfile):
    dirname_(filename),
    firstfile_(firstfile),
    lastfile_(lastfile){
  };
  ~TestCMGMuons(){};

  void testMuon(TString inputtag="cmgMuonSel");

private:
  
  TString dirname_;
  Int_t firstfile_;
  Int_t lastfile_;
  
};



#endif /*TESTCMGMUONS_H_*/
