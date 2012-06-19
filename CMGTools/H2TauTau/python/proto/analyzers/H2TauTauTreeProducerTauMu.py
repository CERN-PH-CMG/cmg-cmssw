from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle


class H2TauTauTreeProducerTauMu( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

       tr = self.tree

       var( tr, 'run', int)
       var( tr, 'lumi', int)
       var( tr, 'evt', int)
       
       var( tr, 'visMass')
       var( tr, 'svfitMass')
       var( tr, 'mt') 
       var( tr, 'met')
       var( tr, 'pfmet')
       
       bookParticle(tr, 'diTau')
       bookTau(tr, 'l1')
       bookMuon(tr, 'l2')
       bookParticle(tr, 'l1Jet')
       bookParticle(tr, 'l2Jet')
       
       var( tr, 'nJets')
       bookJet(tr, 'jet1')
       bookJet(tr, 'jet2')

       # b jets
       var( tr, 'nBJets')
       bookJet(tr, 'bjet1')

       bookVBF( tr, 'VBF' )
       
       var( tr, 'weight')
       var( tr, 'vertexWeight')
       
       var( tr, 'nVert')
       
       var( tr, 'isFake')
       var( tr, 'isSignal')
       
    def declareHandles(self):
        super(H2TauTauTreeProducerTauMu, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
            'cmgPFMETRaw',
            'std::vector<cmg::BaseMET>' 
            )
        
    def process(self, iEvent, event):
       self.readCollections( iEvent )
            
       tr = self.tree
       tr.reset()
       
       fill( tr, 'run', event.run) 
       fill( tr, 'lumi',event.lumi)
       fill( tr, 'evt', event.eventId)

       fill(tr, 'visMass', event.diLepton.mass())
       fill(tr, 'svfitMass', event.diLepton.massSVFit())
       fill(tr, 'mt', event.diLepton.mTLeg2())
       fill(tr, 'met', event.diLepton.met().pt())

       # import pdb; pdb.set_trace()
       pfmet = self.handles['pfmetraw'].product()[0]
       fill(tr, 'pfmet', pfmet.pt())

       
       fillParticle(tr, 'diTau', event.diLepton)
       fillTau(tr, 'l1', event.diLepton.leg1() )
       fillMuon(tr, 'l2', event.diLepton.leg2() )
       fillParticle(tr, 'l1Jet', event.diLepton.leg1().jet )
       fillParticle(tr, 'l2Jet', event.diLepton.leg2().jet )


       nJets = len(event.cleanJets)
       fill(tr, 'nJets', nJets )
       if nJets>=1:
           fillJet(tr, 'jet1', event.cleanJets[0] )
       if nJets>=2:
           fillJet(tr, 'jet2', event.cleanJets[1] )

       nBJets = len(event.cleanBJets)
       if nBJets>0:
           fillJet(tr, 'bjet1', event.cleanBJets[0] )           
       fill(tr, 'nBJets', nBJets)

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
