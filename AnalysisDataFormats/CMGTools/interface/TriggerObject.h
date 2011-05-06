#ifndef _AnalysisDataFormats_CMGTools_TriggerObject_H_
#define _AnalysisDataFormats_CMGTools_TriggerObject_H_

#include "AnalysisDataFormats/CMGTools/interface/NamedObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

namespace cmg {

class TriggerObjectFactory;

/**
 * Store the 4-vector of the object that fired the trigger
 */
class TriggerObject : public PhysicsObjectWithPtr<pat::TriggerObjectPtr>, public cmg::NamedObject{

   public:
    explicit TriggerObject():
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(),
      cmg::NamedObject(){
    }
    explicit TriggerObject(const value& cand, const std::string& name):
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(cand),
      cmg::NamedObject(name){
    }
    virtual ~TriggerObject(){
    }
    
   private:
    friend class TriggerObjectFactory;
};

}

#endif /*_AnalysisDataFormats_CMGTools_TriggerObject_H_*/
