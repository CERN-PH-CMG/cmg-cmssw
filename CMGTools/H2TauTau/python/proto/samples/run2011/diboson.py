import CMGTools.RootTools.fwlite.Config as cfg

# exclusive madgraph samples
# -- -- -- -- -- -- -- -- 

WWJetsTo2L2Nu = cfg.MCComponent(
    name = 'WWJetsTo2L2Nu',
    files = [],
    xSection = 4.514,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZJetsTo2L2Q = cfg.MCComponent(
    name = 'WZJetsTo2L2Q',
    files = [],
    xSection = 1.243,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZJetsTo3LNu = cfg.MCComponent(
    name = 'WZJetsTo3LNu',
    files = [],
    xSection = 0.596,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZJetsTo2L2Nu = cfg.MCComponent(
    name = 'ZZJetsTo2L2Nu',
    files = [],
    # xSection = 0.119,
    xSection = 0.283, # from Andrew, not checked
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZJetsTo2L2Q = cfg.MCComponent(
    name = 'ZZJetsTo2L2Q',
    files = [],
    # xSection = 0.417,
    xSection = 0.833, # from Andrew, not checked
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZJetsTo4L = cfg.MCComponent(
    name = 'ZZJetsTo4L',
    files = [],
    xSection = 0.060,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


mc_diboson_xcl = [
    WWJetsTo2L2Nu,
    WZJetsTo2L2Q,
    WZJetsTo3LNu,
    ZZJetsTo2L2Nu,
    ZZJetsTo2L2Q,
    ZZJetsTo4L
    ]


# inclusive pythia samples
# -- -- -- -- -- -- -- -- 

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 43.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 18.2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 5.9,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


mc_diboson = [
    WWJetsTo2L2Nu,
    WZJetsTo2L2Q,
    WZJetsTo3LNu,
    ZZJetsTo2L2Nu,
    ZZJetsTo2L2Q,
    ZZJetsTo4L,
#     WW,
#     WZ,
#    ZZ
    ]

