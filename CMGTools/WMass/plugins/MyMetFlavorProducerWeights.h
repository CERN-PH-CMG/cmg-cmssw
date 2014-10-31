#include <memory>
#include <cmath>
#include <algorithm>

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/CMGTools/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

#include "FWCore/Utilities/interface/Exception.h"
// #include "CMGTools/WMass/plugins/MyMetFlavorProducer.h"

#include <utility>


#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CMGTools/External/interface/PileupJetIdAlgo.h"

#include "CMGTools/Common/interface/MetUtilities.h"


// #include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"

// for now leave this for the VertexCollection ?
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

#include "DataFormats/Math/interface/Point3D.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <TSystem.h>
#include <TMath.h>
#include <TLorentzVector.h>

using namespace std;
using namespace edm;
using namespace reco;

class MyMetFlavorProducerWeights : public edm::EDProducer {
public:
  explicit MyMetFlavorProducerWeights(const edm::ParameterSet&);
  ~MyMetFlavorProducerWeights();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual double DZ(const math::XYZTLorentzVector & p4, const math::XYZPointD vtx, const math::XYZPointD vertexPos);

  edm::InputTag vtxLabel_;
  double weightMin_;
  bool isMC_;
  std::string histoname_;

  edm::InputTag fCorrJetName;     
  edm::InputTag fUnCorrJetName; 
  edm::InputTag fPFCandName; 
  edm::InputTag fVertexName; 
  edm::InputTag fRhoName; 

  double                  fJetPtMin;
  double                  fDZMin;
  double                  fCandEtaMin;
  double                  fCandEtaMax;
  double                  fChargedCandPtMin;
  double                  fNeutralCandPMin;
  std::string             fCandFlavor;
  PileupJetIdAlgo        *fPUJetIdAlgo;
  PileupJetIdAlgo        *fPUJetIdAlgoLowPt;
  MetUtilities           *fUtils;
  int                     fMetFlavor;

};

