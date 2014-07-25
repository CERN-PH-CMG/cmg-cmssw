#ifndef __ROO_MYPDF_RDL__
#define __ROO_MYPDF_RDL__

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;
class RooAbsReal;

class RhhCruijffPdf : public RooAbsPdf {
public:
  RhhCruijffPdf(const char *name, const char *title, RooAbsReal& _m,
		RooAbsReal& _m0, 
		RooAbsReal& _sigmaL, RooAbsReal& _sigmaR,
		RooAbsReal& _alphaL, RooAbsReal& _alphaR) ;
  
  RhhCruijffPdf(const RhhCruijffPdf& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { 
    return new RhhCruijffPdf(*this,newname); }

  inline virtual ~RhhCruijffPdf() { }

protected:

  RooRealProxy m;
  RooRealProxy m0;
  RooRealProxy sigmaL;
  RooRealProxy sigmaR;
  RooRealProxy alphaL;
  RooRealProxy alphaR;

  Double_t evaluate() const;

private:
  
  ClassDef(RhhCruijffPdf,0)
};

#endif
