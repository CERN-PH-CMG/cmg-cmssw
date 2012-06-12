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
    'visMass': PlotInfo('visMass', 50, 0, 200),
    'svfitMass': PlotInfo('svfitMass', 50, 0, 300),
    'nJets': PlotInfo('nJets', 7, 0, 7),    
    }

plots_J1 = {
    'jet1_pt': PlotInfo('jet1_pt', 30, 0, 300),
    'jet1_eta': PlotInfo('jet1_eta', 10, -5, 5),
    'svfitMass': PlotInfo('svfitMass', 30, 0, 300)    
    }

def drawAll(cut, plots):
    for plot in plots.values():
        print plot.var
        ss, os, ssQ, osQ = makePlot( plot.var, weights, fwss, fwos,
                                     plot.nbins, plot.xmin, plot.xmax,
                                     cut, weight=weight, embed=False)
        draw(osQ)
        plot.ssign = cp(ss)
        plot.osign = cp(os)
        plot.ssQCD = cp(ssQ)
        plot.osQCD = cp(osQ)
        time.sleep(1)
