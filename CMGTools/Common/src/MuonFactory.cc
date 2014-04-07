#include "CMGTools/Common/interface/MuonFactory.h"


#include "TrackingTools/AnalyticalJacobians/interface/JacobianCurvilinearToCartesian.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h"


cmg::MuonFactory::event_ptr cmg::MuonFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	
  edm::Handle<pat::MuonCollection> muonCands;
  cmg::MuonFactory::event_ptr result(new cmg::MuonFactory::collection);
  iEvent.getByLabel(muonLabel_,muonCands);

  unsigned index = 0;
  for(pat::MuonCollection::const_iterator m = muonCands->begin();
      m != muonCands->end(); ++m, ++index){
  			
    pat::MuonPtr muonPtr(muonCands, index);
    cmg::Muon mu(muonPtr);
    set(muonPtr,&mu,iEvent,iSetup);
    result->push_back(mu);
  }
  return result;
}

void cmg::MuonFactory::set(const pat::MuonPtr& input, cmg::Muon* const output, const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
        
    //set the generic quantities first
    leptonFactory_.set(input,output,iEvent,iSetup);
    leptonFactory_.set(getTrackerTrack(input),output,iEvent,iSetup);

    //now the muon like ones
    output->type_ = input->type();
    output->isGlobal_ = cmg::toTriBool(input->isGlobalMuon());
    output->isTracker_ = cmg::toTriBool(input->isTrackerMuon());
    
    //NOT NEEDED as implemented in Muon.h accessing the source pointer 
    //output->isPF_ = cmg::toTriBool(input->isPFMuon()); //this is not functional in release < 52X
    //assert(input->hasUserFloat("isPFMuon")); //this is normally added by PATPFMuonEmbedder module. see comment above
    //output->isPF_ = cmg::toTriBool(input->userFloat("isPFMuon")); 
    //output->isTMOST_ = cmg::toTriBool(input->muonID("TMOneStationTight")); 

    output->muonID_ = cmg::toTriBool(input->muonID(muonIDType_));//useless
    output->nMatches_ = input->numberOfMatches(); 
    
    //NOT NEEDED as implemented in Muon.h accessing the source pointer 
    //output->nMatchedStations_ = input->numberOfMatchedStations(); 

    output->dB3D_ = input->dB( pat::Muon::PV3D );
    output->edB3D_ = input->edB( pat::Muon::PV3D );


    //get the magnetic field and calculate the covariance
    //matrix in cartesian coordinates
    if( input->muonBestTrack().isNonnull()) {
      edm::ESHandle<MagneticField> magfield;
      iSetup.get<IdealMagneticFieldRecord>().get(magfield); 
      
      GlobalTrajectoryParameters gp(GlobalPoint(input->vx(), input->vy(),  input->vz()),
				    GlobalVector(input->muonBestTrack()->px(),input->muonBestTrack()->py(),input->muonBestTrack()->pz()),
				    input->muonBestTrack()->charge(),
				    magfield.product());
      JacobianCurvilinearToCartesian curv2cart(gp);
      CartesianTrajectoryError cartErr= ROOT::Math::Similarity(curv2cart.jacobian(), input->muonBestTrack()->covariance());
      AlgebraicSymMatrix66  m = cartErr.matrix();
      output->covarianceMatrix_ = m;
    }



    //we need the tracker track for this
    reco::TrackRef track = input->track();
    if(track.isNonnull() && track.isAvailable()){
      
      output->pixelHits_ = track->hitPattern().numberOfValidPixelHits();
      output->trackerHits_ = track->hitPattern().numberOfValidTrackerHits();
      output->trackerLayersWithMeasurement_ = track->hitPattern().trackerLayersWithMeasurement();
      //NOT NEEDED as implemented in Muon.h accessing the source pointer 
      //output->pixelLayersWithMeasurement_ = track->hitPattern().pixelLayersWithMeasurement();
      //output->tkNormChi2_ = track->normalizedChi2();
    }

    reco::TrackRef globaltrack = input->globalTrack();
    if(globaltrack.isNonnull() && globaltrack.isAvailable()){
      output->globalNormChi2_ = globaltrack->normalizedChi2();
      output->muonHits_ = globaltrack->hitPattern().numberOfValidMuonHits();         
    }
    
    
    // The following variables have been previously uniquely defined given a particular track type

    //  reco::TrackRef combinedMuon = getTrack(input);
    //     if(combinedMuon.isNonnull() && combinedMuon.isAvailable()){
    //       //output->pixelHits_ = combinedMuon->hitPattern().numberOfValidPixelHits();
    //       //output->trackerHits_ = combinedMuon->hitPattern().numberOfValidTrackerHits();
    //         output->globalNormChi2_ = combinedMuon->normalizedChi2();
    //         output->muonHits_ = combinedMuon->hitPattern().numberOfValidMuonHits();   
    //     }
   
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

reco::TrackRef cmg::MuonFactory::getTrackerTrack(const pat::MuonPtr& input) const{
    
    //get the track related quantities
    reco::TrackRef combinedMuon;
    switch (type_){
     case Global:
     case Other:
     case Inner:
       combinedMuon = input->innerTrack(); //NOTE: innerTrack() and track() return the same thing, therefore this function is useless
        break;
     case Track:
        combinedMuon = input->track();
        break;
    }
    return combinedMuon;  
}
