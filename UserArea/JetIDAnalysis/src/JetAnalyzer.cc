// -*- C++ -*-
//
// Package:    JetAnalyzer
// Class:      JetAnalyzer
// 
/**\class JetAnalyzer JetAnalyzer.cc HggAnalysis/JetAnalyzer/src/JetAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Martina Malberti,27 2-019,+41227678349,
//         Created:  Mon Mar  5 16:39:53 CET 2012
// $Id: JetAnalyzer.cc,v 1.10 2012/03/26 09:11:18 musella Exp $
//
//


// system include files
#include "CMG/JetIDAnalysis/interface/JetAnalyzer.h"


// 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

//
// constructors and destructor
//
JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig)

{
  //--- inputs
  MCPileupTag_ = iConfig.getParameter<edm::InputTag>("MCPileupTag");
  PVTag_       = iConfig.getParameter<edm::InputTag>("PVTag");
  JetTag_      = iConfig.getParameter<edm::InputTag>("JetTag");
  GenJetTag_   = iConfig.getParameter<edm::InputTag>("GenJetTag");
  MuonTag_     = iConfig.getParameter<edm::InputTag>("MuonTag");

  dataFlag_    = iConfig.getUntrackedParameter<bool>("dataFlag");

  //--- PU jet identifier 
  puIdAlgo_ = new PileupJetIdNtupleAlgo(iConfig);
  computeTMVA_ = iConfig.getUntrackedParameter<bool>("computeTMVA");
  
  requireZ_ = iConfig.getUntrackedParameter<bool>("requireZ",true);
  
  //-- loose jet ID
  pfjetIdLoose_  = iConfig.getParameter<edm::ParameterSet>("pfjetIdLoose") ;

  //-- jet pt threshold : only jets above this thr are saved in the tree
  jetPtThreshold_ = iConfig.getUntrackedParameter<double>("jetPtThreshold");
  
  //-- output tree
  edm::Service<TFileService> fs ;
  tree =        fs -> make <TTree>("tree","tree"); 

  puIdAlgo_->bookBranches(tree);

  tree -> Branch ("PUit_n",&PUit_n, "PUit_n/I");
  tree -> Branch ("PUit_nTrue",&PUit_nTrue, "PUit_nTrue/F");
  tree -> Branch ("PUoot_early_n",&PUoot_early_n, "PUoot_early_n/I");
  tree -> Branch ("PUoot_early_nTrue",&PUoot_early_nTrue, "PUoot_early_nTrue/F");
  tree -> Branch ("PUoot_late_n",&PUoot_late_n, "PUoot_late_n/I");
  tree -> Branch ("PUoot_late_nTrue",&PUoot_late_nTrue, "PUoot_late_nTrue/F");
  tree -> Branch ("nvtx",&nvtx, "nvtx/I");
  tree -> Branch ("jetLooseID", &jetLooseID, "jetLooseID/O");
  tree -> Branch ("isMatched", &isMatched, "isMatched/O");
  tree -> Branch ("jetFlavour",&jetFlavour, "jetFlavour/I");
  tree -> Branch ("jetGenPt",&jetGenPt, "jetGenPt/F");
  tree -> Branch ("jetGenDr",&jetGenDr, "jetGenDr/F");
  tree -> Branch ("njets",&njets, "njets/I");

 }

void JetAnalyzer::ResetTreeVariables()
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

}


JetAnalyzer::~JetAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}



// ------------ method called for each event  ------------
void
JetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  

  // *** PILEUP INFO
  if (!dataFlag_) FillMCPileUpInfo(iEvent, iSetup);

  // *** VERTEX COLLECTION
  edm::Handle<reco::VertexCollection> vertexHandle;
  iEvent.getByLabel(PVTag_, vertexHandle);
  reco::VertexCollection vertexCollection = *(vertexHandle.product());
  
  //-- first vtx 
  const reco::Vertex vtx  = *(vertexCollection.begin());;

  // *** MUONS
  edm::Handle<edm::View<pat::Muon> > muonHandle;
  iEvent.getByLabel(MuonTag_,muonHandle);
  edm::View<pat::Muon> muons = *muonHandle;

  // *** PAT JETS
  edm::Handle<edm::View<pat::Jet> > jetHandle;
  iEvent.getByLabel(JetTag_,jetHandle);
  edm::View<pat::Jet> jets = *jetHandle;
  
  // *** GEN JETS
  edm::Handle<edm::View<reco::GenJet> > genJetHandle;
  edm::View<reco::GenJet> genJets;
  if ( !dataFlag_ ){
    iEvent.getByLabel(GenJetTag_, genJetHandle);
    genJets = *genJetHandle;
  }

  int goodMuon1=-1, goodMuon2=-1;
  bool isZ=false;
  DiMuonSelection(muons, goodMuon1, goodMuon2, isZ);
  if( ! isZ && requireZ_ ) { return; }
  
  int numberOfJets = 0;
   
  // *** Loop over jets : to count the number of jets
  for ( unsigned int i=0; i<jets.size(); ++i ) {
  
    const pat::Jet patjet = jets.at(i);
    
    if ( patjet.pt() <  jetPtThreshold_ )  continue;
 
    //-- remove muons from jets 
    if (isZ) {
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
  for ( unsigned int i=0; i<jets.size(); ++i ) {
    
    ResetTreeVariables();
    
    const pat::Jet patjet = jets.at(i);
    
    if ( patjet.pt() <  jetPtThreshold_ )  continue;
 
    //-- remove muons from jets 
    if (isZ) {
      float dr1 = deltaR( muons.at(goodMuon1).eta(),  muons.at(goodMuon1).phi(),  patjet.eta(),  patjet.phi());
      float dr2 = deltaR( muons.at(goodMuon2).eta(),  muons.at(goodMuon2).phi(),  patjet.eta(),  patjet.phi());
      //std::cout << i << " " << dr1 << " " << dr2 << std::endl; 
      if (dr1<0.5 || dr2<0.5) {
	continue;
      }
    }

    //-- loose jet ID 
    pat::strbitset ret = pfjetIdLoose_.getBitTemplate(); // ? a cosa serve ?
    jetLooseID = pfjetIdLoose_(patjet, ret);


    //-- pu jet identifier
    PileupJetIdentifier puIdentifier = puIdAlgo_->computeIdVariables(&patjet, 0, &vtx, vertexCollection, computeTMVA_);

    // --- fill jet variables
    puIdAlgo_->setIJetIEvent(i,0);
    if ( !dataFlag_ ){
      jetFlavour  = patjet.partonFlavour();
      if ( matchingToGenJet(patjet, genJets, jetGenPt, jetGenDr) ) isMatched = 1;
    }
    else {
      jetFlavour  = -999;
      isMatched   = false;
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
        
    tree->Fill();
  
  }

 }


void
JetAnalyzer::FillMCPileUpInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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


void JetAnalyzer::DiMuonSelection(edm::View<pat::Muon> muons, int& goodMuon1, int& goodMuon2, bool& isZcandidate)
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
    float neutralHadronIso = muons.at(imu).neutralHadronIso();
    float chargedHadronIso = muons.at(imu).chargedHadronIso();
    float photonIso        = muons.at(imu).photonIso();
    float puChargedHadronIso = muons.at(imu).puChargedHadronIso();
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


bool JetAnalyzer::matchingToGenJet( const pat::Jet jet, edm::View<reco::GenJet> genJets , float& genPt, float& genDr)
 {
   bool isMcMatched = false;
   genPt = -999;
   genDr = -999;

   for ( unsigned int i=0; i<genJets.size(); ++i ) {
     const pat::Jet genJet = genJets.at(i);
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


// ------------ method called once each job just before starting event loop  ------------
void 
JetAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetAnalyzer::endJob() 
{
  //tree->Write();
}

