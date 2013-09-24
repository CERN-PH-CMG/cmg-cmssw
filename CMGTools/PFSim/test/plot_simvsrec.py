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
        h = TH1F( hname, hname, 100, 0.5, 1.5)
        h.SetStats(0)
        h.SetXTitle('p_{T,rec}/p_{T,gen}')
        var = 'jet1_{ext}_pt/jet1_pt>>{hname}'.format(
            ext=ext, hname=hname
            )
        # cut = 'abs(jet1_eta)<1. && jet1_rec_n==1'
        cut = 'abs(jet1_eta)<1. && jet1_{ext}_pt>0'.format(ext=ext)
        self.tree.Draw(var, cut,'goff')
        # h.Fit('gaus')
        # need to fit in a given range around the main peak
        # fun.SetLineColor( h.GetLineColor() ) 
        return h


class Fitter(object):
    def __init__(self, name, tree, ext):
        self.tree = tree
        self.name = name
        hname = 'h2d_' + name
        self.h2d = TH2F(hname, hname ,20,20, 100, 20, 0., 2.)
        chain.Draw('jet1_{ext}_pt/jet1_pt:jet1_pt>>{hname}'.format(ext=ext,hname=hname), 'jet1_{ext}_pt>0'.format(ext=ext), 'goff')
        self.h2d.FitSlicesY()
        self.hmean = gDirectory.Get(hname+'_1')
        self.hsigma = gDirectory.Get(hname+'_2')



import sys

chain = Chain(None, sys.argv[1])
c1 = Comparison( 'c1', chain )
c1.draw(simple=True)
c1.save()

## fun = c1.hsim.GetFunction('gaus')
## amplitude = fun.GetParameter(0)
## mean = fun.GetParameter(1)
## sigma = fun.GetParameter(2)

## frec = Fitter('frec', chain, 'rec')
## fsim = Fitter('fsim', chain, 'sim')

## sBlue.formatHisto(frec.hmean)
## sBlack.formatHisto(fsim.hmean)
## sBlue.formatHisto(frec.hsigma)
## sBlack.formatHisto(fsim.hsigma)


## cmean = HistComparator('cmean', frec.hmean, fsim.hmean, title1='fast sim', title2='pf sim'); cmean.draw()
## csigma = HistComparator('csigma', frec.hsigma, fsim.hsigma, title1='fast sim', title2='pf sim'); csigma.draw()
