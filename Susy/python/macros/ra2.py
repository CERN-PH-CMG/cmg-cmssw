objects = {
    'mht':'cmgBaseMETs_RA2MHTPFJet30__',
    'ht':'cmgBaseMETs_RA2MHTPFJet50Central__',
    'seles':'cmgElectrons_susyElectron__',
    'smus':'cmgMuons_susyMuon__',
    'ra2jets':'cmgPFJets_RA2PFJet50Central__'
    }

from CMGTools.Susy.macros.jetId import *

ra2 = dict( objects.items() + jetId.items() )
