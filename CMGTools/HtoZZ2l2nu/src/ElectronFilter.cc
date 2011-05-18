#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace electron{
  
  //
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hEle, edm::Handle<edm::View<reco::Candidate> > &hMu, const edm::ParameterSet &iConfig)
  {
    std::vector<reco::CandidatePtr> selElectrons;

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

      //iterate over the electrons
      for(size_t iElec=0; iElec< hEle.product()->size(); ++iElec)
        {
	  reco::CandidatePtr elePtr = hEle->ptrAt(iElec);
	  const pat::Electron *ele = dynamic_cast<const pat::Electron *>( elePtr.get() );
	  bool isEcalDriven(true);
	  try{
	    isEcalDriven = ele->ecalDrivenSeed();
	  }catch(std::exception &e){
	    //it may happen that GsfElectronCore is not stored (rely on the skim)
	  }
	  if(!isEcalDriven) continue;

	  int eid=1;
	  if( !id.empty() ) eid = (int) ele->electronID(id);
	  
	  //kinematics
	  double ePt = ele->pt();
	  reco::SuperClusterRef sc = ele->superCluster();
	  double eSuperClusterEt  = sc->energy()/cosh(sc->eta());
	  double eEta = ele->eta();
	  if(ePt<minPt || fabs(eEta)>maxEta || eSuperClusterEt<minSuperClusterEt) continue; 
	  if(vetoTransitionElectrons && fabs(eEta)>1.4442 && fabs(eEta)<1.566) continue;
	  
	  //conversion veto (from track and info on electron id - 2nd bit)
	  const reco::GsfTrackRef & eTrack = ele->gsfTrack();
	  int nTrackLostHits=eTrack->trackerExpectedHitsInner().numberOfLostHits();
	  if( nTrackLostHits>maxTrackLostHits) continue;
	  bool hasConversionTag(false);
	  if(id.find("simple") != string::npos) hasConversionTag = !((eid>>2) & 0x1);	 
	  if(applyConversionVeto && hasConversionTag) continue;

	  //electron id (just need the first bit)
	  if( !(eid & 0x1) ) continue;
	  
	  //isolation
	  double norm = std::max(ePt,(double)20.);
	  double trackIso = ele->trackIso();
	  double ecalIso = ele->ecalIso();
	  double hcalIso = ele->hcalIso();
	  double totalIso = trackIso+hcalIso;
	  if (ele->isEB()) {
	    if( ecalIso > 1.0 ) totalIso += ecalIso - 1;
	    else totalIso += ecalIso;
	  } else {
	    totalIso += ecalIso;
	  }

	  double relIso = totalIso / norm;
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

	  //the electron is selected
	  selElectrons.push_back(elePtr);
	}
    }catch(std::exception &e){
      cout << "[electron::filter] failed with " << e.what() << endl;
    }

    return selElectrons;
  }
}



