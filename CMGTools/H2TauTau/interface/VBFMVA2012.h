#ifndef CMGTools_H2TauTau_VBFMVA2012_H
#define CMGTools_H2TauTau_VBFMVA2012_H

#include <math.h> 
#include "TMath.h" 
#include <limits>
#include <string>
#include "TMVA/Reader.h"


class VBFMVA2012 {
public:

  VBFMVA2012 () {}

  VBFMVA2012 (const char * weightsFile) ;

  ~VBFMVA2012 () ;
  
  double val (
    double mjj      , // the invariant mass of the two tag jets
    double dEta     , // the pseudorapidity difference between the two tag jets
    double C1       , // the pseudorapidity difference between the *visible* di-tau vector and the closest tag jet
    double C2         // the *visible* pT of the di-tau
  ) ;

private:

  void init (const char * weightsFile) ;

  std::vector<Float_t> vbfvars_ ;
  TMVA::Reader *reader_ ;
};




#endif 


