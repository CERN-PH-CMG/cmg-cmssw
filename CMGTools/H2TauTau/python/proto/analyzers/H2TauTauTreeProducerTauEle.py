from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple import *


class H2TauTauTreeProducerTauEle( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):
       print 'PIETRO in H2TauTauTreeProducerTauEle'

       tr = self.tree
       var( tr, 'visMass')
       var( tr, 'svfitMass')
       var( tr, 'mt') 
       var( tr, 'met')
       
       bookParticle(tr, 'diTau')
       bookTau(tr, 'l1')
       bookEle(tr, 'l2')
       
       var( tr, 'nJets')
       bookJet(tr, 'jet1')
       bookJet(tr, 'jet2')
       
       var( tr, 'weight')
       var( tr, 'vertexWeight')
       
       var( tr, 'nVert')
       
       var( tr, 'isFake')
       

    def process(self, iEvent, event):
            
       tr = self.tree
       fill(tr, 'visMass', event.diLepton.mass())
       fill(tr, 'svfitMass', event.diLepton.massSVFit())
       fill(tr, 'mt', event.diLepton.mTLeg2())
       fill(tr, 'met', event.diLepton.met().pt())
       
       fillParticle(tr, 'diTau', event.diLepton)
       fillTau(tr, 'l1', event.diLepton.leg1() )
       fillEle(tr, 'l2', event.diLepton.leg2() )

       nJets = len(event.cleanJets)
       fill(tr, 'nJets', nJets )
       if nJets>=1:
           fillJet(tr, 'jet1', event.cleanJets[0] )
## 	    fill('mttj', sqrt(pow(event.diLepton.energy()+event.cleanJets[0].energy(),2) - pow(event.diLepton.px()+event.cleanJets[0].px(),2) - pow(event.diLepton.py()+event.cleanJets[0].py(),2) - pow(event.diLepton.pz()+event.cleanJets[0].pz(),2)))
       if nJets>=2:
           fillJet(tr, 'jet2', event.cleanJets[1] )

       fill(tr, 'weight', event.eventWeight)
##         if hasattr( event, 'leg1_eff'): 
##             fill('l1EffData', event.leg1_eff)
##             fill('l1EffMC', event.leg1_effMC)
##             fill('l1Weight', event.leg1_weight)
##         if hasattr( event, 'leg2_eff'): 
##             fill('l2EffData', event.leg2_eff)
##             fill('l2EffMC', event.leg2_effMC)
##             fill('l2Weight', event.leg2_weight)

       if hasattr( event, 'vertexWeight'): 
          fill(tr, 'vertexWeight', event.vertexWeight)
          fill(tr, 'nVert', len(event.vertices) ) 
          
       isFake = 1
       if hasattr( event, 'genMatched'): 
          if event.genMatched == 1:
             isFake = 0
       fill(tr, 'isFake', isFake)
       
       self.tree.tree.Fill()
