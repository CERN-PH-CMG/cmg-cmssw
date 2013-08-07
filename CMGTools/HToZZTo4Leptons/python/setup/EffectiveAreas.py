from CMGTools.RootTools.fwlite.Config import CFG

effectiveAreas2011 = CFG(
        name='EffAreas',
        eGamma = [ \
           {'etaMin':0, 'etaMax':1, 'area':0.180},
           {'etaMin':1, 'etaMax':1.479, 'area':0.200},
           {'etaMin':1.479, 'etaMax':2.000, 'area':0.150},
           {'etaMin':2., 'etaMax':2.2, 'area':0.190},
           {'etaMin':2.2, 'etaMax':2.3, 'area':0.210},
           {'etaMin':2.3, 'etaMax':2.4, 'area':0.220},
           {'etaMin':2.4, 'etaMax':100, 'area':0.290}],
        
        muon=[ \
           {'etaMin':0, 'etaMax':1, 'area':0.132},
           {'etaMin':1, 'etaMax':1.479, 'area':0.120},
           {'etaMin':1.479, 'etaMax':2.000, 'area':0.114},
           {'etaMin':2., 'etaMax':2.2, 'area':0.139},
           {'etaMin':2.2, 'etaMax':2.3, 'area':0.168},
           {'etaMin':2.3, 'etaMax':100., 'area':0.189}]
)        


effectiveAreas2012 = CFG(
        name='EffAreas',
        eGamma = [ \
           {'etaMin':0, 'etaMax':1, 'area':0.19},
           {'etaMin':1, 'etaMax':1.479, 'area':0.25},
           {'etaMin':1.479, 'etaMax':2.000, 'area':0.12},
           {'etaMin':2., 'etaMax':2.2, 'area':0.210},
           {'etaMin':2.2, 'etaMax':2.3, 'area':0.270},
           {'etaMin':2.3, 'etaMax':2.4, 'area':0.44},
           {'etaMin':2.4, 'etaMax':100, 'area':0.52}],
        
        muon=[ \
           {'etaMin':0, 'etaMax':1, 'area':0.674},
           {'etaMin':1, 'etaMax':1.479, 'area':0.565},
           {'etaMin':1.479, 'etaMax':2.000, 'area':0.442},
           {'etaMin':2., 'etaMax':2.2, 'area':0.515},
           {'etaMin':2.2, 'etaMax':2.3, 'area':0.821},
           {'etaMin':2.3, 'etaMax':100., 'area':0.660}]
)        
