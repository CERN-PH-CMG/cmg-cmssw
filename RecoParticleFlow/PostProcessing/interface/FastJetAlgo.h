#ifndef PhysicsTools_PFCandProducer_FastJetAlgo
#define PhysicsTools_PFCandProducer_FastJetAlgo

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

namespace pf2pat {
  
  //COLIN change name to FastJetAlgo
  template <typename T, typename U>
    class FastJetAlgo {
       public:
	  typedef std::vector< fastjet::PseudoJet > PseudoJetCollection;
	  typedef PseudoJetCollection::const_iterator PJI;
	  typedef std::vector<bool> CollectionMasks;  
	  
          typedef T                                InputType_T;
	  typedef edm::Handle< T >                 InputHandle_T;
	  
	  typedef U                                JetType_U;
	  typedef std::vector< U >                 JetCollection_U; 
	  
	  ///// over load the Fast Jet contructor to be able to call while running
	  FastJetAlgo( const edm::ParameterSet& ps ); 
	  FastJetAlgo( fastjet::JetDefinition& jd); 
	  FastJetAlgo( unsigned fastjetalgo_, double rfastjet_, 
		       double extrapamfastjet_, int recombination_, 
		       int strat_);  ////// Overload to call the constructor using all the fastjet parameters
	  
	  /// get jet definition from parameter set
	  /// set user defined jet definition
	  void setJetDefinition( const edm::ParameterSet& ps);  
	  void setJetDefinition( const fastjet::JetDefinition& jetDef);

	  void resetJetDefinition(); ////////// Function used to reset the jetdefinition 
	  
	  /// run the jet clustering on the input collection, and produce the reco jets
	  const JetCollection_U& produce( const InputHandle_T& inputColl, bool exclusive, bool forced_, double ptMin_, int n_jets); 
	  const JetCollection_U& produce( const InputHandle_T& inputColl, const CollectionMasks &inputMasks,
					  bool exclusive , double ptMin_, int n_jets); //////// overload to implement masking in an specific analysis contact pedro silva 
	  const JetCollection_U& produce( const std::vector< edm::Ptr< reco::Candidate > >  & inputColl, bool exclusive, 
					  bool forced_ , double ptMin_, int n_jets); /////// overload to be able to send several containers into one vector of candidates

	  /// print internal pseudojets
	  void printPseudoJets( std::ostream& out = std::cout) const;
	  
	  /// print output jets
	  void printJets( std::ostream& out = std::cout) const;

	  double maxDmerge(int n_jets);	
	  double maxDistance();	
	  std::vector<unsigned> usedPseudoJets();
  
       private:
	  /// convert input elements from CMSSW (e.g. PFCandidates) 
	  /// into fastjet input. 
	  /// InputType could be a template parameter of the class
	  void recoToFastJet(const InputType_T& inputColl); 
	  void recoToFastJet(const InputType_T& inputColl,const CollectionMasks &inputMasks); 
	  
	  /// run fast jet
	  void runJetClustering( double ptMin_); 
	  void runJetClustering_exclusive( int n_jets ); 
	  void runJetClustering_inclusive_force( int n_jets ); ///// Force two jets using the inclusive algorithm. Long implementation 
	  void runJetClustering_exclusive_force( int n_jets ); ///// Force two jets using the inclusive algorithm. Long implementation 
	  
	  /// convert fastjet output to RECO data format (e.g. PFJet)
	  const JetCollection_U& fastJetToReco();
	  
	  /// build a JetType (e.g. PFJet) from a pseudo-jet. Jet type
	  /// could be a template parameter of the class. 
	  JetType_U makeJet( const fastjet::PseudoJet& pseudoJet) const;
	  
	  /// build the vector< Ptr<Candidate> > pointing to constituents
	  /// from the PseudoJet and the Handle information.
	  reco::Jet::Constituents makeConstituents(const fastjet::PseudoJet& pseudoJet) const; 
	  
	  /// keep track of the input handle - set in the produce function.
	  InputHandle_T      inputHandle_;
	  std::vector< edm::Ptr < reco::Candidate > > inputCollection_;  //// input vector needed to put all the containers in one vector 
	  
	  /// fastjet input
	  PseudoJetCollection  input_;
	  
	  /// fastjet output
	  PseudoJetCollection  output_;
	  
	  /// output jet collection
	  JetCollection_U jetCollection_; 
	  
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

	  ///// vector of constituents used to create the constituents when the exclusive algorithm is forced !!
	  std::vector< reco::Jet::Constituents >     ptrvector;
	  bool forced;
	  
  };

  /////////// especialization for recoToFastJet when using View(candidate)  
  template<>
    void FastJetAlgo< edm::View< reco::Candidate > , reco::PFJet >::recoToFastJet(const edm::View< reco::Candidate >& inputColl) {
    input_.clear();
    unsigned i;
    for(i=0; i!=inputColl.size(); ++i) {
      input_.push_back( fastjet::PseudoJet( inputColl[i].px(), inputColl[i].py(), inputColl[i].pz(), inputColl[i].energy() ) );
      input_.back().set_user_index( i );
    }
  }

  //////// especialization for makeConstituents when using a vector of ptr to candidates as input   
  template<>
    reco::Jet::Constituents FastJetAlgo< std::vector< edm::Ptr< reco::Candidate > > , reco::PFJet >::makeConstituents(const fastjet::PseudoJet& pseudoJet) const {
      reco::Jet::Constituents ptrsToConstituents; 
      const PseudoJetCollection& constituents 
	= clusterSequence_->constituents(pseudoJet); 
      for(PJI jc=constituents.begin(); jc!=constituents.end(); ++jc) {
	ptrsToConstituents.push_back( inputCollection_[jc->user_index()] ); 
      } 
      return ptrsToConstituents;
  }
  
  //// especialization of the produce function for a vector of candidate-pointers as input and a vector of PFJet as output
  template<>
    const std::vector< reco::PFJet >& FastJetAlgo< std::vector< edm::Ptr< reco::Candidate > >, reco::PFJet >::produce( const std::vector< edm::Ptr< reco::Candidate > > & inputHandle, bool exclusive , bool forced_, double ptMin_, int n_jets) {
       // the input handle will be necessary to build the Ptrs to the jet constituents.
       inputCollection_ = inputHandle;
       recoToFastJet( inputHandle );
       if(exclusive && forced_ && n_jets == 2)
	 runJetClustering_exclusive_force(n_jets);
       else if(exclusive && !forced_ )
	 runJetClustering_exclusive(n_jets);
       else if(!exclusive && forced_)
	 runJetClustering_inclusive_force(n_jets);
       else
	 runJetClustering(ptMin_);
       return fastJetToReco(); 
  }
  
  ////// especialization of makeJet to be able to force the exclusive algorithm to two jets in the final state
  template<>
    reco::PFJet FastJetAlgo< std::vector< edm::Ptr< reco::Candidate > > , reco::PFJet >::makeJet( const fastjet::PseudoJet& pseudoJet) const {
       reco::Particle::LorentzVector p4( pseudoJet.px(), 
					 pseudoJet.py(),
					 pseudoJet.pz(),
					 pseudoJet.E() );
       reco::Particle::Point vertex; 
       reco::PFJet::Specific specific; 
       // need to add the constituents as well (see base Jet, or CompositePtrCandidate)
       if(!forced){
	 reco::Jet::Constituents ptrsToConstituents;
	 ptrsToConstituents = makeConstituents( pseudoJet );
	 return reco::PFJet(p4, vertex, specific, ptrsToConstituents); 
       }
       else{
	 unsigned it;
	 unsigned sel=0;
	 for(it=0; it < output_.size();it++){
	   if(output_[it].E() == pseudoJet.E() )
	     sel = it;
	 }
	 return reco::PFJet(p4, vertex, specific, ptrvector[sel]);
       }
  }
  
}

using namespace pf2pat;

/////// Constructor with the parametr set
template<class T, class U>
FastJetAlgo< T , U >::FastJetAlgo( const edm::ParameterSet& ps ) 
  : clusterSequence_(0) {
  setJetDefinition( ps ); 
  forced = false;
}

///constructor with the jet definition
template<class T, class U>
FastJetAlgo< T , U >::FastJetAlgo( fastjet::JetDefinition& jd ) 
  : clusterSequence_(0) {
  setJetDefinition( jd );
  forced = false;
}

//constructor with the specific values of fastjet
template<class T, class U>
  FastJetAlgo< T , U >::FastJetAlgo( unsigned fastjetalgo_, double rfastjet_, double extrapamfastjet_, int recombination_, int strat_ ) 
    : clusterSequence_(0), algorithm_(fastjetalgo_), 
      distance_(rfastjet_), extra_param_(extrapamfastjet_), 
      recomb_scheme_(recombination_), strategy_(strat_){
    forced = false;
    fastjet::JetDefinition* jetdefptr;
    if(fastjetalgo_ == 3){
      fastjet::JetDefinition jetdef(static_cast<fastjet::JetAlgorithm>(algorithm_),
                                    distance_, extra_param_,
                                    static_cast<fastjet::RecombinationScheme>(recomb_scheme_),
                                    static_cast<fastjet::Strategy>(strategy_));
      jetdefptr = &jetdef;
    }
    else{
      fastjet::JetDefinition jetdef(static_cast<fastjet::JetAlgorithm>(algorithm_),
                                    distance_,
                                    static_cast<fastjet::RecombinationScheme>(recomb_scheme_),
                                    static_cast<fastjet::Strategy>(strategy_));
      jetdefptr = &jetdef;
    }
    fastjet::JetDefinition jd;
    jd = *jetdefptr;
    setJetDefinition( jd );
}

///Jet definition using the parameter set
template<class T, class U>
void FastJetAlgo< T , U >::setJetDefinition( const edm::ParameterSet& ps) {
  // here extract parameter set info to make the jet definition

  algorithm_ = ps.getParameter<unsigned>("fj_algo");
  distance_ = ps.getParameter<double>("distance_par");
  extra_param_ = ps.getParameter<double>("extra_param");
  recomb_scheme_ = ps.getParameter<int>("recombination_scheme");
  strategy_ = ps.getParameter<int>("strategy");

  JetDefinition *jetdefptr;
  JetDefinition jetDef;
  if(algorithm_ == 3){
    JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), 
			    distance_, extra_param_, 
			    static_cast<RecombinationScheme>(recomb_scheme_), 
			    static_cast<Strategy>(strategy_));
    jetdefptr = &jetdef1;
  }
  else{
    JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), 
			    distance_, 
			    static_cast<RecombinationScheme>(recomb_scheme_), 
			    static_cast<Strategy>(strategy_));
    jetdefptr = &jetdef1;
  }
  jetDef = *jetdefptr;
  setJetDefinition( jetDef ); 
}

///// jet definition using the fastjet jet definition 
template<class T, class U>
  void FastJetAlgo< T , U >::setJetDefinition( const fastjet::JetDefinition& jetDef) {
  //cout<<jetDef.description()<<endl;
    jetDefinition_ = jetDef; 
}

/////////// reset jet definition in needed to call fastjet several times when running
template<class T, class U>
  void FastJetAlgo< T , U >::resetJetDefinition() {
   JetDefinition *jetdefptr;
   JetDefinition jetDef;
   if(algorithm_ == 3){
     JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), 
			     distance_, extra_param_, 
			     static_cast<RecombinationScheme>(recomb_scheme_), 
			     static_cast<Strategy>(strategy_));
     jetdefptr = &jetdef1;
   }
   else{
     JetDefinition  jetdef1( static_cast<JetAlgorithm>(algorithm_), 
			     distance_, 
			     static_cast<RecombinationScheme>(recomb_scheme_), 
			     static_cast<Strategy>(strategy_));
     jetdefptr = &jetdef1;
   }
   jetDef = *jetdefptr;
   setJetDefinition( jetDef ); 
}

////////////// produce function
template<class T, class U>
  const std::vector< U >& FastJetAlgo<T,U>::produce( const FastJetAlgo<T,U>::InputHandle_T& inputHandle, bool exclusive , bool forced_, double ptMin_, int n_jets) {
  // the input handle will be necessary to build the Ptrs to the jet constituents.
  inputHandle_ = inputHandle;
  const InputType_T& inputColl = *inputHandle; 
  recoToFastJet( inputColl );
  if(exclusive && forced_ && n_jets == 2)
    runJetClustering_exclusive_force(n_jets);
  else if(exclusive && ! forced_)
    runJetClustering_exclusive(n_jets);
  else if(!exclusive && forced_)
    runJetClustering_inclusive_force(n_jets);
  else
    runJetClustering(ptMin_);
  return fastJetToReco(); 
}

/////produce function overload, pedro silva
template<class T, class U>
  const std::vector< U >& FastJetAlgo<T,U>::produce( const FastJetAlgo<T,U>::InputHandle_T& inputHandle, const CollectionMasks &inputMasks, bool exclusive, double ptMin_, int n_jets) {
  // the input handle will be necessary to build the Ptrs to the jet constituents.
  inputHandle_ = inputHandle;
  const InputType_T& inputColl = *inputHandle; 
  recoToFastJet( inputColl, inputMasks );
  if(exclusive)
    runJetClustering_exclusive(n_jets);
  else
    runJetClustering(ptMin_);
  return fastJetToReco(); 
}

///////// reco to fast jet
template<class T, class U>
void FastJetAlgo<T,U>::recoToFastJet(const InputType_T& inputColl) {
  input_.clear();
  unsigned i;
  for(i=0; i!=inputColl.size(); ++i) {
    input_.push_back( PseudoJet( inputColl[i]->px(), inputColl[i]->py(), inputColl[i]->pz(), inputColl[i]->energy() ) );
    input_.back().set_user_index( i );
  }
}

//// overload "pedro again"
template<class T, class U>
  void FastJetAlgo<T,U>::recoToFastJet(const InputType_T& inputColl,const CollectionMasks &inputMasks) {
  input_.clear();
  bool useMasks(inputMasks.size()==inputColl.size());
  unsigned i;
  for(i=0; i!=inputColl.size(); ++i) {
    if(useMasks && !inputMasks[i]) continue;
    input_.push_back( PseudoJet( inputColl[i].px(), inputColl[i].py(), inputColl[i].pz(), inputColl[i].energy() ) );
    input_.back().set_user_index( i );
  }
}

//// returns the max D to merge from n+1 to n jets
template<class T, class U>
  double FastJetAlgo<T,U>::maxDmerge( int n_jets){
  return clusterSequence_->exclusive_dmerge( n_jets );
}

template<class T, class U>
  double FastJetAlgo<T,U>::maxDistance(){
  return distance_;
}

///// inclusive fast jet
template<class T, class U>
void  FastJetAlgo< T , U >::runJetClustering( double ptMin_ ) {
  output_.clear();
  if(clusterSequence_) delete clusterSequence_;
  clusterSequence_ = new ClusterSequence(input_, jetDefinition_);
  //cout << "calling inclusive " << endl;
  output_ = clusterSequence_->inclusive_jets( ptMin_ );
}

///exclusive fast jet
template<class T, class U>
void  FastJetAlgo< T , U >::runJetClustering_exclusive( int n_jets ) {
  output_.clear();
  if(clusterSequence_) delete clusterSequence_;
  clusterSequence_ = new ClusterSequence(input_, jetDefinition_);
  //cout << "Calling exclusive " << endl;
  output_ = clusterSequence_->exclusive_jets( n_jets );
}

template<class T, class U>
  void  FastJetAlgo< T , U >::runJetClustering_inclusive_force( int n_jets ){
     output_.clear();
     if(clusterSequence_) delete clusterSequence_;
     clusterSequence_ = new ClusterSequence(input_, jetDefinition_);
     output_ = clusterSequence_->inclusive_jets( 2. );
     double cont =0;
     //cycle in case one pases from n> 2 jets to n=1 
     while( output_.size() < 2 && cont < 100){
       output_.clear();
       distance_ = distance_ - 0.5;
       resetJetDefinition();
       clusterSequence_ = new ClusterSequence(input_,jetDefinition_);
       output_ = clusterSequence_->inclusive_jets( 2. );
       cont++;
       if(cont == 100)
	 cout << "Fast Jet was called to go from 1 jet to at least two jets 100 times, plese check the logic of the algorithm!! " << endl;
     }
     ////////////if one needs to force the algorithm, one need to crete the constituents of each jet manually
     unsigned it;
     if(output_.size() > 2){
       forced = true;
       for(it = 0; it < output_.size() ; it++){
	 reco::Jet::Constituents ptrsToConstituents = makeConstituents( output_[it] );
	 ptrvector.push_back(ptrsToConstituents);
       }
     }
     else
       forced = false;
     cont = 0;
     /////////// cycle to really force the production of two final jets, implemented for kt and anti-kt only
     while(output_.size() > 2 &&  cont < 100){
       double dimin = 1e09;
       if(algorithm_ == 2)
	 dimin = 0;
       unsigned sel1 = 0;
       unsigned sel2 = 0;
       double px1 = 0.,py1 = 0.,pt1 = 0.;
       double phi1 = 0.,eta1 = 0.;
       double di = 0;
       for(it = 0; it < output_.size(); it++){
	 px1 = output_[it].px();
	 py1 = output_[it].py();
	 pt1 = sqrt( px1*px1 + py1*py1 );
	 if(algorithm_ == 0){
	   di = pow(pt1,2);
	   if(dimin > di){
	     dimin = di;
	     sel1 = it;
	   }
	 }
	 else if(algorithm_ == 2){
	   di = pow(1/pt1,2);
	   if( di > dimin){
	     dimin = di;
	     sel1 = it;
	   }
	 }
       }
       double dijmin = 1e09;
       if(algorithm_ == 2)
	 dijmin = 0;
       phi1 = output_[sel1].phi();
       eta1 = output_[sel1].eta();
       pt1 = sqrt( output_[sel1].px()*output_[sel1].px() + output_[sel1].py()*output_[sel1].py() );
       for(it = 0; it < output_.size(); it++){
	 if(it != sel1){
	   double px,py,pt;
	   px = output_[it].px();
	   py = output_[it].py();
	   pt = sqrt(px*px+py*py);
	   double phi = output_[it].phi();
	   double eta = output_[it].eta();
	   double r = sqrt( (phi1-phi)*(phi1-phi) + (eta1 -eta)*(eta1-eta) );
	   if(algorithm_ == 0){
	     if( pt1*pt1 < pt*pt )
	       di = pt1*pt1;
	     else
	       di = pt*pt;
	   }
	   else if(algorithm_ == 2 ){
	     if( 1./(pt1*pt1) < 1./(pt*pt) )
	       di = 1./(pt1*pt1);
	     else
	       di = 1./(pt*pt);
	   }
	   double dij = di*r*r/pow(distance_,2); 
	   if(algorithm_ == 0){
	     if(dijmin > dij){
	       dijmin = dij;
	       sel2 = it;
	     }
	   }
	   else if(algorithm_ == 2){
	     if(dijmin < dij){
	       dijmin = dij;
	       sel2 = it;
	     }
	   }
	 }
       }
       distance_ = sqrt(dijmin*pow(distance_,2)/dimin);
       PseudoJetCollection  temp_;
       std::vector<reco::Jet::Constituents> tempC_;
       reco::Jet::Constituents tempb_;
       temp_ = output_;
       output_.clear();
       tempC_ = ptrvector;
       ptrvector.clear();
       for(it = 0; it < temp_.size(); it++){
	 if( it != sel1 && it != sel2  ){
	   output_.push_back(temp_[it]); 
	   ptrvector.push_back(tempC_[it]);
	 }
	 else if(it == sel1){
	   fastjet::PseudoJet psj(temp_[sel1].px() + temp_[sel2].px(),
				  temp_[sel1].py() + temp_[sel2].py(),
				  temp_[sel1].pz() + temp_[sel2].pz(),
				  temp_[sel1].e() + temp_[sel2].e() );
	   unsigned it1;
	   for(it1 = 0; it1 < tempC_[sel1].size() ; it1++){
	     tempb_.push_back(tempC_[sel1][it1]);
	   }
	   for(it1 =0; it1 < tempC_[sel2].size() ; it1++){
	     tempb_.push_back(tempC_[sel2][it1]);
	   }
	   ptrvector.push_back(tempb_);
	   output_.push_back(psj);
	 }
       }
       cont++;
       if(cont == 100) 
	 cout << "If this is printed out, you just tried to create a final state of two jets using an inclusive algorithm a hundred times, please check the logic, something is going on!" << endl;
     }
}

///////// exclusive way forcing two jets using the inclusive algorithm 
template<class T, class U>
  void  FastJetAlgo< T , U >::runJetClustering_exclusive_force( int n_jets ){
     output_.clear();
     if(clusterSequence_) delete clusterSequence_;
     clusterSequence_ = new ClusterSequence(input_, jetDefinition_);
     output_ = clusterSequence_->exclusive_jets( 2 );

     ///////////////
     unsigned size_inp = input_.size(); 
     unsigned i;
     std::vector<unsigned> used = usedPseudoJets(); 
     std::vector<bool> missing;
     for(i=0;i<size_inp;i++){
       missing.push_back(true);
     }
     for(i=0;i<used.size();i++){
       missing[used[i]] = false;
     }
     for(i=0;i<size_inp;i++){
       if(missing[i] == true){
	 output_.push_back(input_[i]);
       }
     }
     /////////////
     double cont =0;
     ////////////if one needs to force the algorithm, one need to create the constituents of each jet manually
     unsigned it;
     if(output_.size() > 2){
       forced = true;
       for(it = 0; it < 2; it++){
	 reco::Jet::Constituents ptrsToConstituents = makeConstituents( output_[it] );
	 ptrvector.push_back(ptrsToConstituents);
       }
       for(i=0;i<size_inp;i++){
	 if(missing[i] == true){
	   reco::Jet::Constituents ptr;
	   ptr.push_back(inputCollection_[i]); 
	   ptrvector.push_back(ptr);
	 }
       }
     }
     else
       forced = false;
     cont = 0;
     /////////// cycle to really force the production of two final jets, implemented for kt and anti-kt only
     while(output_.size() > 2 &&  cont < 100){
       double dimin = 1e09;
       if(algorithm_ == 2)
	 dimin = 0;
       unsigned sel1 = 0;
       unsigned sel2 = 0;
       double px1 = 0.,py1 = 0.,pt1 = 0.;
       double phi1 = 0.,eta1 = 0.;
       double di = 0;
       for(it = 0; it < output_.size(); it++){
	 px1 = output_[it].px();
	 py1 = output_[it].py();
	 pt1 = sqrt( px1*px1 + py1*py1 );
	 if(algorithm_ == 0){
	   di = pow(pt1,2);
	   if(dimin > di){
	     dimin = di;
	     sel1 = it;
	   }
	 }
	 else if(algorithm_ == 2){
	   di = pow(1/pt1,2);
	   if( di > dimin){
	     dimin = di;
	     sel1 = it;
	   }
	 }
       }
       double dijmin = 1e09;
       if(algorithm_ == 2)
	 dijmin = 0;
       phi1 = output_[sel1].phi();
       eta1 = output_[sel1].eta();
       pt1 = sqrt( output_[sel1].px()*output_[sel1].px() + output_[sel1].py()*output_[sel1].py() );
       for(it = 0; it < output_.size(); it++){
	 if(it != sel1){
	   double px,py,pt;
	   px = output_[it].px();
	   py = output_[it].py();
	   /* pz = output_[it].pz(); */
	   pt = sqrt(px*px+py*py);
	   double phi = output_[it].phi();
	   double eta = output_[it].eta();
	   double r = sqrt( (phi1-phi)*(phi1-phi) + (eta1 -eta)*(eta1-eta) );
	   if(algorithm_ == 0){
	     if( pt1*pt1 < pt*pt )
	       di = pt1*pt1;
	     else
	       di = pt*pt;
	   }
	   else if(algorithm_ == 2 ){
	     if( 1./(pt1*pt1) < 1./(pt*pt) )
	       di = 1./(pt1*pt1);
	     else
	       di = 1./(pt*pt);
	   }
	   double dij = di*r*r/pow(distance_,2); 
	   if(algorithm_ == 0){
	     if(dijmin > dij){
	       dijmin = dij;
	       sel2 = it;
	     }
	   }
	   else if(algorithm_ == 2){
	     if(dijmin < dij){
	       dijmin = dij;
	       sel2 = it;
	     }
	   }
	 }
       }
       distance_ = sqrt(dijmin*pow(distance_,2)/dimin);
       PseudoJetCollection  temp_;
       std::vector<reco::Jet::Constituents> tempC_;
       reco::Jet::Constituents tempb_;
       temp_ = output_;
       output_.clear();
       tempC_ = ptrvector;
       ptrvector.clear();
       for(it = 0; it < temp_.size(); it++){
	 if( it != sel1 && it != sel2  ){
	   output_.push_back(temp_[it]); 
	   ptrvector.push_back(tempC_[it]);
	 }
	 else if(it == sel1){
	   fastjet::PseudoJet psj(temp_[sel1].px() + temp_[sel2].px(),
				  temp_[sel1].py() + temp_[sel2].py(),
				  temp_[sel1].pz() + temp_[sel2].pz(),
				  temp_[sel1].e() + temp_[sel2].e() );
	   unsigned it1;
	   for(it1 = 0; it1 < tempC_[sel1].size() ; it1++){
	     tempb_.push_back(tempC_[sel1][it1]);
	   }
	   for(it1 =0; it1 < tempC_[sel2].size() ; it1++){
	     tempb_.push_back(tempC_[sel2][it1]);
	   }
	   ptrvector.push_back(tempb_);
	   output_.push_back(psj);
	 }
       }
       cont++;
       if(cont == 100) 
	 cout << "If this is printed out, you just tried to create a final state of two jets using an inclusive algorithm a hundred times, please check the logic, something is going on!" << endl;
     }
}

////////// converting fastjets into reco jets
template<class T, class U>
const std::vector< U >& FastJetAlgo< T, U >::fastJetToReco() {
  jetCollection_.clear();
  for(PJI i=output_.begin(); i!=output_.end(); ++i) {
    jetCollection_.push_back( makeJet( *i ) ); 
  }
  return jetCollection_;
}

///////making the jets
template<class T,class U>
U FastJetAlgo<T,U>::makeJet( const PseudoJet& pseudoJet) const {
  
  reco::Particle::LorentzVector p4( pseudoJet.px(), 
				    pseudoJet.py(),
				    pseudoJet.pz(),
				    pseudoJet.E() );
  reco::Particle::Point vertex; 
  reco::PFJet::Specific specific; 
  // need to add the constituents as well (see base Jet, or CompositePtrCandidate)
  reco::Jet::Constituents ptrsToConstituents = makeConstituents( pseudoJet );
  makeSpecific( ptrsToConstituents, &specific );
  return U(p4, vertex, specific, ptrsToConstituents); 
}
  
//////////making the jet constituents 
template<class T, class U>
reco::Jet::Constituents FastJetAlgo<T,U>::makeConstituents(const fastjet::PseudoJet& pseudoJet) const {
  reco::Jet::Constituents ptrsToConstituents; 
  const PseudoJetCollection& constituents 
    = clusterSequence_->constituents(pseudoJet); 
  for(PJI jc=constituents.begin(); jc!=constituents.end(); ++jc) {
    ptrsToConstituents.push_back( edm::Ptr<reco::Candidate>(inputHandle_, jc->user_index() ) ); 
  } 

  return ptrsToConstituents;
}

//// printing pseudojet info
template<class T, class U>
void FastJetAlgo<T,U>::printPseudoJets( ostream& out) const {
  unsigned index = 0;
  for(PJI i=output_.begin(); i!=output_.end(); ++i, ++index) {
    cout<<index<<" "<<i->Et()<<endl;
    const PseudoJetCollection& constituents = clusterSequence_->constituents( *i );
    for(PJI jc=constituents.begin(); jc!=constituents.end(); ++jc) {
      cout<<"\t"<<jc->user_index()<<" "<<jc->Et()<<endl;
    }
  }
}

template<class T, class U>
std::vector<unsigned> FastJetAlgo<T,U>::usedPseudoJets(){
  std::vector<unsigned> out;
  unsigned index = 0;
  for(PJI i=output_.begin(); i!=output_.end(); ++i, ++index) {
    const PseudoJetCollection& constituents = clusterSequence_->constituents( *i );
    for(PJI jc=constituents.begin(); jc!=constituents.end(); ++jc) {
      out.push_back((unsigned)jc->user_index());
    }
  }
  return out;
}

/////printing jets 
template<class T, class U>
void FastJetAlgo<T,U>::printJets( ostream& out) const {
 unsigned i = 0;
  for(i=0; i!=jetCollection_.size(); ++i) {
    cout<<i<<" "<<jetCollection_[i]<<endl;
    cout<<jetCollection_[i]->print()<<endl;
  }
}

#endif
