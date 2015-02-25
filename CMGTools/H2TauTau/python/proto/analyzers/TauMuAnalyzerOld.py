from PhysicsTools.Heppy.analyzers.examples.DiLeptonAnalyzer import DiLeptonAnalyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import TauMuon
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Muon, GenParticle

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

        if hasattr(self.cfg_ana, 'mvametsigs'):
            self.handles['mvametsigs'] = AutoHandle(
                self.cfg_ana.mvametsigs, # 'mvaMETTauMu'
                'std::vector<cmg::METSignificance>'
                )
        
        self.handles['leptons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
        
        # FIXME reading the genparticlespruned collection. problem elsewhere?
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

    def buildDiLeptons(self, cmgDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID muon.
        The tight ID selection is done so that dxy and dz can be computed
        (the muon must not be standalone).
        '''
        diLeptons = []
        for index, dil in enumerate(cmgDiLeptons):
            pydil = self.__class__.DiObjectClass(dil)
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg2().associatedVertex = event.goodVertices[0]
            if not self.testMuonIDTight( pydil.leg2() ):
                continue
            if hasattr(self.cfg_ana, 'mvametsigs'):
                pydil.mvaMetSig = mvaMetSig = self.handles['mvametsigs'].product()[index]
            diLeptons.append( pydil )
        return diLeptons

    def buildLeptons(self, cmgLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = self.__class__.LeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            if not self.testMuonIDLoose( pyl ):
                continue
            leptons.append( pyl )
        return leptons


    def process(self, event):

##         if event.iEv == 421:
##             import pdb; pdb.set_trace()

        result = super(TauMuAnalyzer, self).process(event)
        
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


    def testMuonIDTight(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.pt() > self.cfg_ana.pt2 and \
               abs( muon.eta() ) < self.cfg_ana.eta2 and \
               muon.tightId() and \
               self.testVertex( muon )

    def testMuonTight(self, muon ):
        '''Tight muon selection, with isolation requirement'''
        return self.testMuonIDTight(muon) and \
               self.muonIso(muon)<self.cfg_ana.iso2

    def testMuonIDLoose(self, muon):
        '''Loose muon selection, no isolation requirement'''        
        return muon.pt() > 15 and \
               abs( muon.eta() ) < 2.5 and \
               muon.looseId() and \
               self.testVertex( muon ) 
        
    
    def testMuonLoose( self, muon ):
        '''Loose muon selection, with isolation requirement (for di-lepton veto)'''
        #COLIN: not sure the vertex constraints should be kept 
        return self.testMuonIDLoose(muon) and \
               self.muonIso(muon)<0.3


    def muonIso(self, muon ):
        return muon.relIsoAllChargedDB05()
    

    def leptonAccept(self, leptons):
        looseLeptons = set(filter( self.testMuonLoose, leptons ))
        return len(looseLeptons)<2
