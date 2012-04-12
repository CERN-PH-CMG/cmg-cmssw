// system include files
#include <memory>
#include <cmath>
#include <algorithm>

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>

#include "FWCore/Utilities/interface/Exception.h"

#include "CMG/MetAnalysis/plugins/PUMetProducer.h"

using namespace edm;
using namespace std;
using namespace reco;

PUMetProducer::PUMetProducer(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  isData_         = iConfig.getParameter<bool>("isData");
  utils           = new MetUtilities(iConfig.getParameter<edm::ParameterSet>("puJetIDAlgo"));      
  dZCut_          = iConfig.getParameter<double>("dZCut");
  jetPtThreshold_ = iConfig.getParameter<double>("jetPtThreshold");
}

PUMetProducer::~PUMetProducer() { 
  delete utils;
}

void PUMetProducer::beginJob() { }

void PUMetProducer::endJob() { } 

void PUMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // PF candidates
  edm::Handle< edm::View<reco::Candidate> > PFcandCollHandle;
  try { iEvent.getByLabel("particleFlow", PFcandCollHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("PUMetProducer") << "Can't get candidate collection: particleFlow"; }
  const edm::View<reco::Candidate> *PFcandColl = PFcandCollHandle.product();

  // uncorrected PF jets
  edm::Handle< edm::View<reco::Candidate> > uncorrPFJetCollectionHandle;
  try { iEvent.getByLabel("ak5PFJets",uncorrPFJetCollectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: ak5PFJets"; }
  const edm::View<reco::Candidate> *uncorrPFJetColl = uncorrPFJetCollectionHandle.product();

  // fully corrected PF jets
  edm::Handle< edm::View<reco::Candidate> > corrPFJetCollectionHandle;
  if(isData_) { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3Residual",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3Residual"; }
  } else { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3"; }
  }
  const edm::View<reco::Candidate> *corrPFJetColl = corrPFJetCollectionHandle.product();

  // vertices                                                                                                             
  edm::Handle<reco::VertexCollection> primaryVertex;
  try { iEvent.getByLabel("offlinePrimaryVertices", primaryVertex); }
  catch(cms::Exception& ex ) {edm::LogWarning("PUMetProducer") << "Can't get candidate collection: offlinePrimaryVertices"; }
    
  // Rho
  edm::Handle<double> hRho;
  try { iEvent.getByLabel(edm::InputTag("kt6PFJets","rho"),hRho); }
  catch(cms::Exception& ex ) {edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: rho"; }


  // --------------------------------------------------------------
  // First the PV
  VertexCollection::const_iterator vMax = primaryVertex->begin();
  reco::Vertex vtx;
  if (primaryVertex->size()>0) vtx = *vMax;

  // Now the Met basics
  Candidate::LorentzVector totalP4(0,0,0,0);
  double sumet = 0.0;
  
  // Track MET with reverted dZ cut
  for(int index = 0; index < (int)PFcandColl->size(); index++) {
    const PFCandidate* pflowCand  = dynamic_cast< const PFCandidate * >(&(PFcandColl->at(index)));
    if(primaryVertex->size()==0) continue;
    double pDZ  = utils->pfCandDz(pflowCand,&vtx);
    if(pDZ < dZCut_) continue;   
    totalP4 -= pflowCand->p4();
    sumet   += pflowCand->pt();
  }
  // Neutrals from the Jets - with reverted jetID cut
  for(int index = 0; index < (int)uncorrPFJetColl->size(); index++) {      // uncorrected jets collection
    const Candidate *uncorrCand   = &(uncorrPFJetColl->at(index));
    const PFJet     *pUncorrPFJet = dynamic_cast< const PFJet * > ( &(*uncorrCand) );    
    
    for(int index2 = 0; index2 < (int)corrPFJetColl->size(); index2++) {   // corrected jets collection
      const Candidate *corrCand   = &(corrPFJetColl->at(index2));
      const PFJet     *pCorrPFJet = dynamic_cast< const PFJet * > ( &(*corrCand) );    
      
      if(  pUncorrPFJet->jetArea() == pCorrPFJet->jetArea() ) {      // to match corrected and uncorrected jets
	if(  fabs(pUncorrPFJet->eta() - pCorrPFJet->eta())<0.01 ) {  // to match corrected and uncorrected jets
	  if( pCorrPFJet->pt()< jetPtThreshold_ ) continue;  
	  if(utils->passJetId(pCorrPFJet,pUncorrPFJet,vtx,*primaryVertex)) continue;
	  utils->addNeut(pCorrPFJet,&totalP4,&sumet,1);             
	  break;
	}
      }
    }
  }

  CommonMETData output;
  output.mex = totalP4.px();
  output.mey = totalP4.py();
  output.mez = totalP4.pz();
  output.met = totalP4.pt();
  output.sumet = sumet;
  output.phi = atan2(totalP4.py(),totalP4.px());
  
  PFSpecificAlgo pf;
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset (new reco::PFMETCollection);
  pfmetcoll->push_back( pf.addInfo(PFcandCollHandle, output) );

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}

