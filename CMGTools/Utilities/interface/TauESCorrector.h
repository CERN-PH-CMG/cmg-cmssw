#ifndef TAUESCORRECTOR_H_
#define TAUESCORRECTOR_H_

#include <iomanip>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CMGTools/Common/interface/Factory.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"


namespace cmg{

class TauESCorrector : public Factory<cmg::Tau> {

public:
  TauESCorrector(const edm::ParameterSet& ps);
  virtual ~TauESCorrector() {};
  typedef cmg::Factory<cmg::Tau>::event_ptr event_ptr;
  virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

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
}

cmg::TauESCorrector::event_ptr cmg::TauESCorrector::create(const edm::Event& iEvent,const edm::EventSetup&)
{
  typedef std::vector<cmg::Tau> collection;
  edm::Handle<collection> candCands;
  cmg::TauESCorrector::event_ptr result(new collection);
  iEvent.getByLabel(candLabel_, candCands);

  size_t index = 0;
  for(collection::const_iterator mi = candCands->begin();
      mi != candCands->end(); ++mi, ++index) {

    edm::Ref<collection> candPtr(candCands, index);
    cmg::Tau cand(*candPtr);

    reco::Candidate::LorentzVector fourVec = cand.p4();

    //corrections depending on decay mode
    if(cand.decayMode()==0) fourVec *= OneProngNoPi0Correction_;
    if(cand.decayMode()==1 || cand.decayMode()==2) fourVec *= OneProng1Pi0Correction_ + OneProng1Pi0CorrectionPtSlope_ * TMath::Min(TMath::Max(cand.pt()-45.,0.),10.);
    if(cand.decayMode()==10) fourVec *= ThreeProngCorrection_ + ThreeProngCorrectionPtSlope_ * TMath::Min(TMath::Max(cand.pt()-32.,0.),18.);
    //other decaymodes are not corrected

    cand.setP4(fourVec);

    result->push_back(cand);
  }
  return result;
}

//#include "FWCore/Framework/interface/MakerMacros.h"
 
//DEFINE_FWK_MODULE(TauESCorrector);

#endif /*TAUESCORRECTOR_H_*/
