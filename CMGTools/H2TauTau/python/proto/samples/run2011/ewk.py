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
    xSection = 31314.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W1Jets = cfg.MCComponent(
    name = 'W1Jets',
    files = [],
    xSection = 1.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

W2Jets = cfg.MCComponent(
    name = 'W2Jets',
    files = [],
    xSection = 1.,
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

W4Jets = cfg.MCComponent(
    name = 'W4Jets',
    files = [],
    xSection = 1.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 164.4 * valentinaFactor,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_s = cfg.MCComponent(
    name = 'T_s',
    files = [],
    xSection = 2.72,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_t = cfg.MCComponent(
    name = 'T_t',
    files = [],
    xSection = 42.6,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_tW = cfg.MCComponent(
    name = 'T_tW',
    files = [],
    xSection = 5.3,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_s = cfg.MCComponent(
    name = 'Tbar_s',
    files = [],
    xSection = 1.49,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_t = cfg.MCComponent(
    name = 'Tbar_t',
    files = [],
    xSection = 22.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_tW = cfg.MCComponent(
    name = 'Tbar_tW',
    files = [],
    xSection = 5.3,
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
#    T_s,
#    T_t,
    T_tW,
    Tbar_tW,
#    Tbar_s,
#    Tbar_t,
    ]


