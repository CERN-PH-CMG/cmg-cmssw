
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
    'keep recoLeafCandidates_pfsim_*_*',
    'keep recoLeafCandidates_jets_*_*',
    ]


simple = gen + reco + sim
