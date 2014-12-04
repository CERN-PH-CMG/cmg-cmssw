#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"


class EENoiseFilter : public edm::EDFilter {

  public:

    explicit EENoiseFilter(const edm::ParameterSet & iConfig);
    ~EENoiseFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag eeRHSrc_;
    unsigned int maxNrRecHits_;
    bool taggingMode_;
};


EENoiseFilter::EENoiseFilter(const edm::ParameterSet & iConfig) {
  eeRHSrc_      = iConfig.getParameter<edm::InputTag>("EERecHitSource");
  maxNrRecHits_ = iConfig.getParameter<unsigned int>("MaxNrRecHits");
  taggingMode_ = iConfig.getParameter<bool>("TaggingMode");

  produces<bool>("Result");
}


bool EENoiseFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  edm::Handle<EcalRecHitCollection> eeRHs;
  iEvent.getByLabel(eeRHSrc_, eeRHs);
  
  bool result = eeRHs->size() < maxNrRecHits_;
  
  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  if(taggingMode_) return true; else return result;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EENoiseFilter);
