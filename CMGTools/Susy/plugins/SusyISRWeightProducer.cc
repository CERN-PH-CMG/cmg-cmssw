#ifndef _CMGTools_Susy_SusyISRWeightProducer_H_
#define _CMGTools_Susy_SusyISRWeightProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include <cmath>
#include <iostream>
#include <vector>

class SusyISRWeightProducer : public edm::EDFilter{
 public:

  SusyISRWeightProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")){
    produces<std::vector< double  > >();
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    typedef edm::View<reco::Candidate> view;
    edm::Handle<view> handle;
    iEvent.getByLabel( src_, handle );
  
    reco::LeafCandidate::LorentzVector result;
   
    for(view::const_iterator it = handle->begin(); it != handle->end(); ++it){
      if(it->status() != 3){
	continue;
      }
      if(abs(it->pdgId()) == 1000006){
	result += it->p4();
      }
    }
    double pt_distop = result.pt();
    std::vector<double> weights;
   
    if (pt_distop <= 120.) {
      weights.push_back(1.00);
      weights.push_back(1.00);
      weights.push_back(1.00);
    }
    else if (pt_distop <= 150.){
      weights.push_back(1.00);
      weights.push_back(0.95);
      weights.push_back(0.90);
    }
    else if (pt_distop <= 250.){
      weights.push_back(1.00);
      weights.push_back(0.90);
      weights.push_back(0.80);
    }
    else {
      weights.push_back(1.00);
      weights.push_back(0.80);
      weights.push_back(0.60);
    }

   
    std::auto_ptr<std::vector <double > > output( new std::vector<  double  >() ); 
    output->push_back(weights[0]);
    output->push_back(weights[1]);
    output->push_back(weights[2]);
    iEvent.put( output );

    return true; 
  }

 private:
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyISRWeightProducer);

#endif
