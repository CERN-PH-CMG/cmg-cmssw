import logging 

from CMGTools.H2TauTau.proto.framework.TriggerList import TriggerList
from CMGTools.H2TauTau.proto.statistics.Counter import Counters

class Analyzer(object):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        self.name = cfg_ana.name
        self.verbose = cfg_ana.verbose
        self.cfg_ana = cfg_ana
        self.cfg_comp = cfg_comp
        self.looperName = looperName
        # this is the main logger corresponding to the looper.
        # each analyzer could also declare its own logger
        self.mainLogger = logging.getLogger( looperName )
        self.triggerList = TriggerList( self.cfg_comp.triggers )

    def declareHandles(self):
        self.handles = {}
        self.mchandles = {}
        self.embhandles = {}

    def beginLoop(self):
        self.declareHandles()
        self.counters = Counters()
        self.mainLogger.warning( 'beginLoop ' + self.cfg_ana.name ) 

        
    def endLoop(self):
        #print self.cfg_ana
        self.mainLogger.warning( '' )
        self.mainLogger.warning( str(self) )
        self.mainLogger.warning( '' )

    def process(self, iEvent, event ):
        print self.cfg_ana.name 

    def readCollections(self, iEvent ):
        '''Overload that to fill event.diLeptons and event.leptons.'''
        for str,handle in self.handles.iteritems():
            handle.Load( iEvent )
        if self.cfg_comp.isMC:
            for str,handle in self.mchandles.iteritems():
                handle.Load( iEvent )
        if self.cfg_comp.isEmbed:
            for str,handle in self.embhandles.iteritems():
                handle.Load( iEvent )

    def write(self):
        print 'writing not implemented for', self.cfg_ana.name 

    def __str__(self):
        ana = str( self.cfg_ana )
        count = ''
        if hasattr(self, 'counters') and len( self.counters.counters ) > 0:
            # print 'Counters:'
            count = '\n'.join(map(str, self.counters.counters))
            # print '\n'.join( count )
        return '\n'.join( [ana, count] )
        # comp = str( self.cfg_comp )
        # return '\n'.join( [ana, comp] )
        

