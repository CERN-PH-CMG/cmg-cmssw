from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauTau
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Tau, GenParticle
from CMGTools.H2TauTau.proto.analyzers.CountLeptons import electronAccept

class TauTauAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauTau
    LeptonClass = Tau

    def declareHandles(self):
        super(TauTauAnalyzer, self).declareHandles()
        # print 'TauTauAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgDiTauCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Tau>>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgTauSel',
            'std::vector<cmg::Tau>'
            )
        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(TauTauAnalyzer, self).process(iEvent, event)
        
        if result is False:
            return result

        # Simone, that's what you probably want to check.
        # Also have a look at CMGTools.RootTools.physicsobjects.DiObject
        event.genMatched = None
        if self.cfg_comp.isMC and "DY" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                event.genMatched = True
            else:
                event.genMatched = False
            event.isPhoton=False
	    for gen in genParticles:
                if abs(gen.pdgId())==15 and (gen.mother().mass()<80 or gen.mother().mass()>100):
                    event.isPhoton=True
        if self.cfg_comp.isMC and "W" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.matchW( event.genParticles ) 
            if (leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
               (leg2DeltaR>-1 and leg2DeltaR < 0.1):
                event.genMatched = True
            else:
                event.genMatched = False
		
        return True
        

    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg2( leg )


    def leptonAccept(self, leptons):
        return electronAccept( leptons )
