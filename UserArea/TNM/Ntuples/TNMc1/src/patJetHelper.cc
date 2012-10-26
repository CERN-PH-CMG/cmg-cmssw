//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Jet
// Created:     Tue May  8 17:05:51 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patJetHelper.h"

#include <fastjet/PseudoJet.hh>
#include "Nsubjettiness.h"

//-----------------------------------------------------------------------------
using namespace std;
using namespace pat;
//-----------------------------------------------------------------------------
// This constructor is called once per job
JetHelper::JetHelper()
  : HelperFor<pat::Jet>() {}
    
JetHelper::~JetHelper() {}

// -- Called once per event
void JetHelper::analyzeEvent()
{
  event->getByLabel(labelname, jets_);
  //event->getByLabel(labelname+"puJetId", puJetId_);
  //event->getByLabel("ak5PFJetsCHSprunedSubJetspuJetId", subJetpuJetId_);
}

// -- Called once per object
void JetHelper::analyzeObject()
{
  // write only jets with pt > 15:

  if (!(object->pt() > 30)) {
    count = 0;
  }

}

float JetHelper::getTau(int num) const
{
    vector<const reco::PFCandidate*> all_particles;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
          all_particles.push_back( object->getPFConstituent(k).get() );
    } else {
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
	     all_particles.push_back( pfSubjet->getPFConstituent(k).get() );	
       }
    }
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned particle = 0; particle < all_particles.size(); particle++){
        const reco::PFCandidate *thisParticle = all_particles.at(particle);
        FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
    }
    NsubParameters paraNsub = NsubParameters(1.0, 0.7); //assume R=0.7 jet clusering used
    Nsubjettiness routine(nsub_1pass_from_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}

float JetHelper::jetCharge() const
{
    float kappa=0.3;
    float val=0;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
       {
          const reco::PFCandidate* p=object->getPFConstituent(k).get();
          val += p->charge()*pow(p->energy(),kappa);
       }
    } else {
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
	  {
             const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
             val += p->charge()*pow(p->energy(),kappa);
	  }
       }
    }
    return val/object->energy(); 
}


float JetHelper::daughter_0_jetCharge() const
{
    float kappa=0.3;
    float val=0;
    if(object->numberOfDaughters()>0)
    {
       reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(0));
       if (pfSubjet){
       for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
       {
    	  const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
    	  val += p->charge()*pow(p->energy(),kappa);
       }
       val/=pfSubjet->energy();
       }
    }
    return val; 
}

float JetHelper::daughter_1_jetCharge() const
{
    float kappa=0.3;
    float val=0;
    if(object->numberOfDaughters()>1)
    {
       reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(1));
       if (pfSubjet){
       for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
       {
    	  const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
    	  val += p->charge()*pow(p->energy(),kappa);
       }
       val/=pfSubjet->energy();
       }
    }
    return val; 
}
