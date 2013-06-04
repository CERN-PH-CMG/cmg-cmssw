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

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include "ShapeWithComponents.hh"
#include "GenericSubtractor.hh"
//-----------------------------------------------------------------------------
using namespace std;
using namespace pat;
//-----------------------------------------------------------------------------

using namespace fastjet;

class NSubjettinessRatio : public contrib::ShapeWithComponents{
public:
  NSubjettinessRatio(int N) : _N(N){
    assert(_N>1);
  }

  // a (rather loosy) description
  virtual std::string description() const{ return "N-subjettiness ratio from components";}

  /// returns the number of components 
  virtual unsigned int n_components() const { return 2;}

  /// computes individually tau_N and tau_{N-1}
  virtual std::vector<double> components(const PseudoJet &jet) const{
    vector<double> comp(n_components());
    NsubParameters paraNsub = NsubParameters(1.0, 0.8); //assume R=0.8 jet clusering used
    Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
    comp[0] = routine.getTau(_N, jet.constituents()); 
    comp[1] = routine.getTau(_N-1, jet.constituents()); 
    return comp;
  }

  /// given the components, determine the result of the event shape
  virtual double result_from_components(const std::vector <double> &components) const{
    return components[0]/components[1];
  }

protected:
  const int _N;
};


// This constructor is called once per job
JetHelper::JetHelper()
  : HelperFor<pat::Jet>() {}
    
JetHelper::~JetHelper() {}

// -- Called once per event
void JetHelper::analyzeEvent()
{
  event->getByLabel(labelname, jets_);
  event->getByLabel("genParticlesForJetsNoNu", genParticles_);
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
    vector<fastjet::PseudoJet> FJparticles;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
          FJparticles.push_back( fastjet::PseudoJet( object->getPFConstituent(k)->px(), object->getPFConstituent(k)->py(), object->getPFConstituent(k)->pz(), object->getPFConstituent(k)->energy() ));
    } else {
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
              FJparticles.push_back( fastjet::PseudoJet( pfSubjet->getPFConstituent(k)->px(), pfSubjet->getPFConstituent(k)->py(), pfSubjet->getPFConstituent(k)->pz(), pfSubjet->getPFConstituent(k)->energy() ));
       }
    }
    NsubParameters paraNsub = NsubParameters(1.0, 0.8); //assume R=0.8 jet clusering used
    Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}

float JetHelper::getGenTau(int num, double minPt, bool CHS) const
{
    double vertexZ=-1000;
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(p) {
         if(vertexZ==-1000)
             vertexZ=p->vz();
         if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
	  if(p->pt()>minPt)
            FJparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
       } else {
         reco::GenJet const *pfSubjet = dynamic_cast <const reco::GenJet *>(object->daughter(j));
         if (!pfSubjet) break;
         for (unsigned k =0; k < pfSubjet->numberOfDaughters(); k++)
	 {
          reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(pfSubjet->daughter(k));
	  if(p->pt()>minPt)
            FJparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
	 }
       }
    }
    NsubParameters paraNsub = NsubParameters(1.0, 0.8); //assume R=0.8 jet clusering used
    Njettiness routine(Njettiness::onepass_kt_axes, paraNsub);
    return routine.getTau(num, FJparticles); 
}

float JetHelper::genTau21PUcorrected(double minPt, bool CHS) const
{
    double vertexZ=-1000;
    vector<fastjet::PseudoJet> FJparticles;
    vector<fastjet::PseudoJet> HARDparticles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
	if(p->pt()>minPt)
          FJparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
       if((abs(p->vz()-vertexZ)<0.1))
	if(p->pt()>minPt)
          HARDparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
    }

    // background estimation
    vector<fastjet::PseudoJet> Allparticles;
    for (unsigned i = 0; i < genParticles_->size(); i++){
        reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(&*genParticles_->refAt(i));
        if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
           if(p->pt()>minPt)
               Allparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
    }
    JetDefinition jet_def_for_rho(kt_algorithm, 0.6);
    AreaDefinition area_def(active_area_explicit_ghosts,
    		      GhostedAreaSpec(SelectorAbsRapMax(5.0)));
    Selector rho_range =  SelectorAbsRapMax(4.4);
    JetMedianBackgroundEstimator bge(rho_range, jet_def_for_rho, area_def);
    bge.set_particles(Allparticles);
    
    // background stubstraction
    Subtractor subtractor(&bge);
    contrib::GenericSubtractor gen_sub(&bge);
    //gen_sub.use_common_bge_for_rho_and_rhom();
    fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, 2.0);
    fastjet::ClusterSequenceArea clust_seq(FJparticles, jet_def, area_def);
    vector<fastjet::PseudoJet> incluisve_jets = clust_seq.inclusive_jets(0);
    NSubjettinessRatio tau21(2);
    
    // validation
    //std::cerr << "rho" << bge.rho() << std::endl;
    //fastjet::ClusterSequenceArea clust_seq_hard(HARDparticles, jet_def, area_def);
    //vector<fastjet::PseudoJet> hard_jets = clust_seq_hard.inclusive_jets(0);
    //std::cerr << "hard" << (hard_jets[hard_jets.size()-1].pt()) << " orig" << (incluisve_jets[incluisve_jets.size()-1].pt()) << " corrected" << (subtractor(incluisve_jets[incluisve_jets.size()-1]).pt()) << std::endl;
    //std::cerr << "orig" << tau21(incluisve_jets[incluisve_jets.size()-1]) << " corrected" << gen_sub(tau21, incluisve_jets[incluisve_jets.size()-1]) << std::endl;

    return gen_sub(tau21, incluisve_jets[incluisve_jets.size()-1]);
}


float JetHelper::getC2beta(float beta) const
{
    vector<fastjet::PseudoJet> FJparticles;
    if(object->isPFJet())
    {
       for (unsigned k =0; k < object->getPFConstituents().size(); k++)
          FJparticles.push_back( fastjet::PseudoJet( object->getPFConstituent(k)->px(), object->getPFConstituent(k)->py(), object->getPFConstituent(k)->pz(), object->getPFConstituent(k)->energy() ));
    } else {
       for (unsigned j = 0; j < object->numberOfDaughters(); j++){
          reco::PFJet const *pfSubjet = dynamic_cast <const reco::PFJet *>(object->daughter(j));
          if (!pfSubjet) break;
          for (unsigned k =0; k < pfSubjet->getPFConstituents().size(); k++)
              FJparticles.push_back( fastjet::PseudoJet( pfSubjet->getPFConstituent(k)->px(), pfSubjet->getPFConstituent(k)->py(), pfSubjet->getPFConstituent(k)->pz(), pfSubjet->getPFConstituent(k)->energy() ));
       }
    }
    if(FJparticles.size()<2) return -1;
    fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, 2.0);
    fastjet::ClusterSequence clust_seq(FJparticles, jet_def);
    vector<fastjet::PseudoJet> incluisve_jets = clust_seq.inclusive_jets(0);
    fastjet::GeneralizedEnergyCorrelatorRatio C2beta(2,beta,fastjet::pT_R);
    return C2beta(incluisve_jets[0]);
}

float JetHelper::getGenC2beta(float beta, bool CHS) const
{
    double vertexZ=-1000;
    vector<fastjet::PseudoJet> FJparticles;
    for (unsigned j = 0; j < object->numberOfDaughters(); j++){
       reco::GenParticle const *p = dynamic_cast <const reco::GenParticle *>(object->daughter(j));
       if(vertexZ==-1000)
           vertexZ=p->vz();
       if((abs(p->vz()-vertexZ)<0.1)||(p->charge()==0)||(!CHS))
           FJparticles.push_back( fastjet::PseudoJet( p->px(), p->py(), p->pz(), p->energy() ));
    }
    if(FJparticles.size()<2) return -1;
    fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, 2.0);
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
