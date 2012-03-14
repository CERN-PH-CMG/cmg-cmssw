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
// $Id$
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

  impactParTkThreshold_ = iConfig.getUntrackedParameter<double>("impactParTkThreshold");
  tmvaWeights_          = iConfig.getUntrackedParameter<std::string>("tmvaWeights");
  tmvaMethod_           = iConfig.getUntrackedParameter<std::string>("tmvaMethod");
  computeTMVA_          = iConfig.getUntrackedParameter<bool>("computeTMVA");
	
  
  //--- PU jet identifier 
  puIdentifier = new PileupJetIdentifier(tmvaWeights_.c_str(),tmvaMethod_.c_str(), impactParTkThreshold_);

  edm::Service<TFileService> fs ;
  tree =        fs -> make <TTree>("tree","tree"); 

  tree -> Branch ("nvtx",&nvtx, "nvtx/I");
  tree -> Branch ("jetPt",&jetPt, "jetPt/F");
  tree -> Branch ("jetEta",&jetEta, "jetEta/F");
  tree -> Branch ("jetPhi",&jetPhi, "jetPhi/F");
  tree -> Branch ("jetM",&jetM, "jetM/F");

  tree -> Branch ("nCharged",&nCharged, "nCharged/F");
  tree -> Branch ("nNeutrals",&nNeutrals, "nNeutrals/F");
  tree -> Branch ("chgEMfrac",&chgEMfrac, "chgEMfrac/F");
  tree -> Branch ("neuEMfrac",&neuEMfrac, "neuEMfrac/F");
  tree -> Branch ("chgHadrfrac",&chgHadrfrac, "chgHadrfrac/F");
  tree -> Branch ("neuHadrfrac",&neuHadrfrac, "neuHadrfrac/F");
  tree -> Branch ("nParticles",&nParticles, "nParticles/F");

  tree -> Branch ("leadPt",&leadPt, "leadPt/F");
  tree -> Branch ("leadEta",&leadEta, "leadEta/F");
  tree -> Branch ("secondPt",&secondPt, "secondPt/F");
  tree -> Branch ("secondEta",&secondEta, "secondEta/F");
  tree -> Branch ("leadNeutPt",&leadNeutPt, "leadNeutPt/F");
  tree -> Branch ("leadNeutEta",&leadNeutEta, "leadNeutEta/F");
  tree -> Branch ("leadEmPt",&leadEmPt, "leadEmPt/F");
  tree -> Branch ("leadEmEta",&leadEmEta, "leadEmEta/F");
  tree -> Branch ("leadChPt",&leadChPt, "leadChPt/F");
  tree -> Branch ("leadChEta",&leadChEta, "leadChEta/F");
  
  tree -> Branch ("dRLeadCent",&dRLeadCent, "dRLeadCent/F");
  tree -> Branch ("dRLead2nd",&dRLead2nd, "dRLead2nd/F");
  tree -> Branch ("dRMean",&dRMean, "dRMean/F");
  tree -> Branch ("dRMeanNeut",&dRMeanNeut, "dRMeanNeut/F");
  tree -> Branch ("dRMeanEm",&dRMeanEm, "dRMeanEm/F");
  tree -> Branch ("dRMeanCh",&dRMeanCh, "dRMeanCh/F");

  tree -> Branch ("ptD",&ptD, "ptD/F");
  

  tree -> Branch ("leadFrac",&leadFrac, "leadFrac/F");
  tree -> Branch ("leadChFrac",&leadChFrac, "leadChFrac/F");
  tree -> Branch ("leadNeutFrac",&leadNeutFrac, "leadNeutFrac/F");
  tree -> Branch ("majW",&majW, "majW/F");
  tree -> Branch ("minW",&minW, "minW/F");
  
  tree -> Branch ("isMatched", &isMatched, "isMatched/F");
  tree -> Branch ("jetFlavour",&jetFlavour, "jetFlavour/F");

  tree -> Branch ("mva",&mva,"mva/F");

 }

void JetAnalyzer::ResetTreeVariables()
{

  nvtx   = -999;
  jetPt  = -999;
  jetEta = -999;
  jetPhi = -999;
  jetM   = -999;

  nCharged = -999;
  nNeutrals= -999; 
  chgEMfrac= -999; 
  neuEMfrac= -999; 
  chgHadrfrac= -999; 
  neuHadrfrac= -999; 
  nParticles =-999;
  leadPt= -999; 
  leadEta= -999; 
  secondPt= -999; 
  secondEta= -999; 
  leadNeutPt= -999; 
  leadNeutEta= -999; 
  leadEmPt= -999; 
  leadEmEta= -999; 
  leadChPt= -999; 
  leadChEta=-999;
  dRLeadCent= -999; 
  dRLead2nd= -999; 
  dRMean= -999; 
  dRMeanNeut= -999; 
  dRMeanEm= -999; 
  dRMeanCh= -999; 
  ptD=-999;

  leadFrac = -999; 
  leadChFrac = -999; 
  leadNeutFrac = -999;
  majW = -999; 
  minW = -999;

  isMatched = -999;
  jetFlavour = -999;
  mva = -999;

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
    
    const pat::Jet patjet = jets.at(i);
  
    //-- remove muons from jets 
    if (isZ) {
      float dr1 = deltaR( muons.at(goodMuon1).eta(),  muons.at(goodMuon1).phi(),  patjet.eta(),  patjet.phi());
      float dr2 = deltaR( muons.at(goodMuon2).eta(),  muons.at(goodMuon2).phi(),  patjet.eta(),  patjet.phi());
      if (dr1<0.5 || dr2<0.5) continue;
    }

    //-- pu jet identifier
    puIdentifier->computeIdVariables(&patjet, 0, &vtx, computeTMVA_);
    
    ResetTreeVariables();

    // --- jet variables
    jetPt  = puIdentifier->jetPt();
    jetEta = puIdentifier->jetEta();
    jetPhi = puIdentifier->jetPhi();

    
    nCharged    = puIdentifier->nCharged();
    nNeutrals   = puIdentifier->nNeutrals();
    chgEMfrac   = puIdentifier->chgEMfrac();
    neuEMfrac   = puIdentifier->neuEMfrac();
    chgHadrfrac = puIdentifier->chgHadrfrac(); 
    neuHadrfrac = puIdentifier->neuHadrfrac(); 
    nParticles  = puIdentifier->nParticles();
    leadPt      = puIdentifier->leadPt(); 
    leadEta     = puIdentifier->leadEta(); 
    secondPt    = puIdentifier->secondPt(); 
    secondEta   = puIdentifier->secondEta(); 
    leadNeutPt  = puIdentifier->leadNeutPt(); 
    leadNeutEta = puIdentifier->leadNeutEta(); 
    leadEmPt    = puIdentifier->leadEmPt(); 
    leadEmEta   = puIdentifier->leadEmEta(); 
    leadChPt    = puIdentifier->leadChPt(); 
    leadChEta   = puIdentifier->leadChEta(); 
    dRLeadCent  = puIdentifier->dRLeadCent(); 
    dRLead2nd   = puIdentifier->dRLead2nd(); 
    dRMean      = puIdentifier->dRMean(); 
    dRMeanNeut  = puIdentifier->dRMeanNeut(); 
    dRMeanEm    = puIdentifier->dRMeanEm(); 
    dRMeanCh    = puIdentifier->dRMeanCh(); 
    ptD         = puIdentifier->ptD();
    leadFrac    = puIdentifier->leadFrac();
    leadChFrac  = puIdentifier->leadChFrac();
    leadNeutFrac= puIdentifier->leadNeutFrac();
    majW        = puIdentifier->majW();
    minW        = puIdentifier->minW();
    mva         = puIdentifier->mva();

    if ( !dataFlag_ ){
      jetFlavour  = patjet.partonFlavour();
      if ( matchingToGenJet(patjet, genJets) ) isMatched = 1;
    }
    else {
      jetFlavour  = -999;
      isMatched   = -999;
    }
    
    nvtx = vertexHandle->size();
        
  }


  tree->Fill();

 }


void JetAnalyzer::DiMuonSelection(edm::View<pat::Muon> muons, int goodMuon1, int goodMuon2, bool isZcandidate)
{

  float minDeltaM = 9999;

  goodMuon1 = -1 ;
  goodMuon2 = -1 ;

  std::vector<int> goodMuonIndex;
  int nLooseMuons  = 0;

  //---preselect muons
  for ( unsigned int imu=0; imu<muons.size(); ++imu ) {
    if (muons.at(imu).isTrackerMuon()==0 || muons.at(imu).isGlobalMuon()==0) continue;
    // FIXME : add  isolation requirement
    goodMuonIndex.push_back(imu);
    nLooseMuons++;
  }

  if (nLooseMuons > 1){
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
