
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
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/Utilities/interface/MVAMet.h"
#include "CMGTools/Common/interface/MetUtilities.h"
#include "CMGTools/External/interface/PileupJetIdentifier.h"

#include <sstream>


class MVAMETProducerPreselLep : public edm::EDProducer {

public:
  typedef reco::PFMET MetType;
  typedef cmg::PFJet   JetType;
  typedef std::vector<JetType>           JetCollectionType;
  typedef math::XYZTLorentzVector LorentzVector;

  explicit MVAMETProducerPreselLep(const edm::ParameterSet & iConfig);
  virtual ~MVAMETProducerPreselLep() { 
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

  edm::InputTag preselMuonListTag_;
  edm::InputTag preselElectronListTag_;
  edm::InputTag preselTauListTag_;

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



MVAMETProducerPreselLep::MVAMETProducerPreselLep(const edm::ParameterSet & iConfig) : 
  pfmetSrc_( iConfig.getParameter<edm::InputTag>("pfmetSrc") ), 
  tkmetSrc_( iConfig.getParameter<edm::InputTag>("tkmetSrc") ), 
  nopumetSrc_( iConfig.getParameter<edm::InputTag>("nopumetSrc") ), 
  pucmetSrc_( iConfig.getParameter<edm::InputTag>("pucmetSrc") ), 
  pumetSrc_( iConfig.getParameter<edm::InputTag>("pumetSrc") ), 
  preselMuonListTag_( iConfig.getParameter<edm::InputTag>("preselMuonListTag") ),
  preselElectronListTag_( iConfig.getParameter<edm::InputTag>("preselElectronListTag") ),
  preselTauListTag_( iConfig.getParameter<edm::InputTag>("preselTauListTag") ),
  jetSrc_( iConfig.getParameter<edm::InputTag>("jetSrc") ),
  leadJetSrc_( iConfig.getParameter<edm::InputTag>("leadJetSrc") ),
  vertexSrc_( iConfig.getParameter<edm::InputTag>("vertexSrc") ),
  nJetsPtGt1Src_( iConfig.getParameter<edm::InputTag>("nJetsPtGt1Src") ),
  rhoSrc_( iConfig.getParameter<edm::InputTag>("rhoSrc") ),
  deltaRCut_(0.5),
  enable_( iConfig.getParameter<bool>("enable") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) ) {
    
  mvaMet_ = new MVAMet(0.1);

  mvaMet_->Initialize(iConfig,
		      TString(iConfig.getParameter<std::string>("weights_gbrmet")),        //U
		      TString(iConfig.getParameter<std::string>("weights_gbrmetphi")),     //U Phi
		      TString(iConfig.getParameter<std::string>("weights_gbrmetu1cov")),   //U1 Cov
		      TString(iConfig.getParameter<std::string>("weights_gbrmetu2cov")));  //U2 Cov
		      
   
  // will produce one BaseMET for each recBoson 
  produces< std::vector<MetType> >();
  produces< std::vector<cmg::METSignificance> >();
}


void MVAMETProducerPreselLep::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

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


  edm::Handle< std::vector<cmg::Muon> > preselMuonList_;
  iEvent.getByLabel(preselMuonListTag_, preselMuonList_);

  edm::Handle< std::vector<cmg::Electron> > preselElectronList_;
  iEvent.getByLabel(preselElectronListTag_, preselElectronList_);

  edm::Handle< std::vector<cmg::Tau> > preselTauList_;
  iEvent.getByLabel(preselTauListTag_, preselTauList_);

  
  if(verbose_)cout<<"Muons "<< preselMuonList_->size() <<" Electrons "<< preselElectronList_->size() <<" Taus"<< preselTauList_->size()<<endl;

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

  edm::Handle< std::vector<JetType> > jetH;
  iEvent.getByLabel(jetSrc_, jetH);

  edm::Handle< std::vector<cmg::BaseJet> > leadJetH;
  iEvent.getByLabel(leadJetSrc_, leadJetH);

  //assert( leadJetH->size() > 1 );
  const LorentzVector *leadJet   =0; 
  const LorentzVector *leadJet2 = 0; 
  if(leadJetH->size() > 0 ) leadJet  = &(*leadJetH)[0].p4();
  if(leadJetH->size() > 1 ) leadJet2 = &(*leadJetH)[1].p4();

  edm::Handle< reco::VertexCollection > vertexH;
  iEvent.getByLabel(vertexSrc_, vertexH);

  unsigned nGoodVtx = vertexH->size();

  edm::Handle< int > nJetsPtGt1H;
  iEvent.getByLabel(nJetsPtGt1Src_, nJetsPtGt1H);
  int nJetsPtGt1 = *nJetsPtGt1H;

  edm::Handle< double > rhoH;
  iEvent.getByLabel(rhoSrc_, rhoH);
  double rho = *rhoH;


  std::vector<MetUtilities::JetInfo> jetInfo; 
  makeJets( jetInfo, *jetH, *vertexH, rho );
  
  OutPtr pOut(new std::vector< MetType > ); 
  std::auto_ptr< std::vector<cmg::METSignificance> > pOutSig( new std::vector<cmg::METSignificance>() );


  std::vector<const cmg::Muon * >  cleanMuonList_;
  std::vector<const cmg::Electron *>  cleanElectronList_;
  std::vector<const cmg::Tau * >  cleanTauList_;
  
  //clean the muons from electrons based on pT
  for(std::vector<cmg::Muon>::const_iterator candM=preselMuonList_->begin(); candM!=preselMuonList_->end(); ++candM){
    bool pass=1;
    for(std::vector<cmg::Electron>::const_iterator candE=preselElectronList_->begin(); candE!=preselElectronList_->end(); ++candE)
      if( deltaR(candM->p4(),candE->p4())<0.5 && candM->pt()<candE->pt() )pass=0;
    if(pass)cleanMuonList_.push_back(&(*candM));
  }   
  //clean the electrons from muons based on pT
  for(std::vector<cmg::Electron>::const_iterator candE=preselElectronList_->begin(); candE!=preselElectronList_->end(); ++candE){
    bool pass=1;
    for(std::vector<cmg::Muon>::const_iterator candM=preselMuonList_->begin(); candM!=preselMuonList_->end(); ++candM)
      if(deltaR(candE->p4(),candM->p4())<0.5 && candE->pt()<candM->pt())pass=0;
    if(pass)cleanElectronList_.push_back(&(*candE));
  }   
  //clean the taus from muons and electrons
  for(std::vector<cmg::Tau>::const_iterator candT=preselTauList_->begin(); candT!=preselTauList_->end(); ++candT){
    bool pass=1;
    for(std::vector<cmg::Muon>::const_iterator candM=preselMuonList_->begin(); candM!=preselMuonList_->end(); ++candM)
      if(deltaR(candT->p4(),candM->p4())<0.5)pass=0;
    for(std::vector<cmg::Electron>::const_iterator candE=preselElectronList_->begin(); candE!=preselElectronList_->end(); ++candE)
      if(deltaR(candT->p4(),candE->p4())<0.5)pass=0;
    if(pass)cleanTauList_.push_back(&(*candT));
  }   
  if(verbose_)cout<<"clean Muons "<< cleanMuonList_.size() <<" Electrons "<< cleanElectronList_.size() <<" Taus"<< cleanTauList_.size()<<endl;


  /* Horrible Lepton Overlap code from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/JetMETCorrections/METPUSubtraction/plugins/PFMETProducerMVA.cc?revision=1.7&view=markup
     To make this work for CMG Tuples you must have a vector of leptons. The code below that will then remove the overlapping leptons and add the good leptons to a final vector
     Once you have the final vector, the code beyond needs to be cleand to 
     1. Run over the vector and clean up overlapping jets
     2. Chooose the two leading "Cleaned" Jets (should be straight forward)
     3. Correct the final METs with this info
  */
//   int  lId         = 0;
//   bool lHasPhotons = false;
//   std::vector<mvaMEtUtilities::leptonInfo> leptonInfo;
//   for ( vInputTag::const_iterator srcLeptons_i = srcLeptons_.begin(); srcLeptons_i != srcLeptons_.end(); ++srcLeptons_i ) {
//     edm::Handle<CandidateView> leptons;
//     evt.getByLabel(*srcLeptons_i, leptons);
//     for ( CandidateView::const_iterator lepton1 = leptons->begin(); lepton1 != leptons->end(); ++lepton1 ) {
//       bool pMatch = false;

//       for ( vInputTag::const_iterator srcLeptons_j = srcLeptons_.begin(); srcLeptons_j != srcLeptons_.end(); ++srcLeptons_j ) {
// 	edm::Handle<CandidateView> leptons2;
// 	evt.getByLabel(*srcLeptons_j, leptons2);
// 	for ( CandidateView::const_iterator lepton2 = leptons2->begin(); lepton2 != leptons2->end(); ++lepton2 ) {
// 	  if(&(*lepton1) == &(*lepton2)) continue;
// 	  if(deltaR(lepton1->p4(),lepton2->p4()) < 0.5)                                                                    pMatch = true;
// 	  if(pMatch 
// 	     &&     !istau(&(*lepton1)) &&  istau(&(*lepton2)))                                                     pMatch = false;
// 	  if(pMatch 
// 	     &&    ( (istau(&(*lepton1)) && istau(&(*lepton2))) || (!istau(&(*lepton1)) && !istau(&(*lepton2)))) 
// 	     &&     lepton1->pt() > lepton2->pt())                                                                  pMatch = false;
// 	  if(pMatch && lepton1->pt() == lepton2->pt()) {
// 	    pMatch = false;
// 	    for(unsigned int i0 = 0; i0 < leptonInfo.size(); i0++) {
// 	      if(fabs(lepton1->pt() - leptonInfo[i0].p4_.pt()) < 0.1) pMatch = true;
// 	    }
// 	  }
// 	  if(pMatch) break;
// 	}
// 	if(pMatch) break;
//       }

//       if(pMatch) continue;
//       mvaMEtUtilities::leptonInfo pLeptonInfo;
//       pLeptonInfo.p4_          = lepton1->p4();
//       pLeptonInfo.chargedFrac_ = chargedFrac(&(*lepton1),*pfCandidates,hardScatterVertex);
//       leptonInfo.push_back(pLeptonInfo); 
//       if(lepton1->isPhoton()) lHasPhotons = true;
//     }
//     lId++;
//   }

    


  int nJetsPtGt30     = 0; 
  int nJetsPtGt1Clean = nJetsPtGt1;
  for( unsigned i=0; i<jetH->size(); ++i) {
    if( jetH->at(i).pt() < 1 ) continue; 
    for( unsigned l=0; l<cleanMuonList_.size(); ++l)
      if(deltaR(jetH->at(i).p4(),cleanMuonList_.at(l)->p4()) < 0.5) {nJetsPtGt1Clean--; continue;}
    for( unsigned l=0; l<cleanElectronList_.size(); ++l)
      if(deltaR(jetH->at(i).p4(),cleanElectronList_.at(l)->p4()) < 0.5) {nJetsPtGt1Clean--; continue;}
    for( unsigned l=0; l<cleanTauList_.size(); ++l)
      if(deltaR(jetH->at(i).p4(),cleanTauList_.at(l)->p4()) < 0.5) {nJetsPtGt1Clean--; continue;}
    if( jetH->at(i).pt() < 30 ) continue;
    nJetsPtGt30++; 
  }
  if(nJetsPtGt1Clean<0) nJetsPtGt1Clean=0;


  std::vector<LorentzVector> lVisible;
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    lVisible.push_back(cleanMuonList_.at(l)->p4());
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    lVisible.push_back(cleanElectronList_.at(l)->p4());
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    lVisible.push_back(cleanTauList_.at(l)->p4());

  //Redefine Lead Jet
  const LorentzVector *cleanLeadJet  = leadJet;
  const LorentzVector *cleanLeadJet2 = leadJet2;
  int lIndex = 0;
  for(int i0 = 0; i0 < 2; i0++) {
    if(cleanLeadJet  == 0)            continue;
    if(cleanLeadJet2 == 0 && i0 == 1) continue;
    bool pClean = false;
      
    if(i0==0){
      for( unsigned l=0; l<cleanMuonList_.size(); ++l)
	if(deltaR(*cleanLeadJet,cleanMuonList_.at(l)->p4()) < 0.5) pClean = true;
      for( unsigned l=0; l<cleanElectronList_.size(); ++l)
	if(deltaR(*cleanLeadJet,cleanElectronList_.at(l)->p4()) < 0.5) pClean = true;
      for( unsigned l=0; l<cleanTauList_.size(); ++l)
	if(deltaR(*cleanLeadJet,cleanTauList_.at(l)->p4()) < 0.5) pClean = true;
    }
    
    if(i0==1){
      for( unsigned l=0; l<cleanMuonList_.size(); ++l)
	if(deltaR(*cleanLeadJet2,cleanMuonList_.at(l)->p4()) < 0.5) pClean = true;
      for( unsigned l=0; l<cleanElectronList_.size(); ++l)
	if(deltaR(*cleanLeadJet2,cleanElectronList_.at(l)->p4()) < 0.5) pClean = true;
      for( unsigned l=0; l<cleanTauList_.size(); ++l)
	if(deltaR(*cleanLeadJet2,cleanTauList_.at(l)->p4()) < 0.5) pClean = true;
    }
    
    
    if(pClean) { 
      lIndex++;
      if(i0 == 0 && int(leadJetH->size())  > lIndex   ) cleanLeadJet  = &(*leadJetH)[lIndex].p4();
      if(           int(leadJetH->size())  > lIndex+1 ) cleanLeadJet2 = &(*leadJetH)[lIndex+1].p4();
      if(i0 == 0 && int(leadJetH->size())  < lIndex+1 ) cleanLeadJet  = 0;
      if(           int(leadJetH->size())  < lIndex+2 ) cleanLeadJet2 = 0;
      i0--; 
    }
  }

  math::XYZPoint dummyVertex;
  // need to clean up the MET from di-lepton legs. 

  ////////////PFMET
  LorentzVector cleanpfmetp4 = pfmet->p4();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleanpfmetp4 += cleanMuonList_.at(l)->p4();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleanpfmetp4 += cleanElectronList_.at(l)->p4();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleanpfmetp4 += cleanTauList_.at(l)->p4();
  double cleanpfmetsumet = pfmet->sumEt();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleanpfmetsumet -= cleanMuonList_.at(l)->pt();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleanpfmetsumet -= cleanElectronList_.at(l)->pt();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleanpfmetsumet -= cleanTauList_.at(l)->pt();
  reco::PFMET cleanpfmet( pfmet->getSpecific(), cleanpfmetsumet, cleanpfmetp4, dummyVertex);


  ////PUC MET
  LorentzVector cleanpucmetp4 = pucmet->p4();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleanpucmetp4 += cleanMuonList_.at(l)->p4();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleanpucmetp4 += cleanElectronList_.at(l)->p4();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleanpucmetp4 += cleanTauList_.at(l)->p4();
  double cleanpucmetsumet = pucmet->sumEt();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleanpucmetsumet -= cleanMuonList_.at(l)->pt();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleanpucmetsumet -= cleanElectronList_.at(l)->pt();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleanpucmetsumet -= cleanTauList_.at(l)->pt();
  reco::PFMET cleanpucmet( pucmet->getSpecific(), cleanpucmetsumet, cleanpucmetp4, dummyVertex);


  /////////////////////////////////Here use only the charged components

  //////////Track MET
  LorentzVector cleantkmetp4 = tkmet->p4();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleantkmetp4 += cleanMuonList_.at(l)->p4();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleantkmetp4 += cleanElectronList_.at(l)->p4();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleantkmetp4 += cleanTauList_.at(l)->p4() * cleanTauList_.at(l)->signalChargedFraction();
  double cleantkmetsumet = tkmet->sumEt();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleantkmetsumet -= cleanMuonList_.at(l)->pt();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleantkmetsumet -= cleanElectronList_.at(l)->pt();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleantkmetsumet -= cleanTauList_.at(l)->pt()* cleanTauList_.at(l)->signalChargedFraction();
  reco::PFMET cleantkmet( tkmet->getSpecific(), cleantkmetsumet, cleantkmetp4, dummyVertex);

  ///////No PU MET
  LorentzVector cleannopumetp4 = nopumet->p4();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleannopumetp4 += cleanMuonList_.at(l)->p4();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleannopumetp4 += cleanElectronList_.at(l)->p4();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleannopumetp4 += cleanTauList_.at(l)->p4() * cleanTauList_.at(l)->signalChargedFraction();
  double cleannopumetsumet = nopumet->sumEt();
  for( unsigned l=0; l<cleanMuonList_.size(); ++l)
    cleannopumetsumet -= cleanMuonList_.at(l)->pt();
  for( unsigned l=0; l<cleanElectronList_.size(); ++l)
    cleannopumetsumet -= cleanElectronList_.at(l)->pt();
  for( unsigned l=0; l<cleanTauList_.size(); ++l)
    cleannopumetsumet -= cleanTauList_.at(l)->pt() * cleanTauList_.at(l)->signalChargedFraction();
  reco::PFMET cleannopumet( nopumet->getSpecific(), cleannopumetsumet, cleannopumetp4, dummyVertex);
  ////////////////////////////////////////////////////////////////////////////////




  if(verbose_) {
    std::cout<<"---"<<std::endl;
    std::cout<<"MVAMETProducerPreselLep"<<std::endl;
    std::cout<<"\tpfmet = "<<pfmet->pt()<<std::endl;
    std::cout<<"\ttkmet = "<<tkmet->pt()<<std::endl;
    std::cout<<"\tnopumet = "<<nopumet->pt()<<std::endl;
    std::cout<<"\tpucmet = "<<pucmet->pt()<<std::endl;
    std::cout<<"\tpumet = "<<pumet->pt()<<std::endl;
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
		       false );

  // if I do that, sumEt is incorrect...
  pOut->push_back( met );
  pOutSig->push_back( lMVAMetInfo.second );
  pOut->back().setP4( lMVAMetInfo.first ); 

  if(verbose_) {
    std::cout<<"  ---------------- "<<std::endl;
    //       std::cout<<"\trec boson: "<<recBoson<<std::endl;
    //       std::cout<<"\t\tleg1: "<<recBoson.leg1()<<std::endl;
    //       std::cout<<"\t\tleg2: "<<recBoson.leg2()<<std::endl;
    std::cout<<"\t\tNEW MET: "<<lMVAMetInfo.first.Pt()<<std::endl;
    std::cout<<"\t\tNEW MET Cov00: "<<lMVAMetInfo.second[0][0]<<std::endl;
  }
  // FIXME add matrix

  
  iEvent.put( pOut ); 
  iEvent.put( pOutSig ); 

  if(verbose_) {
    std::cout<<"MVAMETProducerPreselLep done"<<std::endl;
    std::cout<<"***"<<std::endl;
  }
}

void MVAMETProducerPreselLep::makeJets(std::vector<MetUtilities::JetInfo> &iJetInfo,
				       const std::vector<JetType>& iCJets,
				       const reco::VertexCollection &iVertices,double iRho) const { 
  for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                         
    const JetType     *pCJet  = &(iCJets.at(i1));
    
    //PF jet id 
    if(fabs(pCJet->eta()) <= 2.4){
      if(!(pCJet->component(5).fraction() < 0.99
           &&pCJet->component(4).fraction() < 0.99
           &&pCJet->nConstituents() > 1
           &&pCJet->component(1).fraction() > 0
           &&pCJet->component(1).number() > 0
           &&pCJet->component(2).fraction() < 0.99) 
         ) continue ;
    } else {
      if(!(pCJet->component(5).fraction() < 0.99
           &&pCJet->component(4).fraction() < 0.99
           &&pCJet->nConstituents() > 1)
         ) continue;
    }

    double lMVA = pCJet->puMva("met53x"); 
    double lNeuFrac = 1.;
    if (fabs(pCJet->eta())<2.5)
      lNeuFrac = pCJet->component( reco::PFCandidate::gamma ).fraction() + pCJet->component( reco::PFCandidate::h0 ).fraction();


    MetUtilities::JetInfo pJetObject; 
    pJetObject.p4       = pCJet->p4(); 
    pJetObject.mva      = lMVA;
    pJetObject.neutFrac = lNeuFrac;

    //std::cout<<" MVAMETProducer::makeJets  final:  "<<i1<<"  "<<pJetObject.p4.pt()<<" "<<pJetObject.mva<<" "<<pJetObject.neutFrac<<std::endl;

    iJetInfo.push_back(pJetObject);

  }
}
