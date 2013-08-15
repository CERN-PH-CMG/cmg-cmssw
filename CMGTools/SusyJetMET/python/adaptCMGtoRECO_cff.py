from PhysicsTools.PatAlgos.patTemplate_cfg import *

def adaptCMGtoRECO( process ):
    process.cmgPFJet.cfg.inputCollection = 'ak5PFJets'
    process.cmgPFBaseJet.cfg.inputCollection = 'ak5PFJets'
    process.cmgCaloBaseJet.cfg.inputCollection = 'ak5CaloJets'

    process.cmgMHTPFJets.cfg.inputCollection = 'ak5PFJets'
    # warning no jet correction here!
    process.cmgMHTCaloJets.cfg.inputCollection = 'ak5CaloJets'


