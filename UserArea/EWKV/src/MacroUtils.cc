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

    //
    bool isBhadron(int pdgId)
    {
      int absid=abs(pdgId);
      return ( (absid>= 5122 && absid<= 5554) ||    //baryons
	       (absid>=20513 && absid<=20543) ||    //mesons
	       (absid>=10511 && absid<=10543) || 
	       (absid>=  511 && absid<=  545) ||
	       (absid>=  551 && absid<=  557) ||    //bbar mesons
	       (absid>=10551 && absid<=10557) ||
	       (absid>=100551 && absid<=100557) ||
	       (absid>=110551 && absid<=110557) ||
	       (absid>=200551 && absid<=200557) ||
	       (absid>=210551 && absid<=210557) );
    }

    Float_t getEffectiveArea(int id,float eta)
    {
      Float_t Aeff(1.0);
      if(abs(id)==1)
	{
	  if(fabs(eta)<1.0) Aeff=0.13;
	  else if(fabs(eta)>1.0 && fabs(eta)<1.479) Aeff=0.14;
	  else if(fabs(eta)>1.479 && fabs(eta)<2.0) Aeff=0.07;
	  else if(fabs(eta)>2.0 && fabs(eta)<2.2)   Aeff=0.09;
	  else if(fabs(eta)>2.2 && fabs(eta)<2.3)   Aeff=0.11;
	  else if(fabs(eta)>2.3 && fabs(eta)<2.4)   Aeff=0.11;
	  else Aeff=0.14;
	}
      return Aeff;
    }
  }
}
