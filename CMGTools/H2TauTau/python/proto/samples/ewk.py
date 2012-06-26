import CMGTools.RootTools.fwlite.Config as cfg

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3048.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


WJets = cfg.MCComponent(
    name = 'WJets',
    files = [],
    xSection = 31314.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = [],
    xSection = 1.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 165.8,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_ewk = [
    DYJets,
    WJets,
    W3Jets, 
    TTJets
    ]
