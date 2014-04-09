#ifndef __CMGTools_Common_CMGTools_h__
#define __CMGTools_Common_CMGTools_h__

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/StructuredPFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/Common/interface/GenericHistograms.h"
#include "CMGTools/Common/interface/GenericRooDataSets.h"
#include "CMGTools/Common/interface/JetHistograms.h"
#include "CMGTools/Common/interface/TriggerResultHistograms.h"
#include "CMGTools/Common/interface/HemiFactory.h"
#include "CMGTools/Common/interface/FatJetFactory.h"
#include "CMGTools/Common/interface/PhotonFactory.h"
#include "CMGTools/Common/interface/GenericFactories.h"
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "CMGTools/Common/interface/MuonFactory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"
#include "CMGTools/Common/interface/StructuredPFJetFactory.h"
#include "CMGTools/Common/interface/FatJetFactory.h"

#include "CMGTools/Common/interface/TauFactory.h"
#include "CMGTools/Common/interface/BaseJetFactory.h"
#include "CMGTools/Common/interface/BaseMETFactory.h"
#include "CMGTools/Common/interface/TriggerObjectFactory.h"
#include "CMGTools/Common/interface/L1TriggerObjectFactory.h"

#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

typedef PhysicsObjectProducer<cmg::PhotonFactory> PhotonPOProducer;
typedef PhysicsObjectProducer<cmg::HemisphereFactory> HemispherePOProducer;
typedef PhysicsObjectProducer<cmg::ElectronFactory> ElectronPOProducer;
typedef PhysicsObjectProducer<cmg::MuonFactory> MuonPOProducer;
typedef PhysicsObjectProducer<cmg::TauFactory> TauPOProducer;
typedef PhysicsObjectProducer<cmg::BaseJetFactory> BaseJetPOProducer;
typedef PhysicsObjectProducer<cmg::PFJetFactory> PFJetPOProducer;
typedef PhysicsObjectProducer<cmg::StructuredPFJetFactory> StructuredPFJetPOProducer;
typedef PhysicsObjectProducer<cmg::FatJetFactory> FatJetPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<> > BaseMETPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<pat::MET> > PATMETPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<reco::PFMET> > PFMETPOProducer;
typedef PhysicsObjectProducer<cmg::PFCandidateFactory> PFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::GenParticleFactory> GenParticlePOProducer;
typedef PhysicsObjectProducer<cmg::GenJetFactory> GenJetPOProducer;
typedef PhysicsObjectProducer<cmg::TriggerObjectFactory> TriggerObjectPOProducer;
typedef PhysicsObjectProducer<cmg::L1TriggerObjectFactory> L1TriggerObjectPOProducer;

#include "CMGTools/Common/interface/METScaler.h"
#include "CMGTools/Common/interface/BaseMETModificationFactory.h"
typedef PhysicsObjectProducer<cmg::BaseMETModificationFactory> BaseMETModifier;
typedef PhysicsObjectProducer<cmg::METScaler> METPOScaler;
#endif
