from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauMuon
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, GenParticle
from CMGTools.H2TauTau.proto.analyzers.CountLeptons import muonAccept

class TauMuAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauMuon
    LeptonClass = Muon

    def declareHandles(self):
        super(TauMuAnalyzer, self).declareHandles()
        # print 'TauMuAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgTauMuCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
        # FIXME reading the genparticlespruned collection. problem elsewhere?
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(TauMuAnalyzer, self).process(iEvent, event)
        
        if result is False:
            selDiLeptons = [ diL for diL in event.diLeptons if \
                             self.testMass(diL) ]
            if len(selDiLeptons)==0:
                return False
            event.diLepton = self.bestDiLepton( selDiLeptons )
            event.leg1 = event.diLepton.leg1()
            event.leg2 = event.diLepton.leg2()
            event.isSignal = False
        else:
            event.isSignal = True

        event.genMatched = None
        if self.cfg_comp.isMC:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                event.genMatched = True
            else:
                event.genMatched = False
                
        return True
        
    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauMuAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        # import pdb; pdb.set_trace()
        return self.testMuonTight(leg) and \
               super( TauMuAnalyzer, self).testLeg2( leg )


    def testTau(self, tau):
        '''Returns True if a tau passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.'''
        if tau.decayMode() == 0 and \
               tau.calcEOverP() < 0.2: #reject muons faking taus in 2011B
            return False
        return tau.tauID("byLooseIsoMVA")>0.5 and \
               tau.tauID("againstMuonTight")>0.5 and \
               tau.tauID("againstElectronLoose")>0.5 and \
               self.testVertex( tau )
               # tau.tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5 and \   


    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and tau'''
        return abs(lepton.dxy()) < 0.045 and \
               abs(lepton.dz()) < 0.2 


    def testMuonTight(self, muon ):
        '''Tight muon selection'''
        if muon.pt() > self.cfg_ana.pt2 and \
           abs( muon.eta() ) < self.cfg_ana.eta2 and \
           muon.tightId() and \
           self.testVertex( muon ) and \
           self.muonIso(muon)<self.cfg_ana.iso2: 
            return True
        else:
            return False


    def testMuonLoose( self, muon ):
        '''Loose muon selection, for the lepton veto'''
        #COLIN: not sure the vertex constraints should be kept 
        if muon.pt() > 15 and \
               abs( muon.eta() ) < 2.5 and \
               muon.looseId() and \
               self.testVertex( muon ) and \
               self.muonIso(muon)<0.3:
            return True
        else:
            return False


    def muonIso(self, muon ):
        return muon.relIso(0.5)
    

    def leptonAccept(self, leptons):
        looseLeptons = set(filter( self.testMuonLoose, leptons ))
        return len(looseLeptons)<2
