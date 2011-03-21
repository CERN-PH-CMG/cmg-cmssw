#ifndef _objectfilters_h_
#define _objectfilters_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "DataFormats/PatCandidates/interface/MET.h"

namespace vertex
{
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hVtx, const edm::ParameterSet &iConfig);
  std::pair<bool,Measurement1D> getImpactParameter(reco::TransientTrack &tt, reco::Vertex *vtx, bool is3d=false);
}

namespace muon
{
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hMu, const edm::ParameterSet &iConfig);
}

namespace electron
{
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hEle, edm::Handle<edm::View<reco::Candidate> > &hMu, const edm::ParameterSet &iConfig);
}

namespace dilepton
{
  std::vector<reco::CandidatePtr> filter(std::vector<reco::CandidatePtr> &selMuons, 
					 std::vector<reco::CandidatePtr> &selElectrons, 
					 std::vector<reco::CandidatePtr> &selVtx, 
					 const edm::ParameterSet &iConfig);
}

namespace jet
{
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hJet, std::vector<reco::CandidatePtr> &selLeptons, const edm::ParameterSet &iConfig);
  double fAssoc(const pat::Jet *jet, const reco::Vertex *vtx);
}



#endif
