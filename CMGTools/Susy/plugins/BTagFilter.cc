#ifndef _CMGTools_Susy_BTagFilter_H_
#define _CMGTools_Susy_BTagFilter_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"

#include <cmath>
#include <iostream>

class BTagFilter : public edm::EDFilter{
 public:

  BTagFilter(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")){
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    typedef edm::View<cmg::BaseJet> view;
    edm::Handle<view> handle;
    iEvent.getByLabel( src_, handle );

    int nCSVL = 0;
    int nCSVM = 0;

    for(view::const_iterator it = handle->begin(); it != handle->end(); ++it){
      const double csv = it->btag(6);
      if(csv >= 0.244){
	nCSVL++;
      }
      if(csv >= 0.679){
	nCSVM++;
      }
      if(nCSVM > 0) break;
    }
    return ( (nCSVL == 0) || (nCSVM > 0) ); 
  }

 private:
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(BTagFilter);

#endif

