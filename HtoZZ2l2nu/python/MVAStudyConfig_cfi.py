import FWCore.ParameterSet.Config as cms

mass=200

# simple study
simpleDiscriminator = cms.PSet(  evCategories = cms.vint32(0,1,2,3),
                                 input      = cms.string("EventSummaries.root"),
                                 studyTag   = cms.string("simpleDiscriminator_%d" % mass),
                                 weightsDir = cms.string("${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/weights"),
                                 methodList = cms.vstring('Likelihood','BDT'),
                                 varsList   = cms.vstring('drll', 'dphizleadl', 'mtsum'),
                                 procList   = cms.vstring('GGtoH%dtoZZto2L2Nu' % mass,
                                                          'GGtoH%dtoWWto2L2Nu' % mass,
                                                          'VBFtoH%dtoZZto2L2Nu' % mass,
                                                          'VBFtoH%dtoWWto2L2Nu' % mass,
                                                          'WW',
                                                          'WZ',
                                                          'ZZ'),
                                 procType   = cms.vint32 (1,1,1,1,2,2,2),
                                 procWeight = cms.vdouble(1,1,1,1,1,1,1)
                                 )

if(mass==200): simpleDiscriminator.varsList   = cms.vstring('dphizleadl', 'mtsum','redMetL')
