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

 
class ttHIsoTrackAnalyzer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHIsoTrackAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
    def declareHandles(self):
        super(ttHIsoTrackAnalyzer, self).declareHandles()
        self.handles['cmgCand'] = AutoHandle(self.cfg_ana.candidates,self.cfg_ana.candidatesTypes) 

    def beginLoop(self):
        super(ttHIsoTrackAnalyzer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('has >=1 selected Track')
        count.register('has >=1 selected Iso Track')

    #------------------
    # MAKE LIST
    #------------------
    def makeIsoTrack(self, event):
        event.selectedIsoTrack = []
        event.preIsoTrack = []
        event.typeIsoTrack=-99999
        event.isoIsoTrack=-1
        event.dzIsoTrack=-1
        event.ptIsoTrack=-1
        
        foundIsoTrack = False
        minIsoSum=99999999
        indexIsoSum=-1
        index=-1;

        def dz(self, vertex=None):
            if vertex is None:
                vertex = self.associatedVertex
            selfp4 = self.p4();
            return  (self.vz()-vertex.position().z()) - ((self.vx()-vertex.position().x())*self.px()+(self.vy()-vertex.position().y())*self.py())/ self.pt() *  self.pz()/ self.pt();

        for track in self.handles['cmgCand'].product():
            track.associatedVertex = event.goodVertices[0]

## ===> require Track Candidate above some pt and charged
            if track.charge()==0  : continue
            if ( (track.pdgId()!=11) and (track.pdgId()!=12) and (track.pt() < self.cfg_ana.ptMin) ): continue
            if ( track.pt() < self.cfg_ana.ptMinEMU ): continue

## ===> require is not the leading lepton and opposite to the leading lepton 
            if( (self.cfg_ana.doSecondVeto) and len(event.selectedLeptons)>0) : 
               if( deltaR(event.selectedLeptons[0].eta(), event.selectedLeptons[0].phi(), track.eta(), track.phi()) <0.01) : continue
               if ( (track.pdgId()!=11) and (track.pdgId()!=12) and (track.charge()*event.selectedLeptons[0].charge()) ): continue

## ===> require the Track Candidate with a  minimum dz
            if abs(dz(track))>self.cfg_ana.dzMax : continue 

## ===> compute the isolation and find the most isolated track
            isoSum=0
            for part in self.handles['cmgCand'].product():
                part.associatedVertex = event.goodVertices[0]
                ### ===> skip pfcands outside the cone (this should be 0.3)
                if deltaR(part.eta(), part.phi(), track.eta(), track.phi()) > self.cfg_ana.isoDR : continue
                ### ===> skip pfcands with a pt min (this should be 0)
                if part.pt()<self.cfg_ana.ptPartMin : continue
                ### ===> skip pfcands with a dz (this should be 0.1)
                if abs(dz(part))>self.cfg_ana.dzPartMax : continue
                isoSum += part.pt()

            ### ===> the sum should not contain the track candidate
            isoSum -= track.pt()
            isoSum = isoSum/track.pt()

            ### store a temporary a track
            event.preIsoTrack.append(track)
            index+=1;
            
            if (isoSum < minIsoSum ) :
                minIsoSum = isoSum
                indexIsoSum=index
                foundIsoTrack = True

        if(foundIsoTrack):
            event.selectedIsoTrack.append(event.preIsoTrack[indexIsoSum])
            event.typeIsoTrack=event.selectedIsoTrack[0].pdgId()
            event.isoIsoTrack=minIsoSum
            event.dzIsoTrack=abs(dz(event.selectedIsoTrack[0]))
            event.ptIsoTrack=event.selectedIsoTrack[0].pt()

        self.counters.counter('events').inc('all events')
        if(len(event.preIsoTrack)): self.counters.counter('events').inc('has >=1 selected Track') 
        if foundIsoTrack: self.counters.counter('events').inc('has >=1 selected Iso Track')

    def printInfo(self, event):
        print 'event to Veto'
        print '----------------'

        if len(event.selectedIsoTrack)>0:
            print 'track candidate type: ',event.typeIsoTrack
            print 'pt: ',event.ptIsoTrack
            print 'dz: ',event.dzIsoTrack
            print 'iso: ',event.isoIsoTrack
                
        for lepton in event.selectedLeptons:
            print 'good lepton type: ',lepton.pdgId()
            print 'pt: ',lepton.pt()
            
        for tau in event.selectedTaus:
            print 'good lepton type: ',tau.pdgId()
            print 'pt: ',tau.pt()
            
        print '----------------'


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.makeIsoTrack(event)

        if len(event.selectedIsoTrack)==0 : return True
        
        if (self.cfg_ana.doSecondVeto and (event.selectedIsoTrack[0].pdgId()!=11) and (event.selectedIsoTrack[0].pdgId()!=12) and event.isoIsoTrack < self.cfg_ana.MaxIsoSum ) :
#            self.printInfo(event)
            return False

        if ((self.cfg_ana.doSecondVeto and event.selectedIsoTrack[0].pdgId()==11 or event.selectedIsoTrack[0].pdgId()==12) and event.isoIsoTrack < self.cfg_ana.MaxIsoSumEMU ) :
#            self.printInfo(event)
            return False

        return True
