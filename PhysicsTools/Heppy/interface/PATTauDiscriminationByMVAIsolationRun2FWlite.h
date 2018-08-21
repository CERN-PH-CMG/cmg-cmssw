#ifndef _heppy_PATTauDiscriminationByMVAIsolationRun2FWlite_h
#define _heppy_PATTauDiscriminationByMVAIsolationRun2FWlite_h
#include "CondFormats/EgammaObjects/interface/GBRForest.h"
#include "DataFormats/PatCandidates/interface/Tau.h"


namespace heppy {
    class PATTauDiscriminationByMVAIsolationRun2FWlite {
        public:
            PATTauDiscriminationByMVAIsolationRun2FWlite() : mvaReader_(nullptr) {}
            PATTauDiscriminationByMVAIsolationRun2FWlite(const std::string & fileName, const std::string & mvaName, const std::string &mvaKind) ;
            ~PATTauDiscriminationByMVAIsolationRun2FWlite();
            PATTauDiscriminationByMVAIsolationRun2FWlite(const PATTauDiscriminationByMVAIsolationRun2FWlite & other) = delete;
            PATTauDiscriminationByMVAIsolationRun2FWlite & operator=(const PATTauDiscriminationByMVAIsolationRun2FWlite & other) = delete;

            float operator()(const pat::Tau& tau) const ;

        private:
            const GBRForest* mvaReader_;
            enum { kOldDMwoLT, kOldDMwLT, kNewDMwoLT, kNewDMwLT, kDBoldDMwLT, kDBnewDMwLT, kPWoldDMwLT, kPWnewDMwLT, kDBoldDMwLTwGJ, kDBnewDMwLTwGJ };
            int mvaOpt_;
            float* mvaInput_;

            std::string chargedIsoPtSums_;
            std::string neutralIsoPtSums_;
            std::string puCorrPtSums_;
            std::string photonPtSumOutsideSignalCone_;
            std::string footprintCorrection_;
    };
}

#endif
