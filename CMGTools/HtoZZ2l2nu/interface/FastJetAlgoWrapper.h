#ifndef FastJetAlgoWrapper_C
#define FastJetAlgoWrapper_C

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "RecoJets/JetProducers/interface/JetSpecific.h"

#include "fastjet/ClusterSequence.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "math.h"

#include <iostream>

using namespace std;
using namespace fastjet;


bool pf2patFastJetAlgoWrapperJetPtOrdering (reco::PFJet i,reco::PFJet j) { return (i.pt()>j.pt()); }

namespace pf2pat {


class FastJetAlgoWrapper{
       public:          

	  typedef std::vector< fastjet::PseudoJet > PseudoJetCollection;
	  typedef PseudoJetCollection::const_iterator PJI;

	  FastJetAlgoWrapper ( const edm::ParameterSet& ps ); 
  
	  
	  /// get jet definition from parameter set
	  /// set user defined jet definition
	  void setJetDefinition( const edm::ParameterSet& ps);  
	  void setJetDefinition( const fastjet::JetDefinition& jetDef);

	  void resetJetDefinition(); ////////// Function used to reset the jetdefinition 
	  
	  /// run the jet clustering on the input collection, and produce the reco jets
          const std::vector<reco::PFJet>& produce( const std::vector< reco::PFJet >  & inputColl, double ptMin_);


       private:
          void recoJetToFastJet(const std::vector< reco::PFJet >  & inputColl);
	  
	  /// run fast jet
	  void runJetClustering( double ptMin_); 
	  
	  /// convert fastjet output to RECO data format (e.g. PFJet)
	  const std::vector<reco::PFJet>& fastJetToReco();
	  
	  /// build a JetType (e.g. PFJet) from a pseudo-jet.
	  reco::PFJet makeJet( const fastjet::PseudoJet& pseudoJet) const;
	  
	  /// build the vector< Ptr<Candidate> > pointing to constituents
	  /// from the PseudoJet and the Handle information.
	  reco::Jet::Constituents makeConstituents(const fastjet::PseudoJet& pseudoJet) const; 
	  
	  /// fastjet
	  PseudoJetCollection  input_;
	  PseudoJetCollection  output_;

          /// input jet collection
          std::vector< reco::PFJet > inputJetCollection_;
	  
	  /// output jet collection
	  std::vector<reco::PFJet> jetCollection_; 
	  
	  /// definition of the algorithm, and of the algorithm parameters
	  fastjet::JetDefinition  jetDefinition_;
	  
	  /// cluster sequence
	  fastjet::ClusterSequence* clusterSequence_;

	  //fast jet parameters saved to be able to change them when forcing two jets with the inclusive algorithm  
	  unsigned algorithm_;
	  double distance_;
	  double extra_param_;
	  int recomb_scheme_;
	  int strategy_;
};

reco::Jet::Constituents FastJetAlgoWrapper::makeConstituents(const fastjet::PseudoJet& pseudoJet) const {
      reco::Jet::Constituents ptrsToConstituents;
      const PseudoJetCollection& constituents = clusterSequence_->constituents(pseudoJet);
      for(PJI jc=constituents.begin(); jc!=constituents.end(); ++jc) {
         for(unsigned int i=0;i<inputJetCollection_[jc->user_index()].getPFConstituents().size();i++){
             ptrsToConstituents.push_back( inputJetCollection_[jc->user_index()].getPFConstituent(i) );
         }
      }
      return ptrsToConstituents;
}


const std::vector< reco::PFJet >& FastJetAlgoWrapper::produce( const std::vector< reco::PFJet > & inputHandle, double ptMin_) {
       // the input handle will be necessary to build the Ptrs to the jet constituents.
       inputJetCollection_ = inputHandle;
       recoJetToFastJet( inputHandle );
       runJetClustering(ptMin_);
       return fastJetToReco();
}


}
using namespace pf2pat;


/////// Constructor with the parametr set
FastJetAlgoWrapper::FastJetAlgoWrapper( const edm::ParameterSet& ps ) : clusterSequence_(0) {
  setJetDefinition( ps ); 
}

///Jet definition using the parameter set
void FastJetAlgoWrapper::setJetDefinition( const edm::ParameterSet& ps) {
  // here extract parameter set info to make the jet definition

  algorithm_ = ps.getParameter<unsigned>("fj_algo");
  distance_ = ps.getParameter<double>("distance_par");
  extra_param_ = ps.getParameter<double>("extra_param");
  recomb_scheme_ = ps.getParameter<int>("recombination_scheme");
  strategy_ = ps.getParameter<int>("strategy");

  JetDefinition *jetdefptr;
  JetDefinition jetDef;
  if(algorithm_ == 3){
    JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), distance_, extra_param_, static_cast<RecombinationScheme>(recomb_scheme_), static_cast<Strategy>(strategy_));
    jetdefptr = &jetdef1;
  }else{
    JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), distance_, static_cast<RecombinationScheme>(recomb_scheme_), static_cast<Strategy>(strategy_));
    jetdefptr = &jetdef1;
  }
  jetDef = *jetdefptr;
  setJetDefinition( jetDef ); 
}

///// jet definition using the fastjet jet definition 
void FastJetAlgoWrapper::setJetDefinition( const fastjet::JetDefinition& jetDef) {
  //cout<<jetDef.description()<<endl;
    jetDefinition_ = jetDef; 
}

/////////// reset jet definition in needed to call fastjet several times when running
void FastJetAlgoWrapper::resetJetDefinition() {
   JetDefinition *jetdefptr;
   JetDefinition jetDef;
   if(algorithm_ == 3){
     JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), distance_, extra_param_, static_cast<RecombinationScheme>(recomb_scheme_), static_cast<Strategy>(strategy_));
     jetdefptr = &jetdef1;
   }else{
     JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), distance_, static_cast<RecombinationScheme>(recomb_scheme_), static_cast<Strategy>(strategy_));
     jetdefptr = &jetdef1;
   }
   jetDef = *jetdefptr;
   setJetDefinition( jetDef ); 
}


///////// especialization of reco to fast jet
void FastJetAlgoWrapper::recoJetToFastJet(const std::vector< reco::PFJet >& inputColl) {
  input_.clear();
  unsigned i;
  for(i=0; i!=inputColl.size(); ++i) {
    input_.push_back( PseudoJet( inputColl[i].px(), inputColl[i].py(), inputColl[i].pz(), inputColl[i].energy() ) );
    input_.back().set_user_index( i );
  }
}

///// inclusive fast jet
void  FastJetAlgoWrapper::runJetClustering( double ptMin_ ) {
  output_.clear();
  if(clusterSequence_) delete clusterSequence_;
  clusterSequence_ = new ClusterSequence(input_, jetDefinition_);
  //cout << "calling inclusive " << endl;
  output_ = clusterSequence_->inclusive_jets( ptMin_ );
}


////////// converting fastjets into reco jets
const std::vector< reco::PFJet >& FastJetAlgoWrapper::fastJetToReco() {
  jetCollection_.clear();
  for(PJI i=output_.begin(); i!=output_.end(); ++i) {
    jetCollection_.push_back( makeJet( *i ) ); 
  }
 
  //order the jet vector
  std::sort(jetCollection_.begin(), jetCollection_.end(), pf2patFastJetAlgoWrapperJetPtOrdering); 
  return jetCollection_;
}

///////making the jets
reco::PFJet FastJetAlgoWrapper::makeJet( const PseudoJet& pseudoJet) const { 
  reco::Particle::LorentzVector p4( pseudoJet.px(),    pseudoJet.py(),   pseudoJet.pz(),   pseudoJet.E() );
  reco::Particle::Point vertex; 
  reco::PFJet::Specific specific; 
  // need to add the constituents as well (see base Jet, or CompositePtrCandidate)
  reco::Jet::Constituents ptrsToConstituents = makeConstituents( pseudoJet );
  makeSpecific( ptrsToConstituents, &specific );
  return reco::PFJet(p4, vertex, specific, ptrsToConstituents); 
}


#endif
