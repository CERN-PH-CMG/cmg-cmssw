#include "CMGTools/SusyJetMET/plugins/SusyJetMETAnalyzer.h"

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

using namespace std;

cmg::SusyJetMETAnalyzer::SusyJetMETAnalyzer(const edm::ParameterSet& ps) {

}


void cmg::SusyJetMETAnalyzer::analyze(const edm::Event& iEvent, 
				      const edm::EventSetup& iSetup) {
  
  edm::Handle< pat::TriggerObjectStandAloneCollection > triggerObjects;
  iEvent.getByLabel("hltPrimitives", triggerObjects);

  typedef pat::TriggerObjectStandAloneCollection::const_iterator ITO;
  for(ITO ito = triggerObjects->begin(); ito!=triggerObjects->end(); ++ito) {
    const pat::TriggerObjectStandAlone& trigObj = *ito;

//     // printing 
//     cout<<"TO "<<trigObj.pt()<<" "<<trigObj.pdgId()<<endl;
//     printPaths(trigObj.pathNames());

  }

}


void cmg::SusyJetMETAnalyzer::printPaths( const std::vector<string>& paths ) {
  
  for (unsigned i=0; i<paths.size(); ++i ) {
    std::cout<<paths[i]<<std::endl;
  }
}
