#ifndef CMGTools_H2TauTau_SelectionEfficiency_H
#define CMGTools_H2TauTau_SelectionEfficiency_H

#include <math.h> 
#include "TMath.h" 
#include <limits>



class SelectionEfficiency {
public:
  SelectionEfficiency(){} ;

  /*
    Factors correcting for id and isolation differences between MC and Data
  */  
  //****************
  //parameters taken from AN-11-390 v8, should be for Fall11 MC 
  //*****************  
  double effCorrMu2011AB(double pt,double eta){
    if(fabs(eta)<1.479) {//Barrel
      if(10.0<pt&&pt<=15.0) return 0.92;
      if(15.0<pt&&pt<=20.0) return 0.948;
      if(20.0<pt) return 0.9933;
      return 0;
    }
    //Endcap
    if(10.0<pt&&pt<=15.0) return 0.98;
    if(15.0<pt&&pt<=20.0) return 0.962;
    if(20.0<pt) return 0.9982;
    return 0;
  }

private:
  
} ;
#endif 


