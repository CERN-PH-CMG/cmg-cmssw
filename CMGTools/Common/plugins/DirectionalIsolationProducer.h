#ifndef _CMGTools_Susy_DirectionalIsolationProducer_H_
#define _CMGTools_Susy_DirectionalIsolationProducer_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "Muon/MuonAnalysisTools/interface/MuonMVAEstimator.h"

#include "TMath.h"
#include "Math/VectorUtil.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "CMGTools/Common/interface/SAKLooseLepton.h"

template <class Lepton>
class DirectionalIsolationProducer : public edm::EDProducer{
 public:
  
  typedef std::vector<Lepton> collection;
 
  DirectionalIsolationProducer(const edm::ParameterSet& ps):
  src_(ps.getParameter<edm::InputTag>("src")),
    pfCands_(ps.getParameter<edm::InputTag>("pfCands")),
    vertexCollection_(ps.getParameter<edm::InputTag>("vertexCollection")){
    produces<collection>();
  }

  bool isLooseLepton(const Lepton& lepton, const math::XYZPoint & primaryVertex, const reco::PFCandidateCollection & pfCandidates) const;

  virtual void produce(edm::Event& iEvent, const edm::EventSetup&){
    
    //the leptons
    edm::Handle<collection> handle;
    iEvent.getByLabel( src_, handle );

    //the pf candidates
    edm::Handle<reco::PFCandidateCollection> pfhandle;
    iEvent.getByLabel( pfCands_, pfhandle );
    const reco::PFCandidateCollection* pf = pfhandle.product();

    //the vertex to use
    edm::Handle<reco::VertexCollection> vhandle;
    iEvent.getByLabel( vertexCollection_, vhandle );

    std::auto_ptr<collection> result(new collection);
    if(vhandle->size()){
      const reco::Vertex::Point& leadingVertex = vhandle->at(0).position(); 
      for(typename collection::const_iterator it = handle->begin(); it != handle->end(); ++it){
	bool selected = false;
	if(it->isMuon()){
	  selected = isLooseLepton(*it, leadingVertex, *pf);
	}else if(it->isElectron()){
	  selected = isLooseLepton(*it, leadingVertex, *pf);
	}
	
	Lepton lep = *it;
	float f_selected = selected;
	lep.addUserFloat(std::string("isLooseLeptonSAK"),f_selected);
	result->push_back(lep);
      }
    }
    iEvent.put( result );
  }

 private:
  edm::InputTag src_;
  edm::InputTag pfCands_;
  edm::InputTag vertexCollection_;

};

template <>
bool DirectionalIsolationProducer<pat::Electron>::isLooseLepton(const pat::Electron& lepton, const math::XYZPoint& primaryVertex, const reco::PFCandidateCollection & pfCandidates) const{
  return isLooseElectron(lepton, primaryVertex, pfCandidates);
}

template <>
bool DirectionalIsolationProducer<pat::Muon>::isLooseLepton(const pat::Muon& lepton, const math::XYZPoint& primaryVertex, const reco::PFCandidateCollection & pfCandidates) const{
  return isLooseMuon(lepton, primaryVertex, pfCandidates);
}


#endif
