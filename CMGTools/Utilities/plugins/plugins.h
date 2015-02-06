#include "CMGTools/Utilities/interface/RecoilCorrectedMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducerPreselLep.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "CMGTools/Utilities/interface/TauESCorrector.h"

typedef MVAMETProducer<pat::Tau, pat::Muon> MVAMETProducerTauMu;
typedef MVAMETProducer<pat::Tau, pat::Electron> MVAMETProducerTauEle;
typedef MVAMETProducer<pat::Muon, pat::Electron> MVAMETProducerMuEle;
typedef MVAMETProducer<pat::Tau, pat::Tau> MVAMETProducerDiTau;

// JAN: Re-add these typedefs once we have defined a di-object data format

typedef RecoilCorrectedMETProducer<pat::Tau, pat::Muon> RecoilCorrectedMETProducerTauMu; 
typedef RecoilCorrectedMETProducer<pat::Tau, pat::Electron> RecoilCorrectedMETProducerTauEle; 
typedef RecoilCorrectedMETProducer<pat::Muon, pat::Electron> RecoilCorrectedMETProducerMuEle; 
typedef RecoilCorrectedMETProducer<pat::Tau, pat::Tau> RecoilCorrectedMETProducerDiTau; 

// typedef MVAMETProducer<cmg::TauMu>    MVAMETProducerTauMu; 
// typedef MVAMETProducer<cmg::TauEle>    MVAMETProducerTauEle; 
// typedef MVAMETProducer<cmg::DiTau>    MVAMETProducerDiTau; 

typedef MVAMETProducerPreselLep   MVAMETProducerPreselectedLeptons; 

// typedef PhysicsObjectProducer<cmg::TauESCorrector> TauESCorrectorProducer;
