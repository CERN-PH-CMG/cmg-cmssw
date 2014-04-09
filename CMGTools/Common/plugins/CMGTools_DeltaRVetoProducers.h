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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"


#include "CMGTools/Common/plugins/DeltaRVetoProducer.h"

typedef cmg::DeltaRVetoProducer<cmg::BaseJet> DeltaRVetoProducerBaseJet;
typedef cmg::DeltaRVetoProducer<cmg::PFJet> DeltaRVetoProducerPFJet;
typedef cmg::DeltaRVetoProducer<cmg::Electron> DeltaRVetoProducerElectron;
typedef cmg::DeltaRVetoProducer<cmg::Muon> DeltaRVetoProducerMuon;
typedef cmg::DeltaRVetoProducer<cmg::Tau> DeltaRVetoProducerTau;

#endif
