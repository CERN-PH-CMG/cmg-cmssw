#ifndef macroutils_h
#define macroutils_h

#include "DataFormats/Candidate/interface/Candidate.h"

namespace utils
{
  namespace cmssw
  {
    
    //retrieve last state before decay
    const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p);
  }
}

#endif
