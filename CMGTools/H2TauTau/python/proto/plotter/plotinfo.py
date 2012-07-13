class PlotInfo(object):
    def __init__(self, var, nbins, xmin, xmax):
        self.var = var
        self.nbins = nbins
        self.xmin = xmin
        self.xmax = xmax
        

plots_All = {
    'l2_pt': PlotInfo('l2_pt', 50, 0, 200),
    'l2_eta': PlotInfo('l2_eta', 20, -3, 3),
    'l2_relIso05': PlotInfo('l2_relIso05', 50, 0, 0.1),
    'l1_pt': PlotInfo('l1_pt', 50, 0, 200),
    'l1_eta': PlotInfo('l1_eta', 20, -3, 3),
    # 'mt': PlotInfo('mt', 50, 0, 150),
    'visMass': PlotInfo('visMass', 30, 0, 210),
    'svfitMass': PlotInfo('svfitMass', 30, 0, 300),
    'nJets': PlotInfo('nJets', 5, 0, 5),
    'nVert': PlotInfo('nVert', 20, 0, 40)
    }

plots_J1 = {
    'jet1_pt': PlotInfo('jet1_pt', 30, 0, 300),
    'jet1_eta': PlotInfo('jet1_eta', 10, -5, 5),
    'svfitMass': PlotInfo('svfitMass', 30, 0, 300)    
    }

plots_J2 = {
    'jet1_pt': PlotInfo('jet1_pt', 30, 0, 300),
    'jet1_eta': PlotInfo('jet1_eta', 10, -5, 5),
    'jet2_pt': PlotInfo('jet1_pt', 30, 0, 300),
    'jet2_eta': PlotInfo('jet1_eta', 10, -5, 5),
    'svfitMass': PlotInfo('svfitMass', 30, 0, 300)    
    }

plots_TauEle = {
    'l1_pt'      : PlotInfo ('l1_pt',       50,  0,    100),
    'l1_eta'     : PlotInfo ('l1_eta',      25,  -2.5, 2.5),
    'l1_phi'     : PlotInfo ('l1_phi',      24,  -3.2, 3.2),
    'l1_charge'  : PlotInfo ('l1_charge',   3,   -1.5, 1.5),
    'l1_EOverp'  : PlotInfo ('l1_EOverp',   100, 0,    2  ),
    'l2_pt'      : PlotInfo ('l2_pt',       50,  0,    100),
    'l2_eta'     : PlotInfo ('l2_eta',      25,  -2.5, 2.5),
    'l2_phi'     : PlotInfo ('l2_phi',      24,  -3.2, 3.2),
    'l2_charge'  : PlotInfo ('l2_charge',   3,   -1.5, 1.5),
    'l2_relIso05': PlotInfo ('l2_relIso05', 50,  0,    0.1),
    'mt'         : PlotInfo ('mt',          50,  0,    200),
    'visMass'    : PlotInfo ('visMass',     50,  0,    200), 
    'met'        : PlotInfo ('met',         50,  0,    150),
    'svfitMass'  : PlotInfo ('svfitMass',   75,  0,    200),
    'pfmet'      : PlotInfo ('pfmet',       100, 0,    100),
    'nJets'      : PlotInfo ('nJets',       15,  0,    15 ),
    'nVert'      : PlotInfo ('nVert',       50,  0,    50 ),
    'pZetaDisc'  : PlotInfo ('pZetaDisc',   100, -150, 100),
    }
