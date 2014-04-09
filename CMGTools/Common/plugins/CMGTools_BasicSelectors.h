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
#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"



// COLIN why not in plugins?
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"
#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "CMGTools/Common/plugins/CollectionSizeProducer.h"

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::PhysicsObjectWithPtr<cmg::GenJetPtr> > > CmgGenJetPOSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Photon> > CmgPhotonSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::PFJet> > CmgPFJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::StructuredPFJet> > CmgStructuredPFJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::BaseJet> > CmgBaseJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::BaseMET> > CmgBaseMETSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Electron> > CmgElectronSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Muon> > CmgMuonSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::Tau> > CmgTauSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TriggerObject> > CmgTriggerObjectSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<reco::GenParticle> > CmgGenParticleSelector;

#endif
