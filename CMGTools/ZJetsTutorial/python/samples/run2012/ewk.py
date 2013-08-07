## definition of the electroweak MC components.
## for each component, the following attributes are dummy,
## and will be set by the connect function:
##     - files
##     - nGenEvents
##     - triggers
##     - triggers 
## the reason for that is that these attributes depend on the final state,
## e.g. mu-mu or ele-ele, and on the set of input datasets.


import CMGTools.RootTools.fwlite.Config as cfg

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3503.71 * 1.011, # correction factor from Valentina through Jose (18/10)
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

WJets = cfg.MCComponent(
    name = 'WJets',
    files = [],
    xSection = 36257.2 ,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W1Jets = cfg.MCComponent(
    name = 'W1Jets',
    files = [],
    xSection = 6440.4, #PG inclusive scaled according to LO XS
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W2Jets = cfg.MCComponent(
    name = 'W2Jets',
    files = [],
    xSection = 2087.2, #PG inclusive scaled according to LO XS
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = [],
    xSection = 619.0, #PG inclusive scaled according to LO XS
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W4Jets = cfg.MCComponent(
    name = 'W4Jets',
    files = [],
    xSection = 255.2, #PG inclusive scaled according to LO XS
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 225.197, # correction factor from Valentina removed as it depends on the jet binning
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_tW = cfg.MCComponent(
    name = 'T_tW',
    files = [],
    xSection = 11.1, # from the sync twiki
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_tW = cfg.MCComponent(
    name = 'Tbar_tW',
    files = [],
    xSection = 11.1, # from the sync twiki
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_ewk = [
    DYJets,
    WJets,
    W1Jets,
    W2Jets,
    W3Jets,
    W4Jets,
    TTJets,
    T_tW,
    Tbar_tW
    ]

