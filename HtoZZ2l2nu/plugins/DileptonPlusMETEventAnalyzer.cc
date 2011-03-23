#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/EventHypothesis.h"
#include "DataFormats/PatCandidates/interface/EventHypothesisLooper.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "CMGTools/HtoZZ2l2nu/plugins/ObjectFilters.h"

//
class DileptonPlusMETEventAnalyzer : public edm::EDAnalyzer 
{
public:
  DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig) ;
  virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  void runTests( const pat::EventHypothesis &h) ;

private:
  edm::InputTag source_;
};


//
DileptonPlusMETEventAnalyzer::DileptonPlusMETEventAnalyzer(const edm::ParameterSet &iConfig) :
  source_(iConfig.getParameter<edm::InputTag>("source"))
{
}

//
void DileptonPlusMETEventAnalyzer::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
  using namespace edm; using namespace std;
  using reco::Candidate; 
  using reco::CandidatePtr;
  
  //retrieve the event hypothesis
  Handle<vector<pat::EventHypothesis> > hyps;
  edm::InputTag selEvtTag(source_.label()+":selectedEvent");
  iEvent.getByLabel(selEvtTag, hyps);

  //retrieve the selection path
  Handle<int> selectionPath;
  edm::InputTag selPathTag(source_.label()+":selectionPath");
  iEvent.getByLabel(selPathTag, selectionPath);

  //retrieve the selection step
  Handle<int> selectionStep;
  edm::InputTag selStepTag(source_.label()+":selectionStep");
  iEvent.getByLabel(selStepTag, selectionStep);

  //retrieve the selected vertex
  Handle<reco::VertexCollection> selectedVertex;
  edm::InputTag selVtxTag(source_.label()+":selectedVertices");
  iEvent.getByLabel(selVtxTag, selectedVertex);

  if(hyps->size()==0) return;
  const pat::EventHypothesis &h = (*hyps)[0];

  //dump selected event 
  cout << "Retrieve an event hypothesis selected at step at step=" << *selectionStep  << " for path " << *selectionPath 
       << " with " << selectedVertex->size() << " vertices" << std::endl;

  if(*selectionPath>0)
    {
      CandidatePtr lep1 = h["leg1"];
      CandidatePtr lep2 = h["leg2"];
      cout << "\t dilepton leg1: " << lep1->pt() << ";" << lep1->eta() << ";" << lep1->phi() << endl
	   << "\t          leg2: " << lep2->pt() << ";" << lep2->eta() << ";" << lep2->phi() << endl;
    }
  else
    {
      cout << "\t no dilepton has been selected" << std::endl;
    }

  const pat::MET *met = h.getAs<pat::MET>("met");
  cout << "\t met:" << met->pt() << ";" << met->phi() << endl;

  for (pat::eventhypothesis::Looper<pat::Electron> elec = h.loopAs<pat::Electron>("electron"); elec; ++elec) {
    cout << "\t e: " << elec->pt() << ";" << elec->eta() << ";" << elec->phi() << std::endl;
  }

  for (pat::eventhypothesis::Looper<pat::Muon> muon = h.loopAs<pat::Muon>("muon"); muon; ++muon) {
    cout << "\t mu: " << muon->pt() << ";" << muon->eta() << ";" << muon->phi() << std::endl;
  }

  for (pat::eventhypothesis::Looper<pat::Jet> jet = h.loopAs<pat::Jet>("jet"); jet; ++jet) {
    cout << "\t jet: " << jet->pt() << ";" << jet->eta() << ";" << jet->phi() << std::endl;
  }
  
}

DEFINE_FWK_MODULE(DileptonPlusMETEventAnalyzer);


 
