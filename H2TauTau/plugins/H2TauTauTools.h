#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"
#include "CMGTools/H2TauTau/interface/TauTauWithSVFitProducer.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "CMGTools/H2TauTau/interface/RecoilCorrectedMETProducer2012.h"

typedef DiTauWithSVFitProducer< cmg::TauMu > TauMuWithSVFitProducer;
typedef DiTauWithSVFitProducer< cmg::TauEle > TauEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< cmg::MuEle > MuEleWithSVFitProducer;

typedef RecoilCorrectedMETProducer2012<cmg::TauMu> RecoilCorrectedMETProducer2012TauMu; 
typedef RecoilCorrectedMETProducer2012<cmg::TauEle> RecoilCorrectedMETProducer2012TauEle; 
typedef RecoilCorrectedMETProducer2012<cmg::MuEle> RecoilCorrectedMETProducer2012MuEle; 
typedef RecoilCorrectedMETProducer2012<cmg::DiTau> RecoilCorrectedMETProducer2012DiTau; 
