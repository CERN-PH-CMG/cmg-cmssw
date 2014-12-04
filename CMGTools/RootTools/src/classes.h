#include "CMGTools/RootTools/interface/BTagSF.h"
#include "CMGTools/RootTools/interface/RochCor.h"
#include "CMGTools/RootTools/interface/RochCor2012.h"
#include "CMGTools/RootTools/interface/FSRWeightAlgo.h"
#include "CMGTools/RootTools/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "CMGTools/RootTools/interface/TriggerBitChecker.h"
#include "CMGTools/RootTools/interface/MuScleFitCorrector.h"
#include "CMGTools/RootTools/interface/EGammaMvaEleEstimatorFWLite.h"
#include "CMGTools/RootTools/interface/Davismt2.h"
#include "CMGTools/RootTools/interface/mt2w_bisect.h"
#include "CMGTools/RootTools/interface/Hemisphere.h"
#include "CMGTools/RootTools/interface/AlphaT.h"
#include "CMGTools/RootTools/interface/RecoilCorrector.h"
#include "CMGTools/RootTools/interface/ReclusterJets.h"

namespace {
  namespace {
    BTagSF  bTagSF_; 
    RochCor rc_;
    RochCor2012 rc2012_;
    FSRWeightAlgo walgo_;
    TriggerBitChecker checker;
    CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
    EGammaMvaEleEstimatorFWLite egMVA;
    Hemisphere hemisphere(vector<float> px, vector<float> py, vector<float> pz, vector<float> E, int hemi_seed, int hemi_association);
    ReclusterJets reclusterJets(vector<float> px, vector<float> py, vector<float> pz, vector<float> E, double ktpower, double rparam);
    Davismt2 mt2;
    mt2w_bisect::mt2w mt2wlept;
    AlphaT alphaT;
    RecoilCorrector recoilCorrector(string nameFileToCorrect, int iSeed);
  }
}
