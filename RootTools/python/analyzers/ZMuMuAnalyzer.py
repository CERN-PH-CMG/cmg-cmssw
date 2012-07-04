from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import DiMuon
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon


class ZMuMuAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = DiMuon
    LeptonClass = Muon

    def declareHandles(self):
        super(ZMuMuAnalyzer, self).declareHandles()
        print 'ZMuMuAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgDiMuonSel',
            'std::vector<cmg::DiObject<cmg::Muon,cmg::Muon>>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )

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
    

    def testLeg1(self, leg, isoCut):
        return self.testMuonLoose(leg) and \
               super( ZMuMuAnalyzer, self).testLeg1( leg, isoCut )

    def testLeg2(self, leg, isoCut):
        return self.testMuonLoose(leg) and \
               super( ZMuMuAnalyzer, self).testLeg2( leg, isoCut )

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
               self.muonIso(muon)<0.5


    def muonIso(self, muon ):
        return muon.relIsoAllChargedDB05()
    

