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
from CMGTools.H2TauTau.macros.Counter import Counter, Counters
from CMGTools.H2TauTau.macros.Average import Average
from CMGTools.H2TauTau.macros.CountLeptons import leptonAccept
from CMGTools.H2TauTau.macros.PhysicsObjects import DiTau, Lepton, Jet, Tau, bestDiTau
from CMGTools.H2TauTau.macros.Jets import testJet
from CMGTools.H2TauTau.macros.Jets import VBF
from CMGTools.H2TauTau.macros.TriggerEfficiency import TriggerEfficiency
from CMGTools.H2TauTau.macros.TriggerList import TriggerList
from CMGTools.H2TauTau.macros.Regions import H2TauTauRegions

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


def inclusiveRegionName(name):
    newName = name.split('_')[0:2]
    newName.append('Inclusive')
    return '_'.join( newName )

class Loop:
    '''Manages looping and navigation on a set of events.'''
    def __init__(self, name, component, cfg):
        '''Build a loop object.

        listOfFiles can be "*.root".
        name will be used to make the output directory'''

        self.name = name
        self.cmp = component
        self.cfg = cfg 
        self.events = Events( glob.glob( self.cmp.files) )
        self.triggerList = TriggerList( self.cmp.triggers )
        if self.cmp.isMC:
            self.trigEff = TriggerEfficiency()
            self.trigEff.tauEff = None
            self.trigEff.lepEff = None
            if self.cmp.tauEffWeight is not None:
                self.trigEff.tauEff = getattr( self.trigEff,
                                               self.cmp.tauEffWeight )
            if self.cmp.muEffWeight is not None:
                self.trigEff.lepEff = getattr( self.trigEff,
                                               self.cmp.muEffWeight )
        
        #self.cmp.turnOnCurve = None
        #if self.cmp.isMC:
        #    if self.cmp.tauTriggerTOC is not None:
        #        self.cmp.turnOnCurve = TurnOnCurve( self.cmp.tauTriggerTOC )
 
        self._MakeOutputDir()
        self.counters = Counters()
        self.averages = {}        
        # self.histograms = []
        self.InitHandles()


    def _MakeOutputDir(self):
        index = 0
        name = self.name
        while True:
            try:
                # print 'mkdir', self.name
                os.mkdir( name )
                break
            except OSError:
                index += 1
                name = '%s_%d' % (self.name, index)
            
        self.logger = logging.getLogger(self.name)
        self.logger.addHandler(logging.FileHandler('/'.join([self.name,
                                                            'log.txt'])))
        

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
        if self.cmp.isMC and self.cmp.vertexWeight is not None: 
            self.handles['vertexWeight'] = AutoHandle( self.cmp.vertexWeight,
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
        self.counters = Counters()
        self.counters.addCounter('triggerPassed')
        self.counters.addCounter('exactlyOneDiTau')
        self.counters.addCounter('singleDiTau')
        self.counters.addCounter('VBF')
        
        # self.averages['triggerWeight']=Average('triggerWeight')
        self.averages['lepEffWeight']=Average('lepEffWeight')
        self.averages['tauEffWeight']=Average('tauEffWeight')
        self.averages['vertexWeight']=Average('vertexWeight')
        self.averages['eventWeight']=Average('eventWeight')

        self.regions = H2TauTauRegions( self.cfg.cuts )
        self.histoLists = {}
        inclusiveRegions = set()
        for regionName in self.regions.regionNames():
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName])) 
            incRegName = inclusiveRegionName( regionName )
            inclusiveRegions.add( incRegName )
        for regionName in inclusiveRegions:
            self.histoLists[ regionName ] = H2TauTauHistogramList( '/'.join([self.name, regionName ])) 
            
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
        self.event.diTaus = [ DiTau(diTau) for diTau in cmgDiTaus ]
        self.event.leptons = [ Lepton(lepton) for lepton in cmgLeptons ]
        self.event.jets = [ Jet(jet) for jet in cmgJets if testJet(jet, self.cfg.cuts) ]
        self.event.jets = []
        for cmgJet in cmgJets:
            if not testJet( jet, self.cfg.cuts):
                continue
            jet = Jet( cmgJet )
            # print jet.energy()
            jet.scaleEnergy( 1 )
            # print jet.energy()
            self.event.jets.append(jet)

        self.counters.counter('triggerPassed').inc('a: All events')
        if not self.triggerList.triggerPassed(self.event.triggerObject):
            return False
        self.counters.counter('triggerPassed').inc('b: Trig OK ')
        
        self.counters.counter('exactlyOneDiTau').inc('a: any # of di-taus ')
        if len(self.event.diTaus)==0:
            print 'Event %d : No tau mu.' % i
            return False
        if len(self.event.diTaus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.event.diTaus)) 
            #COLIN could be nice to have a counter class
            # which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-tau : n = %d' % (iEv,
                                                                        len(self.event.diTaus)))

        self.counters.counter('exactlyOneDiTau').inc('b: at least 1 di-tau ')
        
        if not leptonAccept(self.event.leptons):
            return False 
        self.counters.counter('exactlyOneDiTau').inc('c: exactly one lepton ')        

        self.event.diTau = self.event.diTaus[0]
        if len(self.event.diTaus)>1:
            self.event.diTau = bestDiTau( self.event.diTaus )
        elif len(self.event.diTaus)==1:
            self.counters.counter('exactlyOneDiTau').inc('d: exactly 1 di-tau ')
        else:
            raise ValueError('should not happen!')

        cuts = self.cfg.cuts
        
        self.counters.counter('singleDiTau').inc('a:  best di-tau')
        self.event.tau = Tau( self.event.diTau.leg1() )
        if self.event.tau.calcEOverP() > 0.2:
            self.counters.counter('singleDiTau').inc('b:   E/p > 0.2 ')
        else:
            return False

        if self.event.tau.pt()>cuts.tauPt:
            self.counters.counter('singleDiTau').inc('c:  tau pt > {ptCut:3.1f}'.format(ptCut = cuts.tauPt))
        else:
            return False

        self.event.lepton = Lepton( self.event.diTau.leg2() )
        if self.event.lepton.pt()>cuts.lepPt:
            self.counters.counter('singleDiTau').inc('d:  lep pt > {ptCut:3.1f}'.format(ptCut = cuts.lepPt))
        else:
            return False

        if abs( self.event.lepton.eta() ) < cuts.lepEta:
            self.counters.counter('singleDiTau').inc('e:  lep |eta| <{etaCut:3.1f}'.format(etaCut = cuts.lepEta))
        else:
            return False

        self.counters.counter('VBF').inc('a: all events ') 
        if len(self.event.jets)>1:
            self.counters.counter('VBF').inc('b: at least 2 jets ') 
            self.event.vbf = VBF( self.event.jets )
            if self.event.vbf.mjj > cuts.VBF_Mjj:
                self.counters.counter('VBF').inc('c: Mjj > {mjj:3.1f}'.format(mjj = cuts.VBF_Mjj))
                if abs(self.event.vbf.deta) > cuts.VBF_Deta:
                    self.counters.counter('VBF').inc('d: deta > {deta:3.1f}'.format(deta = cuts.VBF_Deta))
                    if len(self.event.vbf.centralJets)==0:
                        self.counters.counter('VBF').inc('e: no central jet ')
                        
                
            # print self.event.vbf


        self.event.eventWeight = 1
        # self.event.triggerWeight = 1
        self.event.vertexWeight = 1
        self.event.tauEffWeight = 1
        self.event.lepEffWeight = 1
        if self.cmp.isMC:
            self.event.vertexWeight = self.handles['vertexWeight'].product()[0]
            self.event.eventWeight *= self.event.vertexWeight
            if self.trigEff.tauEff is not None:
                self.event.tauEffWeight = self.trigEff.tauEff(self.event.tau.pt())
            if self.trigEff.lepEff is not None:
                self.event.lepEffWeight = self.trigEff.lepEff(
                    self.event.lepton.pt(),
                    self.event.lepton.eta() )
            self.event.eventWeight = self.event.vertexWeight * \
                                     self.event.tauEffWeight * \
                                     self.event.lepEffWeight
            
            # if self.cmp.turnOnCurve is not None:
            #    self.event.triggerWeight = self.cmp.turnOnCurve.weight(
            #        self.event.tau.pt() )
            #    self.event.eventWeight *= self.event.triggerWeight

        # self.averages['triggerWeight'].add( self.event.triggerWeight )
        self.averages['tauEffWeight'].add( self.event.tauEffWeight )
        self.averages['lepEffWeight'].add( self.event.lepEffWeight )
        self.averages['vertexWeight'].add( self.event.vertexWeight )
        self.averages['eventWeight'].add( self.event.eventWeight ) 

        # exclusive analysis 
        regionName = self.regions.test( self.event )
        histoList = self.histoLists[regionName]
        histoList.Fill( self.event, self.event.eventWeight )

        # inclusive analysis
        incRegionName = inclusiveRegionName( regionName )
        histoList = self.histoLists[incRegionName]
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
        component = str(self.cmp)
        counters = map(str, self.counters.counters) 
        strave = map(str, self.averages.values())
        # triggers = ': '.join( ['triggers', str(self.triggers)] )
        # trigs = str( self.triggerList )
        # vertexWeight = ': '.join( ['vertex weight', str(self.cmp.vertexWeight) ])
        return '\n'.join([name, component] +
                         counters + strave )
    


if __name__ == '__main__':

    print 'See MultiLoop.py for the main.'
