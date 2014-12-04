#ifndef DITAUOBJECTFACTORY_H_
#define DITAUOBJECTFACTORY_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CMGTools/H2TauTau/interface/METSignificance.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include <algorithm>
#include <set>

namespace cmg {

typedef pat::CompositeCandidate DiTauObject;

template<typename T, typename U>
class DiTauObjectFactory : public edm::EDProducer 
{
    public:
        DiTauObjectFactory(const edm::ParameterSet& ps) :             
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")),
            metLabel_(ps.getParameter<edm::InputTag>("metCollection")),
            metSigLabel_(ps.getParameter<edm::InputTag>("metsigCollection"))
        {
          produces<std::vector<DiTauObject>>();
          // JAN - do we still need to produce the MET significance?
          produces<std::vector<METSignificance>>();
        }

        void produce(edm::Event&, const edm::EventSetup&);
        static void set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, cmg::DiTauObject& obj);
        static void set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, const cmg::METSignificance& metSig, cmg::DiTauObject& obj);

    private:
        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
        const edm::InputTag metLabel_;
        const edm::InputTag metSigLabel_;
};

///Make when the types are different
template< typename T, typename U >
cmg::DiTauObject makeDiTau(const T& l1, const U& l2){
    cmg::DiTauObject diTauObj = cmg::DiTauObject();
    diTauObj.addDaughter(l1);
    diTauObj.addDaughter(l2);
    return diTauObj;
}

///Make when the types are the same - sorts the legs. It's still ensured below
///that it's only run if the tau collections are identical.
template<typename T>
cmg::DiTauObject makeDiTau(const T& l1, const T& l2){
    cmg::DiTauObject diTauObj = cmg::DiTauObject();
    if(l1.pt() >= l2.pt()){
        diTauObj.addDaughter(l1);
        diTauObj.addDaughter(l2);
    }else{
        diTauObj.addDaughter(l2);
        diTauObj.addDaughter(l1);
    }
    return diTauObj;
}


template<typename T, typename U>
void cmg::DiTauObjectFactory<T, U>::set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, cmg::DiTauObject& obj) {

  T first = pair.first;
  U second = pair.second;

  // reset daughters
  obj.clearDaughters();

  obj.addDaughter(first);
  obj.addDaughter(second);
  
  obj.setP4(first.p4() + second.p4());
  obj.setCharge(first.charge() + second.charge());
  obj.addDaughter(met);

  // JAN: Let's see where we set all this stuff,
  // maybe only in python!
  // It could also be set as user floats

  // obj->mT_ = cmg::DiObjectFactory<T, U>::mT(pair.first, pair.second);
  // if (pair.first.isElectron() || pair.first.isMuon())
  //   obj->lp_ = cmg::DiObjectFactory<T, U>::lp(pair.first, obj);
  // //calculate the Razor variables with MET
  // obj->mRT_ = cmg::DiObjectFactory<T, U>::mRT(pair.first, pair.second, met);
  // std::pair<double,double> pZetaVars = cmg::DiObjectFactory<T, U>::pZeta(pair.first, pair.second, met);
  // obj->pZetaVis_ = pZetaVars.first;
  // obj->pZetaMET_ = pZetaVars.second;    
  // obj->mTLeg1_ = cmg::DiObjectFactory<T, U>::mT(pair.first, met);
  // obj->mTLeg2_ = cmg::DiObjectFactory<T, U>::mT(pair.second, met);    
}

template<typename T, typename U>
void cmg::DiTauObjectFactory<T, U>::set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, const cmg::METSignificance& metSig, cmg::DiTauObject& obj) {
  set(pair, met, obj);

  // JAN - FIXME, important, we have to put the MET significance into the event
  // in an extra collection that's aligned with the di tau objects

  // obj.metSig_ = metSig;
}

template< typename T, typename U>
void cmg::DiTauObjectFactory<T, U>::produce(edm::Event& iEvent, const edm::EventSetup&){
  
  typedef edm::View<T> collection1;
  typedef edm::View<U> collection2;
  typedef edm::View<reco::LeafCandidate> met_collection;
  typedef edm::View<cmg::METSignificance> metSig_collection;
  
  edm::Handle<collection1> leg1Cands;
  iEvent.getByLabel(this->leg1Label_, leg1Cands);
  
  edm::Handle<collection2> leg2Cands;
  iEvent.getByLabel(this->leg2Label_, leg2Cands);
  
  edm::Handle<met_collection> metCands;
  bool metAvailable = false;
  if (!(metLabel_ == edm::InputTag())) {
    metAvailable = true; 
    iEvent.getByLabel(this->metLabel_, metCands);
  }

  edm::Handle<metSig_collection> metSigCands;

  bool metSigAvailable = false;
  if (!(metSigLabel_ == edm::InputTag())) {
    metSigAvailable = true; 
    iEvent.getByLabel(this->metSigLabel_, metSigCands);
  }
  
  std::auto_ptr<std::vector<DiTauObject>> result(new std::vector<DiTauObject>);
  std::auto_ptr<std::vector<METSignificance>> resultMETSig(new std::vector<METSignificance>);

  // Necessary?
  if( !leg1Cands->size() || !leg2Cands->size() ){
      iEvent.put<std::vector<DiTauObject>>(result);
      iEvent.put<std::vector<METSignificance>>(resultMETSig); 
      return;
  }

  const bool sameCollection = (leg1Cands.id () == leg2Cands.id());
  for (size_t i1 = 0; i1 < leg1Cands->size(); ++i1) {
    for (size_t i2 = 0; i2 < leg2Cands->size(); ++i2) {
      // if the same collection, only produce each possible pair once
      if (sameCollection && (i1 >= i2)) 
        continue;
      
      //enable sorting only if we are using the same collection - see Savannah #20217
      cmg::DiTauObject cmgTmp = sameCollection ? cmg::makeDiTau<T>((*leg1Cands)[i1], (*leg2Cands)[i2]) : cmg::makeDiTau<T, U>((*leg1Cands)[i1], (*leg2Cands)[i2]); 
      
      if (metAvailable && ! metCands->empty()) {
          T* first = dynamic_cast<T*>(cmgTmp.daughter(0));
          U* second = dynamic_cast<U*>(cmgTmp.daughter(1));
          if (metSigAvailable && !metSigCands->empty()) {
              cmg::DiTauObjectFactory<T, U>::set(std::make_pair(*first, *second), metCands->at(0), metSigCands->at(0), cmgTmp);
          }
          else
              cmg::DiTauObjectFactory<T, U>::set(std::make_pair(*first, *second), metCands->at(0), cmgTmp);
          result->push_back(cmgTmp);
      }
    }
  }

  iEvent.put(result); 
  iEvent.put(resultMETSig); 
}

} // namespace cmg


#endif /*DITAUOBJECTFACTORY_H_*/
