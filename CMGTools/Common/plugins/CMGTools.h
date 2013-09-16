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
#include "CMGTools/Common/interface/CompoundFactories.h"
#include "CMGTools/Common/interface/MultiObjectFactory.h"
#include "CMGTools/Common/interface/HemiFactory.h"
#include "CMGTools/Common/interface/FatJetFactory.h"
#include "CMGTools/Common/interface/PhotonFactory.h"
#include "CMGTools/Common/interface/GenericFactories.h"
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "CMGTools/Common/interface/MuonFactory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"
#include "CMGTools/Common/interface/StructuredPFJetFactory.h"

#include "CMGTools/Common/interface/TauFactory.h"
#include "CMGTools/Common/interface/Scaler.h"
#include "CMGTools/Common/interface/ObjectSmear.h"
#include "CMGTools/Common/interface/BaseJetFactory.h"
#include "CMGTools/Common/interface/BaseMETFactory.h"
#include "CMGTools/Common/interface/BaseMETModificationFactory.h"
#include "CMGTools/Common/interface/METScaler.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"
#include "CMGTools/Common/interface/TriggerObjectFactory.h"
#include "CMGTools/Common/interface/L1TriggerObjectFactory.h"

#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"



// COLIN why not in plugins?
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"
#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "CMGTools/Common/plugins/LeadingObjectsSelector.h"
#include "CMGTools/Common/plugins/IndexObjectsSelector.h"
#include "CMGTools/Common/plugins/PhysicsObjectPrinter.h"
#include "CMGTools/Common/plugins/DeltaRVetoProducer.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "CMGTools/Common/plugins/CollectionSizeProducer.h"

#include "CMGTools/Common/plugins/JetEnergyCorrector.h"


typedef cmg::JetEnergyCorrector<cmg::BaseJet> BaseJetCorrector;
typedef cmg::JetEnergyCorrector<cmg::PFJet> PFJetCorrector;
typedef cmg::JetEnergyCorrector<cmg::StructuredPFJet> StructuredPFJetCorrector;


// Colin: should add cmg to the type names where applicable.


typedef PhysicsObjectProducer<cmg::MultiObjectFactory> MultiObjectPOProducer;
typedef PhysicsObjectProducer<cmg::PhotonFactory> PhotonPOProducer;

typedef PhysicsObjectProducer<cmg::DiPFCandidateFactory> DiPFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::DiPhotonFactory> DiPhotonPOProducer;
typedef PhysicsObjectProducer<cmg::HemisphereFactory> HemispherePOProducer;
typedef PhysicsObjectProducer<cmg::FatJetFactory> FatJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiFatJetFactory> DiFatJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiHemisphereFactory> DiHemispherePOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
typedef PhysicsObjectProducer<cmg::DiJetFactory> DiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiPFJetFactory> DiPFJetPOProducer;

typedef PhysicsObjectProducer<cmg::ElectronFactory> ElectronPOProducer;
typedef PhysicsObjectProducer<cmg::MuonFactory> MuonPOProducer;
typedef PhysicsObjectProducer<cmg::TauFactory> TauPOProducer;
typedef PhysicsObjectProducer<cmg::QuadMuonFactory> QuadMuonPOProducer;
typedef PhysicsObjectProducer<cmg::BaseJetFactory> BaseJetPOProducer;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::BaseJet> > BaseJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::PFJet> > PFJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::StructuredPFJet> > StructuredPFJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Tau> > TauPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Muon> > MuonPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Electron> > ElectronPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::BaseMET> > BaseMETPOScaler;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::BaseJet> > BaseJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::PFJet> > PFJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::StructuredPFJet> > StructuredPFJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Tau> > TauPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Muon> > MuonPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Electron> > ElectronPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::BaseMET> > BaseMETPOObjectSmear;
typedef PhysicsObjectProducer<cmg::PFJetFactory> PFJetPOProducer;
typedef PhysicsObjectProducer<cmg::StructuredPFJetFactory> StructuredPFJetPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<> > BaseMETPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETModificationFactory> BaseMETModifier;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<pat::MET> > PATMETPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<reco::PFMET> > PFMETPOProducer;
typedef PhysicsObjectProducer<cmg::METScaler> METPOScaler;
typedef PhysicsObjectProducer<cmg::PFCandidateFactory> PFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::GenParticleFactory> GenParticlePOProducer;
typedef PhysicsObjectProducer<cmg::GenJetFactory> GenJetPOProducer;
typedef PhysicsObjectProducer<cmg::WENuFactory> WENuPOProducer;
typedef PhysicsObjectProducer<cmg::WMuNuFactory> WMuNuPOProducer;
typedef PhysicsObjectProducer<cmg::WTauNuFactory> WTauNuPOProducer;
typedef PhysicsObjectProducer<cmg::TriggerObjectFactory> TriggerObjectPOProducer;
typedef PhysicsObjectProducer<cmg::L1TriggerObjectFactory> L1TriggerObjectPOProducer;

//Michalis : Four lepton Types and helpfull gen types
typedef PhysicsObjectProducer<cmg::QuadMuonFactory> QuadMuonPOProducer;
typedef PhysicsObjectProducer<cmg::DiMuonDiElectronFactory> DiMuonDiElectronPOProducer;
typedef PhysicsObjectProducer<cmg::QuadElectronFactory> QuadElectronPOProducer;
// typedef PhysicsObjectProducer<cmg::TriMuonMETFactory> TriMuonMETPOProducer;


// delta R cleaners

typedef cmg::DeltaRVetoProducer<cmg::BaseJet> DeltaRVetoProducerBaseJet;
typedef cmg::DeltaRVetoProducer<cmg::PFJet> DeltaRVetoProducerPFJet;
typedef cmg::DeltaRVetoProducer<cmg::Electron> DeltaRVetoProducerElectron;
typedef cmg::DeltaRVetoProducer<cmg::Muon> DeltaRVetoProducerMuon;
typedef cmg::DeltaRVetoProducer<cmg::Tau> DeltaRVetoProducerTau;


// histogrammers 

/* // For datasets. */
typedef cmg::HistogramAnalyzer<cmg::GenericRooDataSets<cmg::DiMuon> > CmgDiMuonDataSet;
typedef cmg::HistogramAnalyzer<cmg::GenericRooDataSets<cmg::DiTau> > CmgDiTauDataSet;

typedef cmg::RunInfoAccountingAnalyzer RunInfoAccountingAnalyzer;
typedef cmg::CutSummaryAnalyzer CutSummaryAnalyzer;
typedef cmg::PhysicsObjectPrinter PhysicsObjectPrinter;



// Leading-objects selectors.

typedef cmg::LeadingObjectsSelector<pat::Jet> LeadingPatJetSelector;
typedef cmg::LeadingObjectsSelector<cmg::AbstractPhysicsObject> LeadingCMGPOSelector;
typedef cmg::LeadingObjectsSelector<cmg::BaseJet> LeadingCMGBaseJetSelector;
typedef cmg::LeadingObjectsSelector<cmg::PFJet> LeadingCMGPFJetSelector;
typedef cmg::LeadingObjectsSelector<cmg::StructuredPFJet> LeadingCMGStructuredPFJetSelector;
typedef cmg::LeadingObjectsSelector<cmg::Electron> LeadingCMGElectronSelector;
typedef cmg::LeadingObjectsSelector<cmg::Muon> LeadingCMGMuonSelector;

// Index objects selector

typedef cmg::IndexObjectsSelector<pat::Jet> IndexPatJetSelector;
typedef cmg::IndexObjectsSelector<cmg::AbstractPhysicsObject> IndexCMGPOSelector;
typedef cmg::IndexObjectsSelector<cmg::BaseJet> IndexCMGBaseJetSelector;
typedef cmg::IndexObjectsSelector<cmg::PFJet> IndexCMGPFJetSelector;
typedef cmg::IndexObjectsSelector<cmg::StructuredPFJet> IndexCMGStructuredPFJetSelector;
typedef cmg::IndexObjectsSelector<cmg::Electron> IndexCMGElectronSelector;
typedef cmg::IndexObjectsSelector<cmg::Muon> IndexCMGMuonSelector;



// Single object selectors.


typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::PhysicsObjectWithPtr<cmg::GenJetPtr> > > CmgGenJetPOSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiPFCandidate> > CmgDiPFCandidateSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiFatJet> > CmgDiFatJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiPFJet> > CmgDiPFJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiJet> > CmgDiBaseJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Photon> > CmgPhotonSelector;

//COLIN need to use this selector instead of the standard one, so that sourceCandidatePtrs are properly set
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::PFJet> > CmgPFJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::StructuredPFJet> > CmgStructuredPFJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::BaseJet> > CmgBaseJetSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::BaseMET> > CmgBaseMETSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Electron> > CmgElectronSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Muon> > CmgMuonSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuon> > CmgDiMuonSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Tau> > CmgTauSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiTau> > CmgDiTauSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TriggerObject> > CmgTriggerObjectSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiElectron> > CmgDiElectronSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WENu> > CmgWENuSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WMuNu> > CmgWMuNuSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TauEle> > CmgTauEleSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TauMu> > CmgTauMuSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::MuEle> > CmgMuEleSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<reco::GenParticle> > CmgGenParticleSelector;

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::QuadMuon> > CmgQuadMuonSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuonDiElectron> > CmgDiMuonDiElectronSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::QuadElectron> > CmgQuadElectronSelector;

typedef CollectionSizeProducer< std::vector<reco::Vertex> > VertexSizeProducer;

typedef CollectionSizeProducer< edm::View<reco::Candidate> > CandidateSizeProducer;

#endif
