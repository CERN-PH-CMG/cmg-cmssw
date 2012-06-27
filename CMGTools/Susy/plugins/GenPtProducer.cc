#ifndef _CMGTools_Susy_GenPtProducer_H_
#define _CMGTools_Susy_GenPtProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include <cmath>
#include <iostream>
#include <vector>

class GenPtProducer : public edm::EDFilter{
 public:

  GenPtProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")){
    produces<std::vector<reco::LeafCandidate> >();
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    typedef edm::View<reco::Candidate> view;
    edm::Handle<view> handle;
    iEvent.getByLabel( src_, handle );
  
    reco::LeafCandidate::LorentzVector result;
    reco::LeafCandidate::Charge charge = 0;
    int count = 0;

    for(view::const_iterator it = handle->begin(); it != handle->end(); ++it){
      if(it->status() != 3){
	continue;
      }
      const int id = it->pdgId();
      if(abs(id) == 6){
	result += it->p4();
	charge += it->charge();
	count++; 
      }
    }

    reco::LeafCandidate cand(charge,result);
    cand.setPdgId(count);

    std::auto_ptr<std::vector<reco::LeafCandidate> > output( new std::vector<reco::LeafCandidate>() ); 
    output->push_back(cand);
    iEvent.put( output );

    return true; 
  }

 private:
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenPtProducer);

#endif
