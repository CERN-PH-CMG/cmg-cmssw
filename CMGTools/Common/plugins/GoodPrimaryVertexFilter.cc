
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/Jet.h"


class GoodPrimaryVertexFilter : public edm::EDFilter {

  public:

    explicit GoodPrimaryVertexFilter(const edm::ParameterSet & iConfig);
    ~GoodPrimaryVertexFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    edm::InputTag vertexSrc_;
    bool taggingMode_, debugOn_;

};


GoodPrimaryVertexFilter::GoodPrimaryVertexFilter(const edm::ParameterSet & iConfig) {

  debugOn_      = iConfig.getUntrackedParameter<bool>("debugOn");
  vertexSrc_   = iConfig.getParameter<edm::InputTag>("VertexSource");
  taggingMode_ = iConfig.getParameter<bool>("TaggingMode");

  produces<bool>("Result");
}


bool GoodPrimaryVertexFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {


  edm::Handle<std::vector<reco::Vertex> > vtxs;
  iEvent.getByLabel(vertexSrc_, vtxs);


  int nGoodVtx = 0;
  int nVtx     = 0;

  for (std::vector<reco::Vertex>::const_iterator PVTX = vtxs->begin(); PVTX != vtxs->end(); ++PVTX) {
     nVtx++;
     if(!PVTX->isFake() && PVTX->ndof() > 4 && abs(PVTX->z()) <= 24 && PVTX->position().rho() <= 2) nGoodVtx++;
  }

  bool result = nGoodVtx>0;
  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  //if (!result)
  //   std::cout << "Failed GoodPrimaryVertexFilter: "
  //	     << iEvent.id().run() << " : " << iEvent.id().luminosityBlock() << " : " << iEvent.id().event()
  //	     << std::endl;
   
  if (debugOn_) {
    std::cout << "ScrapingFilter_debug: Run " << iEvent.id().run() << " Event " << iEvent.id().event() 
              << " Lumi Block " << iEvent.luminosityBlock() << " Number of vertices: " << nVtx 
	      << ", number of good vertices: " << nGoodVtx << " Accepted " << result << std::endl;
  }

  if(taggingMode_) return true; else return result;  
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GoodPrimaryVertexFilter);
