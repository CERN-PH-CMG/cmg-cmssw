#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

using namespace std;

namespace trigger
{
  //
  bool isTriggerCandidate(const pat::Muon *muon, const edm::ParameterSet &iConfig)
  {

    //trigger paths and objects
    std::vector<std::string> triggerPaths = iConfig.getParameter<std::vector<std::string> >("triggerPaths");

    const pat::TriggerObjectStandAloneCollection &coll = muon->triggerObjectMatches();
    cout << muon->pt() << " | " << coll.size() ;
    for(pat::TriggerObjectStandAloneCollection::const_iterator objIt = coll.begin();
	objIt != coll.end(); objIt++)
      {
	std::vector< std::string > pathNames = objIt->filterLabels();
	for(std::vector< std::string >::iterator pIt = pathNames.begin(); pIt != pathNames.end(); pIt++)
	  cout << *pIt << " ";
      }
    cout << endl;

    return false;
  }
  
}
