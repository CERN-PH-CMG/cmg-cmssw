import os 
import logging 

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.utils.DeltaR import deltaR

class ttHIsoTrackSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        #Rather than using the inherited init do own so can choose directory
        #name

        super(ttHIsoTrackSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)

        self.ptCuts = cfg_ana.ptCuts if hasattr(cfg_ana, 'ptCuts') else []
        self.ptCuts += 10*[-1.]

        self.idCut = cfg_ana.idCut if hasattr(cfg_ana, 'idCut') else "True"
        self.idFunc = eval("lambda object : "+self.idCut);

    def declareHandles(self):
        super(ttHIsoTrackSkimmer, self).declareHandles()

    def beginLoop(self):
        super(ttHIsoTrackSkimmer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('too many objects')
        count.register('too few objects')
        count.register('accepted events')



    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        
        objects = []
        selectedObjects = getattr(event, self.cfg_ana.objects)
        for obj, ptCut in zip(selectedObjects, self.ptCuts):
	    allowTrack = False
	    for i in range (0,len(event.selectedMuons)) :
		if (i == self.cfg_ana.allowedMuon): break
		if(deltaR(event.selectedMuons[i].eta(), event.selectedMuons[i].phi(), obj.eta(), obj.phi()) < 0.02) : allowTrack=True

	    for i in range (0,len(event.selectedElectrons)) :
		if (i == self.cfg_ana.allowedElectron): break
		if(deltaR(event.selectedElectrons[i].eta(), event.selectedElectrons[i].phi(), obj.eta(), obj.phi()) < 0.02) : allowTrack=True

            if not self.idFunc(obj):
                continue
            if obj.pt() > ptCut and not allowTrack: 
                objects.append(obj)


        ret = False 
        if len(objects) >= self.cfg_ana.minObjects:
            ret = True
        else:
            self.counters.counter('events').inc('too few objects')

        if len(objects) > self.cfg_ana.maxObjects:
            self.counters.counter('events').inc('too many objects')
            ret = False

        if ret: self.counters.counter('events').inc('accepted events')
        return ret
