from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauElectron
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Electron
from CMGTools.H2TauTau.proto.analyzers.CountLeptons import electronAccept, testEleLooseLorenzo, testEleLoosePhil


class TauEleAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauElectron
    LeptonClass = Electron

    def declareHandles(self):
        super(TauEleAnalyzer, self).declareHandles()
        # print 'TauEleAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgTauEleCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Electron>>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        self.handles['vertices'] = AutoHandle(
            'offlinePrimaryVertices',
            'vector<reco::Vertex>'
            )


    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauEleAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        if leg.dxy() >= 0.045 : return False
        if leg.dz()  >= 0.2   : return False
        if leg.pt () > self.cfg_ana.pt2 and \
           abs( leg.eta() ) < self.cfg_ana.eta2 and \
           testEleLoosePhil (leg, self.cfg_ana.pt2) and \
           leg.relIso (0.5) < self.cfg_ana.iso2: 
            return True
        else:
            return False
            
#           self.testVertex (leg) and 

        
    def testElectronTwiki_2012(self, leg):
        '''reference numbers form here https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection'''
        nInnerHits = leg.sourcePtr().gsfTrack().trackerExpectedHitsInner().numberOfHits()
        if nInnerHits != 0 : return False
        if leg.sourcePtr().passConversionVeto() == False : return False 
        vertices = self.handles['vertices'].product()
        #PG FIME take the one from the cmg electron
        legDZ = leg.sourcePtr().gsfTrack().dz (vertices[0].position())
        if abs(legDZ) > 0.1 : return False
        eta = abs( leg.eta() )
        if eta > 2.1 : return False
        lmvaID = -99999 # identification
        lmvaIS = -99999 # isolation
        if leg.pt() < 20 :
            if   eta<0.8:   
                lmvaID = 0.925
                lmvaIS = 0.815
            elif eta<1.479: 
                lmvaID = 0.915
                lmvaIS = 0.785
            else :          
                lmvaID = 0.965
                lmvaIS = 0.825
        else:
            if   eta<0.8:   
                lmvaID = 0.905
                lmvaIS = 0.805
            elif eta<1.479: 
                lmvaID = 0.955
                lmvaIS = 0.815
            else :          
                lmvaID = 0.975
                lmvaIS = 0.705
        return testEleLooseLorenzo(leg) and \
               leg.sourcePtr().electronID('mvaNonTrigV0') > lmvaID and \
               leg.sourcePtr().userFloat('mvaIsoRings') > lmvaIS


    def testTau(self, tau):
        '''Returns True if a tau passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.'''
        if tau.decayMode() == 0 and \
               tau.calcEOverP() < 0.2: #reject muons faking taus in 2011B #PG FIXME should I put this in?
            return False
        return tau.tauID("byLooseIsoMVA")==True and \
               tau.tauID("againstElectronMVA")==True and \
               tau.tauID("againstElectronMedium")==True and \
               tau.tauID("againstElectronMVA")==True
        # byLooseCombinedIsolationDeltaBetaCorr


    def leptonAccept(self, leptons):
        return electronAccept( leptons )


