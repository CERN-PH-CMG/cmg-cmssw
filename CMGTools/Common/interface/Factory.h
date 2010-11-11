#ifndef FACTORY_H_
#define FACTORY_H_

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"

#include <memory>
#include <vector>

namespace cmg{

template <class CMGPhysicsObject>
class Factory{
 public:
  typedef CMGPhysicsObject object_type; 
  typedef std::vector<object_type> collection;
  typedef std::auto_ptr<collection> event_ptr;
  typedef StringCutObjectSelector<object_type> selector;
  virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const = 0;
};

}

#endif /*FACTORY_H_*/
