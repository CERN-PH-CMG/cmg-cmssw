#ifndef TESTPATTAUS_H_
#define TESTPATTAUS_H_

#include <iomanip>

#include <TString.h>

class TestPatTaus {
public:
  
  TestPatTaus():
    filename_(""){
  };
  TestPatTaus(TString filename):
    filename_(filename){
  };
  ~TestPatTaus(){};

  void testTaus(TString inputtag="selectedPatTausAK5");
  void testMuons(TString inputtag="selectedPatMuonsAK5");
  void testElectrons(TString inputtag="selectedPatElectronsAK5");
  
private:
  
  TString filename_;
  
};



#endif /*TESTPATTAUS_H_*/
