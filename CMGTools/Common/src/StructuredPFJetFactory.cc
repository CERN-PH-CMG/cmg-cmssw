#include "CMGTools/Common/interface/StructuredPFJetFactory.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "TLorentzVector.h"

#include <iostream>

using namespace std;

cmg::StructuredPFJetFactory::StructuredPFJetFactory(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  subJetLabel_(ps.getParameter<edm::InputTag>("subjetCollection")),
  baseJetFactory_(ps.getParameter<edm::ParameterSet>("baseJetFactory")),
  pfJetFactory_(ps.getParameter<edm::ParameterSet>("pfJetFactory"))
{
}

cmg::StructuredPFJetFactory::event_ptr cmg::StructuredPFJetFactory::create(const edm::Event& iEvent, 
						       const edm::EventSetup&){
  
  edm::Handle<pat::JetCollection> jetCands;
  edm::Handle<reco::PFJetCollection> subJetCands;
  
  cmg::StructuredPFJetFactory::event_ptr result(new cmg::StructuredPFJetFactory::collection);
  iEvent.getByLabel(jetLabel_,jetCands);
  iEvent.getByLabel(subJetLabel_,subJetCands);
    
  long unsigned index = 0;
  for(pat::JetCollection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index ){
      
    // Fill PAT jet information of mother jets
    pat::JetPtr jetPtr( jetCands, index ); 
    cmg::StructuredPFJet structuredcmgjet( jetPtr );
    baseJetFactory_.set(jetPtr,&structuredcmgjet);

    // TODO: FILLING OF PF PROPERTIES OF THE MOTHER JET FROM SUM OF ALL DAUGHTER JETS STILL MISSING

    // Fill PF jet information of daughter jets
    std::vector<const reco::Candidate*> candPtrs = jetPtr->getJetConstituentsQuick();
    for(unsigned i=0; i<candPtrs.size(); ++i) {
       const pat::Jet& subjet = pat::Jet(*dynamic_cast<const reco::PFJet*>(&*(candPtrs[i])));
       cmg::PFJet cmgsubjet;
       cmgsubjet.setP4(subjet.p4());
       pfJetFactory_.setPFproperties(subjet,&cmgsubjet, false);
       structuredcmgjet.subJetCollection_.push_back(cmgsubjet);
    }

    result->push_back(structuredcmgjet);
  }

  return result;
}
