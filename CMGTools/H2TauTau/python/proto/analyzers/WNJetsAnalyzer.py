from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.analyzers.GenParticleAnalyzer import *
from CMGTools.RootTools.utils.DeltaR import matchObjectCollection
from CMGTools.RootTools.physicsobjects.genutils import *
from CMGTools.RootTools.statistics.Average import Average

from ROOT import TFile, TH1F

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
        self.averages.add('NUP', Average('NUP') )
        self.averages.add('NJets', Average('NJets') )
        if self.cfg_comp.isMC:
            self.rootfile = TFile('/'.join([self.dirName,
                                            'NUP.root']),
                                  'recreate')
            self.nup = TH1F('nup', 'nup', 20,0,20)
            self.njets = TH1F('njets', 'njets', 10,0,10)
        
        
    def process(self, iEvent, event):
        event.NUP = -1
        try :
            self.readCollections( iEvent )
        except :
            return True
        
        if not self.cfg_comp.isMC:
            return True
        
        # check whether it's a WJets sample, can this be done?
        # save the NUP variable
        if self.mchandles['source'].isValid():
            event.NUP = self.mchandles['source'].product().hepeup().NUP
        else:
            event.NUP = -1

        self.averages['NUP'].add( event.NUP )
        # removing the 2 incoming partons, a boson,
        # and the 2 partons resulting from the decay of a boson
        njets = event.NUP-5
        self.averages['NJets'].add( njets )
        self.nup.Fill(event.NUP)
        self.njets.Fill(njets)
        
        if self.cfg_ana.verbose:
            print 'NUP : ',event.NUP
        return True
    

    def declareHandles(self):
        '''Reads LHEEventsProduct.'''
        super(WNJetsAnalyzer, self).declareHandles()
        self.mchandles['source'] =  AutoHandle(
            'source',
            'LHEEventProduct'
            )
        
    def write(self):
        super(WNJetsAnalyzer, self).write()
        if self.cfg_comp.isMC:
            self.rootfile.Write()
            self.rootfile.Close()
