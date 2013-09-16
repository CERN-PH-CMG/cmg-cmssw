from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import MuonElectron
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Electron

class MuEleAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = MuonElectron
    #COLIN what should I do for the di-lepton veto?? None I guess
    LeptonClass = Electron

    def declareHandles(self):
        super(MuEleAnalyzer, self).declareHandles()
        # print 'MuEleAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgMuEleCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Muon ,cmg::Electron>>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )

    def testLeg1(self, leg):
        return self.testMuon (leg) and \
               super( MuEleAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testElectron(leg) and \
               super( MuEleAnalyzer, self).testLeg2( leg )


    def leptonAccept(self, leptons):
        pass
