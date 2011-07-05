import FWCore.ParameterSet.Config as cms

##
## pileup normalization scenarios
##

# all the samples are generated with similar PU distribution (S4) so all the weights bellow are coming from the DY sample.
def getPUScenario(scenario="Summer11_S4"):
    if(scenario=="Summer11_S4") :
        return cms.vdouble(0.120949,  0.066766,  0.071315,  0.070682,  0.070845,  0.069496,  0.067300,  0.064682,  0.061566,  0.057429,  0.052165,  0.046143,  0.039450,  0.033369,  0.026624,  0.021317,  0.016741,  0.012638,  0.009338,  0.006751,  0.004795,  0.003353,  0.002258,  0.001518,  0.000995,  0.000637,  0.000384,  0.000241,  0.000157,  0.000099)

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
                           integerWeightsOnly = cms.bool(False),
                           mcDistribution = getPUScenario("Summer11_S4"),
                           dataDistribution = getDataScenario("PromptReco")
                           )
