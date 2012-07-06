from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer 
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon



class MuMuFourLeptonAnalyzer( FourLeptonAnalyzer):

    LeptonClass1 = Muon
    LeptonClass2 = Muon

    def declareHandles(self):
        super(MuMuFourLeptonAnalyzer, self).declareHandles()

        muons = 'cmgMuonSelStdLep'
        if self.cfg_ana.PF is True:
            muons = 'cmgMuonSel'
            
        self.handles['leptons1'] = AutoHandle(
            muons, 
            'std::vector<cmg::Muon>'
            )

        self.handles['leptons2'] = AutoHandle(
            muons,
            'std::vector<cmg::Muon>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(MuMuFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        



    def testLeptonSkim1(self, leg,sel = None):
        return self.testMuonLoose(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonSkim1( leg,sel )

    def testLeptonSkim2(self,leg,sel = None):
        return self.testMuonLoose(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonSkim2( leg,sel )


    def testLeptonLoose1(self, leg,sel=None):
        return self.testMuonLoose(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonLoose1( leg,sel )

    def testLeptonLoose2(self, leg,sel=None):
        return self.testMuonLoose(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonLoose2( leg,sel )


    def testLeptonTight1(self, leg,sel=None):
        return self.testMuonTight(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonTight1( leg,sel ) 

    def testLeptonTight2(self, leg,sel=None):
        return self.testMuonTight(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )



    def testFourLeptonSF(self, fourLepton):
        return ( abs(fourLepton.leg1.leg1.pdgId()) == abs(fourLepton.leg2.leg1.pdgId())) and \
               super( MuMuFourLeptonAnalyzer, self).testFourLeptonSF( fourLepton)

