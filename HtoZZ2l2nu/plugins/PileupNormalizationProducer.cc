#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/PileupFilter.h"

class PileupNormalizationProducer : public edm::EDProducer 
{
public:
  PileupNormalizationProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  
private:
  TH1D *puUnfoldedH_;
  float maxWeight_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
{
  produces<float>("puWeight");
  produces<float>("renPuWeight");
  produces<int>("nITpu");  
  produces<int>("nOOTpu");  

  //generate weight distribution
  std::vector<double> mc   = iConfig.getParameter<std::vector<double> >("mcDistribution");
  std::vector<double> data = iConfig.getParameter<std::vector<double> >("dataDistribution");
  puUnfoldedH_ = pileup::generateWeightsFrom(mc,data,maxWeight_);

  cout << "pileup re-weighting:  {";
  for(int ibin=1; ibin<=puUnfoldedH_->GetXaxis()->GetNbins(); ibin++)
    cout << puUnfoldedH_->GetBinContent(ibin) << ", ";
  cout << "};" << endl;
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);
  auto_ptr<float> renPuWeight(new float);
  auto_ptr<int> nITpu(new int);
  auto_ptr<int> nOOTpu(new int);

  if(iEvent.isRealData())  
    {
      *puWeight=1.0;
      *renPuWeight=1.0;
    }
  else
    {
      //get the number of generated pileup events
      edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
      iEvent.getByType(puInfoH);
      int npuevents(0),nootpuevents(0);
      if(puInfoH.isValid())
	{
	  for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	    {
	      if(it->getBunchCrossing() !=0) nootpuevents += it->getPU_NumInteractions(); 
	      else                           npuevents    = it->getPU_NumInteractions();
	    }
	}

      std::vector<double> theWeights = pileup::getWeightsFor(npuevents,puUnfoldedH_,maxWeight_);
      *puWeight    = theWeights[pileup::MCTODATA];
      *renPuWeight = theWeights[pileup::MCTODATAINTEGER];
      *nITpu       = npuevents;
      *nOOTpu      = nootpuevents;
    }
  
  //put in the event
  iEvent.put(puWeight,"puWeight");
  iEvent.put(renPuWeight,"renPuWeight");
  iEvent.put(nITpu,"nITpu");
  iEvent.put(nOOTpu,"nOOTpu");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
