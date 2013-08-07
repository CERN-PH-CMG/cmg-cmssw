import copy
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Muon import Muon

class SingleMuAnalyzer( Analyzer ):

    def declareHandles(self):
        super(SingleMuAnalyzer, self).declareHandles()
        self.handles['muons'] =  AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )


    def beginLoop(self):
        super(SingleMuAnalyzer,self).beginLoop()

       
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.muons = map( Muon, self.handles['muons'].product())
        event.selectedMuons = [mu for mu in event.muons if mu.looseId()]
        event.selectedLeptons = event.selectedMuons

        for lep in event.selectedLeptons:
            lep.associatedVertex = event.goodVertices[0]

    
    
