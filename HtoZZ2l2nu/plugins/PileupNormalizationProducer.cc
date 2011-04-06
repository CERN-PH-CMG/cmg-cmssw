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
  int fixPileupTo_;
  TH1D *vertexDataH_, *fixedPileupH_, *puMCH_;
  TH2D *putovertexMCH_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
  : useVertexDistribution_( iConfig.getParameter<bool>("useVertexDistribution") ),
    fixPileupTo_( iConfig.getParameter<int>("fixPileupTo") )
{
  produces<float>("puWeight");

  objConfig["Vertices"]= iConfig.getParameter<edm::ParameterSet>("Vertices");

  //taken from dimuon events in runs:
  vertexDataH_ = new TH1D("vertex_data","data;Vertices;Events",25,0.,25.);
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
  putovertexMCH_ = new TH2D("pu_vertex_mc","simulation;Pileup;Vertices;weight",25,0.,25.,25,0.,25.);
  puMCH_ = putovertexMCH_->ProjectionY("pu_mc");

  //fixed pileup (take from poisson distribution)
  if(fixPileupTo_>0)
    {
      fixedPileupH_ = new TH1D("fixpu",";Pileup;weight",25,0.,25.);
      for(int ibin=1; ibin<=fixedPileupH_->GetXaxis()->GetNbins(); ibin++)
	fixedPileupH_->SetBinContent(ibin, TMath::Poisson(ibin-1,fixPileupTo_) );
      fixedPileupH_->Divide(puMCH_);
    }
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

      
      if(fixPileupTo_>0)
	*puWeight = fixedPileupH_->GetBinContent(npuevents+1);
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
