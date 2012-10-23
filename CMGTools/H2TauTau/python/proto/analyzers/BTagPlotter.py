from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from ROOT import TH1F, TFile

class BTagPlotter( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(BTagPlotter,self).__init__(cfg_ana, cfg_comp, looperName)
        fileName = '/'.join([self.dirName,
                             self.name+'_plots.root'])
        self.file = TFile( fileName, 'recreate' )
        self.h_mva_btag = TH1F ('h_mva_btag', 'h_mva_btag', 100, 0, 1)
        self.h_mva_bvet = TH1F ('h_mva_bvet', 'h_mva_bvet', 100, 0, 1)


    def beginLoop(self):
        super(BTagPlotter,self).beginLoop()
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        for jet in event.jets:
            if jet.btagFlag == True :
                self.h_mva_btag.Fill (jet.btagMVA)
            else :
                self.h_mva_bvet.Fill (jet.btagMVA)


    def write(self):
        super(BTagPlotter, self).write()
        self.file.Write() 
        self.file.Close()
        
