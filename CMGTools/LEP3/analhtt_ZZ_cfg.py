import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.LEP3.analhtt_cff import *


ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = ['root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_80.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_81.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_82.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_83.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_84.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_85.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_86.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_87.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_88.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_89.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_90.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_91.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_92.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_93.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_94.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_95.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_96.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_97.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_98.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/ZZ/cmgTuple_ZZ_99.root',],
    xSection = 0.003213106 * 0.863920605, 
    nGenEvents = 1, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )

selectedComponents = [ZZ]

ZZ.splitFactor = 1
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
