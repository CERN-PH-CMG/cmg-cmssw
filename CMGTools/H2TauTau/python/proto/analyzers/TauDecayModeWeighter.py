from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

class TauDecayModeWeighter( Analyzer ):
    '''Gets tau decay mode efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(TauDecayModeWeighter,self).__init__(cfg_ana, cfg_comp, looperName)

            
    def beginLoop(self):
        print self, self.__class__
        super(TauDecayModeWeighter,self).beginLoop()
        self.averages.add('weight', Average('weight') )
        # For 1 prong no pi-zero. Can be extended to more weights
        # if necessary
        self.oneProngNoPiZeroWeight = 0.88

    def process(self, iEvent, event):
        self.weight = 1

        # Not strictly correct, but this is agreed upon for Summer 2013:
        if self.cfg_comp.isEmbed or 'Higgs' in self.cfg_comp.name or ('DY' in self.cfg_comp.name and event.isFake == 0):
            decayMode = event.diLepton.leg1().decayMode()
            if decayMode == 0:
                self.weight = self.oneProngNoPiZeroWeight

            # print decayMode, self.weight, self.cfg_comp.name


        event.eventWeight *= self.weight
        event.tauESWeight = self.weight

        if self.cfg_ana.verbose:
            print 'TauDecayModeWeighter', event.diLepton.leg1().decayMode(), event.isFake, event.tauESWeight

        self.averages['weight'].add( self.weight )
        return True
                
