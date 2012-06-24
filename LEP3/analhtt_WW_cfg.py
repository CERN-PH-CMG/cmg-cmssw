import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.LEP3.analhtt_cff import *


WW = cfg.MCComponent(
    name = 'WW',
    files = ['root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_170.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_171.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_172.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_173.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_174.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_175.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_176.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_177.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_178.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_179.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_180.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_181.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_182.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_183.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_184.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_185.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_186.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_187.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_188.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_189.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_190.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_191.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_192.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_193.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_194.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_195.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_196.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_197.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_198.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_199.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_93.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_179.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/WW/cmgTuple_WW_191.root',
             ],
    xSection = 0.003213106 * 0.863920605, 
    nGenEvents = 1, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )

selectedComponents = [WW]

WW.splitFactor = 1
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
