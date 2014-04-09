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

#include "CMGTools/Common/interface/Scaler.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

typedef PhysicsObjectProducer<cmg::Scaler<cmg::BaseJet> > BaseJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::PFJet> > PFJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::StructuredPFJet> > StructuredPFJetPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Tau> > TauPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Muon> > MuonPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::Electron> > ElectronPOScaler;
typedef PhysicsObjectProducer<cmg::Scaler<cmg::BaseMET> > BaseMETPOScaler;

#endif
