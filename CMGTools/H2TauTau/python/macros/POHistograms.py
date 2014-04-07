from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

from CMGTools.H2TauTau.macros.Histograms import Histograms

class POHistograms(Histograms):
    
    def __init__(self, name):
        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 40, 0, 100)
        self.h_eta = TH1F(name+'_h_eta', ';#eta', 50, -2.5, 2.5)

        super(POHistograms, self).__init__(name)
        

    def Fill(self, pos, weight ):
        for po in pos:
            self.h_pt.Fill( po.pt(), weight )
            self.h_eta.Fill( po.eta(), weight )
