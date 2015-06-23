import operator 
import itertools
import copy
from math import *

from ROOT import std 
from ROOT import TLorentzVector, TVectorD

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

import ROOT
import os

class ttHhistoCounterAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHhistoCounterAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHhistoCounterAnalyzer, self).declareHandles()
	self.mchandles['LHEweights'] = AutoHandle( 'source', 'LHEEventProduct', mayFail = True, lazy = False )
    
    def beginLoop(self, setup):
        super(ttHhistoCounterAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')
        if "outputfile" in setup.services :
            setup.services["outputfile"].file.cd()
            self.inputCounter = ROOT.TH1D("Count","Count",1,0,2)
	    self.inputLHE = ROOT.TH1D("CountLHE","CountLHE",10001,-0.5,10000.5)

    def process(self, event):
        self.readCollections( event.input )
        self.inputCounter.Fill(1)
	if self.mchandles['LHEweights'].isValid():
		for w in self.mchandles['LHEweights'].product().weights():
			id_ = float(w.id)
			wgt_ = float(w.wgt)
        		self.inputLHE.Fill(id_, wgt_)
			

        return True
