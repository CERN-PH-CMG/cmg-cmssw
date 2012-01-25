from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import TauMuon
from CMGTools.H2TauTau.proto.physicsobjects.PhysicsObjects import Muon
from CMGTools.H2TauTau.macros.CountLeptons import muonAccept

class TauMuAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauMuon
    LeptonClass = Muon

    def declareHandles(self):
        super(TauMuAnalyzer, self).declareHandles()
        print 'TauMuAnalyzer.declareHandles'
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


    def leptonAccept(self, leptons):
        return muonAccept( leptons )
