from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from ROOT import TH1F, TFile

class BTagPlotter( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(BTagPlotter,self).__init__(cfg_ana, cfg_comp, looperName)
        fileName = '/'.join([self.dirName,
                             self.name+'_plots.root'])
        self.file = TFile( fileName, 'recreate' )
        self.h_mva_b_btag = TH1F ('h_mva_b_btag', 'h_mva_b_btag', 100, 0, 1)
        self.h_mva_b_bvet = TH1F ('h_mva_b_bvet', 'h_mva_b_bvet', 100, 0, 1)
        self.h_mva_l_btag = TH1F ('h_mva_l_btag', 'h_mva_l_btag', 100, 0, 1)
        self.h_mva_l_bvet = TH1F ('h_mva_l_bvet', 'h_mva_l_bvet', 100, 0, 1)
        self.h_mva_g_btag = TH1F ('h_mva_g_btag', 'h_mva_g_btag', 100, 0, 1)
        self.h_mva_g_bvet = TH1F ('h_mva_g_bvet', 'h_mva_g_bvet', 100, 0, 1)


    def beginLoop(self, setup):
        super(BTagPlotter,self).beginLoop(setup)
        
    def process(self, event):
        self.readCollections( event.input )
        for jet in event.jets:
            pf = abs(jet.partonFlavour())
            if jet.btagFlag == True :
                if (pf < 5)     : self.h_mva_l_btag.Fill (jet.btagMVA)
                elif (pf == 5)  : self.h_mva_b_btag.Fill (jet.btagMVA)
                elif (pf == 21) : self.h_mva_g_btag.Fill (jet.btagMVA)
            else :
                if (pf < 5)     : self.h_mva_l_bvet.Fill (jet.btagMVA)
                elif (pf == 5)  : self.h_mva_b_bvet.Fill (jet.btagMVA)
                elif (pf == 21) : self.h_mva_g_bvet.Fill (jet.btagMVA)


    def write(self):
        super(BTagPlotter, self).write()
        self.file.Write() 
        self.file.Close()
        
