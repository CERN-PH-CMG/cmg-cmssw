import CMGTools.RootTools.fwlite.Config as cfg

valentinaFactor = 0.95

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3048. * valentinaFactor,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


WJets = cfg.MCComponent(
    name = 'WJets',
    files = [],
    xSection = 31314. * valentinaFactor,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = [],
    xSection = 1. * valentinaFactor,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 165.8 * valentinaFactor,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_ewk = [
    DYJets,
    WJets,
    W3Jets, 
    TTJets
    ]
