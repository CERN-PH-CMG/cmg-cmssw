
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "CMGTools/Common/interface/RecoilCorrector.h"

#include <sstream>

class RecoilCorrectedMETProducer : public edm::EDProducer {

public:
  // will template the producer later
  typedef cmg::TauMu RecBosonType;
  typedef RecBosonType::type1 Leg1Type;
  typedef RecBosonType::type2 Leg2Type;
  typedef cmg::BaseMET MetType; 
  typedef cmg::AbstractPhysicsObject   JetType;
  typedef edm::View<JetType>           JetCollectionType;

  explicit RecoilCorrectedMETProducer(const edm::ParameterSet & iConfig);
  virtual ~RecoilCorrectedMETProducer() { 
    // delete corrector_; 
  }
  
private:
  
  enum CorrectionType {
    All = 0, 
    Type1 = 1, 
    Type2 = 2
  };

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// return the number of jets that do not match the legs of the boson 
  /// within deltaR
  int  nJets( const JetCollectionType& jets, 
	      const RecBosonType& boson, float deltaR);

  edm::InputTag metSrc_;
  edm::InputTag genBosonSrc_;
  edm::InputTag recBosonSrc_;
  edm::InputTag jetSrc_;

  /// cut for the matching between the jets and the rec boson legs. 
  float deltaRCut_;
  
  /// 1: leg1; 2: leg2; 0: both legs 
  int           leptonLeg_;

  /// type of correction
  CorrectionType correctionType_;

  RecoilCorrector*  corrector_;

  bool enable_;
  bool verbose_;
};


RecoilCorrectedMETProducer::RecoilCorrectedMETProducer(const edm::ParameterSet & iConfig) : 
  metSrc_( iConfig.getParameter<edm::InputTag>("metSrc") ), 
  genBosonSrc_( iConfig.getParameter<edm::InputTag>("genBosonSrc") ),
  recBosonSrc_( iConfig.getParameter<edm::InputTag>("recBosonSrc") ),
  jetSrc_( iConfig.getParameter<edm::InputTag>("jetSrc") ),
  deltaRCut_(0.5),
  leptonLeg_( iConfig.getParameter<int>("leptonLeg") ),
  correctionType_( static_cast<CorrectionType>( iConfig.getParameter<int>("correctionType") ) ), 
  enable_( iConfig.getParameter<bool>("enable") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
  
  std::string fileCorrectTo = iConfig.getParameter<std::string>("fileCorrectTo");
  std::string fileZmmData = iConfig.getParameter<std::string>("fileZmmData");
  std::string fileZmmMC = iConfig.getParameter<std::string>("fileZmmMC");
  
  if(leptonLeg_ < 0 || leptonLeg_>2) {
    std::string err("leptonLeg must be equal to 0, 1 or 2.");
    throw cms::Exception( err ); 
  }
  
  corrector_ = new RecoilCorrector(fileCorrectTo);
  corrector_->addDataFile( fileZmmData );
  corrector_->addMCFile( fileZmmMC );
   
  // will produce one BaseMET for each recBoson 
  produces< std::vector<MetType> >();
}


void RecoilCorrectedMETProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  typedef std::auto_ptr< std::vector< MetType > >  OutPtr;

  edm::Handle< std::vector<MetType> > metH;
  iEvent.getByLabel(metSrc_, metH);

  edm::Handle< std::vector<RecBosonType> > recBosonH;
  iEvent.getByLabel(recBosonSrc_, recBosonH);


  if( metH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  if( ! enable_ ) {
    const MetType& met = (*metH)[0];
    // when disabled, put one copy of the MET in the output collection
    // for each rec boson in input. 
    OutPtr pOut(new std::vector< MetType >(recBosonH->size(), met) );
    iEvent.put( pOut ); 
    return;
  }

  edm::Handle< std::vector<reco::GenParticle> > genBosonH;
  iEvent.getByLabel(genBosonSrc_, genBosonH);

  edm::Handle< JetCollectionType > jetH;
  iEvent.getByLabel(jetSrc_, jetH);

  if( genBosonH->size()!=1) 
    throw cms::Exception("Input GenBoson collection should have size 1.");
 
  const MetType& metObj = (*metH)[0];
  double uncMet = metObj.et();
  double uncMetPhi = metObj.phi();
  const reco::GenParticle& genBoson = (*genBosonH)[0];
  double genPt = genBoson.pt(); 
  double genPhi = genBoson.phi();


  // check that the user is doing nothing wrong. 
  // leptonLeg = 1 or 2 ok for W, but not for Z 
  // leptonLeg = 0 ok for Z 
  //COLIN should add a force mode to bypass these exceptions
  switch( genBoson.pdgId() ) {
  case 24: // W+
  case -24:// W-
    if( leptonLeg_ != 1 && leptonLeg_ != 2) 
      throw cms::Exception("leptonLeg should be equal to 1 or 2 when running on W events.");
    break;
  case 22: // photon 
  case 23: // Z0
    if( leptonLeg_ != 0 ) 
      throw cms::Exception("leptonLeg should be equal to 0 when running on Drell-Yan events.");   
    break;
  default:
    throw cms::Exception("input genBoson should be a W or a Z0/gamma.");
  }


  // the following variables are dummy, but necessary to call the corrector.
  double u1 = 0;
  double u2 = 0;
  double fluc = 0;
  int jetMult = jetH->size();
  
  if(verbose_) {
    std::cout<<"---"<<std::endl;
    std::cout<<"RecoilCorrectedMETProducer, type = "<<correctionType_<<std::endl;
    std::cout<<"gen boson, pdgId="<<genBoson.pdgId()
	     <<", pt="<<genBoson.pt()<<", phi="<<genBoson.phi()<<std::endl;
    std::cout<<"# rec bosons ="<<recBosonH->size()<<std::endl;
    std::cout<<"# jets ="<<jetMult<<std::endl;
    for( unsigned iJet = 0; iJet<jetH->size(); ++iJet) {
      std::cout<<"\t"<<jetH->at(iJet)<<std::endl;
    }
    std::cout<<"unc MET :  pt="<<uncMet<<", phi="<<uncMetPhi<<std::endl;
    std::cout<<"Looping on reconstructed bosons:"<<std::endl;
  }
  
  OutPtr pOut(new std::vector< MetType > ); 
  for( unsigned i=0; i<recBosonH->size(); ++i) {
    const RecBosonType& recBoson = recBosonH->at(i);

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<recBoson<<std::endl;
      std::cout<<"\t\tleg1: "<<recBoson.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<recBoson.leg2()<<std::endl;
    }

    reco::Candidate::PolarLorentzVector lepton;
    if(leptonLeg_==1) {
      lepton = recBoson.leg1().p4();
    }
    else if(leptonLeg_==2) {
      lepton = recBoson.leg2().p4();
    }
    else {
      lepton = recBoson.leg1().p4();
      lepton += recBoson.leg2().p4();
    }
    double lepPt = lepton.pt();
    double lepPhi = lepton.phi();


    //COLIN: need to add a flag to choose the function
    //the following step is absolutely necessary, as the corrector will change the met....!!
    double met = uncMet; 
    double metphi = uncMetPhi;

    jetMult = nJets( *jetH, recBoson, deltaRCut_ ); 

    switch( correctionType_ ) {
    case Type1:
      corrector_->CorrectType1( met, metphi, 
				genPt, genPhi, 
				lepPt, lepPhi, 
				u1, u2, fluc, jetMult );
      break;
    case Type2:
      corrector_->CorrectType2( met, metphi, 
				genPt, genPhi, 
				lepPt, lepPhi, 
				u1, u2, fluc, jetMult );
      break;
    case All:
      corrector_->CorrectAll( met, metphi, 
			      genPt, genPhi, 
			      lepPt, lepPhi, 
			      u1, u2, fluc, jetMult );
      break;
    default:
      throw cms::Exception("Undefined correction type.");
    }

    if(verbose_) {
      std::cout<<"\tlep pt="<<lepPt<<", lep phi="<<lepPhi<<std::endl;
      std::cout<<"\tjet mult = "<<jetMult<<std::endl;
      std::cout<<"\tnew MET pt="<<met<<", phi="<<metphi<<std::endl;
    }

    reco::Candidate::PolarLorentzVector newMETP4( met,0, metphi, 0);
    pOut->push_back( metObj );
    pOut->back().setP4( newMETP4 );
  }
  
  iEvent.put( pOut ); 

  if(verbose_) {
    std::cout<<"RecoilCorrectedMETProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}



int  RecoilCorrectedMETProducer::nJets( const JetCollectionType& jets, 
					const RecBosonType& boson, float deltaR) {
  
  if(verbose_) {
    std::cout<<"\tMatching jets to boson legs"<<std::endl;
  }
  float deltaR2 = deltaR*deltaR; 
  
  unsigned nJets = 0;
  for(unsigned iJet = 0; iJet<jets.size(); ++iJet ) {
    const JetType& jet = jets[iJet]; 

    double dR2leg1 = reco::deltaR2( jet.eta(), jet.phi(), 
				    boson.leg1().eta(), boson.leg1().phi() ) ;
    double dR2leg2 = reco::deltaR2( jet.eta(), jet.phi(), 
				    boson.leg2().eta(), boson.leg2().phi() ) ;
    
    if( dR2leg1 > deltaR2 && dR2leg2 > deltaR2) {
      // this jet is far enough from both boson legs, and counted as such
      nJets++;
    }
    else{
      if(verbose_) 
      std::cout<<"\t\texcluding jet "<<jet<<", dR1,dR2="
	       <<sqrt(dR2leg1)<<","<<sqrt(dR2leg2)<<std::endl;
    }
  }  

  return nJets;
}



#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(RecoilCorrectedMETProducer);
