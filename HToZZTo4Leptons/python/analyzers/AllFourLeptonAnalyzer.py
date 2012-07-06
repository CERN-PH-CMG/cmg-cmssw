from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzer import FourLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon

class AllFourLeptonAnalyzer( FourLeptonAnalyzer ):

    LeptonClass1 = Muon
    LeptonClass2 = Electron

    def declareHandles(self):
        super(AllFourLeptonAnalyzer, self).declareHandles()

        muons = 'cmgMuonSelStdLep'
        if self.cfg_ana.PF is True:
            muons = 'cmgMuonSel'

        self.handles['leptons1'] = AutoHandle(
            muons,
            'std::vector<cmg::Muon>'
            )

        self.handles['leptons2'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


    def process(self, iEvent, event):
        result = super(AllFourLeptonAnalyzer, self).process(iEvent, event)
        return result
        
    def testLeptonSkim1(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( AllFourLeptonAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( AllFourLeptonAnalyzer, self).testLeptonSkim2( leg,sel )

    def testLeptonSkim2(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( AllFourLeptonAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( AllFourLeptonAnalyzer, self).testLeptonSkim2( leg,sel )



    def testLeptonLoose1(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonLoose1( leg,sel )
        else:
            return self.testElectronLoose(leg) and \
            super( AllFourLeptonAnalyzer, self).testLeptonLoose1( leg,sel )

    def testLeptonLoose2(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonLoose2( leg,sel )
        else:
            return self.testElectronLoose(leg) and \
                   super( AllFourLeptonAnalyzer, self).testLeptonLoose2( leg,sel )



                
    def testLeptonTight1(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonTight(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonTight1( leg,sel )
        if abs(leg.pdgId()) == 11:
            return self.testElectronTight(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonTight1( leg,sel )



    def testLeptonTight2(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonTight(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )
        if abs(leg.pdgId()) == 11:
            return self.testElectronTight(leg) and \
               super( AllFourLeptonAnalyzer, self).testLeptonTight2( leg,sel )








