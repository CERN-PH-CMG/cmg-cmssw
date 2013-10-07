
import operator 
import itertools
import copy
import types

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Tau import Tau

from CMGTools.RootTools.utils.DeltaR import deltaR, deltaPhi, bestMatch
 
class ttHTauAnalyzer( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHTauAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
    def declareHandles(self):
        super(ttHTauAnalyzer, self).declareHandles()
        self.handles['taus'] = AutoHandle( ('cmgTauSel',''),'std::vector<cmg::Tau>')


    def beginLoop(self):
        super(ttHTauAnalyzer,self).beginLoop()

    #------------------
    # MAKE LEPTON LISTS
    #------------------
    def makeTaus(self, event):
        event.selectedTaus = []

        #get all
        alltaus = map( Tau, self.handles['taus'].product() )

        for tau in alltaus:
            tau.associatedVertex = event.goodVertices[0]
            tau.lepVeto = False
            for lep in event.selectedLeptons:
                if deltaR(lep.eta(), lep.phi(), tau.eta(), tau.phi()) < 0.5:
                    tau.lepVeto = True
            if tau.lepVeto: continue
            if tau.pt() < 20: continue
            if abs(tau.dxy()) > 0.5 or abs(tau.dz()) > 1.0: continue
            def id3(tau,X):
                return tau.tauID(X%"Loose") + tau.tauID(X%"Medium") + tau.tauID(X%"Tight") + max(0,tau.tauID(X%"VLoose"))
            tau.idCI   = id3(tau, "by%sCombinedIsolationDeltaBetaCorr")
            tau.idIMVA = id3(tau, "by%sIsoMVA")
            if tau.tauID("byRawIsoMVA") < 0 and tau.tauID("byVLooseCombinedIsolationDeltaBetaCorr") == 0: continue
            event.selectedTaus.append(tau)

        #event.selectedTaus.sort(key = lambda l : l.idCI+0.0001*l.pt(), reverse = True)
        event.selectedTaus.sort(key = lambda l : l.tauID("byRawIsoMVA"), reverse = True)
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.makeTaus(event)
        return True
