import math
import numpy
from DataFormats.FWLite import Handle, Runs
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer

class MCWeighter( Analyzer ):
    '''Retrieves the pre-skim *weighed* event count and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(MCWeighter,self).__init__(cfg_ana, cfg_comp, looperName)
        self.mcweight = 1.
        if hasattr(cfg_ana, 'countSign'):
            self.countSign = cfg_ana.countSign
        else:
            self.countSign = False
            
        self.runs = Runs(cfg_comp.files)

    def beginLoop(self, setup):
        '''
        Compute the weight just once at the beginning of the loop.
        Warning: this works correctly only the file(s) is(are) 
        completely processed.
        '''
        super(MCWeighter, self).beginLoop(setup)
        
        runsHandle = Handle('std::vector<double>')
        runsLabel  = ('genEvtWeightsCounter', 'genWeight')
        
        mcw = []
                
        for run in self.runs:
            run.getByLabel( runsLabel, runsHandle )
            mcw += runsHandle.product()
        
        self.mcweight = numpy.mean(mcw)

        if self.countSign:
            self.mcweight = numpy.mean( [math.copysign(1., x) for x in mcw] )

        print 'aMC@NLO weight', self.mcweight

            
    def process(self, event):
        event.mcweight = float(self.mcweight)
