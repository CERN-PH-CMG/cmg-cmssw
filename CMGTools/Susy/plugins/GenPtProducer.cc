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

    reco::LeafCandidate::LorentzVector result_glu;
    reco::LeafCandidate::Charge charge_glu = 0;
    int count_glu = 0;

    reco::LeafCandidate::LorentzVector result_st;
    reco::LeafCandidate::Charge charge_st = 0;
    int count_st = 0;

    for(view::const_iterator it = handle->begin(); it != handle->end(); ++it){
      if(it->status() != 3){
	continue;
      }
      const int id = it->pdgId();
      if(abs(id) == 6){
	result += it->p4();
	charge += it->charge();
	count++; 
      }else if(abs(id) == 1000021){
	result_glu += it->p4();
        charge_glu += it->charge();
        count_glu++;
      }else if(abs(id) == 1000006){
        result_st += it->p4();
        charge_st += it->charge();
        count_st++;
      }
    }

    reco::LeafCandidate cand(charge,result);
    cand.setPdgId(count);

    reco::LeafCandidate cand_glu(charge_glu,result_glu);
    cand_glu.setPdgId(count_glu);

    reco::LeafCandidate cand_st(charge_st,result_st);
    cand_st.setPdgId(count_st);

    std::auto_ptr<std::vector<reco::LeafCandidate> > output( new std::vector<reco::LeafCandidate>() ); 
    output->push_back(cand);
    output->push_back(cand_st);
    output->push_back(cand_glu);
    iEvent.put( output );

    return true; 
  }

 private:
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenPtProducer);

#endif
