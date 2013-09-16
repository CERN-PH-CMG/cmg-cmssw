#include "CMGTools/Common/plugins/CMGCandidateProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "AnalysisDataFormats/CMGTools/interface/Candidate.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/GsfTrackReco/interface/GsfTrack.h>

using namespace std;
using namespace edm;

CMGCandidateProducer::CMGCandidateProducer(const edm::ParameterSet& iConfig) {
  
  Cands_ = iConfig.getParameter<InputTag>("inputCollection");
  CandsFromPV_ = iConfig.getParameter<InputTag>("inputCollectionFromPV");
  produces< std::vector<cmg::Candidate> > ();

}

CMGCandidateProducer::~CMGCandidateProducer() {}

void CMGCandidateProducer::beginJob() {}

void CMGCandidateProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  edm::Handle<reco::PFCandidateCollection> cands;
  iEvent.getByLabel( Cands_, cands );
  std::vector<reco::Candidate>::const_iterator cand;

  edm::Handle<reco::PFCandidateCollection> candsFromPV;
  iEvent.getByLabel( CandsFromPV_, candsFromPV );
  std::vector<reco::Candidate>::const_iterator candFromPV;

  auto_ptr< std::vector<cmg::Candidate> > outPtr( new std::vector<cmg::Candidate> );
  for(unsigned int ic=0; ic<cands->size(); ic++) {
    const reco::Candidate &cand=(*cands)[ic];
    bool fromPV=false;
    for(unsigned int icFromPV=0; icFromPV<candsFromPV->size(); icFromPV++) {
      const reco::Candidate &candFromPV=(*candsFromPV)[icFromPV];
      if((cand.p4()==candFromPV.p4())&&(cand.pdgId()==candFromPV.pdgId()))
      {
          fromPV=true;
	  break;
      }
    }

    math::XYZPoint vtx = cand.vertex();
    
    outPtr->push_back( cmg::Candidate(cand.p4().pt(),
				      cand.p4().eta(),
				      cand.p4().phi(),
				      cand.p4().M(),
				      vtx,
				      cand.pdgId(),
				      fromPV)
		       );  
    }
  


  
  iEvent.put( outPtr  );
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CMGCandidateProducer);
