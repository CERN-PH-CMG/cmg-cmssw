import os
import sys
import pprint

from DataFormats.FWLite import Events, Handle

from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.H2TauTauHistograms import H2TauTauHistograms
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter



#COLIN Need a Counter class and a HistCounters class to represent all counters. 
#COLIN Need a class to represent the ditau, with a print function etc. can I directly inherit from the diTau that is read from the file? 
#COLIN Need a base Loop class 

class Loop:
    '''Manages looping and navigation on a set of events.'''
    def __init__(self, name, listOfFiles ):
        '''Build a loop object.

        listOfFiles can be "*.root".
        name will be used to make the output directory'''
        self.name = name
        self.events = Events( listOfFiles )
        self.InitHandles()
#         self.nTot = 0
#         self.nSel = 0

        self.diTau = None
        
        # if name exists as a directory, build another name.
        index = 0
        while True:
            try: 
                os.mkdir( self.name )
                break
            except OSError:
                index += 1
                self.name = '%s_%d' % (name, index)

        # declaring histograms
        self.histograms = []
        self.histsOS = H2TauTauHistograms( '/'.join([self.name, 'OS']) )
        self.histograms.append( self.histsOS )
        self.histsSS = H2TauTauHistograms( '/'.join([self.name, 'SS']) )
        self.histograms.append( self.histsSS )

        
    def InitHandles(self):
        '''Initialize all handles for the products we want to read'''
        self.handles = {}
        self.handles['cmgTauMuBaselineSel'] =  AutoHandle( 'cmgTauMuBaselineSel',
                                                           'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>') 

    def LoadCollections(self, event ):
        '''Load all collections'''
        for str,handle in self.handles.iteritems():
            handle.Load( event )
            # could do something clever to get the products... a setattr maybe?

    def ToEvent( self, iEvent ):
        '''Navigate to a given event number, and load all collections'''
        self.events.to(iEvent)
        self.LoadCollections(self.events)
        self.tauMus = self.handles['cmgTauMuBaselineSel'].product()
        # print self.tauMus[0].mass()
        
    def Loop(self, nEvents=float('inf') ):
        '''Loop on a given number of events'''
        # self.nTot = 0
        # self.nSel = 0

        # declaring counters
        self.exactlyOneDiTau = Counter('exactlyOneDiTau')

        for iEv in range(0, self.events.size() ):
            # print event
            if iEv ==nEvents:
                return
            # self.nTot += 1
            self.exactlyOneDiTau.inc('a')
            if iEv%1000 ==0:
                print 'event', iEv

            # self.LoadCollections(event)
            # tauMus = self.handles['cmgTauMuBaselineSel'].product()
            self.ToEvent( iEv )

            # do I want to put the following in ToEvent?
            # should I call that function processEvent? 
            if len(self.tauMus)==0:
                print 'Event %d : No tau mu.' % i
                continue
            if len(self.tauMus)>1:
                print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.tauMus)) 
                #COLIN could be nice to have a counter class which knows why events are rejected. make histograms with that.
                continue
            # self.nSel += 1
            self.exactlyOneDiTau.inc('b')
            self.diTau = self.tauMus[0]
            if self.diTau.charge() == 0:
                self.histsOS.fillDiTau( self.diTau )
            else:
                self.histsSS.fillDiTau( self.diTau )
                

    def Write(self):
        '''Write all histograms to their root files'''
        for hist in self.histograms:
            hist.Write()

    def __str__(self):
        retstr = 'Loop %s\n' % self.name
        retstr += str(self.exactlyOneDiTau)
        return retstr
#        return 'Loop %s : %d / %d = %f events selected' % (self.name,
#                                                           self.nSel,
#                                                           self.nTot,
#                                                           float(self.nSel)/self.nTot) 
        

if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <root files>
    Loop on events in root files and fill histograms
    """
    parser.add_option("-N", "--nevents", 
                      dest="nevents", 
                      help="number of events to process",
                      default=float('inf'))
    (options,args) = parser.parse_args()
    if len(args) == 0:
        print 'ERROR: please provide at least one root file'
        sys.exit(1)

    nEv = float(options.nevents)

    loop = Loop( 'Test', args )
    loop.Loop( nEv )
    loop.Write()
    print loop

