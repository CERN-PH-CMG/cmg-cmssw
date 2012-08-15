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
  event->getByLabel(labelname+"puJetId", puJetId_);
  event->getByLabel(labelname, jets_);
}

// -- Called once per object
void JetHelper::analyzeObject()
{
  // write only jets with pt > 15:

  if (!(object->pt() > 15)) {
    count = 0;
  }

}

float JetHelper::getTau(int num) const
{
    std::cout << "find particles" << std::endl;
    vector<const reco::PFCandidate*> all_particles;
    std::cout << "in PFjet" << std::endl;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
          all_particles.push_back( object->getPFConstituent(k).get() );
    } else {
       std::cout << "in constituents" << std::endl;
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
	     all_particles.push_back( pfSubjet->getPFConstituent(k).get() );	
       }
    }
    std::cout << "make fastjet particle list" << std::endl;
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned particle = 0; particle < all_particles.size(); particle++){
        const reco::PFCandidate *thisParticle = all_particles.at(particle);
        FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
    }
    NsubParameters paraNsub = NsubParameters(1.0, 0.5); //assume R=0.5 jet clusering used
    std::cout << "run n-subjetiness" << std::endl;
    Nsubjettiness routine(nsub_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}
