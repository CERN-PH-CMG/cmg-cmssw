import CMGTools.RootTools.fwlite.Config as cfg
import copy

# the luminosities for these components correspond to the
# HLT triggers used in the H->tau tau analysis
# they might differ from the luminosities you want for a
# Z-> mu mu or Z-> ee analysis.

data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 |       2809 |  96.404(/pb) |  96.404(/pb) |
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       5258 | 316.016(/pb) | 316.016(/pb) |
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 |       6514 | 395.991(/pb) | 395.991(/pb) |
    intLumi = 806.194, # 806.194 from Colin
    triggers = [],
    json = None
    )

data_Run2012A_aug6 = cfg.DataComponent( # update
    name = 'data_Run2012A_aug6',
    files = [],
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       1810 | 82.136(/pb) | 82.136(/pb) |
    intLumi = 82.136, # same from Colin
    triggers = [],
    json = None
    )

data_Run2012B = cfg.DataComponent(
    name = 'data_Run2012B',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 |      51089 | 4.428(/fb) | 4.428(/fb) |
    intLumi = 4420., # 4420. from Colin
    triggers = [],
    json = None
    )

data_Run2012C_v1 = cfg.DataComponent(
    name = 'data_Run2012C_v1',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |       6719 | 495.003(/pb) | 495.003(/pb) |
    intLumi = 493.975, # 493.975 from Colin
    triggers = [],
    json = None
    )

data_Run2012C_v2 = cfg.DataComponent( # update
    name = 'data_Run2012C_v2',
    files = [],
    intLumi = 6397, #  from Colin
    triggers = [],
    json = None
    )

data_list = [
    data_Run2012A,
    data_Run2012A_aug6,
    data_Run2012B,
    data_Run2012C_v1,
    data_Run2012C_v2,
    ]


