from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import DiMuon
from CMGTools.H2TauTau.proto.physicsobjects.PhysicsObjects import Muon


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

    def testLeg1(self, leg):
        return self.testMuon(leg) and \
               super( ZMuMuAnalyzer, self).testLeg1( leg )

    def testLeg2(self, leg):
        return self.testMuon(leg) and \
               super( ZMuMuAnalyzer, self).testLeg2( leg )


