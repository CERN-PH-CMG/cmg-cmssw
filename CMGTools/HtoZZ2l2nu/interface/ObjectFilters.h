#ifndef objectfilters_h
#define objectfilters_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TVector3.h"
#include "TH1D.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;
typedef std::pair<reco::CandidatePtr, reco::VertexRef> CandidateWithVertex;
typedef std::vector<reco::CandidatePtr> CandidateCollection;
typedef std::vector<CandidateWithVertex> CandidateWithVertexCollection;
typedef std::pair<reco::CandidateBaseRef,std::string> TriggerCandidate;
typedef std::vector<TriggerCandidate> TriggerCandidateCollection;

namespace trigger
{
  void getLeadDileptonTriggerCandidates(edm::Event &iEvent, const edm::ParameterSet &iConfig, 
					std::vector<const reco::Candidate *> &muons, 
					std::vector<const reco::Candidate *> &electrons);
}

namespace gen
{
  /**
     @short wrapper to access generator level event (aka MC truth)
   */
  std::map<std::string, std::vector<reco::CandidatePtr> > filter(edm::Handle<edm::View<reco::Candidate> > &hGen, const edm::ParameterSet &iConfig);
  const reco::Candidate *getFinalStateFor(const reco::Candidate *p);

  enum DYchannels {HIGGS, DIL_OTHER, DY_MUMU, DY_EE, DY_TAUTAU };
  int assignDileptonChannel(edm::Handle<edm::View<reco::Candidate> > &genParticles);
}

namespace vertex
{
  /**
     @short wrapper for the selection of vertices
   */
  std::vector<reco::VertexRef> filter(edm::Handle<reco::VertexCollection> &hVtx, const edm::ParameterSet &iConfig);
  float getVertexMomentumFlux(const reco::Vertex *vtx, float minWeight=0.5);

  /**
     @short wrappers for the handling of single track related quantities
   */
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
  reco::VertexRef getClosestVertexTo(const T *trk, std::vector<reco::VertexRef> &selVertices)
    {
      reco::VertexRef bestvtx;
      double bestDz(1.0e7);
      for(std::vector<reco::VertexRef>::iterator vIt = selVertices.begin(); vIt != selVertices.end(); vIt++)
	{
	  double dz = fabs( trk->dz( vIt->get()->position() ) );
	  if( dz>bestDz ) continue;
	  bestvtx=*vIt;
	  bestDz=dz;
	}
      return bestvtx;
    }
}


namespace photon
{
  std::pair<std::string,double> getPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH,
                                                       const edm::TriggerNames &triggerNames,
                                                       std::vector<std::string> &gammaTriggers);
  
  CandidateCollection filter(edm::Handle<edm::View<reco::Candidate> > &hPhoton, 
			     EcalClusterLazyTools &lazyTool,
			     edm::Handle<EcalRecHitCollection> ebrechits,
			     const edm::ParameterSet &iConfig,
			     double minEt=0.);
}


namespace muon
{
  /**
     @short muon selector (associates a primary vertex
   */
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hMu, 
				       std::vector<reco::VertexRef> &goodVertices, 
				       const reco::BeamSpot &theBeamSpot,
				       const edm::ParameterSet &iConfig);
}

namespace electron
{
  /**
     @short electron selector (cross cleans agains muons and associates a primary vertex)
   */
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hEle, 
				       edm::Handle<edm::View<reco::Candidate> > &hMu, 
				       std::vector<reco::VertexRef> &goodVertices, 
				       const reco::BeamSpot &theBeamSpot,
				       const edm::ParameterSet &iConfig);
}

namespace lepton
{
  /**
     @short wrappers for common operations with leptons
   */
  enum {ELECTRON=11, MUON=13};
  enum IsolType { ECAL_ISO=0, HCAL_ISO, TRACKER_ISO, REL_ISO};
  int getLeptonId(reco::CandidatePtr &lepton);
  double getPtErrorFor(reco::CandidatePtr &lepton);
  std::vector<double> getLeptonIso(reco::CandidatePtr &lepton, float minRelNorm=20, float puOffsetCorrection=0);
  const reco::GenParticle *getLeptonGenMatch(reco::CandidatePtr &lepton);
}

namespace dilepton
{
  /**
     @short selects the dileptons
   */
  enum DileptonClassification {UNKNOWN=0,MUMU=1,EE=2,EMU=3,ETAU=4,MUTAU=5, GAMMA=22};
  std::pair<CandidateWithVertex,CandidateWithVertex> filter(CandidateWithVertexCollection &selLeptons, 
							    const edm::ParameterSet &iConfig,
							    const edm::EventSetup &iSetup);
  int classify(std::pair<CandidateWithVertex,CandidateWithVertex> &selDilepton);
}

namespace jet
{
  /**
     @short selects jets and associates the vertex (from the vertex with highest betaAssoc value)
   */
  CandidateWithVertexCollection filter(edm::Handle<edm::View<reco::Candidate> > &hJet, 
				       CandidateWithVertexCollection &selLeptons, 
				       std::vector<reco::VertexRef> &goodVertices,
				       const edm::ParameterSet &iConfig);
  void classifyJetsForDileptonEvent(CandidateWithVertexCollection &selJets, 
				    std::pair<CandidateWithVertex,CandidateWithVertex> &dilepton,
				    CandidateWithVertexCollection &assocJets, 
				    CandidateWithVertexCollection &puJets,
				    double maxDz=0.1);
  double fAssoc(const pat::Jet *jet, const reco::Vertex *vtx);
}

namespace met
{
  /**
     @short returns standard MET, corrected for all the selected jets and corrected for all the PU jets
  */
  enum MetTypes { RAWMET=0, TYPEIMET, CORRECTED_TYPEIMET };
  LorentzVectorCollection filter(LorentzVector &pfMET, std::vector<const pat::Jet *> &assocJets, std::vector<const pat::Jet *> &puJets, bool isRaw=false);

}



#endif
