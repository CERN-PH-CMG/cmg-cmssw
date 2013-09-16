import os
import sys
import pprint
import logging
import copy
import glob
import random

from ROOT import TLorentzVector

from DataFormats.FWLite import Events, Handle

from CMGTools.H2TauTau.macros.DeltaR import cleanObjectCollection
from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter, Counters
from CMGTools.H2TauTau.macros.Average import Average
# from CMGTools.H2TauTau.macros.CountLeptons import leptonAccept
from CMGTools.H2TauTau.macros.PhysicsObjects import DiTau, Lepton, GenParticle, Jet, Tau, bestDiTau
from CMGTools.H2TauTau.macros.TauDecayModes import tauDecayModes
from CMGTools.H2TauTau.macros.Jets import testJet
from CMGTools.H2TauTau.macros.Jets import VBF
from CMGTools.H2TauTau.macros.TriggerEfficiency import TriggerEfficiency
from CMGTools.H2TauTau.macros.TriggerList import TriggerList
from CMGTools.H2TauTau.macros.Regions import H2TauTauRegions
from CMGTools.H2TauTau.macros.Looper import Looper
from CMGTools.H2TauTau.macros.H2TauTauOutput import H2TauTauOutput as H2TauTauOutput 
from CMGTools.H2TauTau.macros.H2TauTauOutput import inclusiveRegionName
from CMGTools.H2TauTau.macros.H2TauTauOutput import wholeMTRegionName

#COLIN need a Counter for each region
#COLIN add jet ID (check the effect of jetID)
#COLIN Can counters have branches?
#COLIN Can counters be merged? use a single counter?
#COLIN Need a base Loop class
#COLIN can the loop be written in a more pythonic way? should be able to iterate over it...
#      get some inspiration from the Events class... can maybe inherit from it? 

class Event:
    '''Just an empty structure to hold event-wise quantities, see Loop'''
    pass


class H2TauTauAnalyzer( Looper ):

    def __init__(self, name, component, cfg):
        self.name = name
        self.cmp = component
        self.cfg = cfg 
        self.events = Events( glob.glob( self.cmp.files) )
        self.triggerList = TriggerList( self.cmp.triggers )
        #SPEC? 
        if self.cmp.isMC or self.cmp.isEmbed:
            self.trigEff = TriggerEfficiency()
            self.trigEff.tauEff = None
            self.trigEff.lepEff = None
            if self.cmp.tauEffWeight is not None:
                self.trigEff.tauEff = getattr( self.trigEff,
                                               self.cmp.tauEffWeight )
            if self.cmp.muEffWeight is not None:
                self.trigEff.lepEff = getattr( self.trigEff,
                                               self.cmp.muEffWeight )
        
        # here create outputs
        self.regions = H2TauTauRegions( self.cfg.cuts )
        self.output = H2TauTauOutput( self.name, self.regions )
        if self.cmp.name == 'DYJets':
            self.outputFakes = H2TauTauOutput( self.name + '_Fakes', self.regions )

        self.logger = logging.getLogger(self.name)
        self.logger.addHandler(logging.FileHandler('/'.join([self.output.name,
                                                            'log.txt'])))

        self.counters = Counters()
        self.averages = {}        
        self.InitHandles()
        self.InitCounters()
    

    def LoadCollections(self, event ):
        '''Load all collections'''
        for str,handle in self.handles.iteritems():
            handle.Load( event )
            # could do something clever to get the products... a setattr maybe?
        if self.cmp.isMC:
            for str,handle in self.mchandles.iteritems():
                handle.Load( event )
        if self.cmp.isEmbed:
            for str,handle in self.embhandles.iteritems():
                handle.Load( event )
            
            
        
    def InitHandles(self):
        '''Initialize all handles for the products we want to read'''
        self.handles = {}
        self.mchandles = {}
        self.embhandles = {}
        self.handles['cmgTriggerObjectSel'] =  AutoHandle( 'cmgTriggerObjectSel',
                                                           'std::vector<cmg::TriggerObject>>')
        if self.cmp.isMC and self.cmp.vertexWeight is not None: 
            self.handles['vertexWeight'] = AutoHandle( self.cmp.vertexWeight,
                                                       'double' )
        self.handles['vertices'] = AutoHandle( 'offlinePrimaryVertices',
                                               'std::vector<reco::Vertex>' )
        self.handles['jets'] = AutoHandle( 'cmgPFJetSel',
                                           'std::vector<cmg::PFJet>' )
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )
        self.embhandles['generatorWeight'] = AutoHandle( ('generator', 'weight'),
                                                         'double')
        self.InitSpecificHandles()
        

    def InitCounters(self):
        '''Initialize histograms physics objects, counters.'''

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
        self.averages['generatorWeight']=Average('generatorWeight')
        self.averages['eventWeight']=Average('eventWeight')

            
    def process( self, iEv ):
        '''Navigate to a given event and process it.'''

        # output event structure
        self.event = Event()

        # navigating to the correct FWLite event
        self.iEvent = iEv
        self.events.to(iEv)
        self.LoadCollections(self.events)

        # reading CMG objects from the handle
        #COLIN this kind of stuff could be automatized
        cmgDiTaus = self.handles['diTau'].product()
        cmgLeptons = self.handles['leptons'].product()
        self.event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        self.event.vertices = self.handles['vertices'].product()
        cmgJets = self.handles['jets'].product()
        if self.cmp.isMC: 
            genParticles = self.mchandles['genParticles'].product()
            self.event.genParticles = map( GenParticle, genParticles)
        # converting them into my own python objects
        self.event.diTaus = [ DiTau(diTau) for diTau in cmgDiTaus ]
        self.event.leptons = [ Lepton(lepton) for lepton in cmgLeptons ]

        # self.event.dirtyJets = [ Jet(jet) for jet in cmgJets if testJet(jet, self.cfg.cuts) ] 
        self.event.dirtyJets = []
        for cmgJet in cmgJets:
            jet = Jet( cmgJet )
            if self.cmp.isMC:
                scale = random.gauss( self.cmp.jetScale, self.cmp.jetSmear)
                jet.scaleEnergy( scale )
            if not testJet( cmgJet, self.cfg.cuts):
                continue
            self.event.dirtyJets.append(jet)

        
        self.counters.counter('triggerPassed').inc('a: All events')
        if not self.triggerList.triggerPassed(self.event.triggerObject):
            return False
        self.counters.counter('triggerPassed').inc('b: Trig OK ')
        
        self.counters.counter('exactlyOneDiTau').inc('a: any # of di-taus ')
        if len(self.event.diTaus)==0:
            print 'Event %d : No di-tau.' % i
            return False
        if len(self.event.diTaus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.event.diTaus)) 
            #COLIN could be nice to have a counter class
            # which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-tau : n = %d' % (iEv,
                                                                        len(self.event.diTaus)))

        self.counters.counter('exactlyOneDiTau').inc('b: at least 1 di-tau ')

        #MUONS
        if not self.leptonAccept(self.event.leptons):
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
        if self.event.tau.decayMode() == 0 and \
               self.event.tau.calcEOverP() < 0.2: #MUONS
            return False
        else:
            self.counters.counter('singleDiTau').inc('b:   E/p > 0.2 ')

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

        ##################  Starting from here, we have the di-tau ###############


        # clean up jet collection

        self.event.jets = cleanObjectCollection( self.event.dirtyJets,
                                                 masks = [self.event.diTau.leg1(),
                                                          self.event.diTau.leg2() ],
                                                 deltaRMin = 0.5 )
#         print '-----------'
#        if len(self.event.dirtyJets)>0:
#            print 'Dirty:'
#            print '\n\t'.join( map(str, self.event.dirtyJets) )
#            print self.event.diTau
#            print 'Clean:'
#            print '\n\t'.join( map(str, self.event.jets) )
        

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
                        
                
        matched = None
        if self.cmp.name == 'DYJets':
            leg1DeltaR, leg2DeltaR = self.event.diTau.match( self.event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                matched = True
            else:
                matched = False

        self.event.eventWeight = 1
        # self.event.triggerWeight = 1
        self.event.vertexWeight = 1
        self.event.tauEffWeight = 1
        self.event.lepEffWeight = 1
        self.event.generatorWeight = 1 
        if self.cmp.isMC:
            self.event.vertexWeight = self.handles['vertexWeight'].product()[0]
        if self.cmp.isEmbed:
            self.event.generatorWeight = self.embhandles['generatorWeight'].product()[0]
        if self.cmp.isMC or self.cmp.isEmbed:
            if self.trigEff.tauEff is not None:
                self.event.tauEffWeight = self.trigEff.tauEff(self.event.tau.pt())
            #MUONS
            if self.trigEff.lepEff is not None:
                self.event.lepEffWeight = self.trigEff.lepEff( self.event.lepton.pt(),
                                                               self.event.lepton.eta() )

        self.event.eventWeight = self.event.vertexWeight * \
                                 self.event.tauEffWeight * \
                                 self.event.lepEffWeight * \
                                 self.event.generatorWeight

        # self.averages['triggerWeight'].add( self.event.triggerWeight )
        self.averages['tauEffWeight'].add( self.event.tauEffWeight )
        self.averages['lepEffWeight'].add( self.event.lepEffWeight )
        self.averages['vertexWeight'].add( self.event.vertexWeight )
        self.averages['generatorWeight'].add( self.event.generatorWeight )
        self.averages['eventWeight'].add( self.event.eventWeight ) 

        regionName = self.regions.test( self.event )
        if matched is None or matched is True:
            self.output.Fill( self.event, regionName )
        elif matched is False: 
            self.outputFakes.Fill( self.event, regionName )
        else:
            raise ValueError('should not happen!')
        return True


    def Write(self):
        '''Write all histograms to their root files'''
        # for hist in self.histograms:
        #     hist.Write()
        self.output.Write()
        if self.cmp.name == 'DYJets':
            self.outputFakes.Write()
        

    def __str__(self):
        name = 'Loop %s' % self.name
        component = str(self.cmp)
        counters = map(str, self.counters.counters) 
        strave = map(str, self.averages.values())
        return '\n'.join([name, component] +
                         counters + strave )
    


if __name__ == '__main__':

    print 'See MultiLoop.py for the main.'
