import itertools

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Electron


class A4Skim( Analyzer ):
    '''This analyzer tags the event if it passes a Z skim.

     Have a look at SkimOperator.'''

    def declareHandles(self):
        super(A4Skim, self).declareHandles()
        
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<cmg::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<cmg::Electron>")            

            

    def beginLoop(self):
        super(A4Skim,self).beginLoop()

    def getLeptons(self):
        muons     = self.handles['muons'].product()
        electrons = self.handles['electrons'].product()
        return muons,electrons
    

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent

        event.passing = False


        event.muons,event.electrons = self.getLeptons()

        event.goodMuons     = filter(lambda x: (x.isTracker() or x.isGlobal()) and (abs(x.eta())<2.5),event.muons)
        event.goodElectrons = filter(lambda x: abs(x.eta())<2.5,event.electrons)

        event.leptons = set(event.goodMuons)
        event.leptons.update(event.goodElectrons)


        event.ee = self.findPairs(event.goodElectrons)
        event.mm = self.findPairs(event.goodMuons)

        event.dileptons = self.findPairs(event.leptons)
        event.cross = filter(lambda x: abs(x.leg1.pdgId()) != abs(x.leg2.pdgId()),event.dileptons)

        event.eeKin    = filter(lambda x: max(x.leg1.pt(),x.leg2.pt())>20 and min(x.leg1.pt(),x.leg2.pt())>10, event.ee)
        event.mmKin    = filter(lambda x: max(x.leg1.pt(),x.leg2.pt())>20 and min(x.leg1.pt(),x.leg2.pt())>10, event.mm)
        event.crossKin = filter(lambda x: max(x.leg1.pt(),x.leg2.pt())>20 and min(x.leg1.pt(),x.leg2.pt())>10, event.cross)

        event.eeMass   = filter(lambda x: x.mass()>40,event.ee)
        event.mmMass   = filter(lambda x: x.mass()>40,event.mm)
        event.crossMass = filter(lambda x: x.mass()>40,event.cross)


        mmK = len(event.mmKin)>0
        mmM = len(event.mmMass)>0
        eeK = len(event.eeKin)>0
        eeM = len(event.eeMass)>0
        cK  = len(event.crossKin)>0
        cM  = len(event.crossMass)>0


        decision = (eeK and eeM) or \
                   (eeK and mmM) or \
                   (mmK and eeK) or \
                   (mmK and mmM) or \
                   (cK and eeM) or \
                   (cK and mmM) 


        if  decision:
            event.passing=True 

        return True
    

    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out


    def  evaluateSkim(self,ptColl,massColl,kin1=True ,kin2 = False):
        fptColl=[]
        fmassColl=[]
        if kin1:
            fptColl   = filter(lambda x: max(x.leg1.pt(),x.leg2.pt())>20 and min(x.leg1.pt(),x.leg2.pt())>10,  ptColl)
        else:    
            fptColl   = filter(lambda x: x.mass()>40  ,  ptColl)

        if kin2:
            fmassColl   = filter(lambda x: max(x.leg1.pt(),x.leg2.pt())>20 and min(x.leg1.pt(),x.leg2.pt())>10,  massColl)
        else:    
            fmassColl   = filter(lambda x: x.mass()>40   ,  massColl)

        return ((len(fptColl)>0) and (len(fmassColl)>0))
