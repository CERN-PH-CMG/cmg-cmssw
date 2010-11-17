#include "AnalysisDataFormats/CMGTools/interface/DiMuon.h"
#include "CMGTools/Common/interface/DiMuonFactory.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "CMGTools/Common/interface/ElectronFactory.h"

#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "CMGTools/Common/interface/MuonFactory.h"

#include "CMGTools/Common/interface/GenericFactories.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "CMGTools/Common/interface/BaseJetFactory.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/PFJetFactory.h"

#include "CMGTools/Common/interface/BaseMETFactory.h"

#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"


#include "FWCore/Framework/interface/MakerMacros.h"

typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
DEFINE_FWK_MODULE(DiMuonPOProducer);

typedef PhysicsObjectProducer<cmg::ElectronFactory> ElectronPOProducer;
DEFINE_FWK_MODULE(ElectronPOProducer);

typedef PhysicsObjectProducer<cmg::MuonFactory> MuonPOProducer;
DEFINE_FWK_MODULE(MuonPOProducer);

typedef PhysicsObjectProducer<cmg::TauFactory> TauPOProducer;
DEFINE_FWK_MODULE(TauPOProducer);

typedef PhysicsObjectProducer<cmg::BaseJetFactory> BaseJetPOProducer;
DEFINE_FWK_MODULE(BaseJetPOProducer);

typedef PhysicsObjectProducer<cmg::PFJetFactory> PFJetPOProducer;
DEFINE_FWK_MODULE(PFJetPOProducer);

typedef PhysicsObjectProducer<cmg::BaseMETFactory> BaseMETPOProducer;
DEFINE_FWK_MODULE(BaseMETPOProducer);

#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/Common/interface/LorentzHistograms.h"
#include "CMGTools/Common/interface/JetHistograms.h"

typedef cmg::HistogramAnalyzer<cmg::LorentzHistogramsFF> LorentzHistogramsAnalyzer;
DEFINE_FWK_MODULE(LorentzHistogramsAnalyzer);

typedef cmg::HistogramAnalyzer<cmg::JetHistograms> PFJetHistogramsAnalyzer;
DEFINE_FWK_MODULE(PFJetHistogramsAnalyzer);

typedef cmg::RunInfoAccountingAnalyzer RunInfoAccountingAnalyzer;
DEFINE_FWK_MODULE(RunInfoAccountingAnalyzer);

typedef PhysicsObjectProducer<cmg::PFCandFactory> PFCandidatePOProducer;
DEFINE_FWK_MODULE(PFCandidatePOProducer);
