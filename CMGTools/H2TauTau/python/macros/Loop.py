import os
import sys
import pprint
import logging
import copy
import glob

from ROOT import TLorentzVector

from DataFormats.FWLite import Events, Handle

from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import H2TauTauHistogramList
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter
from CMGTools.H2TauTau.macros.Average import Average
from CMGTools.H2TauTau.macros.CountLeptons import leptonAccept
from CMGTools.H2TauTau.macros.PhysicsObjects import DiTau, Lepton, Jet, Tau, VBF, bestDiTau
from CMGTools.H2TauTau.macros.TurnOnCurve import TurnOnCurve
from CMGTools.H2TauTau.macros.TriggerList import TriggerList
from CMGTools.H2TauTau.macros.Regions import H2TauTauRegions

#COLIN put the counters in a list so that they stay sorted? 
#COLIN my cuts are defined in different places (Regions and Loop). I feel uncomfortable... 
#COLIN need a Counter for each region
#COLIN add jet ID (check the effect of jetID)
#COLIN Can counters have branches?
#COLIN Can counters be merged? use a single counter?
#COLIN Need a base Loop class
#COLIN can the loop be written in a more pythonic way? should be able to iterate over it...
#      get some inspiration from the Events class... can maybe inherit from it? 

class Event:
    '''Just an empty structure to old event-wise quantities, see Loop'''
    pass


class Loop:
    '''Manages looping and navigation on a set of events.'''
    def __init__(self, name, config, defaults):
        '''Build a loop object.

        listOfFiles can be "*.root".
        name will be used to make the output directory'''

        self.config = config
        self.defaultConfig = defaults 
        self.events = Events( glob.glob( self.config.files) )
        self.triggerList = TriggerList( self.config.triggers )
        self.vertexWeightLabel = self.config.vertexWeight
        self.tauPtCut = self.defaultConfig.tauPtCut
        self.leptonPtCut = self.defaultConfig.leptonPtCut
        self.leptonEtaCut = self.defaultConfig.leptonEtaCut

        if self.config.useTurnOn:
            if not config.isMC:
                raise ValueError('are you sure you want to apply trigger turn on curve efficiency weighting on data??')
            self.turnOnCurve = TurnOnCurve()
        else:
            self.turnOnCurve = None
        
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
        self.counters = {}
        self.averages = {}        
        # self.histograms = []
        self.InitHandles()



    def LoadCollections(self, event ):
        '''Load all collections'''
        for str,handle in self.handles.iteritems():
            handle.Load( event )
            # could do something clever to get the products... a setattr maybe?



        
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
        self.handles['jets'] = AutoHandle( 'cmgPFJetSel',
                                           'std::vector<cmg::PFJet>' )
        

    def InitOutput(self):
        '''Initialize histograms physics objects, counters.'''

        #COLIN do I really need to declare them? 
        # declaring physics objects
        self.diTau = None
        self.triggerObject = None

        # declaring counters and averages
        self.counters = {}
        self.counters['triggerPassed'] = Counter('triggerPassed')
        self.counters['exactlyOneDiTau'] = Counter('exactlyOneDiTau')
        self.counters['singleDiTau'] = Counter('singleDiTau')
        self.counters['jets'] = Counter('jets')
        
        self.averages['triggerWeight']=Average('triggerWeight')
        self.averages['vertexWeight']=Average('vertexWeight')
        self.averages['eventWeight']=Average('eventWeight')

        self.regions = H2TauTauRegions()
        self.histoLists = {}
        for regionName in self.regions.regionNames():
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName])) 
 
    def ToEvent( self, iEv ):
        '''Navigate to a given event and process it.'''

        # output event structure
        self.event = Event()

        # navigating to the correct FWLite event
        self.iEvent = iEv
        self.events.to(iEv)
        self.LoadCollections(self.events)

        # reading CMG objects from the handle
        #COLIN this kind of stuff could be automatized
        cmgDiTaus = self.handles['cmgTauMuCorFullSelSVFit'].product()
        cmgLeptons = self.handles['leptons'].product()
        self.event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        self.event.vertices = self.handles['vertices'].product()
        cmgJets = self.handles['jets'].product()
        
        # converting them into my own python objects
        #COLIN can be automatized
        self.event.diTaus = [ DiTau(diTau) for diTau in cmgDiTaus ]
        self.event.leptons = [ Lepton(lepton) for lepton in cmgLeptons ]
        self.event.jets = [ Jet(jet) for jet in cmgJets if jet.pt()>30 and jet.eta()>-4.5]


        self.counters['triggerPassed'].inc('a: All events')
        if not self.triggerList.triggerPassed(self.event.triggerObject):
            return False
        self.counters['triggerPassed'].inc('b: Trig OK ')
        
        self.counters['exactlyOneDiTau'].inc('a: any # of di-taus ')
        if len(self.event.diTaus)==0:
            print 'Event %d : No tau mu.' % i
            return False
        if len(self.event.diTaus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.event.diTaus)) 
            #COLIN could be nice to have a counter class
            # which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-tau : n = %d' % (iEv,
                                                                        len(self.event.diTaus)))

        self.counters['exactlyOneDiTau'].inc('b: at least 1 di-tau ')
        
        if not leptonAccept(self.event.leptons):
            return False 
        self.counters['exactlyOneDiTau'].inc('c: exactly one lepton ')        

        self.event.diTau = self.event.diTaus[0]
        if len(self.event.diTaus)>1:
            self.event.diTau = bestDiTau( self.event.diTaus )
        elif len(self.event.diTaus)==1:
            self.counters['exactlyOneDiTau'].inc('d: exactly 1 di-tau ')
        else:
            raise ValueError('should not happen!')

        self.counters['singleDiTau'].inc('a:  best di-tau')
        self.event.tau = Tau( self.event.diTau.leg1() )
        if self.event.tau.calcEOverP() > 0.2:
            self.counters['singleDiTau'].inc('b:   E/p > 0.2 ')
        else:
            return False

        if self.event.tau.pt()>self.tauPtCut:
            self.counters['singleDiTau'].inc('c:  tau pt > {ptCut:3.1f}'.format(ptCut = self.tauPtCut))
        else:
            return False

        self.event.lepton = Lepton( self.event.diTau.leg2() )
        if self.event.lepton.pt()>self.leptonPtCut:
            self.counters['singleDiTau'].inc('d:  lep pt > {ptCut:3.1f}'.format(ptCut = self.leptonPtCut))
        else:
            return False

        if abs( self.event.lepton.eta() ) < abs(self.leptonEtaCut):
            self.counters['singleDiTau'].inc('e:  lep |eta| <{etaCut:3.1f}'.format(etaCut = self.leptonEtaCut))
        else:
            return False

        self.counters['jets'].inc('a: all events ') 
        if len(self.event.jets)>1:
            self.counters['jets'].inc('b: at least 2 jets ') 
            self.event.vbf = VBF( self.event.jets )
            if self.event.vbf.mjj > 400:
                self.counters['jets'].inc('c: Mjj > 400   ')
                if abs(self.event.vbf.deta) > 4.:
                    self.counters['jets'].inc('d: deta > 4.0  ')
                    if len(self.event.vbf.centralJets)==0:
                        self.counters['jets'].inc('e: no central jet ')
                        
                
            # print self.event.vbf


        self.event.eventWeight = 1
        self.event.triggerWeight = 1
        self.event.vertexWeight = 1
        if self.vertexWeightLabel is not None:
            self.event.vertexWeight = self.handles['vertexWeight'].product()[0]
            self.event.eventWeight *= self.event.vertexWeight
        if self.turnOnCurve is not None:
            self.event.triggerWeight = self.turnOnCurve.weight( self.event.tau.pt() )
            self.event.eventWeight *= self.event.triggerWeight

        self.averages['triggerWeight'].add( self.event.triggerWeight )
        self.averages['vertexWeight'].add( self.event.vertexWeight )
        self.averages['eventWeight'].add( self.event.eventWeight ) 
            
        regionName = self.regions.test( self.event )
        histoList = self.histoLists[regionName]
        histoList.Fill( self.event, self.event.eventWeight )
        
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
        # for hist in self.histograms:
        #     hist.Write()
        for histoList in self.histoLists.values():
            histoList.Write()


    def __str__(self):
        name = 'Loop %s' % self.name
        config = str(self.config)
        strcount = map(str, self.counters.values())
        strave = map(str, self.averages)
        # triggers = ': '.join( ['triggers', str(self.triggers)] )
        trigs = str( self.triggerList )
        vertexWeight = ': '.join( ['vertex weight', str(self.vertexWeightLabel) ])
        return '\n'.join([name, config, trigs, vertexWeight] + strcount + strave )
    


if __name__ == '__main__':

    print 'See MultiLoop.py for the main.'
