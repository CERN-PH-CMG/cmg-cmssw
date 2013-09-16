import FWCore.ParameterSet.Config as cms

genParticlesPruned = cms.EDProducer(
   "GenParticlePruner",
   src = cms.InputTag("genParticles"),
   select = cms.vstring(
       "drop  *  ",
       "keep status()==3",
       "keep++ pdgId =   {e+}",
       "++keep pdgId =   {e+}",
       "keep++ pdgId =   {e-}",
       "++keep pdgId =   {e-}",
       "keep++ pdgId =  {mu+}",
       "++keep pdgId =  {mu+}",
       "keep++ pdgId =  {mu-}",
       "++keep pdgId =  {mu-}",
       "keep++ pdgId = {tau+}",
       "++keep pdgId = {tau+}",
       "keep++ pdgId = {tau-}",
       "++keep pdgId = {tau-}",
       "keep status = 2 & pdgId = 4",
       "keep status = 2 & pdgId = -4",
       "keep status = 2 & pdgId = 5",
       "keep status = 2 & pdgId = -5",
             
   )
)
