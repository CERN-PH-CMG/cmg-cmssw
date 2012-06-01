import CMGTools.RootTools.fwlite.Config as cfg

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3503.71,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


WJets = cfg.MCComponent(
    name = 'WJets',
    files = [],
    xSection = 36257.2 ,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 225.197,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_ewk = [
    DYJets,
    WJets,
    TTJets
    ]
