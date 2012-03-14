from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauTau
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Tau
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

    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testTau(leg) and \
               super( TauTauAnalyzer, self).testLeg2( leg )


    def leptonAccept(self, leptons):
        return electronAccept( leptons )
