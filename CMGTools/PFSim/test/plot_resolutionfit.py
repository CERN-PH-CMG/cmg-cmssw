from CMGTools.RootTools.PyRoot import *



class Fitter(object):

    def __init__( self, basename, ptmin, ptmax, tree):
        # include ptmin and ptmax in the name
        self.name = basename
        self.ptmin = ptmin
        self.ptmax = ptmax
        self.tree = tree
        self.hist = self.buildHist()
        self.amplitude, self.mean, self.sigma = self.fit(self.h)

    def buildHist(self):
        return None

    def fit(self):
        return None, None, None
        



ptbins = [
    (10, 50),
    (50, 100)
    ]

chain = Chain(None, sys.argv[1])

for ptmin, ptmax in ptbins:
    fitter = Fitter('fit', ptmin, ptmax, chain)
