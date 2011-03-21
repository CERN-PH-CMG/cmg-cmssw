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

  template<class T>
  std::pair<bool,Measurement1D> getImpactParameter(const T &trk, reco::Vertex *vtx, const edm::EventSetup &iSetup, bool is3d=true)
    {
      edm::ESHandle<TransientTrackBuilder> trackBuilder;
      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
      reco::TransientTrack tt = trackBuilder->build(trk);
      if(is3d) return IPTools::absoluteImpactParameter3D(tt, *vtx);
      else     return IPTools::absoluteTransverseImpactParameter(tt, *vtx);
    }
  
  template<class T>
  reco::CandidatePtr getClosestVertexTo(const T *trk, std::vector<reco::CandidatePtr> &selVertices, const edm::EventSetup &iSetup, bool is3d=true)
    {

      reco::CandidatePtr bestvtx;
      double bestDz(1.0e7);
      for(std::vector<reco::CandidatePtr>::iterator vIt = selVertices.begin(); vIt != selVertices.end(); vIt++)
	{
	  const reco::Vertex *vtx = dynamic_cast<const reco::Vertex *>( vIt->get() );
	  double dz = abs(vtx->z() - trk->vz());
	  if( dz>bestDz ) continue;
	  bestvtx=*vIt;
	  bestDz=dz;
	}
      return bestvtx;
    }
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
  std::vector<reco::CandidatePtr> filter(std::vector<reco::CandidatePtr> &selLeptons, 
					 std::vector<reco::CandidatePtr> &selVtx, 
					 const edm::ParameterSet &iConfig,
					 const edm::EventSetup &iSetup);
}

namespace jet
{
  std::vector<reco::CandidatePtr> filter(edm::Handle<edm::View<reco::Candidate> > &hJet, std::vector<reco::CandidatePtr> &selLeptons, const edm::ParameterSet &iConfig);
  double fAssoc(const pat::Jet *jet, const reco::Vertex *vtx);
}



#endif
