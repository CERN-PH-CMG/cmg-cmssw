
// system include files
#include <memory>
#include <cmath>
#include <algorithm>
#include <TVector3.h>

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>

#include "FWCore/Utilities/interface/Exception.h"

#include "CMG/MetAnalysis/plugins/TrackMetProducer.h"

using namespace edm;
using namespace std;
using namespace reco;

TrackMetProducer::TrackMetProducer(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  isData_         = iConfig.getParameter<bool>("isData");
  utils_          = new MetUtilities(iConfig.getParameter<edm::ParameterSet>("puJetIDAlgo"),isData_);      
  dZCut_          = iConfig.getParameter<double>("dZCut");
}

TrackMetProducer::~TrackMetProducer() { 
  delete utils_;
}

void TrackMetProducer::beginJob() { }

void TrackMetProducer::endJob() { } 

void TrackMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // PF candidates 
  edm::Handle< edm::View<reco::Candidate> > collectionHandle;
  try { iEvent.getByLabel("particleFlow", collectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("TrackMetProducer") << "Can't get candidate collection: particleFlow"; }
  const edm::View<reco::Candidate> *collection = collectionHandle.product();

  // vertices
  edm::Handle<reco::VertexCollection> primaryVertex;
  try { iEvent.getByLabel("offlinePrimaryVertices", primaryVertex); }
  catch(cms::Exception& ex ) {edm::LogWarning("TrackMetProducer") << "Can't get candidate collection: offlinePrimaryVertices"; }

  VertexCollection::const_iterator vMax = primaryVertex->begin();
  reco::Vertex vtx;
  if (primaryVertex->size()>0) vtx = *vMax;

  // -------------------------------------------------------

  reco::Candidate::LorentzVector totalP4(0,0,0,0);
  float sumet = 0.0;
  
  for(int index = 0; index < (int)collection->size(); index++) {
    const PFCandidateRef pflowCandRef = collection->refAt(index).castTo<PFCandidateRef>();
    if(primaryVertex->size()==0) continue;
    double pDZ  = utils_->pfCandDz(pflowCandRef,vtx);
    if(pDZ > dZCut_) continue;
    totalP4 -= pflowCandRef->p4();
    sumet   += pflowCandRef->pt();
  }
  
  CommonMETData output;
  output.mex = totalP4.px();
  output.mey = totalP4.py();
  output.mez = totalP4.pz();
  output.met = totalP4.pt();
  output.sumet = sumet;
  output.phi = atan2(invertedP4.py(),invertedP4.px());

  PFSpecificAlgo pf;
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset (new reco::PFMETCollection);

  pfmetcoll->push_back( pf.addInfo(collectionHandle, output) );

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}

