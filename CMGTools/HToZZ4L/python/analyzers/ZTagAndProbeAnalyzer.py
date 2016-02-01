from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from CMGTools.HToZZ4L.tools.DiObject import DiObject

import itertools

    
class ZTagAndProbeAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ZTagAndProbeAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.mode = cfg_ana.mode
        self.tagSelection = cfg_ana.tagSelection
        self.probeSelection = cfg_ana.probeSelection
        self.probeCollection = cfg_ana.probeCollection
        if self.mode not in [ "Z", "Onia" ]: raise RuntimeError, "Unsupported mode"

    def beginLoop(self, setup):
        super(ZTagAndProbeAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('ZTagAndProbe')
        count = self.counters.counter('ZTagAndProbe')
        count.register('all events')
        count.register('all pairs')

    def process(self, event):
        self.readCollections( event.input )

        self.counters.counter('ZTagAndProbe').inc('all events')
        probeCollection = getattr(event, self.probeCollection)
        # count tight leptons
        tags   = [ lep for lep in event.selectedLeptons if self.tagSelection(lep)   ]
        probes = [ lep for lep in probeCollection       if self.probeSelection(lep) ]

        event.allPairs = self.findOSSFPairs(tags, probes) 

        if self.mode == "Z":
            event.zTnP = filter(self.zMassFilter, event.allPairs)
            event.zTnP.sort(key = lambda dilep : abs(dilep.mass() - 91.1876))
            for p in event.zTnP: 
                self.counters.counter('ZTagAndProbe').inc('all pairs')
        elif self.mode == "Onia":
            event.oniaTnP = filter(self.oniaMassFilter, event.allPairs)
            event.oniaTnP = filter(self.tagSelection, event.oniaTnP)
            for p in event.oniaTnP: 
                self.counters.counter('ZTagAndProbe').inc('all pairs')

    def oniaMassFilter(self,twoLepton):
        mll = twoLepton.mll()
        return (2.5 < mll and mll < 4.0) or (8.5  < mll and mll < 12)

    def zMassFilter(self,twoLepton):
        return twoLepton.mass() > 50 and twoLepton.mass() < 160

    def findOSSFPairs(self, tags, probes):
        '''Make combinatorics and make permulations of two leptons
           Include FSR if in cfg file
        '''
        out = []
        for l1, l2 in [ (t,p) for t in tags for p in probes ]:
            if (l1.pdgId()+l2.pdgId())!=0: 
                continue;
            twoObject = DiObject(l1, l2, doSort=False)
            out.append(twoObject)
        return out

