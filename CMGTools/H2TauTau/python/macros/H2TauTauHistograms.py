import os

from ROOT import TFile

from CMGTools.H2TauTau.macros.DiTauHistograms import *
from CMGTools.H2TauTau.macros.LegHistograms import *
#  from CMGTools.RootTools.TaggedFile import *

class H2TauTauHistograms:
    def __init__(self,name):
        self.name = name

        fileName = self.name + '.root'
        index = 0
        while os.path.isfile( fileName ) is True:
            index += 1
            fileName = self.name + '_%d.root' % index
            
        self.file = TFile( fileName, 'recreate')
        
        self.diTau = DiTauHistograms( 'tauMu')
        self.tau = LegHistograms( 'tau','leg1')
        self.mu = LegHistograms( 'mu','leg2')
        
        self.hists = []
        self.hists.append( self.diTau )
        self.hists.append( self.tau )
        self.hists.append( self.mu )
        
    def fillHistos(self, events, cut='', nEvents = 9999999999):
        print 'filling histograms: ' + self.name
        for hist in self.hists:
            hist.fillHistos( events, cut, nEvents )

    def fillDiTau(self, diTau):
        self.diTau.fillDiTau( diTau )
        self.tau.fillLeg( diTau.leg1() )
        self.mu.fillLeg( diTau.leg2() )

    def formatHistos(self, style):
        for hist in self.hists:
            hist.formatHistos( style )

    def Write(self):
        for hist in self.hists:
            hist.Write( self.file )

    def __str__(self):
        print 'Histograms', name

        
