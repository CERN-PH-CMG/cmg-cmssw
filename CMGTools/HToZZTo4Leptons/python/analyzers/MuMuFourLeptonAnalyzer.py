from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, GenParticle
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class MuMuFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = Muon
    LeptonClass2 = Muon

    def declareHandles(self):
        super(MuMuFourLeptonAnalyzer, self).declareHandles()

        self.handles['leptons1'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )

        self.handles['leptons2'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(MuMuFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        
    def testLepton1(self, leg):
        return self.testMuon(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLepton1( leg )


    def testLepton2(self, leg):
        return self.testMuon(leg) and \
               super( MuMuFourLeptonAnalyzer, self).testLepton2( leg )

