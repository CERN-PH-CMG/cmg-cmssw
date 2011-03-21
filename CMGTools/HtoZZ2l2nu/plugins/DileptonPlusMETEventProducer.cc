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

#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

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
  using namespace std;
  using namespace edm;
  using reco::Candidate; 
  using reco::CandidatePtr;
  
  auto_ptr<pat::EventHypothesis> hyp(new pat::EventHypothesis());
  auto_ptr<int> selectionPath(new int);
  auto_ptr<int> selectionStep(new int);
  
  *selectionStep=0;

  //pre-select vertices
  Handle<View<Candidate> > hVtx;
  iEvent.getByLabel(objConfig["Dileptons"].getParameter<edm::InputTag>("vtxsource"), hVtx);  
  std::vector<CandidatePtr> selVertices = vertex::filter(hVtx,objConfig["Vertices"]);
  if(selVertices.size()>0) *selectionStep=1;

  //select muons
  Handle<View<Candidate> > hMu; 
  iEvent.getByLabel(objConfig["Muons"].getParameter<edm::InputTag>("source"), hMu);
  std::vector<CandidatePtr> selMuons = muon::filter(hMu, objConfig["Muons"]);
  
  //select electrons
  Handle<View<Candidate> > hEle; 
  iEvent.getByLabel(objConfig["Electrons"].getParameter<edm::InputTag>("source"), hEle);
  std::vector<CandidatePtr> selElectrons = electron::filter(hEle, hMu, objConfig["Electrons"]);

  //build inclusive collection
  std::vector<CandidatePtr> selLeptons = selMuons;
  selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
  if(selLeptons.size()>0) *selectionStep=2;
  
  //build the dilepton
  std::vector<CandidatePtr> dilepton = dilepton::filter(selLeptons,selVertices,objConfig["Dileptons"],iSetup);
  if(dilepton.size())
    {
      hyp->add(dilepton[0],"vertex");
      hyp->add(dilepton[1],"leg1");
      hyp->add(dilepton[2],"leg2");
      //add the remaining leptons
      for(std::vector<CandidatePtr>::iterator mIt = selMuons.begin(); mIt != selMuons.end(); mIt++)
	{
	  if(mIt->get()== dilepton[1].get() || mIt->get() == dilepton[2].get()) continue;
	  hyp->add(*mIt,"muon");
	}
      for(std::vector<CandidatePtr>::iterator eIt = selElectrons.begin(); eIt != selElectrons.end(); eIt++)
	{
	  if(eIt->get()== dilepton[1].get() || eIt->get() == dilepton[2].get()) continue;
	  hyp->add(*eIt,"electron");
	}
    }
  if(dilepton.size()==3) *selectionStep=3;

  //add the jets
  Handle<View<Candidate> > hJet; 
  iEvent.getByLabel(objConfig["Jets"].getParameter<edm::InputTag>("source"), hJet);
  std::vector<CandidatePtr> selJets = jet::filter(hJet, selLeptons, objConfig["Jets"]);
  for(std::vector<CandidatePtr>::iterator jIt = selJets.begin(); jIt != selJets.end(); jIt++) hyp->add(*jIt,"jet");

  //add the met
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
