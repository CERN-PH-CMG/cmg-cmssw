#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

using namespace std;

namespace muon{
  
  //
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hMu, 
				       std::vector<reco::VertexRef> &goodVertices,
				       const edm::ParameterSet &iConfig)
  {
    CandidateWithVertexCollection selMuons;
    
    try{
    
      //config parameters
      double minPt = iConfig.getParameter<double>("minPt");
      double maxEta = iConfig.getParameter<double>("maxEta");
      std::string id = iConfig.getParameter<string>("id");
      double maxRelIso = iConfig.getParameter<double>("maxRelIso");
      double maxTrackChi2 = iConfig.getParameter<double>("maxTrackChi2");
      int minValidTrackerHits = iConfig.getParameter<int>("minValidTrackerHits");
      int minValidMuonHits = iConfig.getParameter<int>("minValidMuonHits");

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

	  //id 
	  if(!id.empty())
	    {
	      bool hasId = (muon->muonID(id)>0) ;
	      if(!hasId) continue;
	    }
	  
	  //isolation
	  double relIso = lepton::getLeptonIso( muonPtr, mPt )[lepton::REL_ISO];
	  if(relIso>maxRelIso) continue;
	  
	  //muon is selected (add vertex)
	  reco::VertexRef assocVtx=vertex::getClosestVertexTo<reco::Track>(muon->innerTrack().get(),goodVertices);
	  selMuons.push_back(CandidateWithVertex(muonPtr,assocVtx));
	}
      
    }catch(std::exception &e){
      std::cout << "[muon::filter] failed with : " << e.what() << endl;
    }    
    
    return selMuons;
  }
}
