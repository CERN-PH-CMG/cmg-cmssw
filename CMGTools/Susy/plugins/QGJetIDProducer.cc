#ifndef _CMGTools_Susy_QGJetIDProducer_H_
#define _CMGTools_Susy_QGJetIDProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include <cmath>
#include <iostream>
#include <vector>

class QGJetIDProducer : public edm::EDFilter{
 public:

  typedef std::vector<double> collection;
  
  QGJetIDProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")),
    useCharged_(ps.getParameter<bool>("useCharged")){
    produces<collection>("");
  }

  double girth(const cmg::PFJet::value& jet) const{
    //See http://arxiv.org/abs/1106.3076v2 - eqn (2)

    double gsum = 0.0;

    const std::vector<reco::PFCandidatePtr>& constituents = jet->getPFConstituents();
    for(std::vector<reco::PFCandidatePtr>::const_iterator it = constituents.begin(); it != constituents.end(); ++it){
      if(useCharged_ && (*it)->charge() == 0) continue;
      const double dr = reco::deltaR(**it,*jet);
      gsum += (dr*( (*it)->pt()/jet->pt() ) ); 
    }
    return gsum;

  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    typedef std::vector<cmg::PFJet> jet_collection;
    
    edm::Handle<jet_collection> jetHandle;
    iEvent.getByLabel(src_, jetHandle);

    std::auto_ptr<collection> output_girth( new collection() ); 
    for(jet_collection::const_iterator it = jetHandle->begin(); it != jetHandle->end(); ++it){
      
      const cmg::PFJet::value* patJet = it->sourcePtr();
      const double g = girth(*patJet);

      output_girth->push_back(g);

    }
    iEvent.put( output_girth );

    return true; 
  }

 private:
  edm::InputTag src_;
  const bool useCharged_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(QGJetIDProducer);

#endif
