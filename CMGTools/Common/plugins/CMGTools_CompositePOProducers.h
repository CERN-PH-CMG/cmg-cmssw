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

#include "CMGTools/Common/interface/CompoundFactories.h"
#include "CMGTools/Common/interface/MultiObjectFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

typedef PhysicsObjectProducer<cmg::MultiObjectFactory> MultiObjectPOProducer;
typedef PhysicsObjectProducer<cmg::DiPFCandidateFactory> DiPFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::DiPhotonFactory> DiPhotonPOProducer;
typedef PhysicsObjectProducer<cmg::DiFatJetFactory> DiFatJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiHemisphereFactory> DiHemispherePOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
typedef PhysicsObjectProducer<cmg::DiJetFactory> DiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiPFJetFactory> DiPFJetPOProducer;
//typedef PhysicsObjectProducer<cmg::QuadMuonFactory> QuadMuonPOProducer;
//typedef PhysicsObjectProducer<cmg::DiMuonDiElectronFactory> DiMuonDiElectronPOProducer;
//typedef PhysicsObjectProducer<cmg::QuadElectronFactory> QuadElectronPOProducer;

#endif
