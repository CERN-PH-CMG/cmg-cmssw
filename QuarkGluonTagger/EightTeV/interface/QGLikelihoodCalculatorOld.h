#ifndef QGLikelihoodCalculatorOld_h
#define QGLikelihoodCalculatorOld_h

#include <string>

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrector.h"

#include <map>
#include <vector>

class QGLikelihoodCalculatorOld{
 public:
   QGLikelihoodCalculatorOld(const TString dataDir, Bool_t chs = false);
   ~QGLikelihoodCalculatorOld();
   Float_t QGvalue(std::map<TString, Float_t>);

 private:
   std::map<TString,JetCorrectorParameters*> JCP;
   std::map<TString,SimpleJetCorrector*> SJC;
   std::vector<TString> names;
};

#endif
