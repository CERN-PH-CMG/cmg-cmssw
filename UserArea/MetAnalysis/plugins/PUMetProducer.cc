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

#include "CMG/MetAnalysis/interface/MetUtilities.h"

PUMetProducer::PUMetProducer(const edm::ParameterSet& iConfig) {
  
  produces<reco::PFMETCollection>();
  
  // iJetCorrector = new FactorizedJetCorrector(vParam);     // fixme: we have to load the parameters...
}

PUMetProducer::~PUMetProducer() { 

  // delete iJetCorrector;          // fixme
}

void PUMetProducer::beginJob() { }

void PUMetProducer::endJob() { } 

void PUMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  using namespace edm;
  using namespace std;
  using namespace reco;

  MetUtilities utils;

  // PF candidates
  edm::Handle< edm::View<reco::Candidate> > PFcandCollHandle;
  try { iEvent.getByLabel("particleFlow", PFcandCollHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("PUMetProducer") << "Can't get candidate collection: particleFlow"; }
  const edm::View<reco::Candidate> *PFcandColl = PFcandCollHandle.product();

  // uncorrected PF jets
  edm::Handle< edm::View<reco::Candidate> > uncorPFJetCollectionHandle;
  try { iEvent.getByLabel("ak5PFJets",uncorPFJetCollectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("PUMetProducer") << "Can't get candidate collection: ak5PFJets"; }
  const edm::View<reco::Candidate> *uncorPFJetColl = uncorPFJetCollectionHandle.product();
  
  // vertices                                                                                                             
  edm::Handle<reco::VertexCollection> primaryVertex;
  try { iEvent.getByLabel("offlinePrimaryVertices", primaryVertex); }
  catch(cms::Exception& ex ) {edm::LogWarning("PUMetProducer") << "Can't get candidate collection: offlinePrimaryVertices"; }
  
  VertexCollection::const_iterator vMax = primaryVertex->begin();
  reco::Vertex vtx;
  if (primaryVertex->size()>0) vtx = *vMax;


  Candidate::LorentzVector totalP4(0,0,0,0);
  float sumet = 0.0;
  
  for(int index = 0; index < (int)PFcandColl->size(); index++) {

    const PFCandidateRef pflowCandRef = PFcandColl->refAt(index).castTo<PFCandidateRef>();

    if(primaryVertex->size()==0) continue;
    
    float theDz = 999;
    if(pflowCandRef->trackRef().isNonnull()) 
      theDz = fabs(pflowCandRef->trackRef()->dz(vtx.position()));
    else if(pflowCandRef->gsfTrackRef().isNonnull())
      theDz = fabs(pflowCandRef->gsfTrackRef()->dz(vtx.position()));
    else if(pflowCandRef->muonRef().isNonnull() && pflowCandRef->muonRef()->innerTrack().isNonnull())
      theDz = fabs(pflowCandRef->muonRef()->innerTrack()->dz(vtx.position()));

    if( theDz==999 ) continue;     
    if( (fabs(theDz)< iDZCut) ) continue;

    totalP4 += pflowCandRef->p4();
    sumet   += pflowCandRef->pt();
  }
  

  reco::Candidate::LorentzVector invertedP4(-totalP4);

  // jets
  for(int index = 0; index < (int)uncorPFJetColl->size(); index++) {
    
    const Candidate *cand = &(uncorPFJetColl->at(index));
    const PFJet *thisPFJet = dynamic_cast< const PFJet * > ( &(*cand) );    

    // if (fJetIDMVA->correctedPt(thisPFJet,iJetCorrector,iPileupEnergyDensity) < fJetIDMVA->fJetPtMin && 
    // thisPFJet->TrackCountingHighEffBJetTagsDisc() == -100) continue;     
    if(!utils.passPFLooseId(thisPFJet)) continue;
    if(!utils.filter(thisPFJet,iPhi1,iEta1,iPhi2,iEta2)) continue;   
    // if (fJetIDMVA->pass(thisPFJet,vtx,iJetCorrector,iPileupEnergyDensity))  continue;   // fixme 
    // addNeut(thisPFJet,invertedP4,sumet,iJetCorrector,iPileupEnergyDensity);             // fixme 
  }

  CommonMETData output;
  output.mex = invertedP4.px();
  output.mey = invertedP4.py();
  output.mez = invertedP4.pz();
  output.met = invertedP4.pt();
  output.sumet = sumet;
  output.phi = atan2(invertedP4.py(),invertedP4.px());
  
  PFSpecificAlgo pf;
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset (new reco::PFMETCollection);
  pfmetcoll->push_back( pf.addInfo(PFcandCollHandle, output) );

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}

