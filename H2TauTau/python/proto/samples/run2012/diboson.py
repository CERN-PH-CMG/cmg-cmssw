import CMGTools.RootTools.fwlite.Config as cfg

# exclusive madgraph samples
# -- -- -- -- -- -- -- -- 

WW2l2v = cfg.MCComponent(
    name = 'WW2l2v',
    files = [],
    xSection = 5.824, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

WZ2l2q = cfg.MCComponent(
    name = 'WZ2l2q',
    files = [],
    xSection = 2.207, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

WZ3lv = cfg.MCComponent(
    name = 'WZ3lv',
    files = [],
    xSection = 1.058, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

ZZ2l2q = cfg.MCComponent(
    name = 'ZZ2l2q',
    files = [],
    xSection = 2.502, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

ZZ2l2v = cfg.MCComponent(
    name = 'ZZ2l2v',
    files = [],
    xSection = 0.716, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

ZZ4l = cfg.MCComponent(
    name = 'ZZ4l',
    files = [],
    xSection = 0.181, #PG from twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorkingSummer2012#MC_samples_and_cross_sections
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

mc_diboson_xcl = [
    WW2l2v,
    WZ2l2q,
    WZ3lv,
    ZZ2l2q,
    ZZ2l2v,
    ZZ4l
    ]


# inclusive pythia samples
# -- -- -- -- -- -- -- -- 

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
#    xSection = 57.1097, # correction factor from Valentina
    xSection = 54.838, #PG numbers from Andrew
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
#    xSection = 32.3161,
#    xSection = 32.3161 * 0.97, #PG scale factor wrt exclusive samples XS
    xSection = 33.21, #PG number from Andrew
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
#    xSection = 8.25561, # correction factor from Valentina
#    xSection = 8.3 * 2.13, #PG scale factor wrt exclusive samples XS
    xSection = 17.654, #PG number from Andrew
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_diboson = [
    WW,
    WZ,
    ZZ
    ]

