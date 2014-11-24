from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

import ROOT

class ttHReclusterJetsAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHReclusterJetsAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHReclusterJetsAnalyzer, self).declareHandles()
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self):
        super(ttHReclusterJetsAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')

    def makeFatJets(self, event):
        objects40jc = [ j for j in event.cleanJets if j.pt() > 40.0 and abs(j.eta())<2.4 ]
        
        if len(objects40jc)>=1:
            
           objects  = ROOT.std.vector(ROOT.reco.Particle.LorentzVector)()
           for jet in objects40jc:
                objects.push_back(jet.p4())
                
           reclusterJets = ROOT.ReclusterJets(objects, 1.,1.2)
           inclusiveJets = reclusterJets.getGrouping()

           # maybe should dress them as Jet objects in the future
           # for the moment, we just make LorentzVector
           event.reclusteredFatJets = [ ROOT.reco.Particle.LorentzVector(p4) for p4 in inclusiveJets ]
           # note 1: just taking inclusiveJets is not ok, since it's not a python list but a std::vector
           # note 2: [p4 for p4 in inclusiveJets] is also bad, since these are references to values inside a temporary std::vector

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        event.reclusteredFatJets = []
        self.makeFatJets(event)

        return True
