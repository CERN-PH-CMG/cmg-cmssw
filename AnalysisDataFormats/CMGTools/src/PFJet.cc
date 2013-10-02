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

float cmg::PFJet::puMva(const std::string & name) const
{
	int index = puIdIndex(name);
	if( index == -1 ) return UnSet(float);
	return puMvas_.at(index);
}

int   cmg::PFJet::puId(const std::string & name) const
{
	int index = puIdIndex(name);
	if( index == -1 ) return UnSet(int);
	return puIds_.at(index);
}

int cmg::PFJet::puIdIndex(const std::string & name) const
{
	const PuIdNames::const_iterator index = std::find( puIdNames_.begin(), puIdNames_.end(), name);
	if( index == puIdNames_.end() ) { return -1; }
	return index - puIdNames_.begin();
}
