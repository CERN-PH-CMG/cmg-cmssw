#include "UserCode/EWKV/interface/MacroUtils.h"

namespace utils
{
  namespace cmssw
  {
    
    //
    const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p)
    {
      if(p==0) return 0;
      
      const reco::Candidate *prevState=p;
      do{	
	const reco::Candidate *nextState=0;
	int nDaughters = prevState->numberOfDaughters();
	for(int iDaughter=0; iDaughter<nDaughters; iDaughter++)
	  {
	    const reco::Candidate *dau = prevState->daughter(iDaughter);
	    if(dau==0) continue;
	    if(dau->pdgId()!= p->pdgId()) continue;
	    nextState=dau;	   
	    break;
	  }
	if(nextState==0) break;
	if(nextState==prevState) break;
	prevState=nextState;
      }while(1);
      
      return prevState;
    }

  }

}
