from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauElectron
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Electron
from CMGTools.H2TauTau.proto.analyzers.CountLeptons import electronAccept

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

    def testLeg1(self, leg):
        return self.testTau(leg) and \
               super( TauEleAnalyzer, self).testLeg1( leg )


    def testLeg2(self, leg):
        return self.testElectron(leg) and \
               super( TauEleAnalyzer, self).testLeg2( leg )

##     def testTau(self, tau):
##         '''Returns True if a tau passes a set of cuts.
##         Can be used in testLeg1 and testLeg2, in child classes.
        
##         WARNING: the electron filter should be used only in the electron channel.'''
##         if tau.decayMode() == 0 and \
##                tau.calcEOverP() < 0.2: #reject electrons faking taus in 2011B
##             return False
##         else:
##             return True


    def leptonAccept(self, leptons):
        return electronAccept( leptons )
