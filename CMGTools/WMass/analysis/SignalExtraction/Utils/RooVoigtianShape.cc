#include "RooFit.h"

#include "Riostream.h"
#include <math.h>

#include "RooVoigtianShape.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooMath.h"
#include "TMath.h"

ClassImp(RooVoigtianShape);


//_____________________________________________________________________________
RooVoigtianShape::RooVoigtianShape(const char *name, const char *title,
		       RooAbsReal& _m, RooAbsReal& _m0, RooAbsReal& _sigma,
				   RooAbsReal& _alpha, RooAbsReal& _n,RooAbsReal& _w,Bool_t doFast) :
  RooAbsPdf(name, title),
  m("m", "Dependent", this, _m),
  m0("m0", "M0", this, _m0),
  sigma("sigma", "Sigma", this, _sigma),
  alpha("alpha", "Alpha", this, _alpha),
  n("n", "Order", this, _n),
  width("w","width",this,_w),
  _doFast(doFast)
{
   _invRootPi= 1./sqrt(atan2(0.,-1.));
}


//_____________________________________________________________________________
RooVoigtianShape::RooVoigtianShape(const RooVoigtianShape& other, const char* name) :
  RooAbsPdf(other, name), m("m", this, other.m), m0("m0", this, other.m0),
  sigma("sigma", this, other.sigma), alpha("alpha", this, other.alpha),
  n("n", this, other.n),width("w",this,other.width),_doFast(other._doFast)
{
  _invRootPi= 1./sqrt(atan2(0.,-1.));
}

Double_t RooVoigtianShape::voigtian(double iX) const {
  Double_t s = (sigma>0) ? sigma : -sigma ;
  Double_t w = (width>0) ? width : -width ;
  
  Double_t coef= -0.5/(s*s);
  Double_t arg = iX - m0;
  
  // return constant for zero width and sigma
  if (s==0. && w==0.) return 1.;

  // Breit-Wigner for zero sigma
  if (s==0.) return (1./(arg*arg+0.25*w*w));

  // Gauss for zero width
  if (w==0.) return exp(coef*arg*arg);

  // actual Voigtian for non-trivial width and sigma
  Double_t c = 1./(sqrt(2.)*s);
  Double_t a = 0.5*c*w;
  Double_t u = c*arg;
  RooComplex z(u,a) ;
  RooComplex v(0.) ;

  if (_doFast) {
    v = RooMath::FastComplexErrFunc(z);
  } else {
    v = RooMath::ComplexErrFunc(z);
  }
  return c*_invRootPi*v.re();
}

//_____________________________________________________________________________
Double_t RooVoigtianShape::evaluate() const {
  
  Double_t t = (m-m0)/sigma;
  if (alpha < 0) t = -t;

  Double_t absAlpha  = fabs((Double_t)alpha);
  Double_t crossOver = sigma*alpha+m0;

  if (t >= -absAlpha) {
    return voigtian(m);
  }
  else {
    Double_t a =  TMath::Power(n/absAlpha,n)*voigtian(crossOver);
    Double_t b= n/absAlpha - absAlpha;
    return a/TMath::Power(b - t, n);
  }
}

