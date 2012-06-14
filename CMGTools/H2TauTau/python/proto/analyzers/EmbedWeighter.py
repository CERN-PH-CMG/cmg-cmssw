from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average

class EmbedWeighter( Analyzer ):
    '''Gets lepton efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EmbedWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

            
    def beginLoop(self):
        print self, self.__class__
        super(EmbedWeighter,self).beginLoop()
        self.averages.add('weight', Average('weight') )


    def declareHandles(self):
        super(EmbedWeighter,self).declareHandles()
        if self.cfg_comp.isEmbed:
            self.embhandles['minVisPtFilter'] = AutoHandle(
                ('generator', 'minVisPtFilter'),
                'GenFilterInfo'
                )

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.weight = 1

        if self.cfg_comp.isEmbed:
            genfilter = self.embhandles['minVisPtFilter'].product()
            self.weight = genfilter.filterEfficiency()
        if self.cfg_ana.verbose:
            print self.name, 'efficiency =', self.weight
        # import pdb; pdb.set_trace()
        event.eventWeight *= self.weight
        self.averages['weight'].add( self.weight )
                
