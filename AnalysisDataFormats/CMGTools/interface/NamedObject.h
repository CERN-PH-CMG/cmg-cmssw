#ifndef _AnalysisDataFormats_CMGTools_NamedObject_h_
#define _AnalysisDataFormats_CMGTools_NamedObject_h_

#include <string>

namespace cmg{

/**
 * A mixin class for objects that need a name.
 */
class NamedObject{
 
 public:

  NamedObject(const std::string& n):
    name_(n){
  }
  NamedObject():
    name_(""){
  }
  NamedObject(const NamedObject& other):
    name_(other.name_){
  }

  std::string name() const{
      return name_;
  }
 
 protected:

  std::string name_;
    
};

}

#endif /*_AnalysisDataFormats_CMGTools_NamedObject_h_*/
