import CMGTools.RootTools.fwlite.Config as cfg

QCD15 = cfg.MCComponent(
    name = 'QCD15',
    files = [],
    xSection = 8.159e08,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD30 = cfg.MCComponent(
    name = 'QCD30',
    files = [],
    xSection = 5.312e07,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD50 = cfg.MCComponent(
    name = 'QCD50',
    files = [],
    xSection = 6.359e06,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD80 = cfg.MCComponent(
    name = 'QCD80',
    files = [],
    xSection = 7.843e05,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_qcd = [
    QCD15,
    QCD30,
    QCD50,
    QCD80
    ]
