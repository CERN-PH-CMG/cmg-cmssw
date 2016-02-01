from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.utils.deltar import deltaR

from CMGTools.HToZZ4L.tools.DiObject import DiObject
from CMGTools.HToZZ4L.tools.ZLepObject import ZLepObject

import ROOT

    
class ThreeLeptonAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ThreeLeptonAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.srcZ   = cfg_ana.srcZ
        self.lep3Sel = cfg_ana.lep3Sel

    def beginLoop(self, setup):
        super(ThreeLeptonAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('ThreeLepton')
        count = self.counters.counter('ThreeLepton')
        count.register('all events')
        count.register('all with Zs')
        count.register('all Z+l')
        count.register('one Z+l')
        count.register('mll > 4')
        count.register('pt > 20/10')

    def process(self, event):
        event.zlepCands = []
        self.counters.counter('ThreeLepton').inc('all events')

        zeds = getattr(event, self.srcZ)
        if not len(zeds): return True
        self.counters.counter('ThreeLepton').inc('all with Zs')

        leps = filter(self.lep3Sel, event.selectedLeptons)
        cands = []
        for z in zeds:
            for l3 in leps:
                if deltaR(z.leg1.eta(),z.leg1.phi(),l3.eta(),l3.phi())<0.02:
                    continue
                if deltaR(z.leg2.eta(),z.leg2.phi(),l3.eta(),l3.phi())<0.02:
                    continue
                zl = ZLepObject(z,l3) 
                cands.append(zl)

        if len(cands) == 0: return True
        self.counters.counter('ThreeLepton').inc('all Z+l')

        if len(cands) > 1: return True
        self.counters.counter('ThreeLepton').inc('one Z+l')

        zl = cands[0]
        if zl.minPairMll() < 4: return True 
        self.counters.counter('ThreeLepton').inc('mll > 4')

        leading_pt = zl.sortedPtLeg(0).pt() 
        subleading_pt = zl.sortedPtLeg(1).pt() 
        if leading_pt <= 20 or subleading_pt <= 10: return True
        self.counters.counter('ThreeLepton').inc('pt > 20/10')

        event.zlepCands = cands
        return True

