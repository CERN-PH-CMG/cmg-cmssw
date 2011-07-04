#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

using namespace std;

namespace trigger
{
  //
  void getLeadDileptonTriggerCandidates(edm::Event &iEvent, const edm::ParameterSet &iConfig, std::vector<const reco::Candidate *> &muons, std::vector<const reco::Candidate *> &electrons)
  {
    // PAT trigger helper for trigger matching information
    const pat::helper::TriggerMatchHelper matchHelper;
    
    // PAT trigger event
    edm::Handle< pat::TriggerEvent > triggerEvent;
    iEvent.getByLabel( "patTriggerEvent", triggerEvent );
    const pat::TriggerPathCollection *allpaths=triggerEvent->paths();
    for(pat::TriggerPathCollection::const_iterator pit=allpaths->begin(); pit!= allpaths->end(); pit++)
      {
	cout << pit->name() << endl;
      }
    cout << "@@@@@@@@@" << endl;

    // get the trigger objects corresponding to the used matching (HLT muons)
    std::string muonMatch="muTriggerMatchPFlow";
    const reco::Candidate *leadMu=0, *subleadMu=0;
    const pat::TriggerObjectRefVector muTrigRefs( triggerEvent->objects( trigger::TriggerMuon ) );
    for ( pat::TriggerObjectRefVector::const_iterator iTrig = muTrigRefs.begin(); iTrig != muTrigRefs.end(); ++iTrig ) 
      {
	if((*iTrig)->collection().find("hltL3MuonCandidates")==std::string::npos) continue;
	const reco::CandidateBaseRefVector candRefs( matchHelper.triggerMatchCandidates( ( *iTrig), muonMatch, iEvent, *triggerEvent ) );
	if ( candRefs.empty() ) continue;
	for(size_t icands=0; icands<candRefs.size(); icands++)
	  {
	    reco::CandidateBaseRef muonRef( candRefs.at( icands ) );
	    
	    if ( muonRef.isAvailable() && muonRef.isNonnull() ) 
 	      {
		if(leadMu==0) leadMu = muonRef.get();
		else if(subleadMu==0) subleadMu = muonRef.get();
		else if( leadMu->pt() < muonRef->pt() )
		  {
		    subleadMu = leadMu;
		    leadMu = muonRef.get();
		  }
		else if( subleadMu->pt() < muonRef->pt() )
		  {
		    subleadMu = muonRef.get();
		  }
	      }
	  }
      }

    if(leadMu)  muons.push_back(leadMu);
    if(subleadMu) muons.push_back(subleadMu);
   
    // get the trigger objects corresponding to the used matching (HLT electrons) 
    std::string eleMatch="eleTriggerMatchPFlow";
    const reco::Candidate *leadEle=0, *subleadEle=0;
    const pat::TriggerObjectRefVector eleTrigRefs( triggerEvent->objects( trigger::TriggerElectron ) );
    for ( pat::TriggerObjectRefVector::const_iterator iTrig = eleTrigRefs.begin(); iTrig != eleTrigRefs.end(); ++iTrig ) 
      {
	cout << (*iTrig)->collection() << endl;
	const reco::CandidateBaseRefVector candRefs( matchHelper.triggerMatchCandidates( ( *iTrig), eleMatch, iEvent, *triggerEvent ) );
	if ( candRefs.empty() ) continue;
	for(size_t icands=0; icands<candRefs.size(); icands++)
          {
	    continue;
	    reco::CandidateBaseRef electronRef( candRefs.at( icands ) );
	    if ( electronRef.isAvailable() && electronRef.isNonnull() ) 
	      {
		if(leadEle==0) leadEle = electronRef.get();
		else if(subleadEle==0) subleadEle = electronRef.get();
		else if( leadEle->pt() < electronRef->pt() )
		  {
		    subleadEle = leadEle;
		    leadEle = electronRef.get();
		  }
		else if( subleadEle->pt() < electronRef->pt() )
		  {
		    subleadEle = electronRef.get();
		  }
	      }
	  }
      }
    if(leadEle) electrons.push_back(leadEle);
    if(subleadEle) electrons.push_back(subleadEle);
  }
}
