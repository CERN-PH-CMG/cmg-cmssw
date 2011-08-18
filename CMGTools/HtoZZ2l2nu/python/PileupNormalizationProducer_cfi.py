import FWCore.ParameterSet.Config as cms

##
## pileup normalization scenarios
##

# all the samples are generated with similar PU distribution (S4)
# the following weights account for the average pileup in +/- 50 ns
# cf. https://twiki.cern.ch/twiki/bin/view/CMS/PileupMCReweightingUtilities
def getPUScenario(scenario="Summer11_S4"):
    if(scenario=="Summer11_S4") :
        return cms.vdouble(0.104109,  0.0703573,  0.0698445,
                           0.0698254,  0.0697054, 0.0697907,
                           0.0696751, 0.0694486,  0.0680332,
                           0.0651044,  0.0598036, 0.0527395,
                           0.0439513, 0.0352202,  0.0266714,
                           0.019411,   0.0133974, 0.00898536,
                           0.0057516, 0.00351493, 0.00212087,
                           0.00122891, 0.00070592, 0.000384744,
                           0.000219377)

# use the latest certified data estimate
# cf. /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Pileup_2011_to_172802_LP_LumiScale.root
def getDataScenario(scenario="LP"):
    if(scenario=="Wonderland") : 
        return cms.vdouble(1,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0,  0,  0,
                           0)
    if(scenario=="LP") :
        return  cms.vdouble(0.00798674,  0.0217584,    0.0516394,
                            0.0875214,   0.117765,     0.133739,
                            0.133223,    0.119518,     0.0984248,
                            0.0754736,   0.054482,     0.0373365,
                            0.024447,    0.0153678,    0.00930695,
                            0.00544351,  0.00308009,   0.00168798,
                            0.000896724, 0.000462093,  0.00023112,
                            0.000112263, 0.0000529894, 0.00002432,
                            0.0000189525)

           
      
puWeights = cms.EDProducer("PileupNormalizationProducer",
                           integerWeightsOnly = cms.bool(False),
                           mcDistribution = getPUScenario("Summer11_S4"),
                           dataDistribution = getDataScenario("LP")
                           )

puFilter = cms.EDFilter("PileupFilter",
                        source = cms.InputTag("puWeights:renPuWeight")
                        )
