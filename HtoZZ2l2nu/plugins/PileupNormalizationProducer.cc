#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TH1D.h"
#include "TH2D.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

class PileupNormalizationProducer : public edm::EDProducer {
public:
  PileupNormalizationProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;

private:
  std::map<std::string, edm::ParameterSet > objConfig;
  bool useVertexDistribution_;
  TH1D *vertexDataH_, *puUnfoldedH_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
  : useVertexDistribution_( iConfig.getParameter<bool>("useVertexDistribution") )
{
  produces<float>("puWeight");

  objConfig["Vertices"]= iConfig.getParameter<edm::ParameterSet>("Vertices");

  //from vertex distribution ratio
  vertexDataH_ = new TH1D("vertex_data","data;Vertices;Events",25,0.,25.);
  vertexDataH_->SetBinContent(2,0.0338308);
  vertexDataH_->SetBinContent(3,0.121082);
  vertexDataH_->SetBinContent(4,0.203664);
  vertexDataH_->SetBinContent(5,0.202303);
  vertexDataH_->SetBinContent(6,0.158852);
  vertexDataH_->SetBinContent(7,0.10762);
  vertexDataH_->SetBinContent(8,0.072571);
  vertexDataH_->SetBinContent(9,0.0408767);
  vertexDataH_->SetBinContent(10,0.0278872);
  vertexDataH_->SetBinContent(11,0.0269992);
  vertexDataH_->SetBinContent(12,0.00431492);

  //from unfolded disribution
  puUnfoldedH_ = new TH1D("unfold_data","Unfolded data;Pileup;Events",25,0.,25.);
  puUnfoldedH_->SetBinContent(1,0);
  puUnfoldedH_->SetBinContent(2,0.000744032);
  puUnfoldedH_->SetBinContent(3,0.136401);
  puUnfoldedH_->SetBinContent(4,0.525813);
  puUnfoldedH_->SetBinContent(5,0.284197);
  puUnfoldedH_->SetBinContent(6,0.0489245);
  puUnfoldedH_->SetBinContent(7,0.00376914);
  puUnfoldedH_->SetBinContent(8,0.000147238);
  puUnfoldedH_->SetBinContent(9,5.16722e-06);
  puUnfoldedH_->SetBinContent(10,9.23698e-08);
  puUnfoldedH_->SetBinContent(11,5.72378e-10);
  puUnfoldedH_->SetBinContent(12,3.52041e-12);
  puUnfoldedH_->SetBinContent(13,2.9421e-18);
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);

  if(iEvent.isRealData())
    {
      *puWeight=1.0;
    }
  else
    {

      //get the number of generated pileup events
      edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
      iEvent.getByType(puInfoH);
      int npuevents(0);
      if(puInfoH.isValid())
	{
	  for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	    {
	      if(it->getBunchCrossing() !=0) continue;
	      npuevents=it->getPU_NumInteractions();
	    }
	}

      //determine normalization
      if(!useVertexDistribution_)
	{
	  float weight = puUnfoldedH_->GetBinContent( npuevents+1 );
	  *puWeight=weight;
	}
      else if(useVertexDistribution_)
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
    }

  iEvent.put(puWeight,"puWeight");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
