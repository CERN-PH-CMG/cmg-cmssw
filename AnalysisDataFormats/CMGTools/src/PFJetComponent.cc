#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"

cmg::PFJetComponent::PFJetComponent() : 
  number_(-1), fraction_(-1), energy_(-1), pt_(-1) {}


void cmg::PFJetComponent::clear() {
  number_ = -1;
  fraction_ = -1;
  energy_ = -1;
  pt_ = -1;
}

std::ostream& cmg::operator<<(std::ostream& out, const cmg::PFJetComponent& comp) {
  if(!out) return out;
  
  out<<"N,f,E,pT = "
     <<comp.number()<<", "
     <<comp.fraction()<<", "
     <<comp.energy()<<", "
     <<comp.pt();
  return out;
}
