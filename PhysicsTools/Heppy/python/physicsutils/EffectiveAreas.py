### effective area tables and tools

# Tables --------------------------------------
# in the form (etamax, value)
# if eta < etamax, apply this value
# else proceed to next pair
# eta should be either the eta of the supercluster or the eta of the electron,
# which is specified below

areas = dict(
    Data2012 = {
        # the value corresponding to the electron key is called an 
        # "area table" in what follows
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

    scaling_03_04 = 16./9.
    Fall17 = {
        # https://github.com/cms-sw/cmssw/blob/CMSSW_9_4_X/RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_92X.txt
        'electron' : { '03' : 
                       [ (1.000, 0.1566),
                         (1.479, 0.1626),
                         (2.000, 0.1073),
                         (2.200, 0.0854),
                         (2.300, 0.1051),
                         (2.400, 0.1204),
                         (2.500, 0.1524) ],
                       '04' :  # warning: EAs not computed for cone DR=0.4, use the values for DR=0.3 scaled by 16/9 instead
                       [ (1.000, 0.1566 * scaling_03_04),
                         (1.479, 0.1626 * scaling_03_04),
                         (2.000, 0.1073 * scaling_03_04),
                         (2.200, 0.0854 * scaling_03_04),
                         (2.300, 0.1051 * scaling_03_04),
                         (2.400, 0.1204 * scaling_03_04),
                         (2.500, 0.1524 * scaling_03_04) ]
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

def is_ea_table(candidate): 
    '''return True if candidate is an area table'''
    if not hasattr(candidate, '__iter__'):
        return False
    if not 'eta' in candidate: 
        return False
    for key, val in candidate.iteritems():
        if key == 'eta': 
            continue
        else: 
            for ea in val:
                if len(ea) != 2: 
                    return False
    return True
