#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TH1F.h"
#include "TH2F.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

class PileupNormalizationProducer : public edm::EDProducer {
public:
  PileupNormalizationProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
private:
  std::map<std::string, edm::ParameterSet > objConfig;
  bool useVertexDistribution_;
  TH1F *vertexDataH_;
  TH2F *putovertexMCH_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
  : useVertexDistribution_( iConfig.getParameter<bool>("useVertexDistribution") )
{
  produces<float>("puWeight");

  objConfig["Vertices"]= iConfig.getParameter<edm::ParameterSet>("Vertices");

  //taken from dimuon events in runs:
  vertexDataH_ = new TH1F("vertex_data","data;Vertices;Events",25,0.,25.);
  vertexDataH_->SetBinContent(2,0.3515714);
  vertexDataH_->SetBinContent(3,1.155729);
  vertexDataH_->SetBinContent(4,1.808812);
  vertexDataH_->SetBinContent(5,1.744851);
  vertexDataH_->SetBinContent(6,1.218971);
  vertexDataH_->SetBinContent(7,0.8434031);
  vertexDataH_->SetBinContent(8,0.507172);
  vertexDataH_->SetBinContent(9,0.2828953);
  vertexDataH_->SetBinContent(10,0.117738);
  vertexDataH_->SetBinContent(11,0.1664797);

  //taken from DY->MuMu (M>20, Powheg sample)) todo
  putovertexMCH_ = new TH2F("pu_vertex_mc","simulation;Pileup;Vertices;Events",25,0.,25.,25,0.,25.);
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);

  if(useVertexDistribution_)
    {
      Handle<reco::VertexCollection> hVtx;
      iEvent.getByLabel(objConfig["Vertices"].getParameter<edm::InputTag>("source"), hVtx);
      std::vector<reco::VertexRef> selVertices = vertex::filter(hVtx,objConfig["Vertices"]);
      float weight = vertexDataH_->GetBinContent( selVertices.size()+1 );
      *puWeight=weight;
    }
  else
    {
      *puWeight=1.0;
    }

  iEvent.put(puWeight,"puWeight");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
