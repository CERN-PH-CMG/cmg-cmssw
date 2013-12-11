#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"
#include "CMGTools/TTHAnalysis/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "CMGTools/TTHAnalysis/interface/DistributionRemapper.h"
#include "CMGTools/TTHAnalysis/interface/PdfWeightProducerTool.h"
#include "CMGTools/TTHAnalysis/interface/IgProfHook.h"
#include "CMGTools/TTHAnalysis/interface/TriggerBitChecker.h"
#include "CMGTools/TTHAnalysis/src/MuScleFitCorrector.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibrator.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEPcombinator.h"
#include "EgammaAnalysis/ElectronTools/interface/SimpleElectron.h"

namespace {
    struct dictionary {
        SignedImpactParameter sipc;
        CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
        DistributionRemapper remapper;
        PdfWeightProducerTool pdfw;
        SetupIgProfDumpHook hook;
        TriggerBitChecker checker;
        SimpleElectron fuffaElectron;
        ElectronEnergyCalibrator fuffaElectronCalibrator; 
        ElectronEPcombinator fuffaElectronCombinator;
    };
}
