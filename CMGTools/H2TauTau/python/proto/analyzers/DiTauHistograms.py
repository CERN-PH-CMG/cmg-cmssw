from ROOT import TTree, TH1F, TH2F, TProfile, TLegend, gDirectory

from CMGTools.RootTools.statistics.Histograms import Histograms


class DiTauHistograms(Histograms):
    def __init__(self, name):

        self.h_pzeta = TH1F(name+'_h_pzeta', ';p_{#zeta} (GeV)', 60, -200,100)
        self.h_vismass = TH1F(name+'_h_vismass', ';m_{vis} (GeV)', 50, 0, 200)
        self.h_svfitmass = TH1F(name+'_h_svfitmass', ';m_{SVFit} (GeV)', 38, 0, 304)
        self.h_mT = TH1F(name+'_h_mT', ';m_{T} (GeV)', 100,0,200)
        self.h_dr1 = TH1F(name+'_h_dr1', ';#DeltaR', 1000,-2,8)
        self.h_dr2 = TH1F(name+'_h_dr2', ';#DeltaR', 1000,-2,8)
        
        super(DiTauHistograms, self).__init__(name)

    def FillHistos(self, tree, cut='', nEvents=999999999):
        print 'Filling histograms: ' + self.name
        tree.Draw('tauMu.obj[0].pZeta()>>'+self.h_pzeta.GetName(),cut,'goff',nEvents)
        tree.Draw('tauMu.obj[0].mass()>>'+self.h_vismass.GetName(),cut,'goff',nEvents)
        tree.Draw('tauMu.obj[0].mTLeg2()>>'+self.h_mT.GetName(),cut,'goff',nEvents)

    def FillDiTau(self, diTau, weight=1):
        self.h_pzeta.Fill( diTau.pZetaDisc(), weight)
        self.h_vismass.Fill( diTau.mass(), weight)
        self.h_svfitmass.Fill( diTau.massSVFit(), weight)
        self.h_mT.Fill( diTau.mTLeg2(), weight)
        self.h_dr1.Fill( diTau.leg1DeltaR, weight)
        self.h_dr2.Fill( diTau.leg2DeltaR, weight)
            
            
