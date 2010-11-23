#ifndef SinglePATMuonPairProducer_h
#define SinglePATMuonNuPairProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace cmg{

/**
 * A class to select the events from 0 to index in a collection.
 * 
 * if index is negative, count backwards as in python. If the
 * index calculated is larger than the collection length, we
 * return an empty collection.
 */
template <class T>
class SingleObjectCollectionSelector : public edm::EDProducer {

 public:
  
  typedef std::vector<T> collection;
  typedef edm::View<T> view;
  
  explicit SingleObjectCollectionSelector(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("inputCollection")),
    index_(ps.getUntrackedParameter<int>("index",-1)),
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)){
    produces<collection>("");
  }
  void produce(edm::Event& iEvent, const edm::EventSetup&);

 private:

  const edm::InputTag src_;
  const int index_;
  const bool verbose_ ;

};
}

template <class T>
void cmg::SingleObjectCollectionSelector<T>::produce(edm::Event& iEvent, const edm::EventSetup&){
   
    edm::Handle<view> cands;
    iEvent.getByLabel(src_,cands);
    
    if(verbose_){
      std::cout << "The collection " << src_.label() << " has " << cands->size() << " entries." << std::endl;
    }

    std::auto_ptr<collection> selectedCands(new collection());
    if(cands->size()){
        unsigned int index = 0;
        if(index_ < 0){
            index = cands->size() + index_ + 1;
        }else{
            index = static_cast<unsigned int>(index_);   
        }
        if(verbose_){
            std::cout << "The selected index was " << index << std::endl;
        }
        for(unsigned int i = 0; i < index; i++){
            selectedCands->push_back(cands->at(i));
        }
        if(verbose_){
            std::cout << "Copied " << selectedCands->size() << " events" << std::endl;
        }
    }
    iEvent.put(selectedCands);
    return;

}


#endif //SinglePATMuonNuPairProducer_h
