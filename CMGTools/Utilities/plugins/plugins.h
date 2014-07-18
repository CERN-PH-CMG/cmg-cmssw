#include "CMGTools/Utilities/interface/RecoilCorrectedMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducerPreselLep.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "CMGTools/Utilities/interface/TauESCorrector.h"

typedef MVAMETProducer<reco::PFTau, pat::Muon> MVAMETProducerTauMu;
typedef MVAMETProducer<reco::PFTau, pat::Electron> MVAMETProducerTauEle;
typedef MVAMETProducer<pat::Muon, pat::Electron> MVAMETProducerMuEle;
typedef MVAMETProducer<reco::PFTau, reco::PFTau> MVAMETProducerDiTau;

// JAN: Re-add these typedefs once we have defined a di-object data format

// typedef RecoilCorrectedMETProducer<cmg::TauMu> RecoilCorrectedMETProducerTauMu; 
// typedef RecoilCorrectedMETProducer<cmg::TauEle> RecoilCorrectedMETProducerTauEle; 
// typedef RecoilCorrectedMETProducer<cmg::MuEleDiTau> RecoilCorrectedMETProducerMuEle; 
// typedef RecoilCorrectedMETProducer<cmg::DiTau> RecoilCorrectedMETProducerDiTau; 

// typedef MVAMETProducer<cmg::TauMu>    MVAMETProducerTauMu; 
// typedef MVAMETProducer<cmg::TauEle>    MVAMETProducerTauEle; 
// typedef MVAMETProducer<cmg::DiTau>    MVAMETProducerDiTau; 

typedef MVAMETProducerPreselLep   MVAMETProducerPreselectedLeptons; 

// typedef PhysicsObjectProducer<cmg::TauESCorrector> TauESCorrectorProducer;
