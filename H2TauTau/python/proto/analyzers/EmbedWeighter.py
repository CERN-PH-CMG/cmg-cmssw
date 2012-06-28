from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

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
            if cmsswIs52X():
                self.embhandles['minVisPtFilter'] = AutoHandle(
                    ('generator', 'minVisPtFilter'),
                    'GenFilterInfo'
                    )
            else:
                self.embhandles['minVisPtFilter'] = AutoHandle(
                    ('generator','weight'),
                    'double'
                    )
                

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.weight = 1

        if self.cfg_comp.isEmbed:
            try: 
                genfilter = self.embhandles['minVisPtFilter'].product()
            except RuntimeError:
                print 'WARNING EmbedWeighter, cannot find the weight in the event'
                return False
            if cmsswIs52X():
                self.weight = genfilter.filterEfficiency()
            else: 
                self.weight = genfilter[0]
        if self.cfg_ana.verbose:
            print self.name, 'efficiency =', self.weight
        event.eventWeight *= self.weight
        event.embedWeight = self.weight
        self.averages['weight'].add( self.weight )
        return True
                
