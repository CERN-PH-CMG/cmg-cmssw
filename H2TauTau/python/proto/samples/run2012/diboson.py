import CMGTools.RootTools.fwlite.Config as cfg

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 57.1097, # correction factor from Valentina
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 32.3161,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 8.25561, # correction factor from Valentina
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_diboson = [
    WW,
    WZ,
    ZZ
    ]

