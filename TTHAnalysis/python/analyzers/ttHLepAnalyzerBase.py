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


    #-------------------------------------------------------
    # DECLARATION OF HANDLES OF ALL OBJECTS WE POSSIBLY NEED   
    #-------------------------------------------------------   
        

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
        #met
        self.handles['met'] = AutoHandle( ('cmgPFMET',''),'std::vector<cmg::BaseMET>')
       
        #jets
        self.handles['jets'] = AutoHandle( ('cmgPFJetSel',''),'std::vector<cmg::PFJet>')

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

  #     self.handles['BR'] = AutoHandle( ('genInfo','BR'),'int')
  #     self.handles['processID'] = AutoHandle( ('genInfo','processID'),'int')




    def beginLoop(self):
        super(ttHLepAnalyzerBase,self).beginLoop()



    #------------------
    # MAKE LEPTON LISTS
    #------------------


    def makeLooseLeptons(self, event):
        
        event.looseLeptons = []
        event.goodLeptons = []
        
        allmuons = map( Muon, self.handles['muons'].product() )
        for mu in allmuons:
            if mu.pt > 5:
                event.looseLeptons.append(mu)

        allelectrons = map( Electron, self.handles['electrons'].product() )
        for ele in allelectrons:
            if ele.pt > 7:
                event.looseLeptons.append(ele)

        event.looseLeptons.sort(key = lambda l : l.pt(), reverse = True)

        print "Found ",len(event.looseLeptons)," loose leptons"


     #------------------
     # MAKE JET LISTS
     #------------------    

    def makeJets(self, event):

        event.jets = []
        event.cleanedjets = [] #cleaned the jets with up to 4 highest pT goodLeptons available


    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent

        #call the leptons and jets functions
        self.makeLooseLeptons(event)


        #----------------------
        # DEFINE THE CATEGORIES
        #----------------------   

        # here we want to define in which categories an event goes
        # the categories will be carachterized by flags, the flags will be stored in the tree
        
        
        
        return True
