#ifndef CMGTools_H2TauTau_TriggerEfficiency_H
#define CMGTools_H2TauTau_TriggerEfficiency_H

#include <math.h> 
#include "TMath.h" 
#include <limits>


class TriggerEfficiency {
public:
  TriggerEfficiency(){} ;
  double ratio(int trigpt, double pt) const ; 

private:
  double efficiency(double m, double m0, double sigma, double alpha, double n, double norm) const ;
} ;
#endif 


