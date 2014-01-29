#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"
#include "CMGTools/TTHAnalysis/interface/DistributionRemapper.h"
#include "CMGTools/TTHAnalysis/interface/PdfWeightProducerTool.h"
#include "CMGTools/TTHAnalysis/interface/IgProfHook.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibrator.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEPcombinator.h"
#include "EgammaAnalysis/ElectronTools/interface/SimpleElectron.h"

namespace {
    struct dictionary {
        SignedImpactParameter sipc;
        DistributionRemapper remapper;
        PdfWeightProducerTool pdfw;
        SetupIgProfDumpHook hook;
        SimpleElectron fuffaElectron;
        ElectronEnergyCalibrator fuffaElectronCalibrator; 
        ElectronEPcombinator fuffaElectronCombinator;
    };
}
