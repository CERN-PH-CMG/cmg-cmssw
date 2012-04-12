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

MVAMetProducerExample::MVAMetProducerExample(const edm::ParameterSet& iConfig) {
  produces<reco::PFMETCollection>();
  isData_         = iConfig.getParameter<bool>("isData");
  mvaMET_         = new MVAMet();
  mvaMET_         ->Initialize(iConfig,
			       TString((getenv("CMSSW_BASE")+string("/src/MitPhysics/data/gbrmet.root"))),
			       TString((getenv("CMSSW_BASE")+string("/src/MitPhysics/data/gbrmetphi.root")))
			       );

}
MVAMetProducerExample::~MVAMetProducerExample() { 
  delete mvaMET_;
}

void MVAMetProducerExample::beginJob() { }

void MVAMetProducerExample::endJob() { } 

void MVAMetProducerExample::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  //PF Met
  Handle<reco::PFMETCollection> hPFMetProduct;
  iEvent.getByLabel("PFMet"     ,hPFMetProduct);
  PFMET lPFMet = *(hPFMetProduct->begin());

  //TK Met
  Handle<reco::PFMETCollection> hTrackMetProduct;
  iEvent.getByLabel("TrackMet"  ,hTrackMetProduct);
  PFMET lTKMet = *(hTrackMetProduct->begin());

  //No PU Met
  Handle<reco::PFMETCollection> hNPMetProduct;
  iEvent.getByLabel("NoPUMet"  ,hNPMetProduct);
  PFMET lNPMet = *(hNPMetProduct->begin());

  //PU Met
  Handle<reco::PFMETCollection> hPUMetProduct;
  iEvent.getByLabel("PUMet"     ,hPUMetProduct);
  PFMET lPUMet = *(hPUMetProduct->begin());

  //PU Corrected Met
  Handle<reco::PFMETCollection> hPUCorrMetProduct;
  iEvent.getByLabel("PUCorrMet" ,hPUCorrMetProduct);
  PFMET lPCMet = *(hPUCorrMetProduct->begin());
 
  // uncorrected PF jets
  //edm::Handle< edm::View<reco::Candidate> > uncorrPFJetCollectionHandle;
  //try { iEvent.getByLabel("ak5PFJets",uncorrPFJetCollectionHandle); }
  //catch ( cms::Exception& ex ) { edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: ak5PFJets"; }
  //const edm::View<reco::Candidate> *uncorrPFJetColl = uncorrPFJetCollectionHandle.product();

  // fully corrected PF jets
  //edm::Handle< edm::View<reco::Candidate> > corrPFJetCollectionHandle;
  Handle<reco::PFJetCollection> corrPFJetCollectionHandle;
  if(isData_) { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3Residual",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { 
      edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3Residual"; }
  } else { 
    try { iEvent.getByLabel("ak5PFJetsL1FastL2L3",corrPFJetCollectionHandle); } 
    catch ( cms::Exception& ex ) { 
      edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection for: ak5PFJetsL1FastL2L3"; }
  }
  const PFJetCollection *lJets = corrPFJetCollectionHandle.product();
  
  // vertices  
  edm::Handle<reco::VertexCollection> primaryVertex;
  try { iEvent.getByLabel("offlinePrimaryVertices", primaryVertex); }
  catch(cms::Exception& ex ) {edm::LogWarning("NoPUMetProducer") << "Can't get candidate collection: offlinePrimaryVertices"; }

   //PV
  VertexCollection::const_iterator vMax = primaryVertex->begin();
  reco::Vertex pv;
  if (primaryVertex->size()>0) pv = *vMax;
  
  double lDummyPt0  = 10; 
  double lDummyPhi0 = 0;
  double lDummyEta0 = 1;
  double lDummyPt1  = 10;
  double lDummyPhi1 = 2.; 
  double lDummyEta1 = -1;
  
  PFMET lMVAMet = mvaMET_->GetMet(lDummyPt0,lDummyPhi0,lDummyEta0,
				  lDummyPt1,lDummyPhi1,lDummyEta1,
				  &lPFMet,&lTKMet,&lNPMet,&lPUMet,&lPCMet,lJets,int(primaryVertex->size()));

  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset     ( new reco::PFMETCollection);
  pfmetcoll->push_back( lMVAMet);

  // and finally put it in the event
  iEvent.put( pfmetcoll );
}
