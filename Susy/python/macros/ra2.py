objects = {
    'mht':'cmgBaseMETs_RA2MHTPFJet30__',
    'ht':'cmgBaseMETs_RA2MHTPFJet50Central__',
    'seles':'cmgElectrons_susyElectron__',
    'smus':'cmgMuons_susyMuon__',
    'ra2jets':'cmgPFJets_RA2PFJet50Central__',
    'ra2jet0':'cmgPFJets_RA2Jet0__',
    'ra2jet12':'cmgPFJets_RA2Jet12__',
    'ra2dphi0':'float_RA2dPhi0__',
    'ra2dphi12':'float_RA2dPhi12__'
    }

from CMGTools.Susy.macros.jetId import *

ra2 = dict( objects.items() + jetId.items() )
