#ifndef TAUESCORRECTOR_H_
#define TAUESCORRECTOR_H_

#include <iomanip>


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TauReco/interface/PFTau.h"


namespace cmg{

class TauESCorrector : public edm::EDProducer {

typedef std::vector<pat::Tau> collection;

public:
  TauESCorrector(const edm::ParameterSet& ps);
  virtual ~TauESCorrector() {};
  void produce(edm::Event&, const edm::EventSetup&);

private:
  const edm::InputTag candLabel_;
  double OneProngNoPi0Correction_;
  double OneProng1Pi0Correction_;
  double OneProng1Pi0CorrectionPtSlope_;
  double ThreeProngCorrection_;
  double ThreeProngCorrectionPtSlope_;
};


}

cmg::TauESCorrector::TauESCorrector(const edm::ParameterSet& ps):
  candLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  OneProngNoPi0Correction_(ps.getParameter<double>("OneProngNoPi0Correction")),
  OneProng1Pi0Correction_(ps.getParameter<double>("OneProng1Pi0Correction")),
  OneProng1Pi0CorrectionPtSlope_(ps.getParameter<double>("OneProng1Pi0CorrectionPtSlope")),
  ThreeProngCorrection_(ps.getParameter<double>("ThreeProngCorrection")),
  ThreeProngCorrectionPtSlope_(ps.getParameter<double>("ThreeProngCorrectionPtSlope"))
{

  std::cout<< "Scaling OneProngNoPi0 cands by " << OneProngNoPi0Correction_<< std::endl;
  std::cout<< "Scaling OneProng1Pi0 cands by " <<  OneProng1Pi0Correction_<< std::endl;
  std::cout<< "Scaling OneProng1Pi0 pT Slope cands by " << OneProng1Pi0CorrectionPtSlope_<< std::endl;
  std::cout<< "Scaling ThreeProng cands by " << ThreeProngCorrection_<< std::endl;
  std::cout<< "Scaling ThreeProng pT Slope cands by " << ThreeProngCorrectionPtSlope_<< std::endl;

  produces<collection>();
}

void cmg::TauESCorrector::produce(edm::Event& iEvent,const edm::EventSetup&)
{
  edm::Handle<collection> candCands;
  std::auto_ptr<collection> result(new collection);
  iEvent.getByLabel(candLabel_, candCands);

  size_t index = 0;
  for(collection::const_iterator mi = candCands->begin();
      mi != candCands->end(); ++mi, ++index) {

    edm::Ref<collection> candPtr(candCands, index);
    pat::Tau cand(*candPtr);

    reco::Candidate::LorentzVector fourVec = cand.p4();

    //corrections depending on decay mode
    if(cand.decayMode()==0) fourVec *= OneProngNoPi0Correction_;
    if(cand.decayMode()==1 || cand.decayMode()==2) fourVec *= OneProng1Pi0Correction_ + OneProng1Pi0CorrectionPtSlope_ * TMath::Min(TMath::Max(cand.pt()-45.,0.),10.);
    if(cand.decayMode()==10) fourVec *= ThreeProngCorrection_ + ThreeProngCorrectionPtSlope_ * TMath::Min(TMath::Max(cand.pt()-32.,0.),18.);
    //other decaymodes are not corrected

    cand.setP4(fourVec);

    result->push_back(cand);
  }
  iEvent.put(result);
}

//#include "FWCore/Framework/interface/MakerMacros.h"
 
//DEFINE_FWK_MODULE(TauESCorrector);

#endif /*TAUESCORRECTOR_H_*/
