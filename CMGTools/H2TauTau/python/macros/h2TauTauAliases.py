diTauAliases = {
    'tauMuAll':'cmgTaucmgMuoncmgDiObjects_cmgTauMu__',
    'tauMu':'cmgTaucmgMuoncmgDiObjects_cmgTauMuSel__',
    'tauE':'cmgTaucmgElectroncmgDiObjects_cmgTauESel__', 
    'diTau':'cmgTaucmgTaucmgDiObjects_cmgDiTauSel__'    
    }

tauMuBase = {
    'tauMuBase':'cmgTaucmgMuoncmgDiObjects_cmgTauMuBaselineSel__',
    }

h2TauTauAliases = dict(  diTauAliases.items() + tauMuBase.items() )
