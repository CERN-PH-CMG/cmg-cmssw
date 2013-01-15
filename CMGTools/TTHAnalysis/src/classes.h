#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"
#include "CMGTools/TTHAnalysis/interface/PyTMVATool.h"
#include "CMGTools/TTHAnalysis/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "EgammaAnalysis/ElectronTools/interface/PatElectronEnergyCalibrator.h"

namespace {
    struct dictionary {
        SignedImpactParameter sipc;
        PyTMVATool tool;
        CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
        ElectronEnergyCalibrator calibrator;
    };
}
