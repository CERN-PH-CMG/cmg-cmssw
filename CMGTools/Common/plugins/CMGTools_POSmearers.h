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

#include "CMGTools/Common/interface/ObjectSmear.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::BaseJet> > BaseJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::PFJet> > PFJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::StructuredPFJet> > StructuredPFJetPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Tau> > TauPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Muon> > MuonPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::Electron> > ElectronPOObjectSmear;
typedef PhysicsObjectProducer<cmg::ObjectSmear<cmg::BaseMET> > BaseMETPOObjectSmear;

#endif
