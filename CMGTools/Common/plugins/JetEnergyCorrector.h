#ifndef __CMGTools_Common_JetEnergyCorrector_h__
#define __CMGTools_Common_JetEnergyCorrector_h__

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace cmg{

template <class T>
class JetEnergyCorrector : public edm::EDProducer {

 public:
  
  typedef std::vector<T> collection;
  typedef std::auto_ptr<collection> event_ptr;
  typedef edm::View<T> view;
  
  explicit JetEnergyCorrector(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")),
    rho_(ps.getParameter<edm::InputTag>("rho")),
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)){
    produces<collection>("");
  }
  void produce(edm::Event& iEvent, const edm::EventSetup&);

 private:

  const edm::InputTag src_;
  const edm::InputTag rho_;
  const bool verbose_ ;

};
}

template <class T>
void cmg::JetEnergyCorrector<T>::produce(edm::Event& iEvent, const edm::EventSetup&){
   
    edm::Handle<view> cands;
    iEvent.getByLabel(src_,cands);
    
    edm::Handle< double > rhoHandle;
    iEvent.getByLabel(rho_, rhoHandle);
    double rho = *rhoHandle;

    if(verbose_){
      std::cout << "correcting jet collection " << src_.label() << " with " << cands->size() << " entries." << std::endl;
    }
    typename cmg::JetEnergyCorrector<T>::event_ptr result(new collection());

    if(verbose_)
      std::cout<<"rho = "<<rho<<std::endl;

    for(unsigned int i = 0; i < cands->size(); ++i){
      const T& cand = cands->at(i);
      if(verbose_)
	std::cout<<"\t old pt: "<<cand.pt()<<std::endl;
      result->push_back( cand );
    }

    iEvent.put(result);
    return;
}


#endif
