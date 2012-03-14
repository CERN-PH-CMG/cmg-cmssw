from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
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

    def testElectronMITMVA(self, leg):
        eta = abs( leg.eta() )
        lmva = -99999
        if   eta<1: lmva = 0.945
        elif eta<1.5: lmva = 0.942
        elif eta<2.5: lmva = 0.878
        else:
            # out of acceptance, electron is rejected
            return False
        # in acceptance
        return leg.mvaMIT() > lmva

    def testElectronDaniele(self, leg):
        eta = abs( leg.eta() )
        lmva = -99999
        # import pdb; pdb.set_trace()
        if   eta<1: lmva = 0.0013
        elif eta<1.5: lmva = 0.0425
        elif eta<2.5: lmva = 0.0225
        else:
            # out of acceptance, electron is rejected
            return False
        # in acceptance
        return leg.mvaDaniele() > lmva
        
    def testElectron(self, leg):
        # return self.testElectronDaniele(leg)
        return leg.getSelection('cuts_vbtf80ID')
        
    def leptonAccept(self, leptons):
        return electronAccept( leptons )
