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

    def testLeg1(self, leg):
        return self.testMuon(leg) and \
               super( ZMuMuAnalyzer, self).testLeg1( leg )

    def testLeg2(self, leg):
        return self.testMuon(leg) and \
               super( ZMuMuAnalyzer, self).testLeg2( leg )


