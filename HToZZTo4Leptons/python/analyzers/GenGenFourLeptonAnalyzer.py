from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenLepton
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class GenGenFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = GenLepton
    LeptonClass2 = GenLepton

    def declareHandles(self):
        super(GenGenFourLeptonAnalyzer, self).declareHandles()

        self.handles['leptons1'] = AutoHandle(
            'genParticlesPruned',
            'std::vector<reco::GenParticle>'
            )

        self.handles['leptons2'] = AutoHandle(
            'genParticlesPruned',
            'std::vector<reco::GenParticle>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(GenGenFourLeptonAnalyzer, self).process(iEvent, event)
        return result

        
    def testLepton1(self, leg):
        return abs(leg.pdgId())==13 and \
               super( GenGenFourLeptonAnalyzer, self).testLepton1( leg )


    def testLepton2(self, leg):
        return abs(leg.pdgId())==11 and \
               super( GenGenFourLeptonAnalyzer, self).testLepton2( leg )

