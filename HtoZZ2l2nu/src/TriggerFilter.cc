#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

using namespace std;

namespace trigger
{
  //
  bool isTriggerCandidate(edm::Event &iEvent, const edm::ParameterSet &iConfig)
  {

    //trigger paths and objects
    std::vector<std::string> triggerPaths = iConfig.getParameter<std::vector<std::string> >("triggerPaths");

    // PAT trigger event
    edm::Handle< pat::TriggerEvent > triggerEvent;
    iEvent.getByLabel( "patTriggerEvent", triggerEvent );

    // PAT object collection
    edm::Handle< pat::MuonCollection > muons;
    iEvent.getByLabel( "selectedPatMuonsPFlow" , muons );

    // PAT trigger helper for trigger matching information
    const pat::helper::TriggerMatchHelper matchHelper;

    // loop over muon references (PAT muons have been used in the matcher in task 3)
    for( size_t iMuon=0; iMuon<muons->size(); ++iMuon)
      {
	const pat::TriggerObjectRef trigRef( matchHelper.triggerMatchObject( muons, iMuon, "muTriggerMatchPF", iEvent, *triggerEvent ) );
	if ( trigRef.isAvailable() )
	  {
	    //cout << "found for muon=" << iMuon << endl;
	  }
      }
    

    return false;
  }
  
}
