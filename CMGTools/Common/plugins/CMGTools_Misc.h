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
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/Common/interface/GenericHistograms.h"
#include "CMGTools/Common/interface/GenericRooDataSets.h"
#include "CMGTools/Common/interface/JetHistograms.h"
#include "CMGTools/Common/interface/TriggerResultHistograms.h"

#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

// COLIN why not in plugins?
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"
#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "CMGTools/Common/plugins/CollectionSizeProducer.h"
#include "CMGTools/Common/plugins/PhysicsObjectPrinter.h"

#include "CMGTools/Common/plugins/JetEnergyCorrector.h"


typedef cmg::JetEnergyCorrector<cmg::BaseJet> BaseJetCorrector;
typedef cmg::JetEnergyCorrector<cmg::PFJet> PFJetCorrector;
typedef cmg::JetEnergyCorrector<cmg::StructuredPFJet> StructuredPFJetCorrector;
typedef cmg::HistogramAnalyzer<cmg::GenericRooDataSets<cmg::DiMuon> > CmgDiMuonDataSet;
typedef cmg::HistogramAnalyzer<cmg::GenericRooDataSets<cmg::DiTau> > CmgDiTauDataSet;
typedef cmg::RunInfoAccountingAnalyzer RunInfoAccountingAnalyzer;
typedef cmg::CutSummaryAnalyzer CutSummaryAnalyzer;
typedef cmg::PhysicsObjectPrinter PhysicsObjectPrinter;
typedef CollectionSizeProducer< std::vector<reco::Vertex> > VertexSizeProducer;
typedef CollectionSizeProducer< edm::View<reco::Candidate> > CandidateSizeProducer;

#endif
