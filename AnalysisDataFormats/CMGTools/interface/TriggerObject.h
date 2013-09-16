#ifndef _AnalysisDataFormats_CMGTools_TriggerObject_H_
#define _AnalysisDataFormats_CMGTools_TriggerObject_H_

#include "AnalysisDataFormats/CMGTools/interface/NamedObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

namespace cmg {

class TriggerObjectFactory;

/**
 * Store the 4-vector of the object that fired the trigger
 */
class TriggerObject : public PhysicsObjectWithPtr<pat::TriggerObjectPtr>, public cmg::NamedObject{

   public:
    typedef std::map<pat::strbitset::index_type::size_t,int> prescaleMap;
   
    explicit TriggerObject():
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(),
      cmg::NamedObject(),
      minPrescale_(-UnSet(int)){
    }
    explicit TriggerObject(const value& cand, const std::string& name):
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(cand),
      cmg::NamedObject(name),
      minPrescale_(-UnSet(int)){
    }
    virtual ~TriggerObject(){
    }
    
    int getPrescale(const std::string& name) const;
    int getPrescale(const char* s) const{
        return getPrescale(std::string(s));
    }

     bool getDecisionWithMaxPrescaleRegExp(const TRegexp&,const  int ) const; 
     bool getDecisionWithMaxPrescaleRegExp(const std::string s,const  int prescale ) const { 
       return getDecisionWithMaxPrescaleRegExp(TRegexp(s),prescale ); 
     } 

    int getMinimumPrescale() const{
        return minPrescale_;  
    }
   protected:
        int getIndex(const std::string& name) const;
        void addSelectionWithPrescale(const std::string& name, const bool value, const int prescale);

   private:
    friend class TriggerObjectFactory;
    friend class L1TriggerObjectFactory;
    prescaleMap prescales_;
    int minPrescale_;
};

}

#endif /*_AnalysisDataFormats_CMGTools_TriggerObject_H_*/
