from ROOT import TTree, TH1F, TH2F, TProfile, gDirectory

from CMGTools.RootTools.statistics.Histograms import Histograms
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Tau,isTau

    
class LegHistograms(Histograms):
    def __init__(self, name, leg):
        self.leg = leg
        fname = '_'.join([name,leg])
        
        self.h_pt = TH1F(fname+'_h_pt', ';p_{T} (GeV)', 40, 0, 100)
        self.h_eta = TH1F(fname+'_h_eta', ';#eta', 50, -2.5, 2.5)

        self.h_iso = TH1F(fname+'_h_iso', ';rel iso', 50, 0, 0.5)
        self.h_iso_dbeta = TH1F(fname+'_h_iso_dbeta', ';rel iso', 50, 0, 0.5)
        self.h_iso_abs = TH1F(fname+'_h_iso_abs', ';abs iso', 50, 0, 20)
        self.h_iso_abs_dbeta = TH1F(fname+'_h_iso_abs_dbeta', ';abs iso', 50, 0, 20)

        self.h_iso_ch = TH1F(fname+'_h_iso_ch', ';ch iso', 50, 0, 20)
        self.h_iso_nh = TH1F(fname+'_h_iso_nh', ';nh iso', 50, 0, 20)
        self.h_iso_ph = TH1F(fname+'_h_iso_ph', ';ph iso', 50, 0, 20)

        super(LegHistograms, self).__init__(fname)
        

    def FillLeg(self, leg, weight ):
        dbetaFactor = 0.5
        self.h_pt.Fill( leg.pt(), weight )
        self.h_eta.Fill( leg.eta(), weight )
        self.h_iso.Fill( leg.relIso(), weight )
        self.h_iso_dbeta.Fill( leg.relIso(dbetaFactor), weight )
        self.h_iso_abs.Fill( leg.absIso(), weight )
        self.h_iso_abs_dbeta.Fill( leg.absIso(dbetaFactor), weight )
        self.h_iso_ch.Fill( leg.chargedHadronIso(), weight )
        self.h_iso_nh.Fill( leg.neutralHadronIso(), weight )
        self.h_iso_ph.Fill( leg.photonIso(), weight )
        if isTau(leg):
            tau = Tau( leg )
            self.h_eOverP.Fill( tau.calcEOverP(), weight )
            self.h_decMode.Fill( tau.decayMode(), weight )



class TauHistograms(LegHistograms):
    def __init__(self, name, leg):

        self.h_eOverP = TH1F(name+leg+'_h_eOverP', ';E / p', 150, 0, 1.5)
        self.h_decMode = TH1F(name+leg+'_h_decMode', ';decay mode', 16, -0.5, 15.5)
                
        super(TauHistograms, self).__init__(name, leg)


    def FillTau(self, tau, weight):
        #if not isTau(tau):
        #    raise ValueError()
        # tau = Tau( tau ) #COLIN do I need that? 
        self.h_eOverP.Fill( tau.calcEOverP(), weight )
        self.h_decMode.Fill( tau.decayMode(), weight )
        
        super(TauHistograms, self).FillLeg(tau, weight)
        


class MuHistograms(LegHistograms):
    def __init__(self, name, leg):
        super(MuHistograms, self).__init__(name, leg)

    def FillMu(self, mu, weight):
        super(MuHistograms, self).FillLeg(mu, weight)



class EleHistograms(LegHistograms):
    def __init__(self, name, leg):
        super(EleHistograms, self).__init__(name, leg)

    def FillEle(self, ele, weight):
        super(EleHistograms, self).FillLeg(ele, weight)
