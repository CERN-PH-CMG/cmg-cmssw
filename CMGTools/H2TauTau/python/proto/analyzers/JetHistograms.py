from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

from CMGTools.RootTools.statistics.Histograms import Histograms

class JetHistograms(Histograms):
    
    def __init__(self, name, fillFractions=False):

        self.fillFractions = fillFractions
        
        self.h_n = TH1F(name+'_h_n', ';N_{jets}', 7, -0.5, 6.5)
        self.h_pt = TH1F(name+'_h_pt', ';p_{T} (GeV)', 80, 0, 400)
        self.h_eta = TH1F(name+'_h_eta', ';#eta', 50, -5, 5)
        self.h_rawFactor = TH1F(name+'_h_rawFactor', ';raw factor', 100, 0.6, 1.1)

        # need to see entries for frac = 1
        if self.fillFractions:
            self.h_frac_ch = TH1F(name+'_h_frac_ch', ';ch E fraction', 55, 0, 1.1)
            self.h_frac_nh = TH1F(name+'_h_frac_nh', ';nh E fraction', 55, 0, 1.1)
            self.h_frac_ph = TH1F(name+'_h_frac_ph', ';ph E fraction', 55, 0, 1.1)
            self.h_frac_mu = TH1F(name+'_h_frac_mu', ';mu E fraction', 55, 0, 1.1)
            self.h_frac_ele = TH1F(name+'_h_frac_ele', ';ele E fraction', 55, 0, 1.1)
            self.h_frac_hfhad = TH1F(name+'_h_frac_hfhad', ';HF had E fraction', 55, 0, 1.1)
            self.h_frac_hfem = TH1F(name+'_h_frac_hfem', ';HF em E fraction', 55, 0, 1.1)

        super(JetHistograms, self).__init__(name)
        

    def Fill(self, jets, weight ):
        self.h_n.Fill( len(jets), weight )
        for jet in jets:
            self.h_pt.Fill( jet.pt(), weight )
            self.h_eta.Fill( jet.eta(), weight )
            self.h_rawFactor.Fill( jet.rawFactor(), weight )
            # COLIN: commenting the following lines allows to gain some speed
            if self.fillFractions:
                self.h_frac_ch.Fill( jet.component(1).fraction(), weight  )
                self.h_frac_nh.Fill( jet.component(5).fraction(), weight  )
                self.h_frac_ph.Fill( jet.component(4).fraction(), weight  )
                self.h_frac_mu.Fill( jet.component(3).fraction(), weight  )
                self.h_frac_ele.Fill( jet.component(2).fraction(), weight  )
                self.h_frac_hfhad.Fill( jet.component(6).fraction(), weight  )
                self.h_frac_hfem.Fill( jet.component(7).fraction(), weight  )
