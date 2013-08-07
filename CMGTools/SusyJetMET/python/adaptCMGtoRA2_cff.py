from PhysicsTools.PatAlgos.patTemplate_cfg import *

def adaptCMGtoRA2( process ):
    process.cmgPFJet.cfg.inputCollection = 'patJetsAK5PF'
    process.cmgPFBaseJet.cfg.inputCollection = 'patJetsAK5PF'
    process.cmgCaloBaseJet.cfg.inputCollection = 'patJetsAK5Calo'

    # process.cmgMHTPFJets.cfg.inputCollection = 'patJetsAK5PF'
    # process.cmgMHTCaloJets.cfg.inputCollection = 'patJetsAK5Calo'


