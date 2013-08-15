#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "TString.h"

std::ostream& operator<<(std::ostream& o, const cmg::AbstractPhysicsObject& a){
  return a.printOn(o);
}

std::ostream& cmg::AbstractPhysicsObject::printOn(std::ostream& o) const{
  o << "pt/eta/phi/mass: " << pt() << "/" << eta() << "/" << phi() << "/" << mass();
  return o; 
}
bool cmg::AbstractPhysicsObject::operator<(const AbstractPhysicsObject& other) const{
  return pt() < other.pt();
}
bool cmg::AbstractPhysicsObject::operator==(const AbstractPhysicsObject& other) const{
  return (p4() == other.p4()) && (charge() == other.charge())  &&  (pdgId() == other.pdgId());
}

std::ostream& cmg::AbstractPhysicsObject::printSelections(std::ostream& o, const std::string indent) const{
  const std::vector<std::string> strings = selections.strings();
  for(std::vector<std::string>::const_iterator it = strings.begin();
      it != strings.end(); ++it){
    o << indent << *it << "\t" << selections.test(*it) << std::endl;
  }
  return o;
}

/// Itterate over the daughters and return a list, excluding the parent
cmg::AbstractPhysicsObject::Daughters cmg::AbstractPhysicsObject::getDaughters(const bool include_this) const{
    
    class DaughterVisitor : public cmg::AbstractPhysicsObject::AbstractPhysicsObjectVisitor{
    public:
        DaughterVisitor(cmg::AbstractPhysicsObject const * const parent = 0):
            parent_(parent){
        }
        virtual void visit(AbstractPhysicsObject const * o){
            //Compare the addresses of the daughter and parent
            if(parent_ && (parent_ == o) ) return;
            daughters_.push_back(o);
        }
        cmg::AbstractPhysicsObject::Daughters daughters() const{
            return daughters_;  
        }
      private:
        cmg::AbstractPhysicsObject const * const parent_;
        cmg::AbstractPhysicsObject::Daughters daughters_;
    };
    
    cmg::AbstractPhysicsObject const * parent = 0;
    if( !include_this ){
        parent = this;  
    }
    DaughterVisitor dv(parent);
    accept(&dv);
    return dv.daughters();
    
}

bool cmg::AbstractPhysicsObject::getSelectionRegExp(const TRegexp& re) const{
    
    bool result = false;
    
    const cmg::AbstractPhysicsObject::Strings names = getSelectionNames();
    for(cmg::AbstractPhysicsObject::Strings::const_iterator s = names.begin(); s != names.end(); ++s){
        if(getSelection(*s)){
            const TString t(s->c_str());
            if(t.Index(re) >= 0){
                result = true;
                break;
            }
        }
    }
    return result;
}
