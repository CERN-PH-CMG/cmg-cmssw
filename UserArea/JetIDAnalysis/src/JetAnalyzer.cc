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
// $Id: JetAnalyzer.cc,v 1.2 2012/03/14 12:05:22 musella Exp $
//
//


// system include files
#include "CMG/JetIDAnalysis/interface/JetAnalyzer.h"
#include "DataFormats/JetReco/interface/Jet.h"
// 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

//
// constructors and destructor
//
JetAnalyzer::JetAnalyzer(const edm::ParameterSet& iConfig)

{
  //--- inputs
  PVTag_     = iConfig.getParameter<edm::InputTag>("PVTag");
  JetTag_    = iConfig.getParameter<edm::InputTag>("JetTag");
  GenJetTag_ = iConfig.getParameter<edm::InputTag>("GenJetTag");
  MuonTag_   = iConfig.getParameter<edm::InputTag>("MuonTag");

  dataFlag_  = iConfig.getUntrackedParameter<bool>("dataFlag");

  //--- PU jet identifier 
  puIdAlgo_ = new PileupJetIdNtupleAlgo(iConfig);
  
  edm::Service<TFileService> fs ;
  tree =        fs -> make <TTree>("tree","tree"); 

  puIdAlgo_->bookBranches(tree);
  tree -> Branch ("nvtx",&nvtx, "nvtx/I");
  tree -> Branch ("isMatched", &isMatched, "isMatched/O");
  tree -> Branch ("jetFlavour",&jetFlavour, "jetFlavour/I");

 }

void JetAnalyzer::ResetTreeVariables()
{
  nvtx   = -999;
  isMatched  = false;
  jetFlavour = -999;
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
  iEvent.getByLabel(GenJetTag_, genJetHandle);
  edm::View<reco::GenJet> genJets = *genJetHandle;


  int goodMuon1, goodMuon2;
  bool isZ;
  DiMuonSelection(muons, goodMuon1, goodMuon2, isZ);


  // *** Loop over jets 
  for ( unsigned int i=0; i<jets.size(); ++i ) {
    
    ResetTreeVariables();

    const pat::Jet patjet = jets.at(i);
  
    //-- remove muons from jets 
    if (isZ) {
      float dr1 = deltaR( muons.at(goodMuon1).eta(),  muons.at(goodMuon1).phi(),  patjet.eta(),  patjet.phi());
      float dr2 = deltaR( muons.at(goodMuon2).eta(),  muons.at(goodMuon2).phi(),  patjet.eta(),  patjet.phi());
      if (dr1<0.5 || dr2<0.5) continue;
    }

    //-- pu jet identifier
    PileupJetIdentifier puIdentifier = puIdAlgo_->computeIdVariables(&patjet, 0, &vtx, computeTMVA_);

    // --- fill jet variables
    puIdAlgo_->fillJet(puIdentifier,i,0);
    if ( !dataFlag_ ){
      jetFlavour  = patjet.partonFlavour();
      if ( matchingToGenJet(patjet, genJets) ) isMatched = 1;
    }
    else {
      jetFlavour  = -999;
      isMatched   = false;
    }
    
    nvtx = vertexHandle->size();
    
    tree->Fill();
  }

 }


void JetAnalyzer::DiMuonSelection(edm::View<pat::Muon> muons, int goodMuon1, int goodMuon2, bool isZcandidate)
{

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

  if (nGoodMuons > 1){
    for ( unsigned int i = 0; i < goodMuonIndex.size(); ++i ) {
      for ( unsigned int j = i+1; j < goodMuonIndex.size(); ++j ) {
	float invmass = (muons.at(i).pfP4()+muons.at(j).pfP4()).mass();
	if ( fabs(invmass-91.188) < 30 && fabs(invmass-91.188) < minDeltaM) {
	  goodMuon1 = i;
	  goodMuon2 = j;
	  minDeltaM = fabs(invmass-91.188);
	}
      }
    }
  }

  if (goodMuon1!=-1 && goodMuon2!=-1) isZcandidate = true;

}


 bool JetAnalyzer::matchingToGenJet( const pat::Jet jet, edm::View<reco::GenJet> genJets )
 {
   bool isMcMatched = false;

   for ( unsigned int i=0; i<genJets.size(); ++i ) {
     const pat::Jet genJet = genJets.at(i);
     float dR = deltaR( genJet.eta(), genJet.phi(), jet.eta(), jet.phi() );
     if ( dR < 0.25 ) {
       isMcMatched = true;
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

//define this as a plug-in
DEFINE_FWK_MODULE(JetAnalyzer);
