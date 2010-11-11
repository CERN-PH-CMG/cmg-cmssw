#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

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
//   return 	x() == other.x() &&
//     y() == other.y() &&
//     z() == other.z() &&
//     t() == other.t();
  return p4() == other.p4() && charge() == other.charge();
}

std::ostream& cmg::AbstractPhysicsObject::printSelections(std::ostream& o) const{
  const std::vector<std::string> strings = selections.strings();
  for(std::vector<std::string>::const_iterator it = strings.begin();
      it != strings.end(); ++it){
    o << *it << "\t" << selections.test(*it) << std::endl;
  }
  return o;
}
