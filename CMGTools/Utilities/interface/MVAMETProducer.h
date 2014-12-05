
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
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "CMGTools/H2TauTau/interface/METSignificance.h"

#include "CMGTools/Utilities/interface/MVAMet.h"
#include "CMGTools/Common/interface/MetUtilities.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"

#include <sstream>

/// This calculates the MVA MET as for the 2012 analysis.
/// The class should be updated once a proper JetMET recipe based on
/// miniAOD is available.

template< typename T, typename U >
class MVAMETProducer : public edm::EDProducer {

public:
  typedef pat::CompositeCandidate RecBosonType;
  typedef T Leg1Type;
  typedef U Leg2Type;
  typedef reco::PFMET MetType;
  typedef pat::Jet JetType;
  typedef std::vector<JetType> JetCollectionType;
  typedef math::XYZTLorentzVector LorentzVector;

  explicit MVAMETProducer(const edm::ParameterSet & iConfig);
  virtual ~MVAMETProducer() { 
  }
  
private:

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  void makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
		const std::vector<JetType>& iCJets,
		const reco::VertexCollection &iVertices,double iRho) const;

  bool passJetID(const JetType& jet);

  edm::InputTag pfmetSrc_;
  edm::InputTag tkmetSrc_;
  edm::InputTag nopumetSrc_;
  edm::InputTag pucmetSrc_;
  edm::InputTag pumetSrc_;

  edm::InputTag recBosonSrc_;
  edm::InputTag jetSrc_;
  std::string puJetIdLabel_;
  
  double minJetPt_;
  double maxJetEta_;

  edm::InputTag vertexSrc_;

  // edm::InputTag nJetsPtGt1Src_;

  edm::InputTag rhoSrc_;

  /// cut for the matching between the jets and the rec boson legs. 
  float deltaRCut_;
  
  MVAMet*  mvaMet_;

  bool enable_;
  bool verbose_;
};



template< typename T, typename U >
MVAMETProducer< T, U >::MVAMETProducer(const edm::ParameterSet & iConfig) : 
  pfmetSrc_( iConfig.getParameter<edm::InputTag>("pfmetSrc") ), 
  tkmetSrc_( iConfig.getParameter<edm::InputTag>("tkmetSrc") ), 
  nopumetSrc_( iConfig.getParameter<edm::InputTag>("nopumetSrc") ), 
  pucmetSrc_( iConfig.getParameter<edm::InputTag>("pucmetSrc") ), 
  pumetSrc_( iConfig.getParameter<edm::InputTag>("pumetSrc") ), 
  recBosonSrc_( iConfig.getParameter<edm::InputTag>("recBosonSrc") ),
  jetSrc_( iConfig.getParameter<edm::InputTag>("jetSrc") ),
  puJetIdLabel_( iConfig.getParameter<std::string>("puJetIdLabel") ),
  minJetPt_( iConfig.getParameter<double>("minJetPt")),
  maxJetEta_( iConfig.getParameter<double>("maxJetEta")),
  vertexSrc_( iConfig.getParameter<edm::InputTag>("vertexSrc") ),
  // nJetsPtGt1Src_( iConfig.getParameter<edm::InputTag>("nJetsPtGt1Src") ),
  rhoSrc_( iConfig.getParameter<edm::InputTag>("rhoSrc") ),
  deltaRCut_(0.5),
  enable_( iConfig.getParameter<bool>("enable") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
    
  // JAN - This was never deleted?
  mvaMet_ = new MVAMet(0.1);

  mvaMet_->Initialize(iConfig,
		      TString(iConfig.getParameter<std::string>("weights_gbrmet")),        //U
		      TString(iConfig.getParameter<std::string>("weights_gbrmetphi")),     //U Phi
		      TString(iConfig.getParameter<std::string>("weights_gbrmetu1cov")),   //U1 Cov
		      TString(iConfig.getParameter<std::string>("weights_gbrmetu2cov")));  //U2 Cov
		 
//   cout<<" MVAMETProducer weight files "<<endl;
//   cout<<iConfig.getParameter<std::string>("weights_gbrmet")<<endl;
//   cout<<iConfig.getParameter<std::string>("weights_gbrmetphi")<<endl;
//   cout<<iConfig.getParameter<std::string>("weights_gbrmetu1cov")<<endl;
//   cout<<iConfig.getParameter<std::string>("weights_gbrmetu2cov")<<endl;
     
  produces< std::vector<RecBosonType> >();
  produces< std::vector<cmg::METSignificance> >();
}

// JAN - should go to external file (e.g. tools) but leave it here for now
float signalChargedFractionpT(const pat::Tau& tau) {
  float sumE = 0;
  float sumECharged = 0;
  
  const reco::CandidatePtrVector& charged = tau.signalChargedHadrCands();
  const reco::CandidatePtrVector& photons = tau.signalGammaCands();
  const reco::CandidatePtrVector& neutrals = tau.signalNeutrHadrCands();
  

  for( unsigned i=0; i<charged.size(); ++i) {
    float energy = charged[i]->pt();
    sumE += energy;
    sumECharged += energy;
  }

  for( unsigned i=0; i<photons.size(); ++i) {
    sumE += photons[i]->pt();
  }

  for( unsigned i=0; i<neutrals.size(); ++i) {
    sumE += neutrals[i]->pt();
  }

  assert(sumE>0);

  return sumECharged / sumE;
}


template< typename T, typename U >
void MVAMETProducer<T, U>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

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


  // const MetType& met = (*pfmetH)[0];
  MetType met((*pfmetH)[0]);
  if( ! enable_ ) {
    // when disabled, put one copy of the PF MET in the output collection
    // for each rec boson in input. 
    // OutPtr pOut(new std::vector< MetType >(recBosonH->size(), met) );
    // iEvent.put( pOut ); 

    std::auto_ptr< std::vector< RecBosonType > > pOutRecBoson(new std::vector<RecBosonType>(*recBosonH));
    iEvent.put(pOutRecBoson);
    return;
  }

  edm::Handle< std::vector<JetType> > jetH;
  iEvent.getByLabel(jetSrc_, jetH);


  //assert( leadJetH->size() > 1 );
  const LorentzVector *leadJet   =0; 
  const LorentzVector *leadJet2 = 0; 
  if(jetH->size() > 0 ) leadJet  = &(*jetH)[0].p4();
  if(jetH->size() > 1 ) leadJet2 = &(*jetH)[1].p4();

  edm::Handle< reco::VertexCollection > vertexH;
  iEvent.getByLabel(vertexSrc_, vertexH);

  unsigned nGoodVtx = vertexH->size();

  // edm::Handle< int > nJetsPtGt1H;
  // iEvent.getByLabel(nJetsPtGt1Src_, nJetsPtGt1H);
  int nJetsPtGt1 = 0; //*nJetsPtGt1H;

  for (const auto& jet : *jetH) {
    if (passJetID(jet))
      nJetsPtGt1++;
  }


  //cout<<" nJetsPtGt1 "<<nJetsPtGt1<<endl;

  edm::Handle< double > rhoH;
  iEvent.getByLabel(rhoSrc_, rhoH);
  double rho = *rhoH;

  std::vector<MetUtilities::JetInfo> jetInfo; 
  makeJets( jetInfo, *jetH, *vertexH, rho );
  
  // OutPtr pOut(new std::vector< MetType > ); 
  // std::auto_ptr< std::vector<cmg::METSignificance> > pOutSig( new std::vector<cmg::METSignificance>() );

  std::auto_ptr< std::vector<RecBosonType> > pOutRecBoson( new std::vector<RecBosonType>() );
  std::auto_ptr< std::vector<cmg::METSignificance> > pOutSig( new std::vector<cmg::METSignificance>() );

  for( unsigned i=0; i<recBosonH->size(); ++i) {
    const RecBosonType& recBoson = recBosonH->at(i);

    int nJetsPtGt30     = 0; 
    int nJetsPtGt1Clean = nJetsPtGt1;
    for( unsigned i=0; i<jetH->size(); ++i) {
      //cout<<"input jets "<<jetH->at(i).pt()<<" "<<jetH->at(i).eta()<<" "<<jetH->at(i).phi()<<endl;
      if( jetH->at(i).pt() < 1 ) continue; 
      if(deltaR(jetH->at(i).p4(),recBoson.daughter(0)->p4()) < 0.5) {nJetsPtGt1Clean--; continue;}
      if(deltaR(jetH->at(i).p4(),recBoson.daughter(1)->p4()) < 0.5) {nJetsPtGt1Clean--; continue;}
      //cout<<"cleaned jets "<<jetH->at(i).pt()<<" "<<jetH->at(i).eta()<<" "<<jetH->at(i).phi()<<endl;
      if( jetH->at(i).pt() < 30 ) continue;
      nJetsPtGt30++; 
    }
    if(nJetsPtGt1Clean<0) nJetsPtGt1Clean=0;
       
    LorentzVector lCand0 = recBoson.daughter(0)->p4();
    LorentzVector lCand1 = recBoson.daughter(1)->p4();
    std::vector<LorentzVector> lVisible;
    lVisible.push_back(lCand0);
    lVisible.push_back(lCand1);

    //Redefine Lead Jet
    const LorentzVector *cleanLeadJet  = leadJet;
    const LorentzVector *cleanLeadJet2 = leadJet2;
    int lIndex = 0;
    for(int i0 = 0; i0 < 2; i0++) {
      if(cleanLeadJet  == 0)            continue;
      if(cleanLeadJet2 == 0 && i0 == 1) continue;
      bool pClean = false;
      if(i0 == 0 && deltaR(*cleanLeadJet,recBoson.daughter(0)->p4()) < 0.5) pClean = true;
      if(i0 == 0 && deltaR(*cleanLeadJet,recBoson.daughter(1)->p4()) < 0.5) pClean = true;
      if(i0 == 1 && deltaR(*cleanLeadJet2,recBoson.daughter(0)->p4()) < 0.5) pClean = true;
      if(i0 == 1 && deltaR(*cleanLeadJet2,recBoson.daughter(1)->p4()) < 0.5) pClean = true;
      if(pClean) { 
	lIndex++;
	if(i0 == 0 && int(jetH->size())  > lIndex   ) cleanLeadJet  = &(*jetH)[lIndex].p4();
	if(           int(jetH->size())  > lIndex+1 ) cleanLeadJet2 = &(*jetH)[lIndex+1].p4();
	if(i0 == 0 && int(jetH->size())  < lIndex+1 ) cleanLeadJet  = 0;
	if(           int(jetH->size())  < lIndex+2 ) cleanLeadJet2 = 0;
	i0--; 
      }
    }

    math::XYZPoint dummyVertex;

    // need to clean up the MET from di-lepton legs. 
    LorentzVector cleanpfmetp4 = pfmet->p4();
    cleanpfmetp4 += recBoson.daughter(0)->p4();
    cleanpfmetp4 += recBoson.daughter(1)->p4();
    double cleanpfmetsumet = pfmet->sumEt() - recBoson.daughter(0)->pt() - recBoson.daughter(1)->pt();
    reco::PFMET cleanpfmet( pfmet->getSpecific(),
			    cleanpfmetsumet, cleanpfmetp4, dummyVertex);

    LorentzVector cleanpucmetp4 = pucmet->p4();
    cleanpucmetp4 += recBoson.daughter(0)->p4();
    cleanpucmetp4 += recBoson.daughter(1)->p4();
    double cleanpucmetsumet = pucmet->sumEt() - recBoson.daughter(0)->pt() - recBoson.daughter(1)->pt();    
    reco::PFMET cleanpucmet( pucmet->getSpecific(),
			     cleanpucmetsumet, cleanpucmetp4, dummyVertex);

    LorentzVector tau1Chargedp4 = recBoson.daughter(0)->p4();
    if (typeid(T) == typeid(pat::Tau)) {
        tau1Chargedp4 *= signalChargedFractionpT(*dynamic_cast<const pat::Tau*>(recBoson.daughter(0)));
    }
    LorentzVector tau2Chargedp4 = recBoson.daughter(1)->p4();
    if (typeid(U) == typeid(pat::Tau))
        tau2Chargedp4 *= signalChargedFractionpT(*dynamic_cast<const pat::Tau*>(recBoson.daughter(1)));
    
    LorentzVector cleantkmetp4 = tkmet->p4();
    cleantkmetp4 += tau1Chargedp4;
    cleantkmetp4 += tau2Chargedp4;
    double cleantkmetsumet = tkmet->sumEt() - tau1Chargedp4.Pt() - tau2Chargedp4.Pt();    
    reco::PFMET cleantkmet( tkmet->getSpecific(),
			    cleantkmetsumet, cleantkmetp4, dummyVertex);

    LorentzVector cleannopumetp4 = nopumet->p4();
    cleannopumetp4 += tau1Chargedp4;
    cleannopumetp4 += tau2Chargedp4;
    double cleannopumetsumet = nopumet->sumEt() - tau1Chargedp4.Pt() - tau2Chargedp4.Pt();    
    reco::PFMET cleannopumet( nopumet->getSpecific(),
			      cleannopumetsumet, cleannopumetp4, dummyVertex);


    if(verbose_) {
      std::cout<<"---"<<std::endl;
      std::cout<<"MVAMETProducer"<<std::endl;
      std::cout<<"\tpfmet = "<<pfmet->pt()<<std::endl;
      std::cout<<"\ttkmet = "<<tkmet->pt()<<std::endl;
      std::cout<<"\tnopumet = "<<nopumet->pt()<<std::endl;
      std::cout<<"\tpucmet = "<<pucmet->pt()<<std::endl;
      std::cout<<"\tpumet = "<<pumet->pt()<<std::endl;
      std::cout<<"\tnJetsPtGt1 (uncleaned) = "<<nJetsPtGt1<<std::endl;
      std::cout<<"\tnJetsPtGt1 = "<<nJetsPtGt1Clean<<std::endl;
      std::cout<<"\tnJetsPtGt30 = "<<nJetsPtGt30<<std::endl;
      std::cout<<"\tnGoodVtx = "<<nGoodVtx<<std::endl;
      std::cout<<"\trho = "<<rho<<std::endl;
      if(leadJet  != 0) std::cout<<"\tjet1 eta,pt= "<<leadJet->Et()<<","<<leadJet->eta()<<std::endl;
      if(leadJet2 != 0) std::cout<<"\tjet2 eta,pt= "<<leadJet2->Et()<<","<<leadJet2->eta()<<std::endl;
      
    }

    std::pair<LorentzVector,TMatrixD> lMVAMetInfo
      = mvaMet_->GetMet( lVisible,
			 &cleanpfmet,
			 &cleantkmet,
			 &cleannopumet,
			 pumet,
			 &cleanpucmet,
			 cleanLeadJet,
			 cleanLeadJet2,
			 nJetsPtGt30,
			 nJetsPtGt1Clean,
			 nGoodVtx,
			 jetInfo, 
			 verbose_ );


    pOutSig->push_back( lMVAMetInfo.second );

    cmg::METSignificance metSig(lMVAMetInfo.second);
    met.setP4(lMVAMetInfo.first);

    // The MET is saved in the di-tau object as it depends on the event
    // interpretation
    pOutRecBoson->push_back(RecBosonType(recBoson));
    cmg::DiTauObjectFactory<Leg1Type, Leg2Type>::set(std::make_pair(*(dynamic_cast<const T*>(recBoson.daughter(0))), *(dynamic_cast<const U*>(recBoson.daughter(1)))), met, metSig, pOutRecBoson->back());

    if(verbose_) {
      std::cout<<"  ---------------- "<<std::endl;
      std::cout<<"\trec boson: "<<recBoson<<std::endl;
      std::cout<<"\t\tleg1: "<<recBoson.daughter(0)<<std::endl;
      std::cout<<"\t\tleg2: "<<recBoson.daughter(1)<<std::endl;
      std::cout<<"\t\tNEW MET: "<<lMVAMetInfo.first.Pt()<<"   "<<lMVAMetInfo.first.Phi()<<std::endl;
      std::cout<<""<<endl;
    }
  }
  
  iEvent.put( pOutSig ); 
  iEvent.put( pOutRecBoson );

  if(verbose_) {
    std::cout<<"MVAMETProducer done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}

template< typename T, typename U >
void MVAMETProducer< T, U >::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
					      const std::vector<JetType>& iCJets,
					      const reco::VertexCollection &iVertices,double iRho) const { 
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const JetType     *pCJet  = &(iCJets.at(i1));

    if(fabs(pCJet->eta()) <= 2.4){
      if(!(pCJet->neutralHadronEnergyFraction() < 0.99
           &&pCJet->photonEnergyFraction() < 0.99
           &&pCJet->nConstituents() > 1
           &&pCJet->chargedHadronEnergyFraction() > 0
           &&pCJet->chargedHadronMultiplicity() > 0
           &&pCJet-> electronEnergyFraction () < 0.99) 
         ) continue ;
    } else {
      if(!(pCJet->neutralHadronEnergyFraction() < 0.99
           &&pCJet->photonEnergyFraction() < 0.99
           &&pCJet->nConstituents() > 1)
         ) continue;
    }

    if (pCJet->pt() < minJetPt_)
      continue;
    if (std::abs(pCJet->eta()) > maxJetEta_)
      continue;

    // Cut on PU jet ID value is applied in METUtilities
    double lMVA = pCJet->userFloat(puJetIdLabel_.c_str());

    double lNeuFrac = 1.;
    if (fabs(pCJet->eta())<2.5)
      lNeuFrac = pCJet->photonEnergyFraction() + pCJet->neutralHadronEnergyFraction();


    MetUtilities::JetInfo pJetObject; 
    pJetObject.p4       = pCJet->p4(); 
    pJetObject.mva      = lMVA;
    pJetObject.neutFrac = lNeuFrac;

    iJetInfo.push_back(pJetObject);
  }
}

// JAN - this is error-prone - apply jet ID in fewer places
template< typename T, typename U >
bool MVAMETProducer<T, U>::passJetID(const MVAMETProducer::JetType& jet){
      if(fabs(jet.eta()) <= 2.4){
      if(!(jet.neutralHadronEnergyFraction() < 0.99
           &&jet.photonEnergyFraction() < 0.99
           &&jet.nConstituents() > 1
           &&jet.chargedHadronEnergyFraction() > 0
           &&jet.chargedHadronMultiplicity() > 0
           &&jet. electronEnergyFraction () < 0.99) 
         ) return false ;
    } else {
      if(!(jet.neutralHadronEnergyFraction() < 0.99
           &&jet.photonEnergyFraction() < 0.99
           &&jet.nConstituents() > 1)
         ) return false;
    }
  return true;
}
