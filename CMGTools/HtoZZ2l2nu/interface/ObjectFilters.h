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
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TVector3.h"
#include "TH1D.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

enum PhysicsChannels  { NOTDIL, DIL_OTHER, DY_MUMU, DY_EE, DY_TAUTAU, HIGGS };
enum PhysicsObjects   { MET=0,JET=1,ELECTRON=11, MUON=13,PHOTON=22};
enum DileptonChannels { UNKNOWN=0,MUMU=1,EE=2,EMU=3,ETAU=4,MUTAU=5, GAMMA=22};
enum IsolType         { ECAL_ISO=0, HCAL_ISO, TRACKER_ISO, REL_ISO, RELRHOCORR_ISO, N_ISO, C_ISO, CPU_ISO, G_ISO, PFREL_ISO, PFRELBETCORR_ISO};


///                            ///   
/// VERTEX SELECTION UTILITIES ///                                                                                                                                                                                                                                                                 
///                            /// 
std::vector<reco::VertexRef> getGoodVertices(edm::Handle<reco::VertexCollection> &hVtx, const edm::ParameterSet &iConfig);
float getVertexMomentumFlux(const reco::Vertex *vtx, float minWeight=0.5);

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


///                            ///   
/// LEPTON SELECTION UTILITIES ///
///                            ///   
std::vector<reco::CandidatePtr> getGoodMuons(edm::Handle<edm::View<reco::Candidate> > &hMu, const reco::VertexRef &primVertex, const double& rho, const edm::ParameterSet &iConfig);
std::vector<reco::CandidatePtr> getGoodElectrons(edm::Handle<edm::View<reco::Candidate> > &hEle,  edm::Handle<edm::View<reco::Candidate> > &hMu, const reco::VertexRef &primVertex, const double& rho, const edm::ParameterSet &iConfig);
std::vector<reco::CandidatePtr> getDileptonCandidate(std::vector<reco::CandidatePtr> &selLeptons,  const edm::ParameterSet &iConfig,  const edm::EventSetup &iSetup);
int getLeptonId(reco::CandidatePtr &lepton);
int getDileptonId(std::vector<reco::CandidatePtr> &dilepton);
double getLeptonPtError(reco::CandidatePtr &lepton);
std::vector<double> getLeptonIso(reco::CandidatePtr &lepton, float minRelNorm=20, float rho=0);
const reco::GenParticle *getLeptonGenMatch(reco::CandidatePtr &lepton);


///                            ///   
/// PHOTON SELECTION UTILITIES ///
///                            ///   
// cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/Vgamma2011PhotonID
std::vector<reco::CandidatePtr> getGoodPhotons(edm::Handle<edm::View<reco::Candidate> > &hPhoton, 
					       EcalClusterLazyTools &lazyTool, 
					       edm::Handle<EcalRecHitCollection> ebrechits,
					       double rho, const edm::ParameterSet &iConfig);
int getPhotonTrackVeto(const reco::Photon *pho,
		       edm::Handle<std::vector<reco::Track> > &ctfTracks, 
		       edm::Handle<std::vector<reco::Track> > &gsfTracks,
		       edm::Handle<edm::View<reco::Candidate> > &ele);

///                            ///   
/// JET SELECTION UTILITIES    ///
///                            ///   
std::vector<reco::CandidatePtr> getGoodJets(edm::Handle<edm::View<reco::Candidate> > &hJet, std::vector<reco::CandidatePtr> &selPhysicsObjects, const edm::ParameterSet &iConfig);
double computeVtxAssocFracForJet(const pat::Jet *jet, const reco::Vertex *vtx);


///                           ///
/// GENERATOR LEVEL UTILITIES ///
///                           ///
int assignPhysicsChannel(edm::Handle<edm::View<reco::Candidate> > &genParticles);
std::vector<const reco::Candidate *> getGeneratorEvent(edm::Handle<edm::View<reco::Candidate> > &hGen, const edm::ParameterSet &iConfig);
const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p);


//                    //
// TRIGGER UTILITILES //
//                    //
std::pair<std::string,double> getHighestPhotonTrigThreshold(edm::Handle<edm::TriggerResults> &triggerBitsH, const edm::TriggerNames &triggerNames,  std::vector<std::string> &gammaTriggers);
bool checkIfTriggerFired(edm::Handle<edm::TriggerResults> &allTriggerBis, const edm::TriggerNames &triggerNames, std::vector<std::string> &trigList); 


#endif
