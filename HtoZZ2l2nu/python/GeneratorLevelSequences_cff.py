import FWCore.ParameterSet.Config as cms
import re

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


def parseHiggsMass(url) :
    mh=-1

    #reweighting applies to powheg gg->H processes
    if(url.find('GluGluToH')<0 or url.find('powheg')<0) :  return mh
    
    #find the mass (typ. GluGluToHToPotatoesToFriedPotatoes_M-***_7TeV-powheg-pythia6 )
    url.split('M-')
    substring = re.findall(r'M-?([^\_>]+)',url)
    if(len(substring)==0) :
        print 'Warning could not retrieve the Higgs mass from: ' + url
        return False
    mh=int(substring[0])

    return mh
