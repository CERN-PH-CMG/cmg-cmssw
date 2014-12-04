
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"


class RecovRecHitFilter : public edm::EDFilter {

  public:

    explicit RecovRecHitFilter(const edm::ParameterSet & iConfig);
    ~RecovRecHitFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    edm::InputTag eeRHSrc_;
    double minRecovE_;
    bool taggingMode_;

};


RecovRecHitFilter::RecovRecHitFilter(const edm::ParameterSet & iConfig) {
  eeRHSrc_      = iConfig.getParameter<edm::InputTag>("EERecHitSource");
  minRecovE_ = iConfig.getParameter<double>("MinRecovE");
  taggingMode_ = iConfig.getParameter<bool>("TaggingMode");
  produces<bool>("Result");
}


bool RecovRecHitFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<EcalRecHitCollection> eeRHs;
  iEvent.getByLabel(eeRHSrc_, eeRHs);

  double recovE = 0;
  for (EcalRecHitCollection::const_iterator eerh = eeRHs->begin(); eerh != eeRHs->end(); ++eerh) {
    if (eerh->time()!=0 ||
        eerh->energy()<=0 ||
        eerh->checkFlag(EcalRecHit::kPoorReco) ||
        eerh->checkFlag(EcalRecHit::kOutOfTime) ||
        eerh->checkFlag(EcalRecHit::kFaultyHardware) ||
        eerh->checkFlag(EcalRecHit::kNoisy) ||
        eerh->checkFlag(EcalRecHit::kPoorCalib) ||
        eerh->checkFlag(EcalRecHit::kSaturated) ||
        eerh->checkFlag(EcalRecHit::kLeadingEdgeRecovered) ||
        eerh->checkFlag(EcalRecHit::kNeighboursRecovered) ||
        eerh->checkFlag(EcalRecHit::kTowerRecovered) ||
        eerh->checkFlag(EcalRecHit::kDead) ||
        eerh->checkFlag(EcalRecHit::kKilled) ||
        eerh->checkFlag(EcalRecHit::kL1SpikeFlag) ||
        eerh->checkFlag(EcalRecHit::kWeird) ||
        eerh->checkFlag(EcalRecHit::kDiWeird)) continue;
    recovE += eerh->energy();
    if (recovE > minRecovE_) break;
  }
  bool result = (recovE < minRecovE_);

  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  if(taggingMode_) return true; else return result;

}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(RecovRecHitFilter);
