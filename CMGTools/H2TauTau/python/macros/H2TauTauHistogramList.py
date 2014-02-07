import os

from ROOT import TFile

from CMGTools.H2TauTau.macros.DiTauHistograms import DiTauHistograms
from CMGTools.H2TauTau.macros.LegHistograms import LegHistograms
from CMGTools.H2TauTau.macros.VertexHistograms import VertexHistograms
from CMGTools.H2TauTau.macros.JetHistograms import JetHistograms

class H2TauTauHistogramList( object ):
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
        self.tau0 = LegHistograms( 'tau0','leg1')
        self.tau1 = LegHistograms( 'tau1','leg1')
        self.tau10 = LegHistograms( 'tau10','leg1')
        self.mu = LegHistograms( 'mu','leg2')
        self.vertex = VertexHistograms( 'vertex')
        self.jets = JetHistograms( 'jets')
        self.dirtyJets = JetHistograms( 'dirtyJets')
        self.jetsVBF = JetHistograms( 'jetsVBF')
        self.leadJet = JetHistograms( 'leadJet')
        
        self.hists = []
        self.hists.append( self.diTau )
        self.hists.append( self.tau )
        self.hists.append( self.tau0 )
        self.hists.append( self.tau1 )
        self.hists.append( self.tau10 )
        self.hists.append( self.mu )     
        self.hists.append( self.vertex )
        self.hists.append( self.jets )
        self.hists.append( self.dirtyJets )
        self.hists.append( self.jetsVBF )
        self.hists.append( self.leadJet )
        
    def FillHistos(self, events, cut='', nEvents = 9999999999):
        print 'Filling histograms: ' + self.name
        for hist in self.hists:
            hist.FillHistos( events, cut, nEvents )

    def FillDiTau(self, diTau, weight=1):
        self.diTau.FillDiTau( diTau, weight )
        tau = diTau.leg1()
        self.tau.FillLeg( tau, weight )
        if hasattr(tau, 'decayMode'):
            dec = tau.decayMode()
            if dec == 0:
                self.tau0.FillLeg( diTau.leg1(), weight )
            elif dec == 1:
                self.tau1.FillLeg( diTau.leg1(), weight )
            elif dec == 10:
                self.tau10.FillLeg( diTau.leg1(), weight )
        self.mu.FillLeg( diTau.leg2(), weight )

    def FillVertices(self, vertices, weight=1):
        self.vertex.FillVertices( vertices, weight )

    def Fill(self, event, weight=1):
        self.FillVertices( event.vertices, weight)
        self.FillDiTau( event.diTau, weight)
        self.jets.Fill( event.jets, weight )
        self.dirtyJets.Fill( event.dirtyJets, weight )
        if len( event.jets )>1:
            self.jetsVBF.Fill( event.jets[:2], weight )
        if len( event.jets )>0:
            self.leadJet.Fill( [event.jets[0]], weight )
        
    def FormatHistos(self, style):
        for hist in self.hists:
            hist.FormatHistos( style )

    def Write(self):
        for hist in self.hists:
            hist.Write( self.file )

    def __str__(self):
        print 'Histograms', name

        
