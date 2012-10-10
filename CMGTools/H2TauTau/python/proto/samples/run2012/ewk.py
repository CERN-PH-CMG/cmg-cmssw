import CMGTools.RootTools.fwlite.Config as cfg
# from CMGTools.H2TauTau.proto.samples.sampleShift import sampleShift

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3503.71 * 0.976, # correction factor from Valentina
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

## WJets = cfg.MCComponent(
##     name = 'WJets',
##     files = [],
##     xSection = WJets.xSection ,
##     nGenEvents = 1,
##     triggers = [],
##     effCorrFactor = 1 )


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
    # WJetsExt,
    TTJets,
    T_tW,
    Tbar_tW
    ]

## mc_ewk_up = sampleShift( locals(), [DYJets], 'Up')
## mc_ewk_down = sampleShift( locals(), [DYJets], 'Down')
