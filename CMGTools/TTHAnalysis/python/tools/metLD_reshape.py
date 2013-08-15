#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class MetLDReshaper:
    def __init__(self):
        hists = ROOT.TFile("%s/src/CMGTools/TTHAnalysis/data/metLD_ttbar_emu.root" % os.environ['CMSSW_BASE'])
        self._data = hists.Get("metLD_data").Clone()
        self._mc   = hists.Get("metLD_TT").Clone()
        self._mc.Add(hists.Get("metLD_TW"))
        self._mc.Add(hists.Get("metLD_DY"))
        self._data.Scale(1.0/self._data.Integral())
        self._mc.Scale(1.0/self._mc.Integral())
        ROOT.gSystem.Load("libFWCoreFWLite.so")
        ROOT.gSystem.Load("libDataFormatsFWLite.so")
        ROOT.AutoLibraryLoader.enable()
        #ROOT.gSystem.Load("libCMGToolsTTHAnalysis.so")
        self.mapper = ROOT.DistributionRemapper(self._mc, self._data)
    def listBranches(self):
        return [ 'metLD_corr', 'metLD_2corr' ]
    def __call__(self,event):
        ldcorr  = self.mapper.Eval(event.met*0.00397 + event.mhtJet25*0.00265)
        ldcorr2 = self.mapper.Eval(ldcorr)
        return {
            'metLD_corr' : ldcorr,
            'metLD_2corr': ldcorr2,
        }

metLDReshaper = MetLDReshaper()
if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = metLDReshaper
        def analyze(self,ev):
            uncorr = ev.met*0.00397 + ev.mhtJet25*0.00265
            corrs  = self.sf(ev);
            print "%7.3f   %7.3f   %7.3f" % (uncorr, corrs['metLD_corr'], corrs['metLD_2corr'])
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 1000)
