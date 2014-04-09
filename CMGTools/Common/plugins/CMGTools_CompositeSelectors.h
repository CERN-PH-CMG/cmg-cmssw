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

#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiPFCandidate> > CmgDiPFCandidateSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiFatJet> > CmgDiFatJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiPFJet> > CmgDiPFJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiJet> > CmgDiBaseJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuon> > CmgDiMuonSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiTau> > CmgDiTauSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiElectron> > CmgDiElectronSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WENu> > CmgWENuSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WMuNu> > CmgWMuNuSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TauEle> > CmgTauEleSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::TauMu> > CmgTauMuSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::MuEle> > CmgMuEleSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::QuadMuon> > CmgQuadMuonSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuonDiElectron> > CmgDiMuonDiElectronSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::QuadElectron> > CmgQuadElectronSelector;

#endif
