import FWCore.ParameterSet.Config as cms
from HiggsAnalysis.HiggsToWW2Leptons.HWWKFactorProducer_cfi import KFactorProducer
from math import floor

###
### standard generator level sequence
###
def addGeneratorLevelSequence(process) :
    
    process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
    process.prunedGen = cms.EDProducer( "GenParticlePruner",
                                        src = cms.InputTag("genParticles"),
                                        select = cms.vstring("drop  *  ",
                                                             "keep++ pdgId =   {Z0}",
                                                             "++keep pdgId =   {Z0}",
                                                             "keep++ pdgId =   {W+}",
                                                             "++keep pdgId =   {W+}",
                                                             "keep++ pdgId =   {W-}",
                                                             "++keep pdgId =   {W-}",
                                                             "keep++ pdgId =   {h0}",
                                                             "++keep pdgId =   {h0}",
                                                             "keep++ pdgId =   {e+}",
                                                             "++keep pdgId =   {e+}",
                                                             "keep++ pdgId =   {e-}",
                                                             "++keep pdgId =   {e-}",
                                                             "keep++ pdgId =  {mu+}",
                                                             "++keep pdgId =  {mu+}",
                                                             "keep++ pdgId =  {mu-}",
                                                             "++keep pdgId =  {mu-}",
                                                             "++keep pdgId =      6",
                                                             "keep++ pdgId =      6",
                                                             "keep++ pdgId =     -6",
                                                             "++keep pdgId =     -6",
                                                             "++keep pdgId =      5",
                                                             "++keep pdgId =     -5",
                                                             "++keep pdgId =      4",
                                                             "++keep pdgId =     -4",
                                                             "++keep pdgId =     12",
                                                             "++keep pdgId =     14",
                                                             "++keep pdgId =     16",
                                                             "++keep pdgId =    -12",
                                                             "++keep pdgId =    -14",
                                                             "++keep pdgId =    -16",
                                                             "++keep pdgId = {tau+}",
                                                             "++keep pdgId = {tau-}",
                                                             )
                                        )

    process.genLevelPath = cms.Path(process.prunedGen)


##
## adds the standard Powheg pT spectrum reweighting
##
def addHiggsPtReweighting(process,dtag):
    mh=200
    
    stdFile='HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_Std/kfactors_mh'+str(int(mh))+'_ren'+str(int(mh))+'_fac'+str(int(mh))+'.dat'
    process.hKfactorStd   = KFactorProducer.clone( genParticlesTag = cms.InputTag('prunedGen'),
                                                   Debug = cms.untracked.bool(False),
                                                   ProcessID = cms.untracked.int32(10011),
                                                   inputFilename=cms.untracked.string(stdFile) )
    
    rupFile='HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_ScaleVariations/kfactors_mh'+str(int(mh))+'_ren'+str(int(2*mh))+'_fac'+str(int(mh))+'.dat'
    process.hKfactorRup   = process.hKfactorStd.clone(inputFilename=cms.untracked.string(rupFile))
    rdownFile='HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_ScaleVariations/kfactors_mh'+str(int(mh))+'_ren'+str(int(floor(0.5*mh)))+'_fac'+str(int(mh))+'.dat'
    process.hKfactorRdown = process.hKfactorStd.clone(inputFilename=cms.untracked.string(rdownFile) )
    fupFile='HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_ScaleVariations/kfactors_mh'+str(int(mh))+'_ren'+str(int(mh))+'_fac'+str(int(2*mh))+'.dat'    
    process.hKfactorFup   = process.hKfactorStd.clone(inputFilename=cms.untracked.string(fupFile) )
    fdownFile='HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_ScaleVariations/kfactors_mh'+str(int(mh))+'_ren'+str(int(mh))+'_fac' + str(int(floor(0.5*mh)))+'.dat'
    process.hKfactorFdown = process.hKfactorStd.clone(inputFilename=cms.untracked.string(fdownFile) )

    print 'H pT reweighting sequence is defined from the following files'
    print stdFile
    print rupFile
    print rdownFile
    print fupFile
    print fdownFile
    process.hkfactorSequence = cms.Sequence( process.hKfactorStd * process.hKfactorRup * process.hKfactorRdown * process.hKfactorFup * process.hKfactorFdown )
    return True


