#ifndef _CMGTools_Susy_BoolToIntProducer_H_
#define _CMGTools_Susy_BoolToIntProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include <iostream>

class BoolToIntProducer : public edm::EDFilter{
 public:

  BoolToIntProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")),
    filter_(ps.getUntrackedParameter<bool>("filter",false)){
    produces<int>();
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    edm::Handle<bool> handle; 
    iEvent.getByLabel( src_, handle );
  
    const int result = handle.product() ? 1 : 0;
    std::auto_ptr<int> output( new int(result) ); 
    iEvent.put( output );

    if(!handle.product()){
      std::cout << "Filter: " << result << std::endl;
    }
    if(filter_){
        return handle.product();  
    }
    return true; 
  }

 private:
  edm::InputTag src_;
  bool filter_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BoolToIntProducer);

#endif
