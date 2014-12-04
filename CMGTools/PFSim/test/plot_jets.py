from CMGTools.RootTools.PyRoot import *


class ResolutionPlot(object):
    def __init__(self, name, tree):
        self.h2d = TH2F(name+'_h2d', '', 10, 20, 120, 20, 0, 2)
        tree.Draw('jet1_pt/jet1gen_pt:jet1gen_pt>>'+self.h2d.GetName(),'abs(jet1gen_eta)<1.3','goff')
        self.h2d.FitSlicesY()
        self.hmean = gDirectory.Get(self.h2d.GetName()+'_1')
        self.hsigma = gDirectory.Get(self.h2d.GetName()+'_2')


import sys

chain = Chain(None, sys.argv[1])
res = ResolutionPlot( 'test', chain )


