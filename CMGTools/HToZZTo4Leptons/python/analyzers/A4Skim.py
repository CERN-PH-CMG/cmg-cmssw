import itertools

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Electron import Electron


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

        event.goodMuons     = filter(lambda x: (x.isTracker() or x.isGlobal()) and (x.pt()>3 and abs(x.eta())<2.4),event.muons)
        event.goodElectrons = filter(lambda x: x.pt()>5 and abs(x.eta())<2.5,event.electrons)

        event.leptons = set(event.goodMuons)
        event.leptons.update(event.goodElectrons)


        event.ee = self.findPairs(event.goodElectrons)
        event.mm = self.findPairs(event.goodMuons)

        event.dileptons = self.findPairs(event.leptons)

        #ask for any dilepton with pt>20/10
        diLeptonsAfterThreshold = filter(lambda x: x.leg1.pt()>20 and x.leg2.pt()>10,event.dileptons)
        if len(diLeptonsAfterThreshold)<1:
            return False

        #Ask for SF mass > 40
        diLeptonsMass = filter(lambda x: x.mass()>40 and abs(x.leg1.pdgId())==abs(x.leg2.pdgId()),event.dileptons)
        if len(diLeptonsMass)<1:
            return False
        


        return True
    

    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out

