import os

from ROOT import TFile

from CMGTools.H2TauTau.proto.analyzers.DiTauHistograms import DiTauHistograms
from CMGTools.H2TauTau.proto.analyzers.LegHistograms import TauHistograms, MuHistograms, EleHistograms
from CMGTools.H2TauTau.proto.analyzers.VertexHistograms import VertexHistograms
from CMGTools.H2TauTau.proto.analyzers.JetHistograms import JetHistograms

class H2TauTauHistogramList( object ):
    def __init__(self,name):
        self.name = name
        fileName = self.name + '.root'
        index = 0
        while os.path.isfile( fileName ) is True:
            index += 1
            fileName = self.name + '_%d.root' % index
            
        self.file = TFile( fileName, 'recreate')        
        self.hists = []


    def createGenericHistograms(self):

        self.diTau = DiTauHistograms( 'diTau' )
        self.vertex = VertexHistograms( 'vertex')
        self.cleanJets = JetHistograms( 'cleanJets')
        self.jets = JetHistograms( 'jets')
        self.jetsVBF = JetHistograms( 'jetsVBF')
        self.leadJet = JetHistograms( 'leadJet')

        self.hists.append( self.diTau )
        self.hists.append( self.vertex )
        self.hists.append( self.cleanJets )
        self.hists.append( self.jets )
        self.hists.append( self.jetsVBF )
        self.hists.append( self.leadJet )
        

    def fillGenericHistograms(self, event, weight):
        self.diTau.FillDiTau( event.diLepton, weight )
        self.vertex.FillVertices( event.vertices, weight )
        self.cleanJets.Fill( event.cleanJets, weight )
        self.jets.Fill( event.jets, weight )
        if len( event.cleanJets )>1:
            self.jetsVBF.Fill( event.cleanJets[:2], weight )
        if len( event.cleanJets )>0:
            self.leadJet.Fill( [event.cleanJets[0]], weight )
        

    def createTauHistograms(self, legName):
        if legName != 'tau_2':
            self.tau = TauHistograms( 'tau',legName)
            self.tau0 = TauHistograms( 'tau0',legName)
            self.tau1 = TauHistograms( 'tau1',legName)
            self.tau10 = TauHistograms( 'tau10',legName)
            self.hists.append( self.tau )
            self.hists.append( self.tau0 )
            self.hists.append( self.tau1 )
            self.hists.append( self.tau10 )
        else:
            self.tau2 = TauHistograms( 'tau',legName)
            self.tau2_0 = TauHistograms( 'tau0',legName)
            self.tau2_1 = TauHistograms( 'tau1',legName)
            self.tau2_10 = TauHistograms( 'tau10',legName)
            self.hists.append( self.tau2 )
            self.hists.append( self.tau2_0 )
            self.hists.append( self.tau2_1 )
            self.hists.append( self.tau2_10 )
            


    def fillTauHistograms(self, tau, weight, leg='leg1'):
        dec = tau.decayMode()
        # import pdb; pdb.set_trace()

        htau = self.tau
        htau0 = self.tau0
        htau1 = self.tau1
        htau10 = self.tau10
        if leg == 'leg2':
            htau = self.tau2
            htau0 = self.tau2_0
            htau1 = self.tau2_1
            htau10 = self.tau2_10
            
        htau.FillTau( tau, weight )
        if dec == 0:
            htau0.FillTau( tau, weight )
        elif dec == 1:
            htau1.FillTau( tau, weight )
        elif dec == 10:
            htau10.FillTau( tau, weight )


    def createMuHistograms(self, legName):
        self.mu = MuHistograms( 'mu','leg2')
        self.hists.append( self.mu )     
 

    def fillMuHistograms(self, mu, weight):
        self.mu.FillMu( mu, weight )

               
    def createEleHistograms(self, legName):
        self.ele = EleHistograms( 'ele','leg2')
        self.hists.append( self.ele )     
 

    def fillEleHistograms(self, ele, weight):
        self.ele.FillEle( ele, weight )

               
    def FormatHistos(self, style):
        for hist in self.hists:
            hist.FormatHistos( style )


    def Write(self):
        for hist in self.hists:
            hist.Write( self.file )


    def __str__(self):
        print 'Histograms', name

#COLIN this design looks a bit like C++. Could I do something more clever in python?

class TauMuHistogramList(H2TauTauHistogramList):
    def __init__(self, name, leg1Name, leg2Name ):
        super(TauMuHistogramList, self).__init__(name)
        self.createGenericHistograms()
        self.createTauHistograms('leg1')
        self.createMuHistograms('leg2')

    def Fill(self, event, weight):
        self.fillGenericHistograms( event, weight )
        self.fillTauHistograms( event.diLepton.leg1(), weight )
        self.fillMuHistograms( event.diLepton.leg2(), weight )
        
        
        
class TauEleHistogramList(H2TauTauHistogramList):
    def __init__(self, name, leg1Name, leg2Name ):
        super(TauEleHistogramList, self).__init__(name)
        self.createGenericHistograms()
        self.createTauHistograms('leg1')
        self.createEleHistograms('leg2')

    def Fill(self, event, weight):
        self.fillGenericHistograms( event, weight )
        self.fillTauHistograms( event.diLepton.leg1(), weight )
        self.fillEleHistograms( event.diLepton.leg2(), weight )



class MuEleHistogramList(H2TauTauHistogramList):
    def __init__(self, name, leg1Name, leg2Name ):
        super(MuEleHistogramList, self).__init__(name)
        self.createGenericHistograms()
        self.createMuHistograms('leg1')
        self.createEleHistograms('leg2')

    def Fill(self, event, weight):
        self.fillGenericHistograms( event, weight )
        self.fillMuHistograms( event.diLepton.leg1(), weight )
        self.fillEleHistograms( event.diLepton.leg2(), weight )
        
        
class TauTauHistogramList(H2TauTauHistogramList):
    def __init__(self, name, leg1Name, leg2Name):
        super(TauTauHistogramList, self).__init__(name)
        self.createGenericHistograms()
        # import pdb; pdb.set_trace()
        self.createTauHistograms(leg1Name)
        self.createTauHistograms(leg2Name)

    def Fill(self, event, weight):
        self.fillGenericHistograms( event, weight )
        self.fillTauHistograms( event.diLepton.leg1(), weight )
        self.fillTauHistograms( event.diLepton.leg2(), weight, 'leg2')
        
        
    
        
