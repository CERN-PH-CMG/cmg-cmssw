#ifndef __CMGTools_Common_IndexObjectsSelector_h__
#define __CMGTools_Common_IndexObjectsSelector_h__

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

/**
 * A class to select the events from 0 to index in a collection.
 * 
 * If the index is negative, or greater than the number of entries
 * in the collection, we copy all elements.
 * 
 */
template <class T>
class IndexObjectsSelector : public edm::EDProducer {

 public:
  
  typedef std::vector<T> collection;
  typedef std::auto_ptr<collection> event_ptr;
  typedef edm::View<T> view;
  
  explicit IndexObjectsSelector(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("inputCollection")),
    min_(ps.getParameter<int>("min")),
    max_(ps.getParameter<int>("max")),
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)){
    produces<collection>("");
  }
  void produce(edm::Event& iEvent, const edm::EventSetup&);

 private:

  const edm::InputTag src_;
  const int min_;
  const int max_;
  const bool verbose_ ;

};
}

template <class T>
void cmg::IndexObjectsSelector<T>::produce(edm::Event& iEvent, const edm::EventSetup&){
   
  edm::Handle<view> cands;
  iEvent.getByLabel(src_,cands);
  
  if(verbose_){
    std::cout << "The collection " << src_.label() << " has " << cands->size() << " entries." << std::endl;
  }
  typename cmg::IndexObjectsSelector<T>::event_ptr result(new collection());
  if(cands->size()){
    
    //the max index
    unsigned int max = 0;
    if( (max_ < 0) || (static_cast<unsigned int>(max_) >= cands->size() ) ){
      max = cands->size()-1;
    }else{
      max = static_cast<unsigned int>(max_);
    }
    
    unsigned int min = 0;
    if( min_ > 0 ) min = static_cast<unsigned int>(min_);
    
    if(verbose_){
      std::cout << "IndexSelector: min = " << min << ", max = "<< max << std::endl;
    }
    for(unsigned int i = min; i <= max; i++){
      reco::CandidatePtr ptrToMother( cands, i );
      result->push_back(cands->at(i));
      result->back().setSourceCandidatePtr( ptrToMother );
    }
    if(verbose_){
      std::cout << "Copied " << result->size() << " objects" << std::endl;
    }
  }
  iEvent.put(result);
  return;
}


#endif
