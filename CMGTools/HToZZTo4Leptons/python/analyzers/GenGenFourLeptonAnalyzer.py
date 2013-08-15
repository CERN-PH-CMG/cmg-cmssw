from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenLepton
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class GenGenFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = GenLepton
    LeptonClass2 = GenLepton

    def declareHandles(self):
        super(GenGenFourLeptonAnalyzer, self).declareHandles()
       
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(GenGenFourLeptonAnalyzer, self).process(iEvent, event)
        return result


    def buildLeptonList(self, event):
        event.leptons1 = []
        event.leptons2 = []
        for ptc in self.mchandles['genParticles'].product():
            if abs(ptc.pdgId()) == self.cfg_ana.pdgId1:
                event.leptons1.append( GenLepton(ptc) )
            elif  abs(ptc.pdgId()) == self.cfg_ana.pdgId2:
                event.leptons2.append( GenLepton(ptc) )

                    
    def testLepton1(self, leg):
        if leg.status()!=1:
            return False
        return super( GenGenFourLeptonAnalyzer, self).testLepton1( leg )


    def testLepton2(self, leg):
        if leg.status()!=1:
            return False
        return super( GenGenFourLeptonAnalyzer, self).testLepton2( leg )

