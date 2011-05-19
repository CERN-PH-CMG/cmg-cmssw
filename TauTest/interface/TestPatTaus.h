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

  void test();

private:
  
  TString filename_;
  
};



#endif /*TESTPATTAUS_H_*/
