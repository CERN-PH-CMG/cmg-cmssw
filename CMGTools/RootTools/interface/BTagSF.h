#ifndef BTagSF_h
#define BTagSF_h

#include "TRandom3.h"
#include <iostream>

class BTagSF{

 public:
  BTagSF( int seed=0 );
  ~BTagSF();
  
  // this tells whether the jet is btagged or not
  // intead of checking the discriminator, I call this function
  // every jet in the same way, but:
  // - data flag
  // - 2012 flag
  Bool_t isbtagged(Float_t pt, Float_t eta, Float_t csv, Int_t jetflavor, Bool_t isdata, UInt_t btagsys, UInt_t mistagsys, Bool_t is2012);
  Double_t getSFb(Float_t pt, UInt_t btagsys);
  Double_t getSFl(Float_t pt, Float_t eta, UInt_t mistagsys, Bool_t is2012);
  Double_t getMistag(Float_t pt, Float_t eta);
  
  enum { kNo, kDown, kUp };        // systematic variations 
  
 private:
  TRandom3* randm;
  
};
#endif
