
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

// #include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
// #include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
// #include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "CMGTools/Utilities/interface/RecoilCorrector.h"
#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"

#include <sstream>

template< typename T, typename U >
class RecoilCorrectedMETProducer : public edm::EDProducer {

public:
  typedef pat::CompositeCandidate RecBosonType;
  typedef T Leg1Type;
  typedef U Leg2Type;
  typedef reco::PFMET MetType ;
  typedef reco::PFJet JetType;
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
  
  // disable a few consistency checks to e.g. be able to run the W recoil correction
  // on ersatz DYJets
  bool force_;

  bool verbose_;
};


template< typename T, typename U >
RecoilCorrectedMETProducer< T, U >::RecoilCorrectedMETProducer(const edm::ParameterSet & iConfig) : 
  genBosonSrc_( iConfig.getParameter<edm::InputTag>("genBosonSrc") ),
  recBosonSrc_( iConfig.getParameter<edm::InputTag>("recBosonSrc") ),
  jetSrc_( iConfig.getParameter<edm::InputTag>("jetSrc") ),
  deltaRCut_(0.5),
  leptonLeg_( iConfig.getParameter<int>("leptonLeg") ),
  correctionType_( static_cast<CorrectionType>( iConfig.getParameter<int>("correctionType") ) ), 
  enable_( iConfig.getParameter<bool>("enable") ),
  force_( iConfig.getParameter<bool>("force") ),
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
  // produces< std::vector<MetType> >();
  produces< std::vector<RecBosonType> >();
}


template< typename T, typename U >
void RecoilCorrectedMETProducer<T, U>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {


  edm::Handle< std::vector<RecBosonType> > recBosonH;
  iEvent.getByLabel(recBosonSrc_, recBosonH);

  if( ! enable_) {

    std::auto_ptr< std::vector< RecBosonType > > pOutRecBoson(new std::vector< RecBosonType >(*recBosonH));
    // Just copy rec bosons if no correction is enabled
    iEvent.put( pOutRecBoson );
    return;
  }

  edm::Handle< std::vector<reco::GenParticle> > genBosonH;
  iEvent.getByLabel(genBosonSrc_, genBosonH);

  edm::Handle< JetCollectionType > jetH;
  iEvent.getByLabel(jetSrc_, jetH);

  if( genBosonH->size()!=1) 
    throw cms::Exception("Input GenBoson collection should have size 1.");
 
  const reco::GenParticle& genBoson = (*genBosonH)[0];
  double genPt = genBoson.pt(); 
  double genPhi = genBoson.phi();


  // check that the user is doing nothing wrong. 
  // leptonLeg = 1 or 2 ok for W, but not for Z 
  // leptonLeg = 0 ok for Z 
  //COLIN should add a force mode to bypass these exceptions
  if(!force_) {
    switch( genBoson.pdgId() ) {
    case 24: // W+
    case -24:// W-
      if( leptonLeg_ != 1 && leptonLeg_ != 2) 
	throw cms::Exception("leptonLeg should be equal to 1 or 2 when running on W events.");
      break;
    case 22: // photon 
    case 23: // Z0
    case 25: // Higgs
    case 35: // SUSY Higgs H0
    case 36: // SUSY Higgs A0
      if( leptonLeg_ != 0 ) 
	throw cms::Exception("leptonLeg should be equal to 0 when running on Higgs or Drell-Yan events.");   
      break;
    default:
      throw cms::Exception("input genBoson should be a W or a Z0/gamma.");
    }
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
    //    std::cout<<"unc MET :  pt="<<uncMet<<", phi="<<uncMetPhi<<std::endl;
    std::cout<<"Looping on reconstructed bosons:"<<std::endl;
  }
  
  // OutPtr pOut(new std::vector< MetType > ); 
  std::auto_ptr< std::vector< RecBosonType > > pOutRecBoson(new std::vector<RecBosonType>);
  for( unsigned i=0; i<recBosonH->size(); ++i) {
    const RecBosonType& recBoson = recBosonH->at(i);
    
    //MetType metObj;
    reco::LeafCandidate metObj;
    metObj = *(dynamic_cast<const reco::LeafCandidate*>(recBoson.daughter(2)));

    // if (!useRecBosonMet && metH->size() == 1)
    //   metObj = (*metH)[0];
    // else if(!useRecBosonMet && metH->size()==recBosonH->size())
    //   metObj = (*metH)[i];

    double uncMet = metObj.et();
    double uncMetPhi = metObj.phi();

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\told MET (et,phi): "<<uncMet<<","<<uncMetPhi<<std::endl;
      std::cout<<"\trec boson: "<<recBoson.pt()<<std::endl;
      std::cout<<"\t\tleg1: "<<recBoson.daughter(0)<<std::endl;
      std::cout<<"\t\tleg2: "<<recBoson.daughter(1)<<std::endl;
    }

    reco::Candidate::PolarLorentzVector lepton;
    if(leptonLeg_==1) {
      lepton = recBoson.daughter(0)->p4();
    }
    else if(leptonLeg_==2) {
      lepton = recBoson.daughter(1)->p4();
    }
    else {
      lepton = recBoson.daughter(0)->p4();
      lepton += recBoson.daughter(1)->p4();
    }
    double lepPt = lepton.pt();
    double lepPhi = lepton.phi();


    //COLIN: need to add a flag to choose the function
    //the following step is absolutely necessary, as the corrector will change the met....!!
    double met = uncMet; 
    double metphi = uncMetPhi;

    jetMult = nJets( *jetH, recBoson, deltaRCut_); 

    double zero = 0.;

    switch( correctionType_ ) {
    case Type1:
      corrector_->CorrectType1( met, metphi, 
				genPt, genPhi, 
				lepPt, lepPhi, 
				u1, u2, fluc, zero, jetMult );
      break;
    case Type2:
      corrector_->CorrectType2( met, metphi, 
				genPt, genPhi, 
				lepPt, lepPhi, 
				u1, u2, fluc, zero, jetMult );
      break;
    case All:
      corrector_->CorrectAll( met, metphi, 
			      genPt, genPhi, 
			      lepPt, lepPhi, 
			      u1, u2, fluc, zero, jetMult );
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
    metObj.setP4( newMETP4 );
    pOutRecBoson->push_back(RecBosonType(recBoson));
    // Create new rec boson; also recalculates mT1/mT2 etc
    cmg::DiTauObjectFactory<Leg1Type, Leg2Type>::set(std::make_pair(*dynamic_cast<const T*>(recBoson.daughter(0)), *dynamic_cast<const U*>(recBoson.daughter(1))), metObj, pOutRecBoson->back());
  }
  
  // iEvent.put( pOut ); 
  iEvent.put( pOutRecBoson );

  if(verbose_) {
    std::cout<<"RecoilCorrectedMETProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}



template< typename T, typename U >
int  RecoilCorrectedMETProducer< T, U >::nJets( const JetCollectionType& jets, 
							const RecBosonType& boson, float deltaR) {
  
  // H->tau tau Summer 13 convention: Remove only lepton from jets, not tau,
  // when working with Ws. Not clear what to do for fully-hadronic
  if(verbose_) {
    std::cout<<"\tMatching jets to boson legs"<<std::endl;
  }
  float deltaR2 = deltaR*deltaR; 
  
  unsigned nJets = 0;
  for(unsigned iJet = 0; iJet<jets.size(); ++iJet ) {
    const JetType& jet = jets[iJet]; 

    double dR2leg1 = 99.;
    if (leptonLeg_ != 2)
      dR2leg1 = reco::deltaR2( jet.eta(), jet.phi(), 
				    boson.daughter(0)->eta(), boson.daughter(0)->phi() ) ;
    double dR2leg2 = 99.;
    if (leptonLeg_ != 1)
      dR2leg2 = reco::deltaR2( jet.eta(), jet.phi(), 
				    boson.daughter(1)->eta(), boson.daughter(1)->phi() ) ;
    
    if( dR2leg1 > deltaR2 && dR2leg2 > deltaR2) {
      // this jet is far enough from both boson legs, and counted as such
      nJets++;
    }
    else{
      if(verbose_) {
	std::cout<<"\t\texcluding jet "<<jet<<", dR1,dR2="
		 <<sqrt(dR2leg1)<<", "<<sqrt(dR2leg2)<<std::endl;
	//       std::cout<<jet.rawFactor()<<std::endl;
	//       for(int ic=0; ic<jet.nConstituents(); ++ic) {
	// 	const cmg::PFJetComponent& comp = jet.component(ic);
	// 	std::cout<<comp<<std::endl;
      }
    }
  }  

  return nJets;
}


// #include "FWCore/Framework/interface/MakerMacros.h"

// DEFINE_FWK_MODULE(RecoilCorrectedMETProducer);
