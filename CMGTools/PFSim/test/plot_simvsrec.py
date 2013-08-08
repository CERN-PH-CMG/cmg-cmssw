from CMGTools.RootTools.PyRoot import *
from CMGTools.RootTools.HistComparator import HistComparator


## class ResolutionPlot(object):
##     def __init__(self, name, tree):
##         self.h2d = TH2F(name+'_h2d', '', 10, 20, 120, 20, 0, 2)
##         tree.Draw('jet1_pt/jet1gen_pt:jet1gen_pt>>'+self.h2d.GetName(),'abs(jet1gen_eta)<1.3','goff')
##         self.h2d.FitSlicesY()
##         self.hmean = gDirectory.Get(self.h2d.GetName()+'_1')
##         self.hsigma = gDirectory.Get(self.h2d.GetName()+'_2')


class Comparison(HistComparator):
    def __init__(self, name, tree):
        self.tree = tree
        self.name = name
        self.hsim = self.buildHist('sim')
        self.hrec = self.buildHist('rec')
        sBlue.fillStyle = 0
        sBlack.fillStyle = 0
        sBlue.formatHisto(self.hsim)
        sBlack.formatHisto(self.hrec)
        super(Comparison,self).__init__(self.name, self.hsim, self.hrec)

    def buildHist(self, ext):
        hname = '_'.join([self.name,ext])
        h = TH1F( hname, hname, 200, 0.5, 1.5)
        h.SetStats(0)
        h.SetXTitle('p_{T,rec}/p_{T,gen}')
        var = 'jet1_{ext}_pt/jet1_pt>>{hname}'.format(
            ext=ext, hname=hname
            )
        # cut = 'abs(jet1_eta)<1. && jet1_rec_n==1'
        cut = 'abs(jet1_eta)<1. && jet1_{ext}_pt>0'.format(ext=ext)
        self.tree.Draw(var, cut,'goff')
        h.Fit('gaus')
        # need to fit in a given range around the main peak
        # fun.SetLineColor( h.GetLineColor() ) 
        return h



import sys

chain = Chain(None, sys.argv[1])
c1 = Comparison( 'c1', chain )
c1.draw(simple=True)
c1.save()

fun = c1.hsim.GetFunction('gaus')
amplitude = fun.GetParameter(0)
mean = fun.GetParameter(1)
sigma = fun.GetParameter(2)
