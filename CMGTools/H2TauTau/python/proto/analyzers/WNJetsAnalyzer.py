from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.analyzers.GenParticleAnalyzer import *
from CMGTools.RootTools.utils.DeltaR import matchObjectCollection
from CMGTools.RootTools.physicsobjects.genutils import *

class WNJetsAnalyzer( Analyzer ):
#class WNJetsAnalyzer( GenParticleAnalyzer ):
    '''saves the NUP variable from the LHEEventProduct information.
    
    For the W+jets case:
    NUP = 5 : 0jets
    NUP = 6 : 1jet 
    ...
    
    In case of data, NUP = -1.
    In case of other MCs, the value is saved.
    '''

    def beginLoop(self):
        super(WNJetsAnalyzer,self).beginLoop()        


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.NUP = -1
        if not self.cfg_comp.isMC:
            return True
        
        # check whether it's a WJets sample, can this be done?
        # save the NUP variable
        event.NUP = self.mchandles['source'].product().hepeup().NUP

        if self.cfg_ana.verbose:
            # import pdb; pdb.set_trace()
            print 'NUP : ',event.NUP
        return True
    

    def declareHandles(self):
        '''Reads LHEEventsProduct.'''
        super(WNJetsAnalyzer, self).declareHandles()
        self.mchandles['source'] =  AutoHandle(
            'source',
            'LHEEventProduct'
            )
