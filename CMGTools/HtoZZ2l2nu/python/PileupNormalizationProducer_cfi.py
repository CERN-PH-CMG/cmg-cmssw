import FWCore.ParameterSet.Config as cms

##
## pileup normalization scenarios
##
def getPUScenario(scenario="TTbar_madgraph"):
    if(scenario=="TTbar_madgraph") :
        return cms.vdouble( 108, 36, 49,
                            61, 55, 74,
                            45, 49, 57,
                            43, 47, 50,
                            34, 46, 36,
                            24, 12, 7,
                            6,  2,  3,
                            0,  1,  0,
                            0 )

def getDataScenario(scenario="PromptReco"):
    if(scenario=="PromptReco") :
       return cms.vdouble(0.019091,    0.0293974,    0.0667931,
                          0.108859,    0.139533,     0.149342,
                          0.138629,    0.114582,     0.0859364,
                          0.059324,    0.0381123,    0.0229881,
                          0.0131129,   0.00711764,   0.00369635,
                          0.00184543,  0.000889604,  0.000415683,
                          0.000188921, 0.000146288,  0.0,
                          0.0,         0.0,          0.0,
                          0.0 )

   
puWeights = cms.EDProducer("PileupNormalizationProducer",
                           mcDistribution = getPUScenario("TTbar_madgraph"),
                           dataDistribution = getDataScenario("PromptReco")
                           )
