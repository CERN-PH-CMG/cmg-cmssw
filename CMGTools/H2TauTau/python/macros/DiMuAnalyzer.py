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
# from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.Counter import Counter, Counters
from CMGTools.H2TauTau.macros.Average import Average
# from CMGTools.H2TauTau.macros.CountLeptons import leptonAccept
from CMGTools.H2TauTau.macros.PhysicsObjects import DiLepton, Lepton, GenParticle, Jet
from CMGTools.H2TauTau.macros.PhysicsObjects import bestDiTau as bestDiMu
# from CMGTools.H2TauTau.macros.TauDecayModes import tauDecayModes
from CMGTools.H2TauTau.macros.Jets import testJet
from CMGTools.H2TauTau.macros.Jets import VBF
from CMGTools.H2TauTau.macros.TriggerEfficiency import TriggerEfficiency
from CMGTools.H2TauTau.macros.TriggerList import TriggerList
# from CMGTools.H2TauTau.macros.Regions import H2TauTauRegions
from CMGTools.H2TauTau.macros.Looper import Looper
from CMGTools.H2TauTau.macros.DiMuOutput import DiMuOutput 
# from CMGTools.H2TauTau.macros.H2TauTauOutput import inclusiveRegionName
# from CMGTools.H2TauTau.macros.H2TauTauOutput import wholeMTRegionName


class Event:
    '''Just an empty structure to hold event-wise quantities, see Loop'''
    pass


class DiMuAnalyzer( Looper ):

    def __init__(self, name, component, cfg):
        super( DiMuAnalyzer, self).__init__(component.fraction)
        self.name = name
        self.cmp = component
        self.cfg = cfg 
        self.events = Events( glob.glob( self.cmp.files) )
        self.triggerList = TriggerList( self.cmp.triggers )
##         if self.cmp.isMC or self.cmp.isEmbed:
##             self.trigEff = TriggerEfficiency()
##             self.trigEff.tauEff = None
##             self.trigEff.lepEff = None
##             if self.cmp.tauEffWeight is not None:
##                 self.trigEff.tauEff = getattr( self.trigEff,
##                                                self.cmp.tauEffWeight )
##             if self.cmp.muEffWeight is not None:
##                 self.trigEff.lepEff = getattr( self.trigEff,
##                                                self.cmp.muEffWeight )
        
        # here create outputs
##         self.regions = H2TauTauRegions( self.cfg.cuts )
##         self.output = Output( self.name, self.regions )
##         if self.cmp.name == 'DYJets':
##             self.outputFakes = Output( self.name + '_Fakes', self.regions )

        self.output = DiMuOutput( self.name )

        self.counters = Counters()
        self.averages = {}        
        self.InitHandles()
        self.InitCounters()
    
        self.logger = logging.getLogger(self.name)
        self.logger.addHandler(logging.FileHandler('/'.join([self.output.name,
                                                            'log.txt'])))

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

        self.handles['diMu'] =  AutoHandle( 'cmgDiMuonSel',
                                             'std::vector<cmg::DiObject<cmg::Muon,cmg::Muon>>')
        
                

    def InitCounters(self):
        '''Initialize histograms physics objects, counters.'''

        # declaring counters and averages
        self.counters = Counters()
        self.counters.addCounter('triggerPassed')
        self.counters.addCounter('exactlyOneDiMu')
        self.counters.addCounter('singleDiMu')
        self.counters.addCounter('VBF')
        
        # self.averages['triggerWeight']=Average('triggerWeight')
##         self.averages['lepEffWeight']=Average('lepEffWeight')
##         self.averages['tauEffWeight']=Average('tauEffWeight')
        self.averages['vertexWeight']=Average('vertexWeight')
##         self.averages['generatorWeight']=Average('generatorWeight')
        self.averages['eventWeight']=Average('eventWeight')

            
    def process( self, iEv ):
        '''Navigate to a given event and process it.'''

        cuts = self.cfg.cuts

        # output event structure
        self.event = Event()

        # navigating to the correct FWLite event
        self.iEvent = iEv
        self.events.to(iEv)
        self.LoadCollections(self.events)

        # reading CMG objects from the handle
        #COLIN this kind of stuff could be automatized
        cmgDiMus = self.handles['diMu'].product()
##         cmgLeptons = self.handles['leptons'].product()
        self.event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        self.event.vertices = self.handles['vertices'].product()
        cmgJets = self.handles['jets'].product()
        if self.cmp.isMC: 
            genParticles = self.mchandles['genParticles'].product()
            self.event.genParticles = map( GenParticle, genParticles)
        # converting them into my own python objects
        
        def testMuon(muon):
            if muon.pt() > cuts.muPt:
                return True
            else:
                return False
        def testDiMuon(diMuon):
            return testMuon( diMuon.leg1() ) and testMuon( diMuon.leg2() )
        self.event.diMus = [ DiLepton(diMu) for diMu in cmgDiMus if testDiMuon(diMu) ]
        
        # self.event.leptons = [ Lepton(lepton) for lepton in cmgLeptons ]

        self.event.dirtyJets = []
        for cmgJet in cmgJets:
            jet = Jet( cmgJet )
            if self.cmp.isMC:
                scale = random.gauss( self.cmp.jetScale, self.cmp.jetSmear)
                jet.scaleEnergy( scale )
            if not testJet( cmgJet, cuts):
                continue
            self.event.dirtyJets.append(jet)

        
        self.counters.counter('triggerPassed').inc('a: All events')
        if not self.triggerList.triggerPassed(self.event.triggerObject):
            return False
        self.counters.counter('triggerPassed').inc('b: Trig OK ')
        
        self.counters.counter('exactlyOneDiMu').inc('a: any # of di-mus ')
        if len(self.event.diMus)==0:
            # print 'Event %d : No di-mu.' % iEv
            return False
        if len(self.event.diMus)>1:
            # print 'Event %d : Too many tau-mus: n = %d' % (iEv, len(self.event.diMus)) 
            #COLIN could be nice to have a counter class
            # which knows why events are rejected. make histograms with that.
            self.logger.warning('Ev %d: more than 1 di-mu : n = %d' % (iEv,
                                                                        len(self.event.diMus)))

        self.counters.counter('exactlyOneDiMu').inc('b: at least 1 di-mu ')

        #MUONS
        #if not self.leptonAccept(self.event.leptons):
        #    return False 
        # self.counters.counter('exactlyOneDiMu').inc('c: exactly one lepton ')        

        self.event.diMu = self.event.diMus[0]
        if len(self.event.diMus)>1:
            self.event.diMu = bestDiMu( self.event.diMus )
        elif len(self.event.diMus)==1:
            self.counters.counter('exactlyOneDiMu').inc('d: exactly 1 di-mu ')
        else:
            raise ValueError('should not happen!')

                
    
        ##################  Starting from here, we have the di-mu ###############

        self.event.diTau = self.event.diMu
        diMu = self.event.diMu 

        self.counters.counter('singleDiMu').inc('a:  best di-mu')

        if diMu.leg1().pt() > cuts.muPt and \
           diMu.leg2().pt() > cuts.muPt :
            self.counters.counter('singleDiMu').inc(
                'b:  pt1,2 > {pt:3.1f}'.format( pt = cuts.muPt)
                )
        else:
            return False
        
        if abs(diMu.leg1().eta()) < cuts.muEta and \
           abs(diMu.leg2().eta()) < cuts.muEta :
            self.counters.counter('singleDiMu').inc(
                'c:  |eta1,2| < {eta:3.1f}'.format( eta = cuts.muEta)
                )
        else:
            return False
        
        if cuts.minMass < diMu.mass() and diMu.mass() < cuts.maxMass:
            self.counters.counter('singleDiMu').inc(
                'd:  {min:3.1f} < m < {max:3.1f}'.format(min = cuts.minMass, max = cuts.maxMass)
                )
        else:
            return False

        if diMu.leg1().relIso( 0.5 ) < cuts.muIso and \
           diMu.leg2().relIso( 0.5 ) < cuts.muIso:
            self.counters.counter('singleDiMu').inc(
                'e:  iso1,2 < {iso:3.1f}'.format(iso=cuts.muIso)
                )
        else:
            return False

        #SELECT BARREL MUONS
            
        # clean up jet collection

        self.event.jets = cleanObjectCollection( self.event.dirtyJets,
                                                 masks = [self.event.diMu.leg1(),
                                                          self.event.diMu.leg2() ],
                                                 deltaRMin = 0.5 )
#         print '-----------'
#        if len(self.event.dirtyJets)>0:
#            print 'Dirty:'
#            print '\n\t'.join( map(str, self.event.dirtyJets) )
#            print self.event.diMu
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
                        
        
        self.event.eventWeight = 1
        # self.event.triggerWeight = 1
        self.event.vertexWeight = 1
        if self.cmp.isMC:
            self.event.vertexWeight = self.handles['vertexWeight'].product()[0]
            self.event.eventWeight = self.event.vertexWeight
            
        self.output.Fill( self.event )
        
        self.averages['vertexWeight'].add( self.event.vertexWeight )
        self.averages['eventWeight'].add( self.event.eventWeight ) 

        return True


    def Write(self):
        '''Write all histograms to their root files'''
        self.output.Write()

    def __str__(self):
        name = 'Loop %s' % self.name
        component = str(self.cmp)
        counters = map(str, self.counters.counters) 
        strave = map(str, self.averages.values())
        return '\n'.join([name, component] +
                         counters + strave )
    


if __name__ == '__main__':

    print 'See MultiLoop.py for the main.'
