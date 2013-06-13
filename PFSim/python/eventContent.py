
gen = [
    'keep recoGenParticles_genParticles_*_*',
    'keep recoGenJets_ak5GenJets_*_*'
    ]

reco = [
    'keep recoPFCandidates_particleFlow_*_*',
    'keep recoGenParticles_genParticles_*_*',
    'keep recoPFJets_ak5PFJets_*_*',
    'keep recoPFMETs_pfMet_*_*',
    ]

sim = [
    'keep *_*_*_Sim',
    ]


simple = gen + reco + sim
