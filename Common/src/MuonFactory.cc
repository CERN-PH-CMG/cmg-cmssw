#include "CMGTools/Common/interface/MuonFactory.h"

cmg::MuonFactory::event_ptr cmg::MuonFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
	
  edm::Handle<pat::MuonCollection> muonCands;
  cmg::MuonFactory::event_ptr result(new cmg::MuonFactory::collection);
  iEvent.getByLabel(muonLabel_,muonCands);

  unsigned index = 0;
  for(pat::MuonCollection::const_iterator m = muonCands->begin();
      m != muonCands->end(); ++m, ++index){
  			
    pat::MuonPtr muonPtr(muonCands, index);
    cmg::Muon mu(muonPtr);
    
    //set the generic quantities first
    leptonFactory_.set(muonPtr,&mu,iEvent,iSetup);
    leptonFactory_.set(getTrack(muonPtr),&mu,iEvent,iSetup);
    
    //now the muon like ones
    set(muonPtr,&mu);
  
 		
    result->push_back(mu);
  }
  return result;
}

void cmg::MuonFactory::set(const pat::MuonPtr& input, cmg::Muon* const output) const{
    
    output->isGlobal_ = cmg::toTriBool(input->isGlobalMuon());
    output->isTracker_ = cmg::toTriBool(input->isTrackerMuon());
    output->muonID_ = cmg::toTriBool(input->muonID(muonIDType_));
    output->nMatches_ = input->numberOfMatches();

    reco::TrackRef combinedMuon = getTrack(input);
    if(!combinedMuon.isNull()){
        output->pixelHits_ = combinedMuon->hitPattern().numberOfValidPixelHits();
        output->trackerHits_ = combinedMuon->hitPattern().numberOfValidTrackerHits();
        output->globalNormChi2_ = combinedMuon->normalizedChi2();
        output->muonHits_ = combinedMuon->hitPattern().numberOfValidMuonHits();   
    }
}

reco::TrackRef cmg::MuonFactory::getTrack(const pat::MuonPtr& input) const{
    
    //get the track related quantities
    reco::TrackRef combinedMuon;
    switch (type_){
     case Global:
     case Other:
     default:
        combinedMuon = input->globalTrack();
        break;
     case Inner:
        combinedMuon = input->innerTrack();
        break;
     case Track:
        combinedMuon = input->track();
        break;
    }
    return combinedMuon;  
}
