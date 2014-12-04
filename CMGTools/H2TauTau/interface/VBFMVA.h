#ifndef CMGTools_H2TauTau_VBFMVA_H
#define CMGTools_H2TauTau_VBFMVA_H

#include <math.h> 
#include "TMath.h" 
#include <limits>
#include <string>
#include "TMVA/Reader.h"


class VBFMVA {
public:

  VBFMVA () {}

  VBFMVA (const char * weightsFile) ;

  ~VBFMVA () ;
  
  double val (
    double mjj      , // the invariant mass of the two tag jets
    double dEta     , // the pseudorapidity difference between the two tag jets
    double dPhi     , // the phi difference between the two tag jets
    double ditau_pt , // the vector sum of the pT of the tau + electron/muon + MET
    double dijet_pt , // the vector sum of the pT of the two tag jets
    double dPhi_hj  , // the phi difference between the di-tau vector and the di-jet vector
    double C1       , // the pseudorapidity difference between the *visible* di-tau vector and the closest tag jet
    double C2         // the *visible* pT of the di-tau
  ) ;

private:

  void init (const char * weightsFile) ;

  std::vector<Float_t> vbfvars_ ;
  TMVA::Reader *reader_ ;
};




#endif 


