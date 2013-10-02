import os
import sys
import pprint
import logging
import copy
import glob
import random


from CMGTools.H2TauTau.macros.H2TauTauAnalyzer import H2TauTauAnalyzer, Event 
from CMGTools.H2TauTau.macros.AutoHandle import AutoHandle
from CMGTools.H2TauTau.macros.CountLeptons import muonAccept


class TauMuAnalyzer( H2TauTauAnalyzer ):


    def __init__(self, name, component, cfg):
        super( TauMuAnalyzer, self).__init__(name, component, cfg)


    def InitSpecificHandles(self):
        '''Initialize TauMu specific handles.'''
        
        self.handles['diTau'] =  AutoHandle( 'cmgTauMuCorSVFitFullSel',
                                             'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>')
        self.handles['leptons'] = AutoHandle( 'cmgMuonSel',
                                              'std::vector<cmg::Muon>' )

    def leptonAccept( self, leptons):
        return muonAccept( leptons )
        
            
   
if __name__ == '__main__':

    print 'See MultiLoop.py for the main.'
