from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

from CMGTools.H2TauTau.macros.Histograms import Histograms

class JetHistograms(Histograms):
    
    def __init__(self, name):
        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 50, 0, 200)
        self.h_eta = TH1F(name+'_h_eta', ';#eta', 50, -5, 5)

        super(JetHistograms, self).__init__(name)
        

    def Fill(self, jets, weight ):
        for jet in jets:
            self.h_pt.Fill( jet.pt(), weight )
            self.h_eta.Fill( jet.eta(), weight )
