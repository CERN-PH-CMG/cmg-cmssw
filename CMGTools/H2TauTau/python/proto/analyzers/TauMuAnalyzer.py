from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauMuon
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon
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

    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauMuAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testMuon(leg) and \
               super( TauMuAnalyzer, self).testLeg2( leg )

    def testTau(self, tau):
        '''Returns True if a tau passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.
        
        WARNING: the muon filter should be used only in the muon channel.'''
        if tau.decayMode() == 0 and \
               tau.calcEOverP() < 0.2: #reject muons faking taus in 2011B
            return False
        elif tau.tauID("byLooseCombinedIsolationDeltaBetaCorr")==False:
            return False
        else:
            return True

#    def testMuon(self, muon):
#        return muon.getSelection('cuts_vbtfmuon')

    def leptonAccept(self, leptons):
        return muonAccept( leptons )
