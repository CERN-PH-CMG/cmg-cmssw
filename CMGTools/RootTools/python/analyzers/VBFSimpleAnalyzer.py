import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection

from CMGTools.RootTools.physicsobjects.VBF import VBF
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.utils.DeltaR import deltaR2
from CMGTools.RootTools.utils.cmsswRelease import isNewerThan


class VBFSimpleAnalyzer( Analyzer ):
    """Analyses the collection of jets stored in event.cleanJets,
    and adds a VBF object to the event, as event.vbf.

    The analyzer currently needs also event.diLepton to compute fancy
    variables for the VBF MVA, but we could remove this requirement if needed.

    Example configuration:

    vbfAna = cfg.Analyzer(
        'VBFSimpleAnalyzer',
        Mjj = 500,
        deltaEta = 3.5,
        cjvPtCut = 30.,
    )


    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(VBFSimpleAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def beginLoop(self):
        super(VBFSimpleAnalyzer,self).beginLoop()
        self.counters.addCounter('VBF')
        count = self.counters.counter('VBF')
        count.register('all events')
        count.register('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj))
        count.register('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        count.register('no central jets')

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('VBF').inc('all events')

        if len(event.cleanJets)<2:
            return True

        event.vbf = VBF( event.cleanJets, event.diLepton,
                         None, self.cfg_ana.cjvPtCut )
        if event.vbf.mjj > self.cfg_ana.Mjj:
            self.counters.counter('VBF').inc('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj) )
        else:
            return True 
        if abs(event.vbf.deta) > self.cfg_ana.deltaEta:
            self.counters.counter('VBF').inc('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        else:
            return True 
        if len(event.vbf.centralJets)==0:
            self.counters.counter('VBF').inc('no central jets')
        else:
            return True
        
        return True
