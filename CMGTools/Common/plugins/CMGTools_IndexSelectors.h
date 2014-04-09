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

#include "CMGTools/Common/plugins/IndexObjectsSelector.h"

typedef cmg::IndexObjectsSelector<cmg::AbstractPhysicsObject> IndexCMGPOSelector;
typedef cmg::IndexObjectsSelector<cmg::BaseJet> IndexCMGBaseJetSelector;
typedef cmg::IndexObjectsSelector<cmg::PFJet> IndexCMGPFJetSelector;
typedef cmg::IndexObjectsSelector<cmg::StructuredPFJet> IndexCMGStructuredPFJetSelector;
typedef cmg::IndexObjectsSelector<cmg::Electron> IndexCMGElectronSelector;
typedef cmg::IndexObjectsSelector<cmg::Muon> IndexCMGMuonSelector;

#endif
