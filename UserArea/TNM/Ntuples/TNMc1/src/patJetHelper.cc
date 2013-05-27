//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for pat::Jet
// Created:     Tue May  8 17:05:51 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/patJetHelper.h"

#include <fastjet/PseudoJet.hh>
#include "Njettiness.hh"
#include "GeneralizedEnergyCorrelator.hh"

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
/*  if(labelname=="selectedPatJets")
      event->getByLabel("patJetCorrFactors", jcf_);
  if(labelname=="selectedPatJetsCHS")
      event->getByLabel("patJetCorrFactorsCHS", jcf_);
*/}

// -- Called once per object
void JetHelper::analyzeObject()
{
  // write only jets with pt > 15:

  if (!(object->pt() > 30)) {
    count = 0;
  }
/*
  if((labelname=="selectedPatJets")||(labelname=="selectedPatJetsCHS"))
  {
    const pat::JetCorrFactors* jcf=*jcf_[object];
    object->addJECFactors(*jcf);
    std::vector<std::string> levels = jcf->correctionLabels();
    if(std::find(levels.begin(), levels.end(), "L2L3Residual")!=levels.end())
      object->initializeJEC(jcf->jecLevel("L2L3Residual"));
    else if(std::find(levels.begin(), levels.end(), "L3Absolute")!=levels.end())
      object->initializeJEC(jcf->jecLevel("L3Absolute"));
  }
*/
}

float JetHelper::getPt(unsigned int num) const
{
    if(object->getPFConstituents().size()>num)
    {
        const reco::PFCandidate *thisParticle = object->getPFConstituent(num).get();
        return thisParticle->pt();
    }
    return 0;
}

float JetHelper::getChargedPt(unsigned int num) const
{
    unsigned int counter=-1;
    for (unsigned k =0; k < object->getPFConstituents().size(); k++)
    {
        const reco::PFCandidate *thisParticle = object->getPFConstituent(k).get();
        if(thisParticle->charge()!=0)
	    counter++;
	if(num==counter)
            return thisParticle->pt();
    }
    return 0;
}

int JetHelper::getNcharged(double relPt) const
{
    int n=0;
    for (unsigned k =0; k < object->getPFConstituents().size(); k++)
    {
        const reco::PFCandidate *thisParticle = object->getPFConstituent(k).get();
        if((thisParticle->charge()!=0)&&(thisParticle->pt()>relPt*object->pt()))
            n++;
    }
    return n; 
}

int JetHelper::getNneutral(double relPt) const
{
    int n=0;
    for (unsigned k =0; k < object->getPFConstituents().size(); k++)
    {
        const reco::PFCandidate *thisParticle = object->getPFConstituent(k).get();
        if((thisParticle->charge()==0)&&(thisParticle->pt()>relPt*object->pt()))
            n++;
    }
    return n; 
}

int JetHelper::genNCHS() const
{
    int n=0;
    double vertexZ=-1000;
    vector<const reco::GenParticle*> all_particles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0))
            n++;
    }
    return n; 
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
    NsubParameters paraNsub = NsubParameters(1.0, 0.8); //assume R=0.8 jet clusering used
    Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}

float JetHelper::getGenTau(int num, double minPt, bool CHS) const
{
    double vertexZ=-1000;
    vector<const reco::GenParticle*> all_particles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
           all_particles.push_back( p );    
    }
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned particle = 0; particle < all_particles.size(); particle++){
        const reco::GenParticle *thisParticle = all_particles.at(particle);
	if(thisParticle->pt()>minPt)
          FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
    }
    NsubParameters paraNsub = NsubParameters(1.0, 0.8); //assume R=0.8 jet clusering used
    Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}


float JetHelper::getC2beta(float beta) const
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
    if(all_particles.size()<2) return -1;
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned particle = 0; particle < all_particles.size(); particle++){
        const reco::PFCandidate *thisParticle = all_particles.at(particle);
        FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
    }
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 2.0);
    fastjet::ClusterSequence clust_seq(FJparticles, jet_def);
    vector<fastjet::PseudoJet> incluisve_jets = clust_seq.inclusive_jets(0);
    fastjet::GeneralizedEnergyCorrelatorRatio C2beta(2,beta,fastjet::pT_R);
    return C2beta(incluisve_jets[0]);
}

float JetHelper::getGenC2beta(float beta, bool CHS) const
{
    double vertexZ=-1000;
    vector<const reco::GenParticle*> all_particles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
           all_particles.push_back( p );    
    }
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned particle = 0; particle < all_particles.size(); particle++){
        const reco::GenParticle *thisParticle = all_particles.at(particle);
        FJparticles.push_back( fastjet::PseudoJet( thisParticle->px(), thisParticle->py(), thisParticle->pz(), thisParticle->energy() ) );	
    }
    if(all_particles.size()<2) return -1;
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 2.0);
    fastjet::ClusterSequence clust_seq(FJparticles, jet_def);
    vector<fastjet::PseudoJet> incluisve_jets = clust_seq.inclusive_jets(0);
    fastjet::GeneralizedEnergyCorrelatorRatio C2beta(2,beta,fastjet::pT_R);
    return C2beta(incluisve_jets[0]);
}

float JetHelper::getJetCharge(float kappa) const
{
    float val=0;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
       {
          const reco::PFCandidate* p=object->getPFConstituent(k).get();
          val += p->charge()*pow(p->pt(),kappa);
       }
    } else {
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
	  {
             const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
             val += p->charge()*pow(p->pt(),kappa);
	  }
       }
    }
    return val/object->pt(); 
}


float JetHelper::getGenJetCharge(float kappa, bool CHS) const
{
    double vertexZ=-1000;
    float val=0;
    vector<const reco::GenParticle*> all_particles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
           val += p->charge()*pow(p->pt(),kappa);
    }
    return val/object->pt(); 
}


float JetHelper::getDaughter_0_jetCharge(float kappa) const
{
    float val=0;
    if(object->numberOfDaughters()>0)
    {
       reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(0));
       if (pfSubjet){
       for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
       {
    	  const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
    	  val += p->charge()*pow(p->pt(),kappa);
       }
       val/=pfSubjet->pt();
       }
    }
    return val; 
}

float JetHelper::getDaughter_1_jetCharge(float kappa) const
{
    float val=0;
    if(object->numberOfDaughters()>1)
    {
       reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(1));
       if (pfSubjet){
       for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
       {
    	  const reco::PFCandidate* p=pfSubjet->getPFConstituent(k).get();
    	  val += p->charge()*pow(p->pt(),kappa);
       }
       val/=pfSubjet->pt();
       }
    }
    return val; 
}
