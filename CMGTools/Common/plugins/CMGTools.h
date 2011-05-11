#ifndef __CMGTools_Common_CMGTools_h__
#define __CMGTools_Common_CMGTools_h__

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

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

#include "CMGTools/Common/interface/TauFactory.h"

#include "CMGTools/Common/interface/BaseJetFactory.h"
#include "CMGTools/Common/interface/BaseMETFactory.h"
#include "CMGTools/Common/interface/BaseMETModificationFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"
#include "CMGTools/Common/interface/TriggerObjectFactory.h"

// COLIN why not in plugins?
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"
#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "CMGTools/Common/plugins/LeadingObjectsSelector.h"
#include "CMGTools/Common/plugins/PhysicsObjectPrinter.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

// Colin: should add cmg to the type names where applicable.

typedef PhysicsObjectProducer<cmg::MultiObjectFactory> MultiObjectPOProducer;
typedef PhysicsObjectProducer<cmg::PhotonFactory> PhotonPOProducer;
typedef PhysicsObjectProducer<cmg::DiPhotonFactory> DiPhotonPOProducer;
typedef PhysicsObjectProducer<cmg::HemisphereFactory> HemispherePOProducer;
typedef PhysicsObjectProducer<cmg::FatJetFactory> FatJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiFatJetFactory> DiFatJetPOProducer;

typedef PhysicsObjectProducer<cmg::DiHemisphereFactory> DiHemispherePOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
typedef PhysicsObjectProducer<cmg::DiJetFactory> DiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiPFJetFactory> DiPFJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiTauFactory> DiTauPOProducer;
typedef PhysicsObjectProducer<cmg::ElectronFactory> ElectronPOProducer;
typedef PhysicsObjectProducer<cmg::MuonFactory> MuonPOProducer;
typedef PhysicsObjectProducer<cmg::TauFactory> TauPOProducer;
typedef PhysicsObjectProducer<cmg::QuadMuonFactory> QuadMuonPOProducer;
typedef PhysicsObjectProducer<cmg::BaseJetFactory> BaseJetPOProducer;
typedef PhysicsObjectProducer<cmg::PFJetFactory> PFJetPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<> > BaseMETPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETModificationFactory> BaseMETModifier;
typedef PhysicsObjectProducer<cmg::BaseMETFactory<pat::MET> > PATMETPOProducer;
typedef PhysicsObjectProducer<cmg::PFCandFactory> PFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::GenParticleFactory> GenParticlePOProducer;
typedef PhysicsObjectProducer<cmg::GenJetFactory> GenJetPOProducer;
typedef PhysicsObjectProducer<cmg::WENuFactory> WENuPOProducer;
typedef PhysicsObjectProducer<cmg::WMuNuFactory> WMuNuPOProducer;
typedef PhysicsObjectProducer<cmg::WTauNuFactory> WTauNuPOProducer;
typedef PhysicsObjectProducer<cmg::TriggerObjectFactory> TriggerObjectPOProducer;

typedef cmg::HistogramAnalyzer<cmg::JetHistograms> PFJetHistogramsAnalyzer;
typedef cmg::HistogramAnalyzer<cmg::TriggerResultHistograms> TriggerResultHistogramsAnalyzer;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::BaseMET> > GenericBaseMETHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::AbstractPhysicsObject> > GenericPOHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<pat::MET> > GenericPatMETHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Electron> > CmgElectronHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Muon> > CmgMuonHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Tau> > CmgTauHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Hemisphere> > CmgHemiHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Photon> > CmgPhotonHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::MultiObject> > CmgMultiObjectHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiMuon> > CmgDiMuonHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiTau> > CmgDiTauHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiElectron> > CmgDiElectronHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiHemisphere> > CmgDiHemiHistograms;

typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::FatJet> > CmgFatJetHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiFatJet> > CmgDiFatJetHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiJet> > CmgDiJetHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::DiPFJet> > CmgDiPFJetHistograms;

// For datasets.
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
typedef cmg::LeadingObjectsSelector<cmg::Electron> LeadingCMGElectronSelector;
typedef cmg::LeadingObjectsSelector<cmg::Muon> LeadingCMGMuonSelector;

// Single object selectors.


typedef SingleObjectSelector<
  std::vector<cmg::DiFatJet>,
  StringCutObjectSelector<cmg::DiFatJet>
  > CmgDiFatJetSelector;

typedef SingleObjectSelector<
  std::vector<cmg::DiPFJet>,
  StringCutObjectSelector<cmg::DiPFJet>
  > CmgDiPFJetSelector;

typedef SingleObjectSelector<
  std::vector<cmg::DiJet>,
  StringCutObjectSelector<cmg::DiJet>
  > CmgDiBaseJetSelector;


typedef SingleObjectSelector<
  std::vector<cmg::Photon>,
  StringCutObjectSelector<cmg::Photon>
  > CmgPhotonSelector;

typedef SingleObjectSelector<
  std::vector<cmg::PFJet>,
  StringCutObjectSelector<cmg::PFJet>
  > CmgPFJetSelector;

typedef SingleObjectSelector<
  std::vector<cmg::BaseJet>,
  StringCutObjectSelector<cmg::BaseJet>
  > CmgBaseJetSelector;

typedef SingleObjectSelector<
  std::vector<cmg::BaseMET>,
  StringCutObjectSelector<cmg::BaseMET>
  > CmgBaseMETSelector;

typedef SingleObjectSelector<
  std::vector<cmg::Electron>,
  StringCutObjectSelector<cmg::Electron>
  > CmgElectronSelector;

typedef SingleObjectSelector<
  std::vector<cmg::Muon>,
  StringCutObjectSelector<cmg::Muon>
  > CmgMuonSelector;

typedef SingleObjectSelector<
  std::vector<cmg::DiMuon>,
  StringCutObjectSelector<cmg::DiMuon>
  > CmgDiMuonSelector;

typedef SingleObjectSelector<
  std::vector<cmg::Tau>,
  StringCutObjectSelector<cmg::Tau>
  > CmgTauSelector;

typedef SingleObjectSelector<
  std::vector<cmg::DiTau>,
  StringCutObjectSelector<cmg::DiTau>
  > CmgDiTauSelector;

typedef SingleObjectSelector<
  std::vector<cmg::TriggerObject>,
  StringCutObjectSelector<cmg::TriggerObject>
  > CmgTriggerObjectSelector;

typedef SingleObjectSelector<
  std::vector<cmg::DiElectron>,
  StringCutObjectSelector<cmg::DiElectron>
  > CmgDiElectronSelector;

typedef SingleObjectSelector<
  std::vector<cmg::WENu>,
  StringCutObjectSelector<cmg::WENu>
  > CmgWENuSelector;

typedef SingleObjectSelector<
  std::vector<cmg::WMuNu>,
  StringCutObjectSelector<cmg::WMuNu>
  > CmgWMuNuSelector;

#endif
