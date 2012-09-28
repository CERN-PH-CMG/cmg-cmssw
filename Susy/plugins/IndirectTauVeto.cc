#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "CMGTools/Susy/interface/IndirectTauVeto.h"
#include "CMGTools/Susy/plugins/IndirectTauVeto.h"

#include <cmath>
#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

typedef std::vector<cmg::PFJet> collection;

IndirectTauVetoProducer::IndirectTauVetoProducer(const edm::ParameterSet& iConfig):
  pfjetTag_(iConfig.getParameter<edm::InputTag>("jetTag")),
  metTag_(iConfig.getParameter<edm::InputTag>("metTag")){
  produces<collection>();
}
    
void IndirectTauVetoProducer::produce(edm::Event& iEvent, const edm::EventSetup&){
  
  std::auto_ptr<collection> result(new collection);

  edm::Handle<collection> jetHandle;
  iEvent.getByLabel(pfjetTag_, jetHandle);

  edm::Handle<std::vector<cmg::BaseMET> > metHandle;
  iEvent.getByLabel(metTag_, metHandle);

  const cmg::BaseMET met = metHandle->at(0);

  for(collection::const_iterator it = jetHandle->begin(); it != jetHandle->end(); ++it){
    
    const cmg::PFJet::value* patJet = it->sourcePtr();
    std::vector<const pat::Jet*> jets;
    jets.push_back( patJet->get() );
    const bool isTau = tauVeto(jets,met.pt(),met.phi());
    if(isTau){
      result->push_back(*it);
    }
  }
  iEvent.put(result);

}

DEFINE_FWK_MODULE(IndirectTauVetoProducer);
