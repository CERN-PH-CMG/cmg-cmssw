from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Electron, GenParticle
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class MuEleFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = Muon
    LeptonClass2 = Electron

    def declareHandles(self):
        super(MuEleFourLeptonAnalyzer, self).declareHandles()

        muons = 'cmgMuonSelStdLep'
        if self.cfg_ana.PF is True:
            muons = 'cmgMuonSel'

        self.handles['leptons1'] = AutoHandle(
            muons,
            'std::vector<cmg::Muon>'
            )

        self.handles['leptons2'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(MuEleFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        
    def testLeptonSkim1(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( MuEleFourLeptonAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( MuEleFourLeptonAnalyzer, self).testLeptonSkim2( leg,sel )

    def testLeptonSkim2(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( MuEleFourLeptonAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( MuEleFourLeptonAnalyzer, self).testLeptonSkim2( leg,sel )



    def testLeptonLoose1(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonLoose1( leg,sel )
        else:
            return super( MuEleFourLeptonAnalyzer, self).testLeptonLoose1( leg,sel )

    def testLeptonLoose2(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonLoose2( leg,sel )
        else:
            return super( MuEleFourLeptonAnalyzer, self).testLeptonLoose2( leg,sel )



        
    def testLeptonTight1(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonTight(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonTight1( leg,sel )
        if abs(leg.pdgId()) == 11:
            return self.testElectronTight(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonTight1( leg,sel )



    def testLeptonTight2(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonTight(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )
        if abs(leg.pdgId()) == 11:
            return self.testElectronTight(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )





    def testFourLeptonSF(self, fourLepton):
        return ( abs(fourLepton.leg1.leg1.pdgId()) != abs(fourLepton.leg2.leg1.pdgId())) and \
               super( MuEleFourLeptonAnalyzer, self).testFourLeptonSF( fourLepton)



