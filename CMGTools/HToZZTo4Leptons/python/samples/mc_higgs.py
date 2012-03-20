import CMGTools.RootTools.fwlite.Config as cfg



Hig120GluGlu = cfg.MCComponent(
    name = 'Hig120GluGlu',
    files = [],
    xSection = 1., # dummy 
    nGenEvents = 1, # dummy 
    triggers = [],
    effCorrFactor = 1 )


mc_higgs = [
    Hig120GluGlu
    ]
