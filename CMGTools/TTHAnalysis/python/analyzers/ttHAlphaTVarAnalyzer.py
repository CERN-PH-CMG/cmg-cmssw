import operator 
import itertools
import copy
from math import *

#from ROOT import TLorentzVector, TVectorD

from CMGTools.RootTools.utils.DeltaR import deltaR, deltaPhi
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
# from CMGTools.RootTools.physicsobjects.Lepton import Lepton
# from CMGTools.RootTools.physicsobjects.Photon import Photon
# from CMGTools.RootTools.physicsobjects.Electron import Electron
# from CMGTools.RootTools.physicsobjects.Muon import Muon
# from CMGTools.RootTools.physicsobjects.Tau import Tau
from CMGTools.RootTools.physicsobjects.Jet import Jet

#from CMGTools.RootTools.utils.DeltaR import * 

import ROOT
from ROOT import AlphaT


import os

class ttHAlphaTVarAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHAlphaTVarAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHAlphaTVarAnalyzer, self).declareHandles()
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self):
        super(ttHAlphaTVarAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')


    # Calculate alphaT using jet ET
    def makeAlphaT(self, event):

        if len(event.cleanJets) == 0:
            event.alphaT = 0
            return
        
        px  = ROOT.std.vector('double')()
        py  = ROOT.std.vector('double')()
        et  = ROOT.std.vector('double')()
#Make alphaT from lead 10 jets
	for jet in event.cleanJets[:10]:
            px.push_back(jet.px())
            py.push_back(jet.py())
            et.push_back(jet.et())
            pass

        alphaTCalc   = AlphaT()
        event.alphaT = alphaTCalc.getAlphaT( et, px, py )

        return

    def makeBiasedDPhi(self, event):

        if len(event.cleanJets) == 0:
            event.biasedDPhi = 0
            return 
	mhtPx = event.mhtJet50jvec.px()
	mhtPy = event.mhtJet50jvec.py()

	biasedDPhi = 10;
        for jet in event.cleanJets:
	    newPhi = atan2(mhtPy+jet.py(),mhtPx+jet.px())
	    biasedDPhiTemp = abs(deltaPhi(newPhi,jet.phi()))
	    if biasedDPhiTemp < biasedDPhi:
		biasedDPhi = biasedDPhiTemp
		biasedDPhiJet = jet
            pass

        event.biasedDPhi = biasedDPhi
        event.biasedDPhiJet = biasedDPhiJet

        return

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        event.alphaT = -999
        self.makeAlphaT(event)
	event.biasedDPhi = -999
	#event.biasDPhiJet = -999
	self.makeBiasedDPhi(event)

        return True
