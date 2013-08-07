import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

tautaullAna = cfg.Analyzer(
    'TauTauLLAnalyzer',
    )
def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['TauTauLLTreeProducer','TauTauLLAnalyzer'] ),
                       anaName = 'TauTauLLAnalyzer'
                       )
    return tp


# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

sequence = cfg.Sequence([
    tautaullAna,
    createTreeProducer( tautaullAna ),
    ])

