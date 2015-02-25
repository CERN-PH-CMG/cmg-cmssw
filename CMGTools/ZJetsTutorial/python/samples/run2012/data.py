import CMGTools.RootTools.fwlite.Config as cfg
import copy

# the luminosities for these components correspond to the
# HLT triggers used in the H->tau tau analysis
# they might differ from the luminosities you want for a
# Z-> mu mu or Z-> ee analysis.

data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    intLumi = 890., # 806.194 from Colin
    triggers = [],
    json = None
    )


data_Run2012B = cfg.DataComponent(
    name = 'data_Run2012B',
    files = [],
    intLumi = 4429.,
    triggers = [],
    json = None
    )

data_Run2012C = cfg.DataComponent(
    name = 'data_Run2012C',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |       6719 | 495.003(/pb) | 495.003(/pb) |
    intLumi = 7153.,
    triggers = [],
    json = None
    )

data_Run2012D = cfg.DataComponent(
    name = 'data_Run2012D',
    files = [],
    intLumi = 7318., # I don't know the lumi 
    triggers = [],
    json = None
    )

data_list = [
    data_Run2012A,
    data_Run2012B,
    data_Run2012C,
    data_Run2012D,
    ]


