import FWCore.ParameterSet.Config as cms

##
## pileup normalization scenarios
##
def getNormalizationForScenario(scenario="Spring11"):
    if(scenario=="Spring11") :
        return cms.vdouble(0.29513,0.66352,1.57694,2.12325,2.58587,2.33852,1.84959,1.16579,0.630877,
                                     0.327577,0.158036,0.0816001,0.0373275,0.0180187,0.00976187,0.00472464,0.001945,0.000752556,
                                     0.000425988,0.000692033)
    else :
        return cms.vdouble(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
    

puWeights = cms.EDProducer("PileupNormalizationProducer",
                           normalizationDistribution = getNormalizationForScenario("Spring11")
                           )
