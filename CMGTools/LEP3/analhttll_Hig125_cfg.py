import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.LEP3.analhttll_cff import *


Hig125 = cfg.MCComponent(
    name = 'Hig125',
    files = ['root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_0.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_1.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_2.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_3.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_4.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_5.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_6.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_7.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_8.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/cmgTuple_HZHA_9.root',],
    xSection = 0.003213106 * 0.863920605, 
    nGenEvents = 1, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )

selectedComponents = [Hig125]

Hig125.splitFactor = 1
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
