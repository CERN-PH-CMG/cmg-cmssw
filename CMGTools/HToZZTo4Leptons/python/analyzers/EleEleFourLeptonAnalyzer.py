from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon

class EleEleFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = Electron
    LeptonClass2 = Electron

    def declareHandles(self):
        super(EleEleFourLeptonAnalyzer, self).declareHandles()

        self.handles['leptons1'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )

        self.handles['leptons2'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(EleEleFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        



    def testLeptonTight1(self, leg,sel=None):
        return self.testElectronTight(leg) and \
               super( EleEleFourLeptonAnalyzer, self).testLeptonTight1( leg,sel )

    def testLeptonTight2(self, leg,sel=None):
        return self.testElectronTight(leg) and \
               super( EleEleFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )


    def testFourLeptonSF(self, fourLepton):
        return ( abs(fourLepton.leg1.leg1.pdgId()) == abs(fourLepton.leg2.leg1.pdgId())) and \
               super( EleEleFourLeptonAnalyzer, self).testFourLeptonSF( fourLepton)
