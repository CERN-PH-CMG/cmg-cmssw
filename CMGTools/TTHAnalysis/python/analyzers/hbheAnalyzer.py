import operator 
import itertools
import copy

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters

# New filter parameters taken from slide 11 of 
# https://indico.cern.ch/event/433302/contribution/0/attachments/1126451/1608346/2015-07-15_slides_v3.pdf
#
# HBHE filter requires:
#
# either  maxHPDHits >= 17
# or      maxHPDNoOtherHits >= 10
# or      maxZeros >= 10 (to be removed here)
# or      (HasBadRBXTS4TS5 and not goodJetFoundInLowBVRegion)


class hbheAnalyzer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(hbheAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(hbheAnalyzer, self).declareHandles()
        self.mchandles['hcalnoise'] = AutoHandle( 'hcalnoise', 'HcalNoiseSummary' )

    def beginLoop(self, setup):
        super(hbheAnalyzer,self).beginLoop( setup )

    def process(self, event):
        self.readCollections( event.input )
        self.mchandles['hcalnoise'].Load(event.input )
      
        event.hbheMaxZeros          = self.mchandles['hcalnoise'].product().maxZeros()
        event.hbheMaxHPDHits        = self.mchandles['hcalnoise'].product().maxHPDHits()
        event.hbheMaxHPDNoOtherHits = self.mchandles['hcalnoise'].product().maxHPDNoOtherHits()
        event.hbheHasBadRBXTS4TS5   = self.mchandles['hcalnoise'].product().HasBadRBXTS4TS5()
        event.hbheGoodJetFoundInLowBVRegion = self.mchandles['hcalnoise'].product().goodJetFoundInLowBVRegion()

        event.hbheFilterNew = 1

        if event.hbheMaxHPDHits >= 17: 
            event.hbheFilterNew = 0
        if event.hbheMaxHPDNoOtherHits >= 10 or (event.hbheHasBadRBXTS4TS5 and not event.hbheGoodJetFoundInLowBVRegion): 
            event.hbheFilterNew = 0

        return True
