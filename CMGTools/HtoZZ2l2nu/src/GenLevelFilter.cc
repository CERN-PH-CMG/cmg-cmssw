#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace gen{

  //
  std::map<std::string, std::vector<reco::CandidatePtr> > filter(edm::Handle<edm::View<reco::Candidate> > &hGen, const edm::ParameterSet &iConfig)
  {
    std::vector<reco::CandidatePtr> particles;
    std::map<std::string, std::vector<reco::CandidatePtr> > mothers, daughters, finalstates;
    
    try
      {
	int filterId = iConfig.getParameter<int>("filterId");

	//what follows is rather inneficient but we need to store CandidatePtr 

	//identify the particles of interest (require status=3)
	for(size_t igen=0; igen<hGen.product()->size(); igen++)
	  {
	    reco::CandidatePtr genPtr = hGen->ptrAt(igen);
	    if(genPtr->status()!=3) continue;
	    int pdgid=genPtr->pdgId();
	    if(fabs(pdgid)!= filterId) continue;
	    particles.push_back(genPtr);
	  }

	if(particles.size())
	  {
	    
	    //check the mothers and daugthers
	    for(size_t igen=0; igen<hGen.product()->size(); igen++)
	      {
		reco::CandidatePtr genPtr = hGen->ptrAt(igen);
		
		//check daughters
		bool foundParticleInDaughters(false);
		for(size_t idau=0; idau<genPtr->numberOfDaughters(); idau++)
		  {
		    const reco::Candidate *daughter = genPtr->daughter(idau);
		    for(size_t ipart =0; ipart<particles.size(); ipart++)
		      {
			if(daughter != particles[ipart].get() ) continue;
			char buf[20];
			sprintf(buf,"genmother_%d",int(ipart));
			if( mothers.find(buf)== mothers.end() ) mothers[buf]=std::vector<reco::CandidatePtr>();
			mothers[buf].push_back(genPtr);
			foundParticleInDaughters=true;
			break;
		      }
		    if(!foundParticleInDaughters) continue;
		    break;
		  }
		if(foundParticleInDaughters) continue;
		
		//check mothers
		bool foundParticleInMothers(false);
		for(size_t imother=0; imother<genPtr->numberOfMothers(); imother++)
		  {
		    const reco::Candidate *mother = genPtr->mother(imother);
		    for(size_t ipart =0; ipart<particles.size(); ipart++)
		      {
			if(mother != particles[ipart].get() ) continue;
			if(genPtr->pdgId() == particles[ipart]->pdgId()) continue;
			char buf[20];
			sprintf(buf,"gendaughter_%d",int(ipart));
			if( daughters.find(buf)== daughters.end() ) daughters[buf]=std::vector<reco::CandidatePtr>();
			daughters[buf].push_back(genPtr);
			foundParticleInMothers=true;
			break;
		      }
		    if(!foundParticleInMothers) continue;
		    break;
		  }
	      }
	    
	    //check the final states
	    if(daughters.size())
	      {
		for(size_t igen=0; igen<hGen.product()->size(); igen++)
		  {
		    reco::CandidatePtr genPtr = hGen->ptrAt(igen);
		    
		    //check mothers
		    bool foundParticleInMothers(false);
		    for(std::map<std::string, std::vector<reco::CandidatePtr> >::iterator it = daughters.begin();
			it != daughters.end();
			it++)
		      {
			std::string tag = it->first;
			std::vector<reco::CandidatePtr> &idaughters = it->second;
			
			//match the mothers
			for(size_t imother=0; imother<genPtr->numberOfMothers(); imother++)
			  {
			    const reco::Candidate *mother = genPtr->mother(imother);
			    if( genPtr->pdgId()==mother->pdgId() ) continue;
			    
			    for(size_t ipart =0; ipart<idaughters.size(); ipart++)
			      {
				if( mother->pdgId()!=idaughters[ipart]->pdgId() ) continue;
				double dR = deltaR( *mother, *(idaughters[ipart].get()) );
				if(dR>0.1) continue;
				
				char buf[20];
				sprintf(buf,"_%d",int(ipart));
				std::string thetag=tag + buf;
				if( finalstates.find(thetag)== finalstates.end() ) finalstates[thetag]=std::vector<reco::CandidatePtr>();
				finalstates[thetag].push_back(genPtr);
				foundParticleInMothers=true;
				break;
			      }
			    if(!foundParticleInMothers) continue;
			    break;
			  }
			if(foundParticleInMothers) break;
		      }
		  }
	    
	      }
	  }
      }catch(std::exception &e){
      cout << "[gen::filter] failed with : " << e.what() << endl;
    }

    //build the final product
    std::map<std::string, std::vector<reco::CandidatePtr> > genSummary;
    genSummary["genparticle"] = particles;
    genSummary.insert(mothers.begin(),mothers.end());
    genSummary.insert(daughters.begin(),daughters.end());
    genSummary.insert(finalstates.begin(),finalstates.end());
    return genSummary;
  }


  //
  const reco::Candidate *getFinalStateFor(const reco::Candidate *p)
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
	
