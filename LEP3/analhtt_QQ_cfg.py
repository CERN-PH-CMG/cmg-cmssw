import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.LEP3.analhtt_cff import *


Hig125 = cfg.MCComponent(
    name = 'QQ',
    files = ['root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_270.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_271.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_272.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_273.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_274.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_275.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_276.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_277.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_278.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_279.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_280.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_281.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_282.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_283.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_284.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_285.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_286.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_287.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_288.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_289.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_290.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_291.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_292.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_293.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_294.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_295.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_296.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_297.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_298.root',
             'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_299.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_21.root',
             #'root://eoscms//eos/cms/store/cmst3/user/pjanot/LEP3/QQBAR/cmgTuple_QQBAR_33.root',
             ],
    xSection = 0.003213106 * 0.863920605, 
    nGenEvents = 1, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )

selectedComponents = [QQ]

QQ.splitFactor = 1
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
