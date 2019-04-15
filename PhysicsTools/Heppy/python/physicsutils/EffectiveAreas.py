### effective area tables and tools

# Tables --------------------------------------
# in the form (etamax, value)
# if eta < etamax, apply this value
# else proceed to next pair
# eta should be either the eta of the supercluster or the eta of the electron,
# which is specified below

areas = dict(
    Data2012 = {
        'electron' : { '03' : 
                       [ (1.000, 0.13),
                         (1.479, 0.14),
                         (2.000, 0.07),
                         (2.200, 0.09),
                         (2.300, 0.11),
                         (2.400, 0.11),
                         (2.500, 0.14) ],
                       '04' : 
                       [ (1.000, 0.208),
                         (1.479, 0.209),
                         (2.000, 0.115),
                         (2.200, 0.143),
                         (2.300, 0.183),
                         (2.400, 0.194),
                         (2.500, 0.261) ],
                       'eta' : lambda x: x.superCluster().eta()
                       }
        },

    Phys14_25ns_v1 = { 
        'electron' : { '03' : 
                       [ (0.800, 0.1013),
                         (1.300, 0.0988),
                         (2.000, 0.0572),
                         (2.200, 0.0842), 
                         (2.500, 0.1530) ],
                       '04' : 
                       [ (0.800, 0.1830),
                         (1.300, 0.1734),
                         (2.000, 0.1077),
                         (2.200, 0.1565),
                         (2.500, 0.2680) ],
                       'eta' : lambda x : x.eta()
                       }
        },

    Spring15_50ns_v1 = { 
        'electron' : { '03' :
                           [ (0.800, 0.0973),
                             (1.300, 0.0954),
                             (2.000, 0.0632),
                             (2.200, 0.0727),
                             (2.500, 0.1337) ],
                       'eta' : lambda x: x.superCluster().eta()
                       }
        },

    Spring15_25ns_v1 = {
        'electron' : { '03' : 
                       [ (1.000, 0.1752),
                         (1.479, 0.1862),
                         (2.000, 0.1411),
                         (2.200, 0.1534),
                         (2.300, 0.1903),
                         (2.400, 0.2243),
                         (2.500, 0.2687) ],
                       'eta' : lambda x: x.superCluster().eta()
                       }
        },

    Spring16_25ns_v1 = {
        'electron' : { '03' :
                           [ (1.000, 0.1703),
                             (1.479, 0.1715),
                             (2.000, 0.1213),
                             (2.200, 0.1230),
                             (2.300, 0.1635),
                             (2.400, 0.1937),
                             (2.500, 0.2393) ],
                       'eta' : lambda x: x.superCluster().eta()
                       }
        },

    
    Fall17 = {
        'electron' : { '03' : 
                       [ (1.000, 0.1440),
                         (1.479, 0.1562),
                         (2.000, 0.1032),
                         (2.200, 0.0859),
                         (2.300, 0.1116),
                         (2.400, 0.1321),
                         (2.500, 0.1654) ],
                       'eta' : lambda x: x.superCluster().eta()
                       }
        },
    )


def effective_area_table(lepton, era):
    '''return effective area descriptor
    lepton = 'electron' or 'muon',
    era = see EffectiveAreas.py.
    '''
    leptype = None
    if abs(lepton.pdgid())==11:
        leptype = 'electron'
    elif abs(lepton.pdgid())==13:
        leptype = 'muon'
    else:
        raise ValueError( 
            'no effective areas for lepton with pdgid {}'.format(lepton.pdgid())
            )
    return areas[era][leptype]


def effective_area(lepton, cone_size, table):
    '''return effective area value.
    lepton: the lepton physics object, an electron or a muon;
    cone_size: e.g. '03' for a cone size of 0.3; 
    table: use the effective_area_table function to get it.'''
    eta_func = table['eta']
    eta = abs(eta_func(lepton))
    for etamax, ea in table[cone_size]:
        if eta < etamax: 
            return ea
    return table[cone_size][-1][1] # out of range, take last value
