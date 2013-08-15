import CMGTools.RootTools.fwlite.Config as cfg
# from CMGTools.H2TauTau.proto.samples.sampleShift import sampleShift

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3503.71 * 1.011, # correction factor from Valentina through Jose (18/10)
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

DY1Jets = cfg.MCComponent(
    name = 'DY1Jets',
    files = [],
    xSection = 561.0 * 1.011, # correction factor from Valentina through Jose (18/10)
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

DY2Jets = cfg.MCComponent(
    name = 'DY2Jets',
    files = [],
    xSection = 181. * 1.011, # correction factor from Valentina through Jose (18/10)
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

DY3Jets = cfg.MCComponent(
    name = 'DY3Jets',
    files = [],
    xSection = 51.1 * 1.011, # correction factor from Valentina through Jose (18/10)
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

DY4Jets = cfg.MCComponent(
    name = 'DY4Jets',
    files = [],
    xSection = 23.04 * 1.011, # correction factor from Valentina through Jose (18/10)
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



mc_dy = [
    DYJets,
    DY1Jets,
    DY2Jets,
    DY3Jets,
    DY4Jets,
    ]

mc_w = [
    WJets,
    W1Jets,
    W2Jets,
    W3Jets,
    W4Jets,
    ]

t_mc_ewk = [
    TTJets,
    T_tW,
    Tbar_tW,
    ]


mc_ewk = []
mc_ewk += mc_dy
mc_ewk += mc_w
mc_ewk += t_mc_ewk


# for backward compatibility:
ztt_mc_ewk = mc_dy
ztt_inc_mc_ewk = [DYJets]
w_mc_ewk = mc_w


#stitching:

# from COLIN, measured on inclusive DYJets sample, before any selection.
dy_fractions = [ 0.72328,
                 0.188645,
                 0.0613196,
                 0.0188489,
                 0.00790643
                 ]

for dy in mc_dy:
    dy.fractions = dy_fractions


# from Jose
w_fractions = [ 0.74392452,
                0.175999,
                0.0562617,
                0.0168926,
                0.00692218
                ]

for w in mc_w:
    w.fractions = w_fractions

#COLIN: need to do the same for WJets
