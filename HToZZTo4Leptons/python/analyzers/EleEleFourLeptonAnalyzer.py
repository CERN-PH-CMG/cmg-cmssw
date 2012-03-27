from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Electron, GenParticle
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

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
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(EleEleFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        
    def testLepton1(self, leg):
        return self.testElectron(leg) and \
               super( EleEleFourLeptonAnalyzer, self).testLepton1( leg )


    def testLepton2(self, leg):
        return self.testElectron(leg) and \
               super( EleEleFourLeptonAnalyzer, self).testLepton2( leg )

