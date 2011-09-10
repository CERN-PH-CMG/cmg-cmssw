/*
 *  See header file for a description of this class.
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Cerminara - CERN
 */

#include "CMGTools/HtoZZ2l2nu/plugins/LeptonsPassingPFSelection.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include <iostream>
using namespace std;

LeptonsPassingPFSelection::LeptonsPassingPFSelection(const edm::ParameterSet &iConfig) : muons_(iConfig.getParameter<edm::InputTag>("muons")),
											 pf_(iConfig.getParameter<edm::InputTag>("pf")) {
    produces<edm::RefToBaseVector<reco::Muon> >();    
    string cuts = iConfig.getParameter<std::string>("pfCut");
    cout << cuts << endl;
    pfCut_ = new   StringCutObjectSelector<pat::Muon>(cuts);
;
    
}



LeptonsPassingPFSelection::~LeptonsPassingPFSelection(){}


void 
LeptonsPassingPFSelection::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace edm; 

  Handle<View<reco::Muon> > muons;
  iEvent.getByLabel(muons_, muons);

  Handle<View<reco::Candidate> > pf;
  iEvent.getByLabel(pf_, pf);
    
  /// prepare the vector for the output
    std::auto_ptr<RefToBaseVector<reco::Muon> > out(new RefToBaseVector<reco::Muon>());

    //     View<reco::Candidate>::const_iterator pfit, pfbegin = pf->begin(), pfend = pf->end();
//     cout << "--------------------------------------------" << endl;
    /// Now loop
      for (size_t i = 0, n = muons->size(); i < n; ++i) {
	// read the edm reference to the muon
	RefToBase<reco::Muon> muonRef = muons->refAt(i);
      
	//iterate over the muons
	for(size_t ipfMu=0; ipfMu< pf.product()->size(); ++ipfMu) {

	  reco::CandidatePtr muonPtr = pf->ptrAt(ipfMu);
	  const pat::Muon *muonPF = dynamic_cast<const pat::Muon *>( muonPtr.get() );

	  //kinematics
// 	  cout << "PF muon:" << endl;
// 	  cout << "pt: " << muonPF->pt() << endl;
// 	  cout << "eta: " << muonPF->eta() << endl;
// 	  cout << "phi: " << muonPF->phi() << endl;
	  
// 	  cout << "probe muon:" << endl;
// 	  cout << "pt: " <<muonRef->pt() << endl;
// 	  cout << "eta: " << muonRef->eta() << endl;
// 	  cout << "phi: " << muonRef->phi() << endl;

	  if (!(*pfCut_)(*muonPF)) {
// 	    cout << "doesn't pass the cut" << endl;
	    continue;
	  }
	  

	  if (std::abs(muonRef->eta() - muonPF->eta()) < 1e-5 && 
	      std::abs(muonRef->phi() - muonPF->phi()) < 1e-5) {
// 	    cout << "matches!" << endl;
	    out->push_back(muonRef);
	    break;
	  }
        }

      }

    // Write the output to the event
    iEvent.put(out);
}

/// Register this as a CMSSW Plugin
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(LeptonsPassingPFSelection);
