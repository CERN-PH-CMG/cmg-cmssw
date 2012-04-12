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
#include "DataFormats/METReco/interface/PFMET.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "CMG/MetAnalysis/plugins/MVAMetProducerExample.h"

using namespace edm;
using namespace std;
using namespace reco;

MVAMetProducerFromScratch::MVAMetProducerFromScratch(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  isData_         = iConfig.getParameter<bool>("isData");
  mvaMET_         = new MVAMet();
  mvaMET_         ->Initialize(iConfig,
			       TString((getenv("CMSSW_BASE")+string("/src/MitPhysics/data/gbrmet.root"))),
			       TString((getenv("CMSSW_BASE")+string("/src/MitPhysics/data/gbrmetphi.root")))
			       );

}
MVAMetProducerFromScratch::~MVAMetProducerFromScratch() { 
  delete mvaMET_;
}

void MVAMetProducerFromScratch::beginJob() { }

void MVAMetProducerFromScratch::endJob() { } 

void MVAMetProducerFromScratch::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  std::vector<LorentzVector >                   lVisible;
  std::vector<std::Pair<LorentzVector,double> > lPFInfo;  makeCandidates(lPFInfo, lUCJets,lCJets,lPV);
  std::vector<std::Pair<LorentzVector,double> > lJetInfo; makeJets      (lJetInfo,lUCJets,lCJets,lVertices);
  std::vector<Vector>                           lIPInfo;  makeVertices  (lIPInfo ,lVertices);

  double lDummyPt0  = 10; 
  double lDummyPhi0 = 0;
  double lDummyEta0 = 1;
  double lDummyPt1  = 10;
  double lDummyPhi1 = 2.; 
  double lDummyEta1 = -1;
  LorentzVector lVis0(0); lVis0.SetPtEtaPhiM(lDummyPt0,lDummyEta0,lDummyPhi0,0);
  LorentzVector lVis1(0); lVis1.SetPtEtaPhiM(lDummyPt0,lDummyEta0,lDummyPhi0,0);
  lVisible.push_back(lVis0);
  lVisible.push_back(lVis1);

  LorentzVector lMVAMet = mvaMET_->GetMet(lVis,l
				  
				  

  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset     ( new reco::PFMETCollection);
  pfmetcoll->push_back( lMVAMet);

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}
void MVAMetProducerFromScratch::makeJets(std::vector<std::Pair<LorentzVector,double>> &iJetInfo,PFJetCollection &iUCJets,PFJetCollection &iCJets,VertexCollection &iVertices) { 
  for(int i0   = 0; i0 < (int) iUCJets.size(); i0++) {   // uncorrecte jets collection                                                                                                        
    const PFJet       *pUCJet = &(iUCJets.at(i0));
    for(int i1 = 0; i1 < (int) iCJets .size(); i1++) {   // corrected jets collection                                                                                                   
      const PFJet     *pCJet  = &(iCJets.at(i1));
      if(       pUCJet->jetArea() != pCJet->jetArea()                  ) continue;
      if( fabs(pUCJet->eta() - pCJet->eta())         < 0.01            ) continue;
      if( pCJet->pt()                                < jetPtThreshold_ ) continue;
      double lJec = pCJet ->pt()/pUCJet->pt();
      double lMVA = utils_->jetMVA(pUCJet,lJec,&(iVertices.at(0)),iVertices);
      iJetInfo.push_back(std::Pair<LorentzVector,double> >(pCJet->p4(),lMVA));
    }
  }
}
void MVAMetProducerFromScratch::makeCandidates(std::vector<std::Pair<LorentzVector,double> > &iPFInfo,PFCandidateCollection &iCands,Vertex &iPV) { 
  for(int index = 0; index < (int)iCands.size(); index++) {
    const PFCandidateRef pflowCandRef = iCands->refAt(index).castTo<PFCandidateRef>();
    if(primaryVertex->size()==0) continue;
    double pDZ  = utils_->pfCandDz(pflowCandRef,iPV);
    iPFInfo.push_back(std::Pair<LorentzVector,double> >(pflowCandRef->p4(),pDZ));
  }
}
void MVAMetProducerFromScratch::makeVertices(std::vector<Vector>        &iPVInfo,VertexCollection &iVertices) { 
  for(int i0    = 0; i0 < (int)iVertices.size(); i0++) {
    const Vertex       *pVertex = &(iVertices.at(i0));
    iPFInfo.push_back(iVertices.position());
  }
}
