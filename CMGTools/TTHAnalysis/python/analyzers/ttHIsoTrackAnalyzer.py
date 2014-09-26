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
##from CMGTools.RootTools.physicsobjects.Tau import Tau
from CMGTools.RootTools.physicsobjects.IsoTrack import IsoTrack
from CMGTools.TTHAnalysis.analyzers.ttHLepMCMatchAnalyzer import matchObjectCollection3

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

        charged = []
        pfcands = self.handles['cmgCand'].product()

## ===> require the Track Candidate charge and with a  minimum dz
        for i in xrange(pfcands.size()):
            if (pfcands.at(i).charge()!=0 and (abs(pfcands.at(i).dz())<=self.cfg_ana.dzMax)):
                charged.append(pfcands.at(i))

##        alltrack = map( IsoTrack, self.handles['cmgCand'].product() )
        alltrack = map( IsoTrack, charged )

        for track in alltrack:

            foundNonIsoTrack = False
            foundIsoTrack = False

## ===> require Track Candidate above some pt and charged
            if ( (track.pdgId()!=11) and (track.pdgId()!=13) and (track.pt() < self.cfg_ana.ptMin) ): continue
            if ( track.pt() < self.cfg_ana.ptMinEMU ): continue


## ===> require is not the leading lepton and opposite to the leading lepton 
            if( (self.cfg_ana.doSecondVeto) and len(event.selectedLeptons)>0) : 
               if( deltaR(event.selectedLeptons[0].eta(), event.selectedLeptons[0].phi(), track.eta(), track.phi()) <0.01) : continue
               if ( (track.pdgId()!=11) and (track.pdgId()!=13) and (track.charge()*event.selectedLeptons[0].charge()) ): continue

## ===> Redundant:: require the Track Candidate with a  minimum dz
            track.associatedVertex = event.goodVertices[0]
            if abs(track.dz())>self.cfg_ana.dzMax : continue 

## ===> compute the isolation and find the most isolated track

            allpart = alltrack

            preIsoSum=0
            isoSum=0
            for part in allpart:
                ### ===> skip pfcands with a pt min (this should be 0)
                if part.pt()<self.cfg_ana.ptPartMin : continue
                ### ===> skip pfcands outside the cone (this should be 0.3)
                if deltaR(part.eta(), part.phi(), track.eta(), track.phi()) > self.cfg_ana.isoDR : continue
                preIsoSum += part.pt()
                isoSum += part.pt()
                ### break the loop to save time
                if(preIsoSum > (self.cfg_ana.maxAbsIso + track.pt())):
                    foundNonIsoTrack = True
                    break

            if not foundNonIsoTrack :
                # reset
                isoSum=0
                for part in allpart :
                ### ===> skip pfcands with a pt min (this should be 0)
                    if part.pt()<self.cfg_ana.ptPartMin : continue
                ### ===> skip pfcands outside the cone (this should be 0.3)
                    if deltaR(part.eta(), part.phi(), track.eta(), track.phi()) > self.cfg_ana.isoDR : continue
                    isoSum += part.pt()

                ### ===> the sum should not contain the track candidate
                isoSum -= track.pt()
                ###            isoSum = isoSum/track.pt()  ## <--- this is for relIso

            track.absIso = isoSum

            ### store a preIso track
            event.preIsoTrack.append(track)
            
#            if (isoSum < minIsoSum ) :
            if(track.absIso < min(0.2*track.pt(), self.cfg_ana.maxAbsIso)): 
                foundIsoTrack = True

            if(foundIsoTrack):
                event.selectedIsoTrack.append(track)

        event.selectedIsoTrack.sort(key = lambda l : l.pt(), reverse = True)

        self.counters.counter('events').inc('all events')
        if(len(event.preIsoTrack)): self.counters.counter('events').inc('has >=1 selected Track') 
        if(len(event.selectedIsoTrack)): self.counters.counter('events').inc('has >=1 selected Iso Track')

    def matchIsoTrack(self, event):
        matchTau = matchObjectCollection3(event.selectedIsoTrack, event.gentaus + event.gentauleps + event.genleps, deltaRMax = 0.5)
        for lep in event.selectedIsoTrack:
            gen = matchTau[lep]
            lep.mcMatchId = 1 if gen else 0

    def printInfo(self, event):
        print 'event to Veto'
        print '----------------'

        if len(event.selectedIsoTrack)>0:
            print 'lenght: ',len(event.selectedIsoTrack)
            print 'track candidate pt: ',event.selectedIsoTrack[0].pt()
            print 'track candidate eta: ',event.selectedIsoTrack[0].eta()
            print 'track candidate phi: ',event.selectedIsoTrack[0].phi()
            print 'track candidate mass: ',event.selectedIsoTrack[0].mass()
            print 'pdgId candidate : ',event.selectedIsoTrack[0].pdgId()
            print 'dz: ',event.selectedIsoTrack[0].dz()
            print 'iso: ',event.selectedIsoTrack[0].absIso
            print 'matchId: ',event.selectedIsoTrack[0].mcMatchId 
                
#        for lepton in event.selectedLeptons:
#            print 'good lepton type: ',lepton.pdgId()
#            print 'pt: ',lepton.pt()
            
#        for tau in event.selectedTaus:
#            print 'good lepton type: ',tau.pdgId()
#            print 'pt: ',tau.pt()
            
        print '----------------'


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.makeIsoTrack(event)

        if len(event.selectedIsoTrack)==0 : return True

##        event.pdgIdIsoTrack.append(event.selectedIsoTrack[0].pdgId())
##        event.isoIsoTrack.append(minIsoSum)
##        event.dzIsoTrack.append(abs(dz(event.selectedIsoTrack[0])))

### ===> do matching
        
        if not self.cfg_comp.isMC:
            return True
        
        self.matchIsoTrack(event)        

###        self.printInfo(event)
        
### ===> do veto if needed

#        if (self.cfg_ana.doSecondVeto and (event.selectedIsoTrack[0].pdgId()!=11) and (event.selectedIsoTrack[0].pdgId()!=12) and event.isoIsoTrack < self.cfg_ana.MaxIsoSum ) :
###            self.printInfo(event)
#            return False

#        if ((self.cfg_ana.doSecondVeto and event.selectedIsoTrack[0].pdgId()==11 or event.selectedIsoTrack[0].pdgId()==12) and event.isoIsoTrack < self.cfg_ana.MaxIsoSumEMU ) :
##            self.printInfo(event)
#            return False


        return True
