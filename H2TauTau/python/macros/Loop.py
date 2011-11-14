import os
import sys
import pprint
import logging
import copy

from DataFormats.FWLite import Events, Handle

from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.H2TauTauHistograms import H2TauTauHistograms
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter


#COLIN Need a HistCounters class to represent all counters. 
#COLIN Need a base Loop class 

class DiTau:
    '''Extends the cmg::DiTau functionalities.'''
    def __init__(self, ditau):
        self.ditau = ditau

    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.ditau, name)

    def __str__(self):
        return 'ditau: mvis=%3.2f, mT=%3.2f, pZeta=%3.2f' % (self.ditau.mass(),
                                                             self.ditau.mTLeg2(),
                                                             self.ditau.pZeta() ) 


class Loop:
    '''Manages looping and navigation on a set of events.'''
    def __init__(self, name, listOfFiles, triggers=None):
        '''Build a loop object.

        listOfFiles can be "*.root".
        name will be used to make the output directory'''

        self.events = Events( listOfFiles )
        self.InitHandles()
        self.triggers = self.DecodeTriggerList(triggers)
        
        # if name exists as a directory, build another name.
        self.name = name
        index = 0
        while True:
            try:
                print 'mkdir', self.name
                os.mkdir( self.name )
                break
            except OSError:
                index += 1
                self.name = '%s_%d' % (name, index)

        self.logger = logging.getLogger(name)
        self.logger.addHandler(logging.FileHandler('/'.join([self.name,
                                                            'log.txt'])))
        self.counters = []
        self.histograms = []


    def DecodeTriggerList(self, triggers):
        if triggers is None:
            return None
        triglist = triggers.split('%')
        return triglist


    def LoadCollections(self, event ):
        '''Load all collections'''
        for str,handle in self.handles.iteritems():
            handle.Load( event )
            # could do something clever to get the products... a setattr maybe?


    def triggerPassed(self, triggerObject, triggerList = None):
        '''returns true if at least one of the triggers in self.triggers passes.'''
        if triggerList is None:
            triggerList = self.triggers
        if not triggerList or len(triggerList)==0:
            return True
        # just doing an OR of all triggers: 
        return reduce( lambda x,y : x or y,
                       [triggerObject.getSelectionRegExp( trigger ) for trigger in triggerList])

        
    def InitHandles(self):
        '''Initialize all handles for the products we want to read'''
        self.handles = {}
        self.handles['cmgTauMuFullSel'] =  AutoHandle( 'cmgTauMuFullSel',
                                                       'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>')
        self.handles['cmgTriggerObjectSel'] =  AutoHandle( 'cmgTriggerObjectSel',
                                                           'std::vector<cmg::TriggerObject>>')
        

    def InitOutput(self):
        '''Initialize histograms physics objects, counters.'''
        # declaring histograms
        self.histograms = []
        self.histsOS = H2TauTauHistograms( '/'.join([self.name, 'OS']) )
        self.histograms.append( self.histsOS )
        self.histsSS = H2TauTauHistograms( '/'.join([self.name, 'SS']) )
        self.histograms.append( self.histsSS )

        # declaring physics objects
        self.diTau = None
        self.triggerObject = None

        # declaring counters
        self.counters = []
        self.count_exactlyOneDiTau = Counter('exactlyOneDiTau')
        self.counters.append( self.count_exactlyOneDiTau  )
        self.count_triggerPassed = Counter('triggerPassed')
        self.counters.append( self.count_triggerPassed )
        

    def ToEvent( self, iEv ):
        '''Navigate to a given event and process it.'''
        self.events.to(iEv)
        self.LoadCollections(self.events)
        
        self.diTaus = self.handles['cmgTauMuFullSel'].product()
        self.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]

        self.count_triggerPassed.inc('a: All events')
        if not self.triggerPassed(self.triggerObject):
            return False
        self.count_triggerPassed.inc('b: Trig OK  ')
        
        self.count_exactlyOneDiTau.inc('a')
        if len(self.diTaus)==0:
            print 'Event %d : No tau mu.' % i
            return False
        if len(self.diTaus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.diTaus)) 
            #COLIN could be nice to have a counter class which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-tau : n = %d' % (iEv,
                                                                        len(self.diTaus)))
            return False
        # self.nSel += 1
        self.count_exactlyOneDiTau.inc('b')
        self.diTau = DiTau( self.diTaus[0] )
        #Colin: the following is working, generalize it
        # myDiTau = DiTau( self.diTau )
        # print myDiTau, myDiTau.energy()
        if self.diTau.charge() == 0:
            self.histsOS.fillDiTau( self.diTau )
        else:
            self.histsSS.fillDiTau( self.diTau )
        return True

                
    def Loop(self, nEvents=float('inf') ):
        '''Loop on a given number of events, and call ToEvent for each event.'''
        print 'starting loop'
        self.InitOutput()
        for iEv in range(0, self.events.size() ):
            # print event
            if iEv ==nEvents:
                break
            if iEv%1000 ==0:
                print 'event', iEv
            self.ToEvent( iEv )
        self.logger.warning( str(self) )


    def Write(self):
        '''Write all histograms to their root files'''
        for hist in self.histograms:
            hist.Write()


    def __str__(self):
        name = 'Loop %s' % self.name
        strcount = [str(counter) for counter in self.counters ]
        triggers = str( self.triggers )
        return '\n'.join([name, triggers] + strcount)
    


if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog component_name <root files>
    Loop on events in root files and fill histograms.
    component_name could be e.g. WJets, whatever you want. 
    """
    parser.add_option("-N", "--nevents", 
                      dest="nevents", 
                      help="number of events to process",
                      default=float('inf'))
    parser.add_option("-t", "--triggerlist", 
                      dest="triggerlist", 
                      help="trigger list")
    
    (options,args) = parser.parse_args()
    if len(args) < 2:
        print 'ERROR: please provide the component name and at least one root file'
        sys.exit(1)

    nEv = float(options.nevents)

    # args[0] is the component_name. Then come the root files
    loop = Loop( args[0], args[1:], options.triggerlist)
    print loop
    loop.Loop( nEv )
    loop.Write()
    print loop

