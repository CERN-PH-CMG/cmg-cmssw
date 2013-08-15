#ifndef TESTCMGELECTRONS_H_
#define TESTCMGELECTRONS_H_

#include <iomanip>

#include <TString.h>

class TestCMGElectrons {
public:
  
  TestCMGElectrons():
    dirname_(""),
    firstfile_(1),
    lastfile_(1){
  };
  TestCMGElectrons(TString filename, Int_t firstfile, Int_t lastfile):
    dirname_(filename),
    firstfile_(firstfile),
    lastfile_(lastfile){
  };
  ~TestCMGElectrons(){};

  void testElectron(TString inputtag="cmgElectronSel");

private:
  
  TString dirname_;
  Int_t firstfile_;
  Int_t lastfile_;
  
};



#endif /*TESTCMGELECTRONS_H_*/
