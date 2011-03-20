#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

class DileptonPlusMETEventProducer : public edm::EDProducer {
public:
  DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
private:
  std::map<std::string, edm::ParameterSet > objConfig;
};


using namespace std;


//
DileptonPlusMETEventProducer::DileptonPlusMETEventProducer(const edm::ParameterSet &iConfig)
{
    produces<pat::EventHypothesis>();
    produces<int>("selectionStep");
    produces<int>("selectionPath");
    std::string objs[]={"Electrons", "Muons", "Jets", "MET" };
    for(size_t iobj=0; iobj<sizeof(objs)/sizeof(string); iobj++)
      objConfig[ objs[iobj] ] = iConfig.getParameter<edm::ParameterSet>( objs[iobj] );
}

//
void DileptonPlusMETEventProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace edm;
  using namespace std;
  using reco::Candidate; 
  using reco::CandidatePtr;
  
  auto_ptr<pat::EventHypothesis> hyp(new pat::EventHypothesis());
  auto_ptr<int> selectionPath(new int);
  auto_ptr<int> selectionStep(new int);
  
  Handle<View<Candidate> > hMu; 
  iEvent.getByLabel(objConfig["Muons"].getParameter<edm::InputTag>("source"), hMu);
  //  std::vector<int> muSel = selectMuons(hMu);
  
  Handle<View<Candidate> > hEle; 
  iEvent.getByLabel(objConfig["Electrons"].getParameter<edm::InputTag>("source"), hEle);
  //  std::vector<int> eleSel = selectElectrons(hMu,hEle);
  
  Handle<View<Candidate> > hJet; 
  iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
  //  std::vector<int> jetSel = selectJets();
  
  Handle<View<Candidate> > hMET; 
  iEvent.getByLabel(objConfig["MET"].getParameter<edm::InputTag>("source"), hMET);
  CandidatePtr met = hMET->ptrAt(0);
  hyp->add(met, "met");

  // work done, save results
  iEvent.put(hyp);
  iEvent.put(selectionPath);
  iEvent.put(selectionStep);
}

//  
DEFINE_FWK_MODULE(DileptonPlusMETEventProducer);
