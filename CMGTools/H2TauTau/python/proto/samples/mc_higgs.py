import CMGTools.RootTools.fwlite.Config as cfg


Hig105 = cfg.MCComponent(
    name = 'Hig105',
    files = [],
    xSection = 1., # dummy 
    nGenEvents = 1, # dummy 
    triggers = [],
    effCorrFactor = 1 )



mc_higgs = [
    Hig105
    ]
