/**
 * Scale function classes
 * Author M. De Mattia - 18/11/2008
 * Author S. Casasso   - 25/10/2012
 * Author E. Migliore  - 25/10/2012
 */

#ifndef SCALEFUNCTIONS_H
#define SCALEFUNCTIONS_H

#include <iostream>
#include <vector>
#include <cmath>
#include "TMath.h"
#include "TString.h"
#include "TF1.h"
#include "TRandom.h"

/**
 * Used to define parameters inside the functions.
 */
struct ParameterSet
{
  ParameterSet() {}
  ParameterSet(const TString & inputName, const double & inputStep, const double & inputMini, const double & inputMaxi) :
    step(inputStep),
    mini(inputMini),
    maxi(inputMaxi)
  {
    std::cout << "setting name = " << inputName << std::endl;
    name = inputName;
  }
  TString name;
  double step, mini, maxi;
};



template <class T>
class scaleFunctBase {
 public:
  virtual double scale(const double & pt, const double & eta, const double & phi, const int chg, const T & parScale) const = 0;
  virtual ~scaleFunctBase() = 0;
  virtual int parNum() const { return parNum_; }
 protected:
  int parNum_;
  virtual void setPar(double* Start, double* Step, double* Mini, double* Maxi, int* ind,
		      TString* parname, const T & parResol, const std::vector<int> & parResolOrder, const std::vector<ParameterSet> & parSet ) {
    if( int(parSet.size()) != this->parNum_ ) {
      std::cout << "Error: wrong number of parameter initializations = " << parSet.size() << ". Number of parameters is " << this->parNum_ << std::endl;
      exit(1);
    }
    for( int iPar=0; iPar<this->parNum_; ++iPar ) {
      Start[iPar] = parResol[iPar];
      Step[iPar] = parSet[iPar].step;
      Mini[iPar] = parSet[iPar].mini;
      Maxi[iPar] = parSet[iPar].maxi;
      ind[iPar] = parResolOrder[iPar];
      parname[iPar] = parSet[iPar].name;
    }
  }
};

template <class T> inline scaleFunctBase<T>::~scaleFunctBase() { }  // defined even though it's pure virtual; should be faster this way.

//
// Curvature: (linear eta + sinusoidal in phi (both in 5 eta bins)) * global scale 
// ------------------------------------------------------------
template <class T>
class scaleFunct50 : public scaleFunctBase<T> {
public:
  scaleFunct50() { this->parNum_ = 27; }
  virtual double scale(const double & pt, const double & eta, const double & phi, const int chg, const T & parScale) const {    
    double ampl(0), phase(0), twist(0), ampl2(0), freq2(0), phase2(0);

    // very bwd bin
    if ( eta  < parScale[4] ) {
      ampl = parScale[1]; phase = parScale[2]; ampl2 = parScale[21]; freq2 = parScale[22]; phase2 = parScale[23];
      twist = parScale[3]*(eta-parScale[4])+parScale[7]*(parScale[4]-parScale[8])+parScale[11]*parScale[8]; 
      // bwd bin
    } else if ( parScale[4] <= eta && eta < parScale[8] ) {
      ampl = parScale[5]; phase = parScale[6];
      twist = parScale[7]*(eta-parScale[8])+parScale[11]*parScale[8] ; 
      // barrel bin
    } else if ( parScale[8] <= eta && eta < parScale[12] ) {
      ampl = parScale[9]; phase = parScale[10];
      twist = parScale[11]*eta; 
      // fwd bin
    } else if ( parScale[12] <= eta && eta < parScale[16] ) {
      ampl = parScale[13]; phase = parScale[14];
      twist = parScale[15]*(eta-parScale[12])+parScale[11]*parScale[12]; 
      // very fwd bin
    } else if ( parScale[16] < eta ) {
      ampl = parScale[17]; phase = parScale[18]; ampl2 = parScale[24]; freq2 = parScale[25]; phase2 = parScale[26];
      twist = parScale[19]*(eta-parScale[16])+parScale[15]*(parScale[16]-parScale[12])+parScale[11]*parScale[12]; 
    }
    
    // apply the correction
    double curv = (1.+parScale[0])*((double)chg/pt
				    -twist
				    -ampl*sin(phi+phase)
				    -ampl2*sin(freq2*phi+phase2)
				    -0.5*parScale[20]);
    return 1./((double)chg*curv);
  }

};

/// Service to build the scale functor corresponding to the passed identifier                                                                               
scaleFunctBase<double * > * scaleFunctService( const int identifier ){
  switch ( identifier ) {
  case ( 50 ): return ( new scaleFunct50<double * > ); break;
  default: std::cout << "scaleFunctService error: wrong identifier = " << identifier << std::endl; exit(1);
  }
}



#endif
