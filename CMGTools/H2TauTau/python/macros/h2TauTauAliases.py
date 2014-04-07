tauMuAliases = {
    'tauMuUncAll':'cmgTaucmgMuoncmgDiObjects_cmgTauMu__',
    'tauMuUncPreSel':'cmgTaucmgMuoncmgDiObjects_cmgTauMuPreSel__',
    'tauMuUncFullSel':'cmgTaucmgMuoncmgDiObjects_cmgTauMuFullSel__',
    'tauMuAll':'cmgTaucmgMuoncmgDiObjects_cmgTauMuCor__',
    'tauMuPreSel':'cmgTaucmgMuoncmgDiObjects_cmgTauMuCorSVFitPreSel__',
    'tauMuFullSel':'cmgTaucmgMuoncmgDiObjects_cmgTauMuCorSVFitFullSel__',
    }

tauEleAliases = {
    'tauEleUncAll':'cmgTaucmgElectroncmgDiObjects_cmgTauEle__',
    'tauEleUncPreSel':'cmgTaucmgElectroncmgDiObjects_cmgTauElePreSel__',
    'tauEleUncFullSel':'cmgTaucmgElectroncmgDiObjects_cmgTauEleFullSel__',
    'tauEleAll':'cmgTaucmgElectroncmgDiObjects_cmgTauEleCor__',
    'tauElePreSel':'cmgTaucmgElectroncmgDiObjects_cmgTauEleCorSVFitPreSel__',
    'tauEleFullSel':'cmgTaucmgElectroncmgDiObjects_cmgTauEleCorSVFitFullSel__',
    }




h2TauTauAliases = dict(  tauMuAliases.items() + tauEleAliases.items() )
