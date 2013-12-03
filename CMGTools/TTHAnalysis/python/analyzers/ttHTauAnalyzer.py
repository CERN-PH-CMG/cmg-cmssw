
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
            if self.cfg_ana.vetoLeptons:
                for lep in event.selectedLeptons:
                    if deltaR(lep.eta(), lep.phi(), tau.eta(), tau.phi()) < self.cfg_ana.leptonVetoDR:
                        tau.lepVeto = True
                if tau.lepVeto: continue
            if tau.pt() < self.cfg_ana.ptMin: continue
            if abs(tau.dxy()) > 0.5 or abs(tau.dz()) > 1.0: continue
            ## FIXME: ID part to be done here
            event.selectedTaus.append(tau)

        event.selectedTaus.sort(key = lambda l : l.pt(), reverse = True)
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.makeTaus(event)
        return True
