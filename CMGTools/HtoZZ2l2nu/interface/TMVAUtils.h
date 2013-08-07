#ifndef TMVAUtils_h
#define TMVAUtils_h

#include<iostream>
#include<vector>

#include "TString.h"
#include "TCut.h"
#include "TH1D.h" 

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/Factory.h"

namespace tmva
{
  //books regular TMVA methods with default options
  TMVA::MethodBase *bookMethodWithCategories(TMVA::Factory *factory,TString method,std::vector<TCut> &categDef, std::vector<TString> &categVars);

  //overloaded for single categories
  TMVA::MethodBase *bookMethod(TMVA::Factory *factory, TString method);

  //standard discriminator histograms
  TH1D *getHistogramForDiscriminator(TString method);
}

#endif
