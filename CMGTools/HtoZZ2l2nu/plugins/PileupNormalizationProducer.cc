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

  //from observed vertex distribution ratio
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

  puUnfoldedH_ = new TH1D("unfold_data","Unfolded data;Pileup;Events",25,0.,25.);
  /*
  //from unfolded vertex disribution
  puUnfoldedH_->SetBinContent(1,0.136598);
  puUnfoldedH_->SetBinContent(2,0.548235);
  puUnfoldedH_->SetBinContent(3,1.26893);
  puUnfoldedH_->SetBinContent(4,1.9627);
  puUnfoldedH_->SetBinContent(5,2.10484);
  puUnfoldedH_->SetBinContent(6,1.99065);
  puUnfoldedH_->SetBinContent(7,1.73487);
  puUnfoldedH_->SetBinContent(8,1.28593);
  puUnfoldedH_->SetBinContent(9,1.00456);
  puUnfoldedH_->SetBinContent(10,0.685606);
  puUnfoldedH_->SetBinContent(11,0.508796);
  puUnfoldedH_->SetBinContent(12,0.346366);
  puUnfoldedH_->SetBinContent(13,0.236133);
  puUnfoldedH_->SetBinContent(14,0.183413);
  puUnfoldedH_->SetBinContent(15,0.0938161);
  puUnfoldedH_->SetBinContent(16,0.102228);
  puUnfoldedH_->SetBinContent(17,0.0537016);
  puUnfoldedH_->SetBinContent(18,0.0273493);
  puUnfoldedH_->SetBinContent(19,0.0240031);
  puUnfoldedH_->SetBinContent(20,0.017485);
  puUnfoldedH_->SetBinContent(21,0.0256616);
  puUnfoldedH_->SetBinContent(22,0.00848088);
  puUnfoldedH_->SetBinContent(23,0.0200775);
  puUnfoldedH_->SetBinContent(24,0.035064);
  puUnfoldedH_->SetBinContent(25,0.00959897);
  */

  //from estimatePileup.py
  puUnfoldedH_->SetBinContent(1,0.327021);
  puUnfoldedH_->SetBinContent(2,0.713425);
  puUnfoldedH_->SetBinContent(3,1.58586);
  puUnfoldedH_->SetBinContent(4,2.37524);
  puUnfoldedH_->SetBinContent(5,2.67794);
  puUnfoldedH_->SetBinContent(6,2.42404);
  puUnfoldedH_->SetBinContent(7,1.83504);
  puUnfoldedH_->SetBinContent(8,1.19491);
  puUnfoldedH_->SetBinContent(9,0.683202);
  puUnfoldedH_->SetBinContent(10,0.348418);
  puUnfoldedH_->SetBinContent(11,0.160456);
  puUnfoldedH_->SetBinContent(12,0.0716393);
  puUnfoldedH_->SetBinContent(13,0.0334914);
  puUnfoldedH_->SetBinContent(14,0.0158219);
  puUnfoldedH_->SetBinContent(15,0.00713356);
  puUnfoldedH_->SetBinContent(16,0.00342917);
  puUnfoldedH_->SetBinContent(17,0.00160283);
  puUnfoldedH_->SetBinContent(18,0.00073031);
  puUnfoldedH_->SetBinContent(19,0.000316668);
  puUnfoldedH_->SetBinContent(20,0.000211067);
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
