#ifndef TESTTRIG_H_
#define TESTTRIG_H_

#include <iomanip>

#include <TString.h>

class TestTrig {
public:
  
  TestTrig():
    dirname_(""),
    firstfile_(1),
    lastfile_(1){
  };
  TestTrig(TString filename, Int_t firstfile, Int_t lastfile):
    dirname_(filename),
    firstfile_(firstfile),
    lastfile_(lastfile){
  };
  ~TestTrig(){};


  void testMuTauTrig(TString inputtag="cmgTauSel");

private:
  
  TString dirname_;
  Int_t firstfile_;
  Int_t lastfile_;
  
};



#endif /*TESTTRIG_H_*/
