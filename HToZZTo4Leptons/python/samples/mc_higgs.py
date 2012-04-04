import CMGTools.RootTools.fwlite.Config as cfg

# x sections etc are here: 
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Torino/ZZAnalysis/AnalysisStep/test/Plotter/Xsection_v1.txt?revision=1.12&view=markup


Hig120GluGlu = cfg.MCComponent(
    name = 'Hig120GluGlu',
    files = [],
    xSection = 0.003213106 * 0.863920605, 
    nGenEvents = 1, # dummy 
    triggers = [],
    intLumi = 1000,
    effCorrFactor = 1 )


mc_higgs = [
    Hig120GluGlu
    ]

