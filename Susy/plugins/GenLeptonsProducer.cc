#ifndef _CMGTools_Susy_GenLeptonsProducer_H_
#define _CMGTools_Susy_GenLeptonsProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <cmath>
#include <iostream>

class GenLeptonsProducer : public edm::EDFilter{
 public:

  GenLeptonsProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")){
    produces<int>();
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    typedef edm::View<reco::Candidate> view;
    edm::Handle<view> handle;
    iEvent.getByLabel( src_, handle );
  
    int nTop = 0;
    int nBot = 0;
    int nEle = 0;
    int nMuon = 0;
    int nTau = 0;

    for(view::const_iterator it = handle->begin(); it != handle->end(); ++it){
      if(it->status() != 3){
	continue;
      }
      const int id = it->pdgId();
      if(abs(id) == 6) nTop++;
      if(abs(id) == 5) nBot++;
      if(abs(id) == 11) nEle++;
      if(abs(id) == 13) nMuon++;
      if(abs(id) == 15) nTau++;
    }

    const int result = (nTop*10000) + (nBot*1000) + (nEle*100) + (nMuon*10) + nTau;
    //std::cout << "GenLeptonsProducer: " << result << std::endl;
    std::auto_ptr<int> output( new int(result) ); 
    iEvent.put( output );

    return true; 
  }

 private:
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenLeptonsProducer);

#endif
