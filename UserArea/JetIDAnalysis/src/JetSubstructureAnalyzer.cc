// -*- C++ -*-
//
// Package:    JetSubstructureAnalyzer
// Class:      JetSubstructureAnalyzer
// 
/**\class JetSubstructureAnalyzer JetSubstructureAnalyzer.cc HggAnalysis/JetSubstructureAnalyzer/src/JetSubstructureAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Martina Malberti,27 2-019,+41227678349,
//         Created:  Mon Mar  5 16:39:53 CET 2012
// $Id: JetSubstructureAnalyzer.cc,v 1.23 2012/08/20 13:11:40 musella Exp $
//
//


// system include files
#include "CMG/JetIDAnalysis/interface/JetSubstructureAnalyzer.h"


// 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TROOT.h"

//
// constructors and destructor
//
JetSubstructureAnalyzer::JetSubstructureAnalyzer(const edm::ParameterSet& iConfig)

{
  //--- inputs
  MCPileupTag_ = iConfig.getParameter<edm::InputTag>("MCPileupTag");
  PVTag_       = iConfig.getParameter<edm::InputTag>("PVTag");
  JetTag_      = iConfig.getParameter<edm::InputTag>("JetTag");
  GenJetTag_   = iConfig.getParameter<edm::InputTag>("GenJetTag");
  MuonTag_     = iConfig.getParameter<edm::InputTag>("MuonTag");

  applyJec_   = iConfig.getParameter<bool>("applyJec");
  jecTag_     = iConfig.getParameter<std::string>("jecTag");
  RhoTag_     = iConfig.getParameter<edm::InputTag>("RhoTag");
  residualsFromTxt_ = iConfig.getParameter<bool>("residualsFromTxt");
  residualsTxt_ = iConfig.getParameter<edm::FileInPath>("residualsTxt");

  dataFlag_    = iConfig.getUntrackedParameter<bool>("dataFlag");

  //--- PU jet identifier 
  puIdAlgo_ = new PileupJetIdSubstructureNtupleAlgo(iConfig.getParameter<edm::ParameterSet>("puJetIDAlgo"));
  computeTMVA_ = iConfig.getUntrackedParameter<bool>("computeTMVA");

  MvaTags_ = iConfig.getUntrackedParameter< std::vector<edm::InputTag> >("MvaTags", std::vector<edm::InputTag>() );
  IdTags_ = iConfig.getUntrackedParameter< std::vector<edm::InputTag> >("IdTags", std::vector<edm::InputTag>() );
  
  requireZ_ = iConfig.getUntrackedParameter<bool>("requireZ",true);
  
  //-- loose jet ID
  pfjetIdLoose_  = iConfig.getParameter<edm::ParameterSet>("pfjetIdLoose") ;

  //-- jet pt threshold : only jets above this thr are saved in the tree
  jetPtThreshold_ = iConfig.getUntrackedParameter<double>("jetPtThreshold");

  jecCor_ = 0;
  bookTree();
}

void JetSubstructureAnalyzer::bookTree()
{
  //-- output tree
  edm::Service<TFileService> fs ;
  tree =        fs -> make <TTree>("tree","tree"); 

  puIdAlgo_->bookBranches(tree);
  mvas_.resize(MvaTags_.size(),-2.);
  for(size_t itag=0; itag<MvaTags_.size(); ++itag) {
	  tree->Branch(MvaTags_[itag].instance().c_str(),&mvas_[itag], (MvaTags_[itag].instance()+"/F").c_str());
  }
  ids_.resize(IdTags_.size(),0);
  for(size_t itag=0; itag<IdTags_.size(); ++itag) {
	  tree->Branch(IdTags_[itag].instance().c_str(),&ids_[itag], (IdTags_[itag].instance()+"/I").c_str());
  }
  
  tree -> Branch ("PUit_n",&PUit_n, "PUit_n/I");
  tree -> Branch ("PUit_nTrue",&PUit_nTrue, "PUit_nTrue/F");
  tree -> Branch ("PUoot_early_n",&PUoot_early_n, "PUoot_early_n/I");
  tree -> Branch ("PUoot_early_nTrue",&PUoot_early_nTrue, "PUoot_early_nTrue/F");
  tree -> Branch ("PUoot_late_n",&PUoot_late_n, "PUoot_late_n/I");
  tree -> Branch ("PUoot_late_nTrue",&PUoot_late_nTrue, "PUoot_late_nTrue/F");
  /// tree -> Branch ("nvtx",&nvtx, "nvtx/I"); comes from jetId algo
  tree -> Branch ("jetLooseID", &jetLooseID, "jetLooseID/O");
  tree -> Branch ("isMatched", &isMatched, "isMatched/O");
  tree -> Branch ("jetFlavour",&jetFlavour, "jetFlavour/I");
  tree -> Branch ("jetGenPt",&jetGenPt, "jetGenPt/F");
  tree -> Branch ("jetGenDr",&jetGenDr, "jetGenDr/F");
  tree -> Branch ("njets",&njets, "njets/I");
  tree -> Branch ("dimuonPt",&dimuonPt, "dimuonPt/F");
  tree -> Branch ("dphiZJet",&dphiZJet, "dphiZJet/F");


}


void JetSubstructureAnalyzer::ResetTreeVariables()
{
  PUit_n     = -999;
  PUit_nTrue = -999;
  PUoot_early_n     = -999;
  PUoot_early_nTrue = -999;
  PUoot_late_n     = -999;
  PUoot_late_nTrue = -999;
 
  nvtx       = -999;
  jetLooseID = false;
  isMatched  = false;
  jetFlavour = -999;
  jetGenPt   = -999;
  jetGenDr   = -999;
  njets      = -999;
  dimuonPt   = -999; 
  dphiZJet   = -999; 
}


JetSubstructureAnalyzer::~JetSubstructureAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}



// ------------ method called for each event  ------------
void
JetSubstructureAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;

  std::vector<float> jecs;
  double rho = 0.;
  if( applyJec_  ) {
    edm::Handle< double > rhoH;
    iEvent.getByLabel(RhoTag_,rhoH);
    rho = *rhoH;
    if( jecCor_ == 0 ) {
      initJetEnergyCorrector( iSetup, iEvent.isRealData() );
    }
  }
  
  /// if( tree == 0 ) { bookTree(); }

  // *** PILEUP INFO
  if (!dataFlag_) FillMCPileUpInfo(iEvent, iSetup);

  // *** VERTEX COLLECTION
  edm::Handle<reco::VertexCollection> vertexHandle;
  iEvent.getByLabel(PVTag_, vertexHandle);
  reco::VertexCollection vertexCollection = *(vertexHandle.product());
  
  //-- primary vtx 
  //   require basic quality cuts on the vertexes
  reco::VertexCollection::const_iterator ivtx = vertexCollection.begin();
  while( ivtx != vertexCollection.end() && ( ivtx->isFake() || ivtx->ndof() < 4 ) ) {
	  ++ivtx;
  }
  if( ivtx == vertexCollection.end() ) { ivtx = vertexCollection.begin(); }
  const reco::Vertex * vtx  = &*(ivtx);
  
  // *** MUONS
  //edm::Handle<edm::View<pat::Muon> > muonHandle;
  //iEvent.getByLabel(MuonTag_,muonHandle);
  //edm::View<pat::Muon> muons = *muonHandle;
  edm::Handle<edm::View<reco::Muon> > muonHandle;
  iEvent.getByLabel(MuonTag_,muonHandle);
  edm::View<reco::Muon> muons = *muonHandle;

  // *** PAT JETS
  //edm::Handle<edm::View<pat::Jet> > jetHandle;
  //iEvent.getByLabel(JetTag_,jetHandle);
  //edm::View<pat::Jet> jets = *jetHandle;
  edm::Handle<edm::View<reco::PFJet> > jetHandle;
  iEvent.getByLabel(JetTag_,jetHandle);
  edm::View<reco::PFJet> jets = *jetHandle;
  
  // *** GEN JETS
  edm::Handle<edm::View<reco::GenJet> > genJetHandle;
  edm::View<reco::GenJet> genJets;
  if ( !dataFlag_ ){
    iEvent.getByLabel(GenJetTag_, genJetHandle);
    genJets = *genJetHandle;
  }

  bool selectEvent=true;
  int goodMuon1=-1, goodMuon2=-1;
  if( requireZ_ ) {
	  selectEvent=false;
	  DiMuonSelection(muons, goodMuon1, goodMuon2, selectEvent);
	  if( ! selectEvent ) { return; }
  }

  int numberOfJets = 0;
  
  // *** Loop over jets : to count the number of jets
  for ( unsigned int i=0; i<jets.size(); ++i ) {
  
    const reco::PFJet patjet = jets.at(i);
    float jec = 1.;
    if( applyJec_ ) {
      jecCor_->setJetPt(patjet.pt());
      jecCor_->setJetEta(patjet.eta());
      jecCor_->setJetA(patjet.jetArea());
      jecCor_->setRho(rho);
      float thejec = jecCor_->getCorrection();
      jec = thejec;// * patjet.correctedJet(0).energy() / patjet.energy() ;
      jecs.push_back(jec);
      ////// if( i < 2 ) { std::cout << "JEC " << patjet.correctedJet(0).pt() << " " << patjet.eta() << " " << patjet.jetArea() << " " 
      ////// 			      << rho << " " <<  thejec
      ////// 			      << " " << jec << std::endl; }
    }
    if ( patjet.pt()*jec <  jetPtThreshold_ )  continue;
 
    //-- remove muons from jets 
    if (selectEvent && requireZ_ ) {
      float dr1 = deltaR( muons.at(goodMuon1).eta(),  muons.at(goodMuon1).phi(),  patjet.eta(),  patjet.phi());
      float dr2 = deltaR( muons.at(goodMuon2).eta(),  muons.at(goodMuon2).phi(),  patjet.eta(),  patjet.phi());
      //std::cout << i << " " << dr1 << " " << dr2 << std::endl; 
      if (dr1<0.5 || dr2<0.5) {
	continue;
      }
    }
    numberOfJets++;
  }


  // *** Loop over jets 
  int ijet = 0;
  for ( unsigned int i=0; i<jets.size(); ++i ) {
    
    ResetTreeVariables();
    
    reco::PFJet patjet = jets.at(i);
    
    //-- loose jet ID 
    //pat::strbitset ret = pfjetIdLoose_.getBitTemplate(); // ? a cosa serve ?
    //jetLooseID = pfjetIdLoose_(patjet, ret);
    //if( ! jetLooseID ) { continue; }

    float jec = 0.;
    if( applyJec_ ) {
	    jec = jecs[i];
	    float rawpt = patjet.pt();//.correctedJet(0).pt();
	    patjet.scaleEnergy(jec);
	    jec = patjet.pt() / rawpt ;
    }
    if ( patjet.pt() <  jetPtThreshold_ )  { continue; }
  
    //-- remove muons from jets 
    if (selectEvent  && requireZ_) {
      float dr1 = deltaR( muons.at(goodMuon1).eta(),  muons.at(goodMuon1).phi(),  patjet.eta(),  patjet.phi());
      float dr2 = deltaR( muons.at(goodMuon2).eta(),  muons.at(goodMuon2).phi(),  patjet.eta(),  patjet.phi());
      //std::cout << i << " " << dr1 << " " << dr2 << std::endl; 
      if (dr1<0.5 || dr2<0.5) {
	continue;
      }
    }
    ++ijet;


    //-- pu jet identifier
    PileupJetIdentifierSubstructure puIdentifier = puIdAlgo_->computeIdVariables(&patjet, jec, vtx, vertexCollection, computeTMVA_);

    // --- fill jet variables
    puIdAlgo_->setIJetIEvent(ijet,iEvent.id().event());
    if ( !dataFlag_ ){
      //jetFlavour  = patjet.partonFlavour();
      if ( matchingToGenJet(patjet, genJets, jetGenPt, jetGenDr) ) isMatched = 1;
    }
    else {
      jetFlavour  = -999;
      isMatched   = false;
    }

    for(size_t itag=0; itag<MvaTags_.size(); ++itag) {
	    Handle<ValueMap<float> > vmap;
	    iEvent.getByLabel(MvaTags_[itag],vmap);
	    mvas_[itag] = (*vmap)[jets.refAt(i)];
    }
    for(size_t itag=0; itag<IdTags_.size(); ++itag) {
	    Handle<ValueMap<int> > vmap;
	    iEvent.getByLabel(IdTags_[itag],vmap);
	    ids_[itag] = (*vmap)[jets.refAt(i)];
    }

    //njets             = jetHandle -> size();
    njets             = numberOfJets;
    nvtx              = vertexHandle->size();
    PUit_n            = PUit_NumInteractions;
    PUit_nTrue        = PUit_TrueNumInteractions;
    PUoot_early_n     = PUoot_early_NumInteractions;
    PUoot_early_nTrue = PUoot_early_TrueNumInteractions;
    PUoot_late_n      = PUoot_late_NumInteractions;
    PUoot_late_nTrue  = PUoot_late_TrueNumInteractions;
    if( requireZ_ ) { 
	    dimuonPt          = (muons.at(goodMuon1).p4()+muons.at(goodMuon2).p4()).Pt();
	    dphiZJet          = deltaPhi( (muons.at(goodMuon1).p4()+muons.at(goodMuon2).p4()).Phi(),  patjet.phi()   );
    }
    
    tree->Fill();
  
  }

 }


void
JetSubstructureAnalyzer::FillMCPileUpInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  PUoot_early_NumInteractions    = -999;
  PUoot_early_TrueNumInteractions= -999;
  PUoot_late_NumInteractions     = -999;
  PUoot_late_TrueNumInteractions = -999;
  PUit_NumInteractions           = -999;
  PUit_TrueNumInteractions       = -999;

  edm::Handle<std::vector<PileupSummaryInfo> > PileUpInfo;
  iEvent.getByLabel(MCPileupTag_, PileUpInfo);

  // --- loop on BX
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  
  for(PVI = PileUpInfo->begin(); PVI != PileUpInfo->end(); ++PVI) {
    // in-time pileup
    if( PVI->getBunchCrossing() == 0 ) {
      PUit_NumInteractions     = PVI->getPU_NumInteractions();
      PUit_TrueNumInteractions = PVI->getTrueNumInteractions();
    }
    // ou-of-time pileup 
    else if ( PVI->getBunchCrossing() > 0) {
      PUoot_late_NumInteractions = PVI->getPU_NumInteractions();
      PUoot_late_NumInteractions = PVI->getTrueNumInteractions();
    }
    else {
      PUoot_early_NumInteractions = PVI->getPU_NumInteractions();
      PUoot_early_NumInteractions = PVI->getTrueNumInteractions();
    }
  }
}


void JetSubstructureAnalyzer::DiMuonSelection(edm::View<reco::Muon> muons, int& goodMuon1, int& goodMuon2, bool& isZcandidate)
{

  float ZMASS = 91.188;
  float minDeltaM = 9999;

  goodMuon1 = -1 ;
  goodMuon2 = -1 ;

  std::vector<int> goodMuonIndex;
  int nGoodMuons  = 0;

  //---preselect muons
  for ( unsigned int imu=0; imu<muons.size(); ++imu ) {
    
    // require tracker + global muon
    if (muons.at(imu).isTrackerMuon()==0 || muons.at(imu).isGlobalMuon()==0) continue;
    
    // isolation requirement (PF isolation deltaBeta corrected)-- FIXME : check if working point (cut at 0.1) is OK
    float neutralHadronIso = muons.at(imu).pfIsolationR04().sumNeutralHadronEt;
    float chargedHadronIso = muons.at(imu).pfIsolationR04().sumChargedParticlePt;
    float photonIso        = muons.at(imu).pfIsolationR04().sumPhotonEt;
    float puChargedHadronIso = muons.at(imu).pfIsolationR04().sumPUPt;
    float combRelIso = ( chargedHadronIso + std::max(neutralHadronIso + photonIso - 0.5 * puChargedHadronIso, 0.) ) / muons.at(imu).pt();
    if ( combRelIso > 0.1 ) continue;
    goodMuonIndex.push_back(imu);
    nGoodMuons++;
  }

  //  std::cout <<  goodMuonIndex.size() << "  " << nGoodMuons << std::endl;

  if (nGoodMuons > 1){
    for ( unsigned int i = 0; i < goodMuonIndex.size(); ++i ) {
      for ( unsigned int j = i+1; j < goodMuonIndex.size(); ++j ) {
	int ii =  goodMuonIndex.at(i);
	int jj =  goodMuonIndex.at(j);
	float invmass = (muons.at(ii).p4()+muons.at(jj).p4()).mass();
	if ( fabs(invmass-ZMASS) < 30 && fabs(invmass-ZMASS) < minDeltaM ) {
	  goodMuon1 = ii;
	  goodMuon2 = jj;
	  minDeltaM = fabs(invmass-ZMASS);
	}
      }
    }
  }
  
  if (goodMuon1!=-1 && goodMuon2!=-1) isZcandidate = true;

}


bool JetSubstructureAnalyzer::matchingToGenJet( const reco::PFJet jet, edm::View<reco::GenJet> genJets , float& genPt, float& genDr)
 {
   bool isMcMatched = false;
   genPt = -999;
   genDr = -999;

   for ( unsigned int i=0; i<genJets.size(); ++i ) {
     const reco::Jet genJet = genJets.at(i);
     float dR = deltaR( genJet.eta(), genJet.phi(), jet.eta(), jet.phi() );
     if ( dR < 0.25 ) {
       isMcMatched = true;
       genPt = genJet.pt();
       genDr = dR;
       break;
     }
   }
   return (isMcMatched);
}

// ------------------------------------------------------------------------------------------
void 
JetSubstructureAnalyzer::initJetEnergyCorrector(const edm::EventSetup &iSetup, bool isData)
{
	std::cout << "initJetEnergyCorrector " << " isData " << isData << " " << "jecTag_ " << jecTag_ << std::endl; 
	//jet energy correction levels to apply on raw jet
	std::vector<std::string> jecLevels;
	jecLevels.push_back("L1FastJet");
	jecLevels.push_back("L2Relative");
	jecLevels.push_back("L3Absolute");
	if(isData && ! residualsFromTxt_ ) jecLevels.push_back("L2L3Residual");

	//check the corrector parameters needed according to the correction levels
	edm::ESHandle<JetCorrectorParametersCollection> parameters;
	iSetup.get<JetCorrectionsRecord>().get(jecTag_,parameters);
	for(std::vector<std::string>::const_iterator ll = jecLevels.begin(); ll != jecLevels.end(); ++ll)
	{ 
		const JetCorrectorParameters& ip = (*parameters)[*ll];
		jetCorPars_.push_back(ip); 
	} 
	if( isData && residualsFromTxt_ ) {
		jetCorPars_.push_back(JetCorrectorParameters(residualsTxt_.fullPath())); 
	}

	//instantiate the jet corrector
	jecCor_ = new FactorizedJetCorrector(jetCorPars_);
}


// ------------ method called once each job just before starting event loop  ------------
void 
JetSubstructureAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetSubstructureAnalyzer::endJob() 
{
  //tree->Write();
  if(jecCor_) { delete jecCor_; }
}

