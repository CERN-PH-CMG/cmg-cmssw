/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooVoigtianShape.h,v 1.1 2012/11/26 16:26:42 perrozzi Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_VOIGTIAN_SHAPE
#define ROO_VOIGTIAN_SHAPE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooVoigtianShape : public RooAbsPdf {
public:
  RooVoigtianShape() {}
  RooVoigtianShape(const char *name, const char *title, RooAbsReal& _m,
	     RooAbsReal& _m0, RooAbsReal& _sigma,
		   RooAbsReal& _alpha, RooAbsReal& _n,RooAbsReal& _width,Bool_t doFast);

  RooVoigtianShape(const RooVoigtianShape& other, const char* name = 0);
  virtual TObject* clone(const char* newname) const { return new RooVoigtianShape(*this,newname); }

  inline virtual ~RooVoigtianShape() { }

protected:


  RooRealProxy m;
  RooRealProxy m0;
  RooRealProxy sigma;
  RooRealProxy alpha;
  RooRealProxy n;
  RooRealProxy width;
  Double_t evaluate() const;
  Double_t voigtian(Double_t iX) const;
private:
  Bool_t   _doFast;
  Double_t _invRootPi;
  ClassDef(RooVoigtianShape,1) // Crystal Ball lineshape PDF
};

#endif
