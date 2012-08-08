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
    xSection = 225.197 * 0.92, # correction factor from Valentina
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_ewk = [
    DYJets,
    WJets,
    # WJetsExt,
    TTJets
    ]

## mc_ewk_up = sampleShift( locals(), [DYJets], 'Up')
## mc_ewk_down = sampleShift( locals(), [DYJets], 'Down')
