#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"
#include "CMGTools/TTHAnalysis/interface/PyTMVATool.h"
#include "CMGTools/TTHAnalysis/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "CMGTools/TTHAnalysis/interface/DistributionRemapper.h"
#include "CMGTools/TTHAnalysis/interface/PdfWeightProducerTool.h"
#include "CMGTools/TTHAnalysis/interface/IgProfHook.h"
#include "CMGTools/TTHAnalysis/interface/TriggerBitChecker.h"
#include "EgammaAnalysis/ElectronTools/interface/PatElectronEnergyCalibrator.h"

namespace {
    struct dictionary {
        SignedImpactParameter sipc;
        PyTMVATool tool;
        CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
        DistributionRemapper remapper;
        PdfWeightProducerTool pdfw;
        ElectronEnergyCalibrator calibrator;
        SetupIgProfDumpHook hook;
        TriggerBitChecker checker;
    };
}
