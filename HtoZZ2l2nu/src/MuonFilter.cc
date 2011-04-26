#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace muon{
  
  //
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hMu, const edm::ParameterSet &iConfig, double rho)
  {
    std::vector<reco::CandidatePtr> selMuons;
    
    try{
    
      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      std::string id = iConfig.getParameter<string>("id");
      double maxRelIso = iConfig.getParameter<double>("maxRelIso");
      double maxTrackChi2 = iConfig.getParameter<double>("maxTrackChi2");
      int minValidTrackerHits = iConfig.getParameter<int>("minValidTrackerHits");
      int minValidMuonHits = iConfig.getParameter<int>("minValidMuonHits");
      double maxDxy = iConfig.getParameter<double>("maxDxy");
      double maxDz = iConfig.getParameter<double>("maxDz");

      //iterate over the muons
      for(size_t iMuon=0; iMuon< hMu.product()->size(); ++iMuon)      
	{
	  reco::CandidatePtr muonPtr = hMu->ptrAt(iMuon);
	  const pat::Muon *muon = dynamic_cast<const pat::Muon *>( muonPtr.get() );

	  //muon type
	  bool isTracker = muon->isTrackerMuon();
	  bool isGlobal = muon->isGlobalMuon();
	  if(!isTracker || !isGlobal) continue;

	  //kinematics
	  double mPt = muon->pt();
	  double mEta = muon->eta();
	  if( mPt<minPt || fabs(mEta)>maxEta) continue; 

	  //track selection
	  double chi2 = muon->globalTrack()->normalizedChi2();	  
	  //	  int nValidPixelHits = muon->globalTrack()->hitPattern().numberOfValidPixelHits();
	  int nValidTrackerHits = muon->globalTrack()->hitPattern().numberOfValidTrackerHits();
	  int nValidMuonHits = muon->globalTrack()->hitPattern().numberOfValidMuonHits();
	  //	  int nMatches = muon->numberOfMatches();
	  if(chi2>maxTrackChi2 || nValidTrackerHits < minValidTrackerHits || nValidMuonHits<minValidMuonHits) continue;

	  //beamspot compatibility
	  const reco::TrackRef & innerTrack = muon->innerTrack();
	  double dxy = innerTrack->dxy();
	  double dz = innerTrack->dz();
	  //if(fabs(dxy)>fabs(maxDxy) || fabs(dz)>fabs(maxDz) ) continue;
	  	  
	  //id 
	  if(!id.empty())
	    {
	      bool hasId = (muon->muonID(id)>0) ;
	      if(!hasId) continue;
	    }
	  
	  //isolation
	  double norm = std::max((double)20.0,(double)mPt);
	  double ecalIso = muon->ecalIso();
	  double hcalIso = muon->hcalIso();
	  double trkIso = muon->trackIso();
	  double totalIso = (ecalIso+hcalIso+trkIso);
	  if(rho>0) totalIso -= rho*TMath::Pi()*pow(0.3,2);
	  double relIso = totalIso/norm;
	  if(relIso>maxRelIso) continue;

	  //muon is selected
	  selMuons.push_back(muonPtr);
	}
      
    }catch(std::exception &e){
      std::cout << "[muon::filter] failed with : " << e.what() << endl;
    }    
    
    return selMuons;
  }
}
