
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

// #include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "CMGTools/Common/interface/MVAMet.h"
#include "CMGTools/Common/interface/MetUtilities.h"
#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include <sstream>

template< typename RecBosonType >
class MVAMETProducer : public edm::EDProducer {

public:
  // typedef cmg::TauMu RecBosonType;
  typedef typename RecBosonType::type1 Leg1Type;
  typedef typename RecBosonType::type2 Leg2Type;
/*   typedef cmg::BaseMET MetType;  */
  typedef reco::PFMET MetType;
  typedef cmg::PFJet   JetType;
  typedef std::vector<JetType>           JetCollectionType;
  typedef math::XYZTLorentzVector LorentzVector;

  explicit MVAMETProducer(const edm::ParameterSet & iConfig);
  virtual ~MVAMETProducer() { 
  }
  
private:

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  /// return the number of jets that do not match the legs of the boson 
  /// within deltaR
/*   int  nJets( const JetCollectionType& jets,  */
/* 	      const RecBosonType& boson, float deltaR); */

  void makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
		const std::vector<JetType>& iCJets,
		const reco::VertexCollection &iVertices,double iRho) const;

  edm::InputTag pfmetSrc_;
  edm::InputTag tkmetSrc_;
  edm::InputTag nopumetSrc_;
  edm::InputTag pucmetSrc_;
  edm::InputTag pumetSrc_;

  edm::InputTag recBosonSrc_;
  edm::InputTag jetSrc_;
  edm::InputTag leadJetSrc_;

  edm::InputTag vertexSrc_;

  edm::InputTag nJetsPtGt1Src_;

  edm::InputTag rhoSrc_;

  /// cut for the matching between the jets and the rec boson legs. 
  float deltaRCut_;
  
  MVAMet*  mvaMet_;

  bool enable_;
  bool verbose_;
};



template< typename RecBosonType >
MVAMETProducer< RecBosonType >::MVAMETProducer(const edm::ParameterSet & iConfig) : 
  pfmetSrc_( iConfig.getParameter<edm::InputTag>("pfmetSrc") ), 
  tkmetSrc_( iConfig.getParameter<edm::InputTag>("tkmetSrc") ), 
  nopumetSrc_( iConfig.getParameter<edm::InputTag>("nopumetSrc") ), 
  pucmetSrc_( iConfig.getParameter<edm::InputTag>("pucmetSrc") ), 
  pumetSrc_( iConfig.getParameter<edm::InputTag>("pumetSrc") ), 
  recBosonSrc_( iConfig.getParameter<edm::InputTag>("recBosonSrc") ),
  jetSrc_( iConfig.getParameter<edm::InputTag>("jetSrc") ),
  leadJetSrc_( iConfig.getParameter<edm::InputTag>("leadJetSrc") ),
  vertexSrc_( iConfig.getParameter<edm::InputTag>("vertexSrc") ),
  nJetsPtGt1Src_( iConfig.getParameter<edm::InputTag>("nJetsPtGt1Src") ),
  rhoSrc_( iConfig.getParameter<edm::InputTag>("rhoSrc") ),
  deltaRCut_(0.5),
  enable_( iConfig.getParameter<bool>("enable") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
  
/*   std::string fileCorrectTo = iConfig.getParameter<std::string>("fileCorrectTo"); */
/*   std::string fileZmmData = iConfig.getParameter<std::string>("fileZmmData"); */
/*   std::string fileZmmMC = iConfig.getParameter<std::string>("fileZmmMC"); */
  

  mvaMet_ = new MVAMet(0.1);
  mvaMet_->Initialize(iConfig,
		      TString((getenv("CMSSW_BASE")+string("/src/CMGTools/Common/data/MVAMet/gbrmet_42.root"))),        //U
		      TString((getenv("CMSSW_BASE")+string("/src/CMGTools/Common/data/MVAMet/gbrmetphi_42.root"))),     //U Phi
		      TString((getenv("CMSSW_BASE")+string("/src/CMGTools/Common/data/MVAMet/gbrmetu1cov_42.root"))),   //U1 Cov
		      TString((getenv("CMSSW_BASE")+string("/src/CMGTools/Common/data/MVAMet/gbrmetu2cov_42.root"))));    //U2 Cov );
		      
   
  // will produce one BaseMET for each recBoson 
  produces< std::vector<MetType> >();
}


template< typename RecBosonType >
void MVAMETProducer<RecBosonType>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  typedef std::auto_ptr< std::vector< MetType > >  OutPtr;

  edm::Handle< std::vector<MetType> > pfmetH;
  iEvent.getByLabel(pfmetSrc_, pfmetH);

  edm::Handle< std::vector<MetType> > tkmetH;
  iEvent.getByLabel(tkmetSrc_, tkmetH);

  edm::Handle< std::vector<MetType> > nopumetH;
  iEvent.getByLabel(nopumetSrc_, nopumetH);

  edm::Handle< std::vector<MetType> > pucmetH;
  iEvent.getByLabel(pucmetSrc_, pucmetH);

  edm::Handle< std::vector<MetType> > pumetH;
  iEvent.getByLabel(pumetSrc_, pumetH);


  edm::Handle< std::vector<RecBosonType> > recBosonH;
  iEvent.getByLabel(recBosonSrc_, recBosonH);


  if( pfmetH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  if( tkmetH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  if( nopumetH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  if( pucmetH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  if( pumetH->size()!=1) 
    throw cms::Exception("Input MET collection should have size 1.");

  const reco::PFMET* pfmet = &( (*pfmetH)[0] );
  const reco::PFMET* tkmet = &( (*tkmetH)[0] );
  const reco::PFMET* nopumet = &( (*nopumetH)[0] );
  const reco::PFMET* pucmet = &( (*pucmetH)[0] );
  const reco::PFMET* pumet = &( (*pumetH)[0] );


  const MetType& met = (*pfmetH)[0];
  if( ! enable_ ) {
    // when disabled, put one copy of the PF MET in the output collection
    // for each rec boson in input. 
    OutPtr pOut(new std::vector< MetType >(recBosonH->size(), met) );
    iEvent.put( pOut ); 
    return;
  }

  edm::Handle< std::vector<JetType> > jetH;
  iEvent.getByLabel(jetSrc_, jetH);

  edm::Handle< std::vector<cmg::BaseJet> > leadJetH;
  iEvent.getByLabel(leadJetSrc_, leadJetH);

  assert( leadJetH->size() > 1 );
  LorentzVector leadJet = (*leadJetH)[0].p4();
  LorentzVector leadJet2 = (*leadJetH)[1].p4();

  int nJetsPtGt30 = 0; 
  for( unsigned i=0; i<jetH->size(); ++i) {
    if( jetH->at(i).pt() > 30 )
      nJetsPtGt30++; 
  }

  edm::Handle< reco::VertexCollection > vertexH;
  iEvent.getByLabel(vertexSrc_, vertexH);

  unsigned nGoodVtx = vertexH->size();

  edm::Handle< int > nJetsPtGt1H;
  iEvent.getByLabel(nJetsPtGt1Src_, nJetsPtGt1H);
  int nJetsPtGt1 = *nJetsPtGt1H;

  edm::Handle< double > rhoH;
  iEvent.getByLabel(rhoSrc_, rhoH);
  double rho = *rhoH;

  

  if(verbose_) {
    std::cout<<"---"<<std::endl;
    std::cout<<"MVAMETProducer"<<std::endl;
    std::cout<<"\tpfmet = "<<pfmet->pt()<<std::endl;
    std::cout<<"\ttkmet = "<<tkmet->pt()<<std::endl;
    std::cout<<"\tnopumet = "<<nopumet->pt()<<std::endl;
    std::cout<<"\tpucmet = "<<pucmet->pt()<<std::endl;
    std::cout<<"\tpumet = "<<pumet->pt()<<std::endl;
    std::cout<<"\tnJetsPtGt1 = "<<nJetsPtGt1<<std::endl;
    std::cout<<"\tnJetsPtGt30 = "<<nJetsPtGt30<<std::endl;
    std::cout<<"\tnGoodVtx = "<<nGoodVtx<<std::endl;
    std::cout<<"\trho = "<<rho<<std::endl;
    std::cout<<"\tjet1 eta,pt= "<<leadJet.pt()<<","<<leadJet.eta()<<std::endl;
    std::cout<<"\tjet2 eta,pt= "<<leadJet2.pt()<<","<<leadJet2.eta()<<std::endl;
    
  }

  std::vector<MetUtilities::JetInfo> jetInfo; 
  makeJets( jetInfo, *jetH, *vertexH, rho );
  
  OutPtr pOut(new std::vector< MetType > ); 
  for( unsigned i=0; i<recBosonH->size(); ++i) {
    const RecBosonType& recBoson = recBosonH->at(i);
    
    LorentzVector lCand0 = recBoson.leg1().p4();
    LorentzVector lCand1 = recBoson.leg2().p4();
    std::vector<LorentzVector> lVisible;
    lVisible.push_back(lCand0);
    lVisible.push_back(lCand1);

    // FIXME clean up the jets? 
    std::pair<LorentzVector,TMatrixD> lMVAMetInfo
      = mvaMet_->GetMet( lVisible,
			 pfmet,
			 tkmet,
			 nopumet,
			 pumet,
			 pucmet,
			 &leadJet,
			 &leadJet2,
			 nJetsPtGt30,
			 nJetsPtGt1,
			 nGoodVtx,
			 jetInfo, 
			 false );

    pOut->push_back( met );
    pOut->back().setP4( lMVAMetInfo.first ); 

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<recBoson<<std::endl;
      std::cout<<"\t\tleg1: "<<recBoson.leg1()<<std::endl;
      std::cout<<"\t\tleg2: "<<recBoson.leg2()<<std::endl;
      std::cout<<"\t\tNEW MET: "<<lMVAMetInfo.first.Et()<<std::endl;
    }
    // FIXME add matrix
  }
  
  iEvent.put( pOut ); 

  if(verbose_) {
    std::cout<<"MVAMETProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}

template< typename RecBosonType >
void MVAMETProducer< RecBosonType >::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
					      const std::vector<JetType>& iCJets,
					      const reco::VertexCollection &iVertices,double iRho) const { 
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const JetType     *pCJet  = &(iCJets.at(i1));
/*     if( !passPFLooseId(pCJet) ) continue; */
/*     double lJec = 0; */
/*     double lMVA = jetMVA(pCJet,lJec,iVertices[0],iVertices,false); */
/*     double lNeuFrac = (pCJet->neutralEmEnergy()/pCJet->energy() + pCJet->neutralHadronEnergy()/pCJet->energy()); */
    // FIXME choose the correct mva
    if( ! pCJet->getSelection("cuts_looseJetId") ) continue;
    double lMVA = pCJet->passPuJetId("full", PileupJetIdentifier::kMedium );
    // FIXME compute properly, according to what Phil does
    double lNeuFrac = pCJet->component( reco::PFCandidate::gamma ).fraction() + pCJet->component( reco::PFCandidate::h0 ).fraction() + pCJet->component( reco::PFCandidate::h_HF ).fraction() + pCJet->component( reco::PFCandidate::egamma_HF ).fraction();
    MetUtilities::JetInfo pJetObject; 
    pJetObject.p4       = pCJet->p4(); 
    pJetObject.mva      = lMVA;
    pJetObject.neutFrac = lNeuFrac;
    iJetInfo.push_back(pJetObject);
  }
}

/* template< typename RecBosonType > */
/* int  MVAMETProducer< RecBosonType >::nJets( const JetCollectionType& jets,  */
/* 							const RecBosonType& boson, float deltaR) { */
  
/*   //COLIN : check that I should really remove jets matched to both legs */
/*   // when working with Ws. */
/*   if(verbose_) { */
/*     std::cout<<"\tMatching jets to boson legs"<<std::endl; */
/*   } */
/*   float deltaR2 = deltaR*deltaR;  */
  
/*   unsigned nJets = 0; */
/*   for(unsigned iJet = 0; iJet<jets.size(); ++iJet ) { */
/*     const JetType& jet = jets[iJet];  */

/*     double dR2leg1 = reco::deltaR2( jet.eta(), jet.phi(),  */
/* 				    boson.leg1().eta(), boson.leg1().phi() ) ; */
/*     double dR2leg2 = reco::deltaR2( jet.eta(), jet.phi(),  */
/* 				    boson.leg2().eta(), boson.leg2().phi() ) ; */
    
/*     if( dR2leg1 > deltaR2 && dR2leg2 > deltaR2) { */
/*       // this jet is far enough from both boson legs, and counted as such */
/*       nJets++; */
/*     } */
/*     else{ */
/*       if(verbose_) { */
/* 	std::cout<<"\t\texcluding jet "<<jet<<", dR1,dR2=" */
/* 		 <<sqrt(dR2leg1)<<", "<<sqrt(dR2leg2)<<std::endl; */
/* 	//       std::cout<<jet.rawFactor()<<std::endl; */
/* 	//       for(int ic=0; ic<jet.nConstituents(); ++ic) { */
/* 	// 	const cmg::PFJetComponent& comp = jet.component(ic); */
/* 	// 	std::cout<<comp<<std::endl; */
/*       } */
/*     } */
/*   }   */

/*   return nJets; */
/* } */


// #include "FWCore/Framework/interface/MakerMacros.h"

// DEFINE_FWK_MODULE(MVAMETProducer);
