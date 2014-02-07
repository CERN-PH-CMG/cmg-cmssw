// -*- C++ -*-
//
// Package:    PATMuonTrackVetoSelector
// Class:      PATMuonTrackVetoSelector
// 
/**\class PATMuonTrackVetoSelector PATMuonTrackVetoSelector.cc UWAnalysis/PATMuonTrackVetoSelector/src/PATMuonTrackVetoSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Michail Bachtis
//         Created:  Sun Jan 31 15:04:57 CST 2010
// $Id: PATPFMuonEmbedder.cc,v 1.1 2012/05/06 08:57:26 cbern Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "Math/GenVector/VectorUtil.h"

#include "FWCore/Framework/interface/MakerMacros.h"


//
// class decleration



class PATPFMuonEmbedder : public edm::EDProducer {
   public:

  

  explicit PATPFMuonEmbedder(const edm::ParameterSet& iConfig):
    src_(iConfig.getParameter<edm::InputTag>("src")),
    ref_(iConfig.getParameter<edm::InputTag>("srcPF"))
     {
       produces<pat::MuonCollection>();
     }

  ~PATPFMuonEmbedder() {}
   private:



  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    using namespace edm;
    using namespace reco;
    
    Handle<reco::PFCandidateCollection> ref;
    iEvent.getByLabel(ref_,ref);
    std::auto_ptr<pat::MuonCollection > out(new pat::MuonCollection);

    Handle<pat::MuonCollection > cands;
    if(iEvent.getByLabel(src_,cands)) 
      for(unsigned int  i=0;i!=cands->size();++i){
	pat::Muon muon = cands->at(i);
	edm::Ptr<reco::Candidate> muonref = cands->at(i).originalObjectRef();
	
	bool isPF = false;
	for (reco::PFCandidateCollection::const_iterator it =
	       ref->begin(), ed = ref->end(); it != ed; ++it) {
	  const reco::MuonRef mref = it->muonRef();
	  if (mref.isNonnull() && abs(it->pdgId()) == 13) {
	    if (muonref.key() == mref.key()) {
	      isPF = true; break;
	    }
	  }
	}
	
	if(!isPF)
	  muon.addUserFloat("isPFMuon",0.0);
	else
	  muon.addUserFloat("isPFMuon",1.0);

	out->push_back(muon);
      }
    iEvent.put(out);
  }
     

      // ----------member data ---------------------------
      edm::InputTag src_;
      edm::InputTag ref_;

};

DEFINE_FWK_MODULE(PATPFMuonEmbedder);

