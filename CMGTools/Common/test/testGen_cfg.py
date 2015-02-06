from PhysicsTools.PatAlgos.patTemplate_cfg import *



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )


process.load('CMGTools.Common.gen_cff')

process.drawTree = cms.EDAnalyzer("ParticleTreeDrawer",
                                   src = cms.InputTag("genParticlesStatus3"),                                                                 
                                   printP4 = cms.untracked.bool(False),
                                   printPtEtaPhi = cms.untracked.bool(False),
                                   printVertex = cms.untracked.bool(False),
                                   printStatus = cms.untracked.bool(False),
                                   printIndex = cms.untracked.bool(True),
                                   # status = cms.untracked.vint32( 3 )
                                   )

process.printDecay = cms.EDAnalyzer(
    "ParticleDecayDrawer",
    src = cms.InputTag("genParticlesStatus3"),
    printP4 = cms.untracked.bool(False),
    printPtEtaPhi = cms.untracked.bool(False),
    printVertex = cms.untracked.bool(False)
    )

#COLIN: both ParticleTreeDrawer and ParticleDecayDrawer produce a strange printout...
# I guess these modules are bugged. 

process.p = cms.Path(
    process.genSequence 
    + process.listParticles
    # + process.drawTree
    # + process.printDecay
    )

from CMGTools.Common.eventContent.gen_cff import gen 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( gen ) 
process.out.fileName = cms.untracked.string('tree_testGen.root')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
