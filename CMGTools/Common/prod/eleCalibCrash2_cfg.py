from PhysicsTools.PatAlgos.patTemplate_cfg import *

runOnMC = True

process.source.fileNames = ['/store/relval/CMSSW_5_2_0/RelValProdTTbar/AODSIM/START52_V4A-v1/0250/68FCD498-F969-E111-9366-002618943949.root']


process.stdElectronSeq = cms.Sequence(
    process.makePatElectrons 
    )

process.p = cms.Path( process.stdElectronSeq ) 

process.patElectrons.embedTrack = True

process.load("EgammaCalibratedGsfElectrons.CalibratedElectronProducers.calibratedGsfElectrons_cfi")

# print sep_line
print "Replacing gsfElectrons with calibratedGsfElectrons..."
for modulename in process.p.moduleNames():
    module = getattr(process, modulename)
    ml = dir(module)
    for attr in ml:
        v = getattr(module,attr)
        if (isinstance(v, cms.InputTag) and v == cms.InputTag("gsfElectrons")):
            setattr(module,attr,"calibratedGsfElectrons")
            #print "Setting ",
            #print module,
            #print ".", 
            #print attr,
            #print " = ", 
            #print getattr(module,attr)
process.calibratedGsfElectrons.isMC = cms.bool(runOnMC)
process.calibratedGsfElectrons.updateEnergyError = cms.bool(True)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",

    # Include a PSet for each module label that needs a
    # random engine.  The name is the module label.
    # You must supply a seed or seeds.
    # Optionally an engine type can be specified
    #t1 = cms.PSet(
    #    initialSeed = cms.untracked.uint32(81)
    #),
    calibratedGsfElectrons = cms.PSet(
        initialSeed = cms.untracked.uint32(1),
        engineName = cms.untracked.string('TRandom3')
    ),
)

from CMGTools.Common.Tools.setupEleEnergyCorrections import setupEleEnergyCorrections
setupEleEnergyCorrections(process)

print "Setting process.calibratedGsfElectrons.inputDataset=",
print process.calibratedGsfElectrons.inputDataset


process.p.replace(process.electronMatch,
                  process.calibratedGsfElectrons + process.electronMatch )
