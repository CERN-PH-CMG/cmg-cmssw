#include "CMGTools/TTHAnalysis/interface/SignedImpactParameter.h"
#include "CMGTools/TTHAnalysis/interface/CMGMuonCleanerBySegmentsAlgo.h"
#include "CMGTools/TTHAnalysis/interface/DistributionRemapper.h"
#include "CMGTools/TTHAnalysis/interface/PdfWeightProducerTool.h"
#include "CMGTools/TTHAnalysis/interface/IgProfHook.h"
#include "CMGTools/TTHAnalysis/interface/TriggerBitChecker.h"

namespace {
    struct dictionary {
        SignedImpactParameter sipc;
        CMGMuonCleanerBySegmentsAlgo cmgMuonCleanerBySegmentsAlgo;
        DistributionRemapper remapper;
        PdfWeightProducerTool pdfw;
        SetupIgProfDumpHook hook;
        TriggerBitChecker checker;
    };
}
