#ifndef FACTORY_H_
#define FACTORY_H_

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/ProducerBase.h"

#include <memory>
#include <vector>

namespace cmg{

template <class CMGPhysicsObject>
class Factory {
 public:
  /// type of objects produced by the factory
  typedef CMGPhysicsObject object_type; 

  /// type of object collection 
  typedef std::vector<object_type> collection;

  /// type of auto_ptr to collection 
  typedef std::auto_ptr<collection> event_ptr;

  /// type of StringCutObjectSelector used to select objects created by the factory
  typedef StringCutObjectSelector<object_type> selector;

  /// hook for "produces"
  virtual void produces(edm::ProducerBase* const producer) const{
    producer->produces<collection>("");
  }

  /// access events and create a collection of object_type
  virtual event_ptr create(const edm::Event&, const edm::EventSetup&) = 0;
  
  //hooks for the factory for lumi, run, etc
  virtual bool beginRun(edm::Run&, edm::EventSetup const&){
    return true;
  }
  virtual bool endRun(edm::Run&, edm::EventSetup const&){
    return true;
  }
  virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){
    return true;
  }
  virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&){
    return true;
  }
  
};

}

#endif /*FACTORY_H_*/
