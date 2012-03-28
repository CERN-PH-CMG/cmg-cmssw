from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Electron, GenParticle
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class MuEleFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = Muon
    LeptonClass2 = Electron

    def declareHandles(self):
        super(MuEleFourLeptonAnalyzer, self).declareHandles()

        self.handles['leptons1'] = AutoHandle(
            'cmgMuonSel',
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
        
    def testLepton1(self, leg):
        return self.testMuon(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLepton1( leg )


    def testLepton2(self, leg):
        return self.testElectron(leg) and \
               super( MuEleFourLeptonAnalyzer, self).testLepton2( leg )

