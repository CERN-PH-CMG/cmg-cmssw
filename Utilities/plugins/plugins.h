#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "CMGTools/Utilities/interface/RecoilCorrectedMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducer.h"
#include "CMGTools/Utilities/interface/MVAMETProducerPreselLep.h"

#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"
#include "CMGTools/Utilities/interface/TauESCorrector.h"


typedef RecoilCorrectedMETProducer<cmg::TauMu> RecoilCorrectedMETProducerTauMu; 
typedef RecoilCorrectedMETProducer<cmg::TauEle> RecoilCorrectedMETProducerTauEle; 
typedef RecoilCorrectedMETProducer<cmg::MuEle> RecoilCorrectedMETProducerMuEle; 
typedef RecoilCorrectedMETProducer<cmg::DiTau> RecoilCorrectedMETProducerDiTau; 

typedef MVAMETProducer<cmg::TauMu>    MVAMETProducerTauMu; 
typedef MVAMETProducer<cmg::TauEle>    MVAMETProducerTauEle; 
typedef MVAMETProducer<cmg::DiTau>    MVAMETProducerDiTau; 

typedef MVAMETProducerPreselLep   MVAMETProducerPreselectedLeptons; 

typedef PhysicsObjectProducer<cmg::TauESCorrector> TauESCorrectorProducer;
