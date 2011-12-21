import imp
import glob
from DataFormats.FWLite import Events, Handle

class Event(object):

    def __init__(self, iEv):
        self.iEv = iEv
        
    def __str__(self):
        # I might want to get such a function by default for all of my classes
        header = '{type}:'.format( type=self.__class__.__name__)
        varlines = []
        for var,value in sorted(vars(self).iteritems()):
            # if hasattr(value, '__dict__'):
            #    value = str( vars(value) )
            tmp = None
            try:
                if str(iter( value )).startswith('<ROOT.reco::candidate'):
                    # a single reco::Candidate appears to be iterable...
                    # here, I want to consider it as an object, not a sequence.
                    raise TypeError('is not a vector')
                tmp = map(str, value)
            except TypeError:
                tmp = value
            varlines.append( '\t{var:<15}:   {value}'.format(var=var, value=tmp) )
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )

                
class Looper(object):
    def __init__( self, name, cfg_comp, sequence, nPrint=0):
        '''Handles the processing of an event sample.

        name    : name of the Looper, will determine the output directory
        cfg_comp: information for the input sample, see Config
        sequence: an ordered list of analyzers
        nPrint  : number of events to print
        '''
        self.name = name 
        self.cfg_comp = cfg_comp 
        self.analyzers = map( self._buildAnalyzer, sequence )
        self.nPrint = nPrint 
        # initialize FWLite chain on input file: 
        self.events = Events( glob.glob( self.cfg_comp.files) )

    def _buildAnalyzer(self, cfg_ana):
        print 'building analyzer', cfg_ana.name
        # look for a module with name cfg_ana.name
        # thisFile = self.
        # import it
        # create an instance and return it
        obj = None
        try:
            file, path, desc = imp.find_module( cfg_ana.name )
            mod  = imp.load_module( cfg_ana.name,
                                    file, path, desc )
            cls = mod.__dict__[cfg_ana.name]
            obj = cls( cfg_ana, self.cfg_comp) 
        finally:
            file.close()
        return obj

    def loop(self, nEvents=None):
        '''Loop on a given number of events, and call ToEvent for each event.'''
        print 'starting loop'
        if nEvents is None:
            nEvents = float('+inf')
        else:
            nEvents = int(nEvents)
        eventSize = nEvents
        for analyzer in self.analyzers:
            analyzer.beginLoop() 
        for iEv in range(0, eventSize):
            if iEv == nEvents:
                break
            if iEv%10 ==0:
                print 'event', iEv
            self.process( iEv )
            if iEv<self.nPrint:
                print self.event
        for analyzer in self.analyzers:
            analyzer.endLoop()

    def process(self, iEv ):
        '''Run event processing for all analyzers in the sequence. Could use the returned
        tuple to keep track of the failing analyzer.'''
        self.event = Event( iEv )
        self.iEvent = iEv
        self.events.to(iEv)
        for analyzer in self.analyzers:
            if analyzer.process( self.events, self.event ) == False:
                return (False, analyzer.name)
        return (True, analyzer.name)
            
    def write(self):
        '''Writes all analyzers. See Analyzer for the meaning of Write...'''
        for analyzer in self.analyzers:
            analyzer.write()
        pass 

    
if __name__ == '__main__':


    import imp
    import sys
    import os
    
    
    cfgFileName = sys.argv[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    sys.path.append( '/'.join( [ os.environ['CMSSW_BASE'],
                                 'src/CMGTools/H2TauTau/python/proto/analyzers'] ))
    looper = Looper( 'name', cfg.selectedComponents[0], cfg.sequence, nPrint = 5)
    looper.loop( 1000 )
