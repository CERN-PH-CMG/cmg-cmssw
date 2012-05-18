from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple import *


class H2TauTauTreeProducerTauMu( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

       tr = self.tree
       var( tr, 'visMass')
       var( tr, 'svfitMass')
       var( tr, 'mt') 
       var( tr, 'met')
       
       bookParticle(tr, 'diTau')
       bookTau(tr, 'l1')
       bookMuon(tr, 'l2')
       
       var( tr, 'nJets')
       bookJet(tr, 'jet1')
       bookJet(tr, 'jet2')

       bookVBF( tr, 'VBF' )
       
       var( tr, 'weight')
       var( tr, 'vertexWeight')
       
       var( tr, 'nVert')
       
       var( tr, 'isFake')
       var( tr, 'isSignal')
       

    def process(self, iEvent, event):
            
       tr = self.tree
       fill(tr, 'visMass', event.diLepton.mass())
       fill(tr, 'svfitMass', event.diLepton.massSVFit())
       fill(tr, 'mt', event.diLepton.mTLeg2())
       fill(tr, 'met', event.diLepton.met().pt())
       
       fillParticle(tr, 'diTau', event.diLepton)
       fillTau(tr, 'l1', event.diLepton.leg1() )
       fillMuon(tr, 'l2', event.diLepton.leg2() )


       nJets = len(event.cleanJets)
       fill(tr, 'nJets', nJets )
       if nJets>=1:
           fillJet(tr, 'jet1', event.cleanJets[0] )
       if nJets>=2:
           fillJet(tr, 'jet2', event.cleanJets[1] )

       if hasattr( event, 'vbf'):
           fillVBF( tr, 'VBF', event.vbf )

       fill(tr, 'weight', event.eventWeight)

       if hasattr( event, 'vertexWeight'): 
          fill(tr, 'vertexWeight', event.vertexWeight)
          fill(tr, 'nVert', len(event.vertices) ) 
          
       isFake = 1
       if hasattr( event, 'genMatched'): 
          if event.genMatched == 1:
             isFake = 0
       fill(tr, 'isFake', isFake)
       fill(tr, 'isSignal', event.isSignal)
       
       self.tree.tree.Fill()
