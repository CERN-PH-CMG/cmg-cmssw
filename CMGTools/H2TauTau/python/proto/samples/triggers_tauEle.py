blah = 'CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT'

# COLIN FIXME: wildcards for v* should work, look at the effect. If it works, migrate

data_triggers_2011A = [
    # the first 2 triggers correspond to runs before the good run list
    # why do we get them??
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v1'.format(blah=blah), 
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v2'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau15_v4'.format(blah=blah),  
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v6'.format(blah=blah),  
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v8'.format(blah=blah),  
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v9'.format(blah=blah),  
    # 'HLT_Ele15_{blah}_LooseIsoPFTau20_v4'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v6'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v8'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v9'.format(blah=blah),  
    # 'HLT_Ele15_{blah}_TightIsoPFTau20_v1'.format(blah=blah),
    'HLT_Ele15_{blah}_TightIsoPFTau20_v2'.format(blah=blah),
    'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah),
    # 'HLT_Ele18_{blah}_MediumIsoPFTau20_v2'.format(blah=blah),
    # 'HLT_Ele18_{blah}_MediumIsoPFTau20_v3'.format(blah=blah),
    # 'HLT_Ele18_{blah}_MediumIsoPFTau20_v4'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v1'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v2'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v3'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v4'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v5'.format(blah=blah),
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v6'.format(blah=blah)
    ]

data_triggers_2011B = [
    'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah), 
    # 'HLT_Ele20_{blah}_MediumIsoPFTau20_v1'.format(blah=blah), 
    'HLT_Ele20_{blah}_MediumIsoPFTau20_v5'.format(blah=blah), 
    'HLT_Ele20_{blah}_MediumIsoPFTau20_v6'.format(blah=blah)
    ]

mc_triggers_fall11 = [
    'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah)
    ]

mc_triggers = mc_triggers_fall11
