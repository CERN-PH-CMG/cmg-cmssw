from CMGTools.H2TauTau.macros.DiTauHistograms import *
from CMGTools.H2TauTau.macros.LegHistograms import *
from CMGTools.RootTools.TaggedFile import *

class H2TauTauHistograms:
    def __init__(self,name):
        self.name = name

        self.file = TaggedFile( name + '.root')
        
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

    def formatHistos(self, style):
        for hist in self.hists:
            hist.formatHistos( style )

    def Write(self):
        for hist in self.hists:
            hist.Write( self.file.file )

    def __str__(self):
        print 'Histograms', name

        
