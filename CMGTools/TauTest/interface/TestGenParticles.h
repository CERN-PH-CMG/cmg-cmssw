#ifndef TESTGEN_H_
#define TESTGEN_H_

#include <iomanip>

#include <TString.h>

class TestGenParticles {
public:
  
  TestGenParticles():
    dirname_(""),
    firstfile_(1),
    lastfile_(1){
  };
  TestGenParticles(TString filename, Int_t firstfile, Int_t lastfile):
    dirname_(filename),
    firstfile_(firstfile),
    lastfile_(lastfile){
  };
  ~TestGenParticles(){};

  void testGenParticle(TString inputtag="genParticlesStatus3");

private:
  
  TString dirname_;
  Int_t firstfile_;
  Int_t lastfile_;
  
};



#endif /*TESTGEN_H_*/
