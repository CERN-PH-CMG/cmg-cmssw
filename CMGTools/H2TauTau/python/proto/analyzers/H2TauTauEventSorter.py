from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.statistics.average import Average
from CMGTools.RootTools.statistics.Tree import Tree
from ROOT import TFile

from CMGTools.H2TauTau.proto.analyzers.Regions import H2TauTauRegions
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import H2TauTauOutput as H2TauTauOutput 
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import inclusiveRegionName
from CMGTools.H2TauTau.proto.analyzers.H2TauTauOutput import wholeMTRegionName
from PhysicsTools.Heppy.physicsobjects import GenParticle 


import os, errno

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST:
            pass
        else: raise


class H2TauTauEventSorter( Analyzer ):
    '''Analyze the event content, and fills histograms for the signal
    and control regions.'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(H2TauTauEventSorter,self).__init__(cfg_ana, cfg_comp, looperName)
        self.regions = H2TauTauRegions( self.cfg_ana )
        self.output = H2TauTauOutput(  self.dirName, self.regions,
                                       self.cfg_ana.leg1,
                                       self.cfg_ana.leg2 )
        self.treeFile = TFile('/'.join([self.dirName, 'tree.root']), 'recreate')
        self.tree = Tree('tree','flat ntuple')
        self.tree.addVar('float', 'vismass')
        self.tree.addVar('float', 'svfitmass')
        self.tree.addVar('float', 'l1_pt')
        self.tree.addVar('float', 'l1_eta')
        self.tree.addVar('float', 'l2_pt')
        self.tree.addVar('float', 'l2_eta')
        self.tree.addVar('float', 'j1_pt')
        self.tree.addVar('float', 'j1_eta')
        self.tree.addVar('float', 'j2_pt')
        self.tree.addVar('float', 'j2_eta')
        self.tree.book()
        
        self.fakes = False
        if hasattr(self.cfg_comp, 'fakes') and \
               self.cfg_comp.fakes is True :
            self.fakes = True
            # fakedir = self.dirName.replace('DYJets', 'DYJets_Fakes')
            fakedir = '/'.join([self.looperName, 'Fakes', self.name])
            mkdir_p(fakedir)
            self.outputFakes = H2TauTauOutput( fakedir,
                                               self.regions,
                                               self.cfg_ana.leg1,
                                               self.cfg_ana.leg2 )

    def declareHandles(self):
        super(H2TauTauEventSorter, self).declareHandles()

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesStatus3',
                                                     'std::vector<reco::GenParticle>' )

        self.embhandles['generatorWeight'] = AutoHandle( ('generator', 'weight'),
                                                         'double')
    
    def beginLoop(self, setup):
        super(H2TauTauEventSorter,self).beginLoop(setup)
        self.counters.addCounter('Sorter')
        self.counters.counter('Sorter').register('All events')
        self.counters.counter('Sorter').register('tau iso')
        self.counters.counter('Sorter').register('Matched = None or True')
        self.averages.add('generatorWeight', Average('generatorWeight') )
        self.averages.add('eventWeight', Average('eventWeight') )

        
    def process(self, event):
        self.readCollections( event.input )

        event.generatorWeight = 1
        if self.cfg_comp.isEmbed:
            event.generatorWeight = self.embhandles['generatorWeight'].product()[0]
            event.eventWeight *= event.generatorWeight
        self.averages['generatorWeight'].add(event.generatorWeight)

        # full weight 
        self.averages['eventWeight'].add(event.eventWeight)
            
        matched = None
        if self.fakes:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                matched = True
            else:
                matched = False

        self.tree.s.vismass = event.diLepton.mass()
        self.tree.s.svfitmass = event.diLepton.massSVFit()
        self.tree.s.l1_pt = event.diLepton.leg1().pt()
        self.tree.s.l1_eta = event.diLepton.leg1().eta()
        self.tree.s.l2_pt = event.diLepton.leg2().pt()
        self.tree.s.l2_eta = event.diLepton.leg2().eta()
        if len(event.cleanJets) > 0:
            self.tree.s.j1_pt = event.cleanJets[0].pt()
            self.tree.s.j1_eta = event.cleanJets[0].eta()
        if len(event.cleanJets) > 1:
            self.tree.s.j2_pt = event.cleanJets[1].pt()
            self.tree.s.j2_eta = event.cleanJets[1].eta()
        self.tree.fill()
        
        regionName = self.regions.test( event )
        # print regionName
        self.counters.counter('Sorter').inc('All events')

#        if event.diLepton.leg1().tauID('byLooseCombinedIsolationDeltaBetaCorr') is True:
#            self.counters.counter('Sorter').inc('tau iso')
#        else:
#            print event.diLepton.leg1().tauID('byLooseCombinedIsolationDeltaBetaCorr')
            
        
        if matched is None or matched is True:
            self.output.Fill( event, regionName )
            self.counters.counter('Sorter').inc('Matched = None or True')
        elif matched is False: 
            self.outputFakes.Fill( event, regionName )
        else:
            raise ValueError('should not happen!')
        return True


    def write(self):
        '''Write all histograms to their root files'''
        super(H2TauTauEventSorter, self).write()
        self.output.Write()
        if self.fakes:
            self.outputFakes.Write()
        self.treeFile.Write()
        
