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
  TH1D *puUnfoldedH_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
{
  produces<float>("puWeight");

  std::vector<double> weights= iConfig.getParameter<std::vector<double> >("normalizationDistribution");
  puUnfoldedH_ = new TH1D("puunfoldedweights",";#Pileup;Weight",30,0,30);
  cout << "pileup re-weighting:  {";
  for(size_t iwgt=0; iwgt<weights.size(); iwgt++)
    {
      cout << weights[iwgt] << ";";
      puUnfoldedH_->SetBinContent(iwgt+1,weights[iwgt]);
    }
  cout << "};" << endl;
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);

  if(iEvent.isRealData())  *puWeight=1.0;
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

      float weight = puUnfoldedH_->GetBinContent( puUnfoldedH_->GetXaxis()->FindBin(npuevents) );
      *puWeight=weight;

    }

  iEvent.put(puWeight,"puWeight");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
