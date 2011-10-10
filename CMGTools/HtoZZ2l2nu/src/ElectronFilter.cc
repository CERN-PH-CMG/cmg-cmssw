#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace electron{
  
  //
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hEle, 
				       edm::Handle<edm::View<reco::Candidate> > &hMu, 
				       std::vector<reco::VertexRef> &goodVertices,
				       const reco::BeamSpot &theBeamSpot,
                                       const double& rho,
				       const edm::ParameterSet &iConfig)
  {
    CandidateWithVertexCollection selElectrons;

    try{
      //config parameters                                                                                                                                                 
      double minPt = iConfig.getParameter<double>("minPt");
      double minSuperClusterEt = iConfig.getParameter<double>("minSuperClusterEt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      bool applyConversionVeto = iConfig.getParameter<bool>("applyConversionVeto");
      bool vetoTransitionElectrons = iConfig.getParameter<bool>("vetoTransitionElectrons");
      int maxTrackLostHits = iConfig.getParameter<int>("maxTrackLostHits");
      string id = iConfig.getParameter<string>("id");
      double maxRelIso = iConfig.getParameter<double>("maxRelIso");
      double minDeltaRtoMuons = iConfig.getParameter<double>("minDeltaRtoMuons");
      double maxDistToBeamSpot = iConfig.getParameter<double>("maxDistToBeamSpot");

      //iterate over the electrons
      for(size_t iElec=0; iElec< hEle.product()->size(); ++iElec)
        {
	  reco::CandidatePtr elePtr = hEle->ptrAt(iElec);
	  const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
	 //  bool isEcalDriven(true);
	  // 	  try{
	  // 	    isEcalDriven = ele->ecalDrivenSeed();
	  // 	  }catch(std::exception &e){
	  // 	    //it may happen that GsfElectronCore is not stored (rely on the skim)
	  // 	  }
	  //if(!isEcalDriven) continue;

	  int eid=5;  //assume eid+conversion rejection
	  if( !id.empty() ) eid = (int) ele->electronID(id);

	  //kinematics
	  double ePt = ele->pt();
	  reco::SuperClusterRef sc = ele->superCluster();
	  double eSuperClusterEt  = sc->energy()/cosh(sc->eta());
	  double eEta = ele->eta();
	  double scEta= ele->superCluster()->eta();
	  if(ePt<minPt || fabs(eEta)>maxEta || eSuperClusterEt<minSuperClusterEt) continue; 
	  if(vetoTransitionElectrons && fabs(scEta)>1.4442 && fabs(scEta)<1.566) continue;
	  
	  //conversion veto (from track and info on electron id - 2nd bit)
	  const reco::GsfTrackRef & eTrack = ele->gsfTrack();
	  double d0=eTrack->dxy(theBeamSpot.position());
	  int nTrackLostHits=eTrack->trackerExpectedHitsInner().numberOfLostHits();
	  bool hasId =(eid & 0x1);
	  bool hasConversionTag = !((eid>>2) & 0x1);	 

	  //electron id + conversion veto
	  if(fabs(d0)>maxDistToBeamSpot) continue;
	  if( !hasId ) continue;
	  if(applyConversionVeto && hasConversionTag) continue;
	  if( nTrackLostHits>maxTrackLostHits) continue;
	  
	  //isolation
	  double relIso = lepton::getLeptonIso( elePtr, ePt, rho)[lepton::REL_ISO];
	  if(relIso>maxRelIso) continue;
	  
	  //cross clean with overlapping muons
	  bool isOverLappingWithMuon(false);
	  for(size_t iMuon=0; iMuon<hMu.product()->size(); ++iMuon)
	    {
	      reco::CandidatePtr muPtr = hMu->ptrAt(iMuon);
	      const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muPtr.get() );

	      if( !muon->isGlobalMuon() && !muon->isTrackerMuon() ) continue;
	      if( muon->innerTrack().isNull() ) continue;
	      if( muon->innerTrack()->numberOfValidHits() <=10 ) continue;
	      
	      double dR = deltaR(*muon->innerTrack(),*eTrack);
	      if(dR>minDeltaRtoMuons) continue;
	      isOverLappingWithMuon=true;
	      break;
	    }
	  if(isOverLappingWithMuon) continue;
	  
	  //the electron is selected (add vertex)
	  reco::VertexRef assocVtx=vertex::getClosestVertexTo<reco::Track>(ele->gsfTrack().get(),goodVertices);
	  selElectrons.push_back( CandidateWithVertex(elePtr,assocVtx) );
	}
    }catch(std::exception &e){
      cout << "[electron::filter] failed with " << e.what() << endl;
    }

    return selElectrons;
  }
}



