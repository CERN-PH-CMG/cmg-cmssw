import os
import sys
import pprint
import logging
import copy

from DataFormats.FWLite import Events, Handle

from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import H2TauTauHistogramList
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter
from CMGTools.H2TauTau.macros.CountLeptons import leptonAccept
from CMGTools.H2TauTau.macros.PhysicsObjects import DiTau, Lepton, Jet, bestDiTau


#COLIN Need a HistCounters class to represent all counters.
#COLIN Can counters have branches?
#COLIN Can counters be merged? use a single counter?
#COLIN Need a base Loop class
#COLIN can the loop be written in a more pythonic way? should be able to iterate over it...
#      get some inspiration from the Events class... can maybe inherit from it? 
        
class Loop:
    '''Manages looping and navigation on a set of events.'''
    def __init__(self, name, listOfFiles,
                 triggers=None,
                 vertexWeight=None):
        '''Build a loop object.

        listOfFiles can be "*.root".
        name will be used to make the output directory'''

        self.events = Events( listOfFiles )
        self.triggers = self.DecodeTriggerList(triggers)
        self.vertexWeightLabel = vertexWeight
        
        # if name exists as a directory, build another name.
        self.name = name
        index = 0
        while True:
            try:
                # print 'mkdir', self.name
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
        self.InitHandles()


    def DecodeTriggerList(self, triggers):
        if triggers is None:
            return None
        triglist = triggers.split('|')
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
        self.handles['cmgTauMuCorFullSelSVFit'] =  AutoHandle( 'cmgTauMuCorSVFitFullSel',
                                                               'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>')
##         self.handles['cmgTauMu'] =  AutoHandle( 'cmgTauMu',
##                                                 'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>')
        self.handles['cmgTriggerObjectSel'] =  AutoHandle( 'cmgTriggerObjectSel',
                                                           'std::vector<cmg::TriggerObject>>')
        if self.vertexWeightLabel is not None: 
            self.handles['vertexWeight'] = AutoHandle( self.vertexWeightLabel,
                                                       'double' )
        self.handles['vertices'] = AutoHandle( 'offlinePrimaryVertices',
                                               'std::vector<reco::Vertex>' )
        self.handles['leptons'] = AutoHandle( 'cmgMuonSel',
                                              'std::vector<cmg::Muon>' )
        #self.handles['jets'] = AutoHandle( 'cmgPFJetSel',
        #                                   'std::vector<cmg::PFJet>' )
        

    def InitOutput(self):
        '''Initialize histograms physics objects, counters.'''
        # declaring histograms
        self.histograms = []
        self.histsOS = H2TauTauHistogramList( '/'.join([self.name, 'OS']) )
        self.histograms.append( self.histsOS )
        self.histsSS = H2TauTauHistogramList( '/'.join([self.name, 'SS']) )
        self.histograms.append( self.histsSS )
        self.histsOSLowMT = H2TauTauHistogramList( '/'.join([self.name, 'OSLowMT']) )
        self.histograms.append( self.histsOSLowMT )
        self.histsSSLowMT = H2TauTauHistogramList( '/'.join([self.name, 'SSLowMT']) )
        self.histograms.append( self.histsSSLowMT )

        # declaring physics objects
        self.diTau = None
        self.triggerObject = None

        # declaring counters
        self.counters = []
        self.count_triggerPassed = Counter('triggerPassed')
        self.counters.append( self.count_triggerPassed )
        self.count_exactlyOneDiTau = Counter('exactlyOneDiTau')
        self.counters.append( self.count_exactlyOneDiTau  )
        self.count_singleDiTau = Counter('singleDiTau')
        self.counters.append( self.count_singleDiTau )
        

    def ToEvent( self, iEv ):
        '''Navigate to a given event and process it.'''
        self.iEvent = iEv
        self.events.to(iEv)
        self.LoadCollections(self.events)

        # reading CMG objects from the handle
        #COLIN this kind of stuff could be automatized
        cmgDiTaus = self.handles['cmgTauMuCorFullSelSVFit'].product()
        cmgLeptons = self.handles['leptons'].product()
        self.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        # cmgJets = self.handles['jets'].product()
        
        # converting them into my own python objects
        #COLIN can be automatized
        self.diTaus = [ DiTau(diTau) for diTau in cmgDiTaus ]
        self.leptons = [ Lepton(lepton) for lepton in cmgLeptons ]
        # self.jets = [ Jet(jet) for jet in cmgJets ]
        
        self.count_triggerPassed.inc('a: All events')
        if not self.triggerPassed(self.triggerObject):
            return False
        self.count_triggerPassed.inc('b: Trig OK ')
        
        self.count_exactlyOneDiTau.inc('a: any # of di-taus ')
        if len(self.diTaus)==0:
            print 'Event %d : No tau mu.' % i
            return False
        if len(self.diTaus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.diTaus)) 
            #COLIN could be nice to have a counter class
            # which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-tau : n = %d' % (iEv,
                                                                        len(self.diTaus)))

        self.count_exactlyOneDiTau.inc('b: at least 1 di-tau ')
        
        # diTaus = [ DiTau(diTau) for diTau in self.diTaus ]
        # pprint.pprint( map(str, self.leptons) ) 
        # print map(testMuon, self.leptons)

        #if leptonAcceptFromDiTaus(self.diTaus) != leptonAccept(self.leptons):
        #    raise ValueError('lepton veto discrepancy!')
        # if not leptonAcceptFromDiTaus(self.diTaus):
        if not leptonAccept(self.leptons):
            return False 
        self.count_exactlyOneDiTau.inc('c: exactly one lepton ')        

        self.diTau = self.diTaus[0]
        if len(self.diTaus)>1:
            self.diTau = bestDiTau( self.diTaus )
        elif len(self.diTaus)==1:
            self.count_exactlyOneDiTau.inc('d: exactly 1 di-tau ')
        else:
            raise ValueError('should not happen!')

        self.count_singleDiTau.inc('a:  best di-tau')
        self.tau = Tau( self.diTau.leg1() )
        if self.tau.calcEOverP() > 0.2:
            self.count_singleDiTau.inc('b:   E/p > 0.2 ')
        else:
            return False

        self.eventWeight = 1
        if self.vertexWeightLabel is not None:
            self.vertexWeight = self.handles['vertexWeight'].product()[0]
            self.eventWeight *= self.vertexWeight

        self.vertices = self.handles['vertices'].product()
        
        # COLIN make Filling just one function call?
        # in this class? in the histogram class?
        if self.diTau.charge() == 0:
            self.count_exactlyOneDiTau.inc('e: opposite charge ')
            self.histsOS.FillDiTau( self.diTau, self.eventWeight)
            self.histsOS.FillVertices( self.vertices, self.eventWeight )
            if( self.diTau.mTLeg2()<40 ):
                self.histsOSLowMT.FillDiTau( self.diTau, self.eventWeight)
                self.histsOSLowMT.FillVertices( self.vertices, self.eventWeight )
        else:
            self.histsSS.FillDiTau( self.diTau, self.eventWeight)
            self.histsSS.FillVertices( self.vertices, self.eventWeight )
            if( self.diTau.mTLeg2()<40 ):
                self.histsSSLowMT.FillDiTau( self.diTau, self.eventWeight)
                self.histsSSLowMT.FillVertices( self.vertices, self.eventWeight )
        return True

                
    def Loop(self, nEvents=-1 ):
        '''Loop on a given number of events, and call ToEvent for each event.'''
        print 'starting loop'
        self.InitOutput()
        nEvents = int(nEvents)
        for iEv in range(0, self.events.size() ):
            if iEv == nEvents:
                break
            if iEv%1000 ==0:
                print 'event', iEv
            try:
                self.ToEvent( iEv )
            except ValueError:
                #COLIN should not be a value error
                break 
        self.logger.warning( str(self) )


    def Write(self):
        '''Write all histograms to their root files'''
        for hist in self.histograms:
            hist.Write()


    def __str__(self):
        name = 'Loop %s' % self.name
        strcount = [str(counter) for counter in self.counters ]
        triggers = ': '.join( ['triggers', str(self.triggers)] )
        vertexWeight = ': '.join( ['vertex weight', str(self.vertexWeightLabel) ])
        return '\n'.join([name, triggers, vertexWeight] + strcount)
    


if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog component_name <root files>
    Loop on events in root files and Fill histograms.
    component_name could be e.g. WJets, whatever you want. 
    """
    parser.add_option("-N", "--nevents", 
                      dest="nevents", 
                      help="number of events to process",
                      default=float('inf'))
    parser.add_option("-t", "--triggerlist", 
                      dest="triggerlist", 
                      help="trigger list",
                      default=None)
    parser.add_option("-v", "--vertexWeight", 
                      dest="vertexWeight", 
                      help="vertex weight module name.",
                      default=None)
    
    (options,args) = parser.parse_args()
    if len(args) < 2:
        parser.print_help()
        print 'ERROR: please provide the component name and at least one root file'
        sys.exit(1)

    nEv = float(options.nevents)

    # args[0] is the component_name. Then come the root files
    loop = Loop( args[0], args[1:],
                 triggers = options.triggerlist,
                 vertexWeight = options.vertexWeight)
    print loop
    loop.Loop( nEv )
    loop.Write()
    print loop

