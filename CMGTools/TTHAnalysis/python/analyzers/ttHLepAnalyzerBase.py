import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi


        
class ttHLepAnalyzerBase( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepAnalyzerBase,self).__init__(cfg_ana,cfg_comp,looperName)


    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
        

    def declareHandles(self):
 
        super(ttHLepAnalyzerBase, self).declareHandles()

        #leptons
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<cmg::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<cmg::Electron>")            
    
        
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, 'rho'),
                                          'double')
        #rho for electrons
        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, 'rho'),
                                          'double')

    def beginLoop(self):
        super(ttHLepAnalyzerBase,self).beginLoop()



    #------------------
    # MAKE LEPTON LISTS
    #------------------

    # A SKIM STEP WAS RUN BEFORE:
    # the muons are already corrected with Rochester corrections, are already cleaned with the ghost cleaning
    # the electrons have already the electron energy regression and calibration applied
    # the V5_10_0 cmgTuple, have been corrected with Mike's patch for the SIP computation -> cmgMuons have been remade ->
    # (cvs up -r michalis_sipPatchBranch  CMGTools/Common/src/MuonFactory.cc) 
    # nb: the event vertex needs to be defined first -> using the vertex analyzer
    # nb: in the following dxy and dz are computed with respect to the PV good vertex, sip with respect to the PV

    def makeLeptons(self, event):
        
        event.looseLeptons = []
        event.selectedLeptons = []

        #muons
        allmuons = map( Muon, self.handles['muons'].product() )
        for mu in allmuons:
            mu.associatedVertex = event.goodVertices[0]
            if mu.pt()>5 and abs(mu.dxy())<0.5 and abs(mu.dz())<1. and abs(mu.eta())<2.4 and (mu.isGlobal() or (mu.isTracker() and mu.numberOfMatches()>0)):
                if muon.sourcePtr().userFloat("isPFMuon")>0.5 and mu.sip3D()<10 and mu.relIso(dBetaFactor=0.5)<0.4:
                    event.selectedLeptons.append(mu)
                else:
                    event.looseLeptons.append(mu)
                    
        #electrons        
        allelectrons = map( Electron, self.handles['electrons'].product() )
        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0]
            if ele.pt()>7 and abs(ele.dxy())<0.5 and abs(ele.dz())<1. and abs(ele.eta())<2.5 and ele.numberOfHits()<=1:
                 if ele.mvaIDZZ() and ele.sip3D()<10 and ele.relIso(dBetaFactor=0.5)<0.4:
                    event.selectedLeptons.append(ele)
                else:
                    event.looseLeptons.append(ele)
                    
        event.looseLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedLeptons.sort(key = lambda l : l.pt(), reverse = True)

        print "Found ",len(event.looseLeptons)," loose leptons"
        print "Found ",len(event.selectedLeptons)," good leptons"
       

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        print 'Event ',eventNumber
        
        #import pdb; pdb.set_trace()

        #call the leptons functions
        self.makeLeptons(event)
        if len(event.selectedLeptons)<3:
            return False
        
        return True
