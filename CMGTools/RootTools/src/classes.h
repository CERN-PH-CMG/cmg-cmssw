#include "CMGTools/RootTools/interface/BTagSF.h"
#include "CMGTools/RootTools/interface/RochCor.h"
#include "CMGTools/RootTools/interface/RochCor2012.h"
#include "CMGTools/RootTools/interface/FSRWeightAlgo.h"
#include "CMGTools/RootTools/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "CMGTools/RootTools/interface/TriggerBitChecker.h"
#include "CMGTools/RootTools/interface/MuScleFitCorrector.h"

namespace {
  namespace {
    BTagSF  bTagSF_; 
    RochCor rc_;
    RochCor2012 rc2012_;
    FSRWeightAlgo walgo_;
    TriggerBitChecker checker;
    CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
  }
}
