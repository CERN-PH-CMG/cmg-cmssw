import operator
import itertools
import copy
import types

from ROOT import TLorentzVector

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from PhysicsTools.Heppy.physicsobjects.Tau import Tau
from PhysicsTools.Heppy.physicsobjects.IsoTrack import IsoTrack

from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi, bestMatch , matchObjectCollection3

import PhysicsTools.HeppyCore.framework.config as cfg

from ROOT import heppy




def mtw(x1,x2):
    import math
    return math.sqrt(2*x1.pt()*x2.pt()*(1-math.cos(x1.phi()-x2.phi())))

def makeNearestLeptons(leptons,track, event):

    minDeltaR = 99999
    
    nearestLepton = []
    ibest=-1
    for i,lepton in enumerate(leptons):
        minDeltaRtemp=deltaR(lepton.eta(),lepton.phi(),track.eta(),track.phi())
        if minDeltaRtemp < minDeltaR:
            minDeltaR = minDeltaRtemp
            ibest=i

    if len(leptons) > 0 and ibest!=-1:
        nearestLepton.append(leptons[ibest])

    return nearestLepton
 
class IsoTrackAnalyzer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(IsoTrackAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.IsoTrackIsolationComputer = heppy.IsolationComputer()
        
        self.doIsoAnnulus = getattr(cfg_ana, 'doIsoAnnulus', False)
        self.useLegacy2016 = getattr(cfg_ana, 'useLegacy2016', False)

    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
    def declareHandles(self):
        super(IsoTrackAnalyzer, self).declareHandles()
        if self.useLegacy2016:
            self.handles['met'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )
            self.handles['packedCandidates'] = AutoHandle( 'packedPFCandidates', 'std::vector<pat::PackedCandidate>')
        else:
            self.handles['isolatedTracks'] = AutoHandle( 'isolatedTracks', 'vector<pat::IsolatedTrack>')

    def beginLoop(self, setup):
        super(IsoTrackAnalyzer,self).beginLoop(setup)
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

        alltrack = self.handles['isolatedTracks'].product()
        for track in alltrack:
		track.absIso = track.pfIsolationDR03().chargedHadronIso()
		event.selectedIsoTrack.append(track)

        event.selectedIsoTrack.sort(key = lambda l : l.pt(), reverse = True)
        self.counters.counter('events').inc('all events')
        if(len(event.selectedIsoTrack)): self.counters.counter('events').inc('has >=1 selected Iso Track')

    def makeIsoTrackLegacy(self, event):

        event.selectedIsoTrack = []
        event.selectedIsoCleanTrack = []
        #event.preIsoTrack = []

        patcands = self.handles['packedCandidates'].product()

        charged = [ p for p in patcands if ( p.charge() != 0 and p.fromPV() > 1 ) ]

        self.IsoTrackIsolationComputer.setPackedCandidates(patcands, 1, 9999, 9999.)


        alltrack = map( IsoTrack, charged )


        for track in alltrack:

            if ( abs(track.dz()) > self.cfg_ana.dzMax ): continue
            if ( (abs(track.pdgId())!=11) and (abs(track.pdgId())!=13) and (track.pt() < self.cfg_ana.ptMin) ): continue
            if ( track.pt() < self.cfg_ana.ptMinEMU ): continue

            foundNonIsoTrack = False

## ===> require is not the leading lepton and opposite to the leading lepton 
            if( (self.cfg_ana.doSecondVeto) and len(event.selectedLeptons)>0) : 
               if( deltaR(event.selectedLeptons[0].eta(), event.selectedLeptons[0].phi(), track.eta(), track.phi()) <0.01) : continue
               if ( (abs(track.pdgId())!=11) and (abs(track.pdgId())!=13) and (track.charge()*event.selectedLeptons[0].charge()) ): continue


## ===> Redundant:: require the Track Candidate with a  minimum dz
            track.associatedVertex = event.goodVertices[0] if len(event.goodVertices)>0 else event.vertices[0]

## ===> compute the isolation and find the most isolated track

            isoSum = self.IsoTrackIsolationComputer.chargedAbsIso(track.physObj, self.cfg_ana.isoDR, 0., self.cfg_ana.ptPartMin)
            if( abs(track.pdgId())==211 ): isoSum = isoSum - track.pt() #BM: this is an ugly hack and it is error prone. It needs to be fixed using the addVeto method properly

            if self.cfg_ana.doRelIsolation:
                relIso = (isoSum)/track.pt()
                if ( (abs(track.pdgId())!=11) and (abs(track.pdgId())!=13) and (relIso > self.cfg_ana.MaxIsoSum) ): continue
                elif((relIso > self.cfg_ana.MaxIsoSumEMU)): continue
            else:
                if(isoSum > (self.cfg_ana.maxAbsIso)): continue

            if self.doIsoAnnulus:
                self.attachIsoAnnulus04(track)

            track.absIso = isoSum

            #### store a preIso track
            #event.preIsoTrack.append(track)
            
#            if (isoSum < minIsoSum ) :
            if self.cfg_ana.doRelIsolation or (track.absIso < min(0.2*track.pt(), self.cfg_ana.maxAbsIso)): 
                event.selectedIsoTrack.append(track)

                if self.cfg_ana.doPrune:
                    myMet = self.handles['met'].product()[0]
                    mtwIsoTrack = mtw(track, myMet)
                    if mtwIsoTrack < 100:
                        if abs(track.pdgId()) == 11 or abs(track.pdgId()) == 13:
                            if track.pt()>5 and track.absIso/track.pt()<0.2:

                                myLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 ] 
                                nearestSelectedLeptons = makeNearestLeptons(myLeptons,track, event)
                                if len(nearestSelectedLeptons) > 0:
                                    for lep in nearestSelectedLeptons:
                                        if deltaR(lep.eta(), lep.phi(), track.eta(), track.phi()) > 0.01:
                                            event.selectedIsoCleanTrack.append(track)
                                else: 
                                    event.selectedIsoCleanTrack.append(track)

        event.selectedIsoTrack.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedIsoCleanTrack.sort(key = lambda l : l.pt(), reverse = True)

        self.counters.counter('events').inc('all events')
        if(len(event.selectedIsoTrack)): self.counters.counter('events').inc('has >=1 selected Iso Track')

    
    def attachIsoAnnulus04(self, mu):
        mu.absIsoAnCharged = self.IsoTrackIsolationComputer.chargedAbsIso      (mu.physObj, 0.4, self.cfg_ana.isoDR, 0.0,self.IsoTrackIsolationComputer.selfVetoNone)
        mu.absIsoAnPho     = self.IsoTrackIsolationComputer.photonAbsIsoRaw    (mu.physObj, 0.4, self.cfg_ana.isoDR, 0.0,self.IsoTrackIsolationComputer.selfVetoNone)
        mu.absIsoAnNHad    = self.IsoTrackIsolationComputer.neutralHadAbsIsoRaw(mu.physObj, 0.4, self.cfg_ana.isoDR, 0.0,self.IsoTrackIsolationComputer.selfVetoNone)
        mu.absIsoAnPU      = self.IsoTrackIsolationComputer.puAbsIso           (mu.physObj, 0.4, self.cfg_ana.isoDR, 0.0,self.IsoTrackIsolationComputer.selfVetoNone)
        mu.absIsoAnNeutral = max(0.0, mu.absIsoAnPho + mu.absIsoAnNHad - 0.5*mu.absIsoAnPU)

        mu.absIsoAn04 = mu.absIsoAnCharged + mu.absIsoAnNeutral
        mu.relIsoAn04 = mu.absIsoAn04/mu.pt()


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


    def process(self, event):

        if self.cfg_ana.setOff:
            return True

        self.readCollections( event.input )
        if self.useLegacy2016:
            self.makeIsoTrackLegacy(event)
        else:
            self.makeIsoTrack(event)

        if len(event.selectedIsoTrack)==0 : return True

##        event.pdgIdIsoTrack.append(event.selectedIsoTrack[0].pdgId())
##        event.isoIsoTrack.append(minIsoSum)
##        event.dzIsoTrack.append(abs(dz(event.selectedIsoTrack[0])))

### ===> do matching
        
        if not self.cfg_comp.isMC:
            return True

        if hasattr(event, 'gentaus') and hasattr(event, 'gentauleps') and hasattr(event, 'genleps') and self.cfg_ana.do_mc_match :
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


setattr(IsoTrackAnalyzer,"defaultConfig",cfg.Analyzer(
    class_object=IsoTrackAnalyzer,
    useLegacy2016=False,
    #####
    setOff=True,
    #####
    #candidatesLegacy2016='packedPFCandidates',
    #candidatesTypesLegacy2016='std::vector<pat::PackedCandidate>',
    #candidates='isolatedTracks',
    #candidatesTypes='std::vector<pat::IsolatedTrack>',
    ptMin = 5, # for pion 
    ptMinEMU = 5, # for EMU
    dzMax = 0.1,
    #####
    isoDR = 0.3,
    ptPartMin = 0,
    dzPartMax = 0.1,
    maxAbsIso = 8,
    #####
    doRelIsolation = False,
    MaxIsoSum = 0.1, ### unused
    MaxIsoSumEMU = 0.2, ### unused
    doSecondVeto = False,
    #####
    doIsoAnnulus= False,
    ###
    doPrune = True,
    do_mc_match = True, # note: it will in any case try it only on MC, not on data
  )
)
