from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from CMGTools.HToZZ4L.tools.DiObject import DiObject

import itertools

    
class TwoLeptonAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(TwoLeptonAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.mode = cfg_ana.mode
        if self.mode not in [ "Z", "Onia" ]: raise RuntimeError, "Unsupported mode"

    def beginLoop(self, setup):
        super(TwoLeptonAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('TwoLepton')
        count = self.counters.counter('TwoLepton')
        count.register('all events')
        count.register('all pairs')
        if self.mode == "Z":
            count.register('pass iso')
            count.register('best Z')
        elif self.mode == "Onia":
            count.register('pass onia')

    def process(self, event):
        self.readCollections( event.input )

        self.counters.counter('TwoLepton').inc('all events')
        # count tight leptons
        tight_leptons = [ lep for lep in event.selectedLeptons if self.leptonID_tight(lep) ]

        # make dilepton pairs, possibly attach FSR photons (the latter not yet implemented)
        event.allPairs = self.findOSSFPairs(tight_leptons, []) #event.fsrPhotons)

        # count them, for the record
        for p in event.allPairs:
            self.counters.counter('TwoLepton').inc('all pairs')

        if self.mode == "Z":
            # make pairs of isolated leptons
            event.isolatedPairs = filter(self.twoLeptonIsolation, event.allPairs)
            for pair in event.isolatedPairs:
                self.counters.counter('TwoLepton').inc('pass iso')

            # get the best Z (mass closest to PDG value)
            # still a list, because if there's no isolated leptons it may be empty
            sortedIsoPairs = event.isolatedPairs[:] # make a copy
            sortedIsoPairs.sort(key = lambda dilep : abs(dilep.mass() - 91.1876))
            event.bestIsoZ = sortedIsoPairs[:1] # pick at most 1
            if len(event.bestIsoZ):
                self.counters.counter('TwoLepton').inc('best Z')
        elif self.mode == "Onia":
            event.onia = filter(self.oniaMassFilter, event.allPairs)
            if event.onia: self.counters.counter('TwoLepton').inc('pass onia')

    def leptonID_tight(self,lepton):
        return lepton.tightId()

    def muonIsolation(self,lepton):
        return lepton.relIsoAfterFSR < 0.4

    def electronIsolation(self,lepton):
        return lepton.relIsoAfterFSR < 0.5


    def twoLeptonIsolation(self,twoLepton):
        ##First ! attach the FSR photons of this candidate to the leptons!
        
        leptons = twoLepton.daughterLeptons()
        photons = twoLepton.daughterPhotons()

        for l in leptons:
            if abs(l.pdgId())==11:
                if not self.electronIsolation(l):
                    return False
            if abs(l.pdgId())==13:
                if not self.muonIsolation(l):
                    return False
        return True        

    def findOSSFPairs(self, leptons, photons):
        '''Make combinatorics and make permulations of two leptons
           Include FSR if in cfg file
        '''
        out = []
        for l1, l2 in itertools.permutations(leptons, 2):
            if (l1.pdgId()+l2.pdgId())!=0: 
                continue;
            if (l1.pdgId()<l2.pdgId())!=0: 
                continue;

            twoObject = DiObject(l1, l2)
            out.append(twoObject)

        return out

    def oniaMassFilter(self,twoLepton):
        return twoLepton.mll() > 2.5 and twoLepton.mll() < 12

