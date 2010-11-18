#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

const unsigned cmg::PFJet::NCANDTYPES=8;

cmg::PFJetComponent cmg::PFJet::dummy_;


const cmg::PFJetComponent& cmg::PFJet::component(unsigned ci) const {
  if(ci<1 || ci>=NCANDTYPES) return dummy_;
  else return components_[ci];
}
   
const int cmg::PFJet::nConstituents() const{
  int nConstituents = 0;
  for (unsigned ci = 1; ci < NCANDTYPES; ci++) nConstituents += component(ci).number();
  return nConstituents;

}
