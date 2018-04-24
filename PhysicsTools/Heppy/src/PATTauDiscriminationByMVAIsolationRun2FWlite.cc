#include "PhysicsTools/Heppy/interface/PATTauDiscriminationByMVAIsolationRun2FWlite.h"
#include "RecoTauTag/RecoTau/interface/PFRecoTauClusterVariables.h"
#include <TFile.h>
#include <cassert>

heppy::PATTauDiscriminationByMVAIsolationRun2FWlite::PATTauDiscriminationByMVAIsolationRun2FWlite(const std::string & fileName, const std::string & mvaName, const std::string &mvaKind)
{
    TFile *f = TFile::Open(fileName.c_str());
    assert(f);
    mvaReader_ = (GBRForest*) f->Get(mvaName.c_str());
    assert(mvaReader_);

    if      ( mvaKind == "oldDMwoLT" ) mvaOpt_ = kOldDMwoLT;
    else if ( mvaKind == "oldDMwLT"  ) mvaOpt_ = kOldDMwLT;
    else if ( mvaKind == "newDMwoLT" ) mvaOpt_ = kNewDMwoLT;
    else if ( mvaKind == "newDMwLT"  ) mvaOpt_ = kNewDMwLT;
    else if ( mvaKind == "DBoldDMwLT"  ) mvaOpt_ = kDBoldDMwLT;
    else if ( mvaKind == "DBnewDMwLT"  ) mvaOpt_ = kDBnewDMwLT;
    else if ( mvaKind == "PWoldDMwLT"  ) mvaOpt_ = kPWoldDMwLT;
    else if ( mvaKind == "PWnewDMwLT"  ) mvaOpt_ = kPWnewDMwLT;
    else if ( mvaKind == "DBoldDMwLTwGJ" ) mvaOpt_ = kDBoldDMwLTwGJ;
    else if ( mvaKind == "DBnewDMwLTwGJ" ) mvaOpt_ = kDBnewDMwLTwGJ;
    else assert(0);

    if      ( mvaOpt_ == kOldDMwoLT || mvaOpt_ == kNewDMwoLT ) mvaInput_ = new float[6];
    else if ( mvaOpt_ == kOldDMwLT  || mvaOpt_ == kNewDMwLT  ) mvaInput_ = new float[12];
    else if ( mvaOpt_ == kDBoldDMwLT || mvaOpt_ == kDBnewDMwLT ||
            mvaOpt_ == kPWoldDMwLT || mvaOpt_ == kPWnewDMwLT ||
            mvaOpt_ == kDBoldDMwLTwGJ || mvaOpt_ == kDBnewDMwLTwGJ) mvaInput_ = new float[23];
    else assert(0);   

    // these are trivial for the moment
    chargedIsoPtSums_ = "chargedIsoPtSum";
    neutralIsoPtSums_ = "neutralIsoPtSum";
    puCorrPtSums_ = "puCorrPtSum";
    footprintCorrection_ = "footprintCorrection";
    photonPtSumOutsideSignalCone_ = "photonPtSumOutsideSignalCone";

    if (mvaName.find("dR0p3") != std::string::npos) {
        chargedIsoPtSums_ = "chargedIsoPtSumdR03";
        neutralIsoPtSums_ = "neutralIsoPtSumdR03";
        puCorrPtSums_ = "puCorrPtSum";
        footprintCorrection_ = "footprintCorrectiondR03";
        photonPtSumOutsideSignalCone_ = "photonPtSumOutsideSignalConedR03";
    }
}

heppy::PATTauDiscriminationByMVAIsolationRun2FWlite::~PATTauDiscriminationByMVAIsolationRun2FWlite()
{
    delete mvaReader_;
}

float heppy::PATTauDiscriminationByMVAIsolationRun2FWlite::operator()(const pat::Tau& tau) const 
{
    // CV: computation of MVA value requires presence of leading charged hadron
    if ( tau.leadChargedHadrCand().isNull() ) return 0.;

    if (reco::tau::mva::fillMVAInputs(mvaInput_, tau, mvaOpt_, chargedIsoPtSums_, neutralIsoPtSums_, puCorrPtSums_, footprintCorrection_, photonPtSumOutsideSignalCone_)) {
        return mvaReader_->GetClassifier(mvaInput_);
    } 
    return -1.;
}
