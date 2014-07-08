from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Photon import Photon

from CMGTools.TTHAnalysis.analyzers.ttHLepMCMatchAnalyzer import matchObjectCollection3
from CMGTools.RootTools.utils.DeltaR import bestMatch

class ttHPhotonAnalyzerSusy( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHPhotonAnalyzerSusy,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHPhotonAnalyzerSusy, self).declareHandles()

    #----------------------------------------                                                                                                                                   
    # DECLARATION OF HANDLES OF PHOTONS STUFF                                                                                                                                   
    #----------------------------------------     

        #photons
        self.handles['photons'] = AutoHandle( self.cfg_ana.photons,'std::vector<pat::Photon>')

    def beginLoop(self):
        super(ttHPhotonAnalyzerSusy,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def makePhotons(self, event):
        event.allphotons = map( Photon, self.handles['photons'].product() )
        event.allphotons.sort(key = lambda l : l.pt(), reverse = True)

        event.selectedPhotons = []
        event.loosePhotons = []

        foundPhoton = False
        for gamma in event.allphotons:
            if gamma.pt() < self.cfg_ana.ptMin: continue
            foundPhoton = True
            event.selectedPhotons.append(gamma)

        event.selectedPhotons.sort(key = lambda l : l.pt(), reverse = True)

       
    def matchPhotons(self, event):
        event.genPhotons = [ x for x in event.genParticles if x.status() == 3 and abs(x.pdgId()) == 22 ]
        match = matchObjectCollection3(event.allphotons, event.genPhotons, deltaRMax = 0.5)
        for gamma in event.allphotons:
            gen = match[gamma]
            gamma.mcMatchId = 1 if gen else 0

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        #call the photons functions
        self.makePhotons(event)

## ===> do matching                                                                                                                                                                                                     
        if not self.cfg_comp.isMC:
            return True

        self.matchPhotons(event)


        return True
