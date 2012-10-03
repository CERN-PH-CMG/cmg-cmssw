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
       var( tr, 'NUP', int)
       
       bookDiLepton(tr)
       
       var( tr, 'pfmet')
       
       bookParticle(tr, 'diTau')
       bookTau(tr, 'l1')
       bookMuon(tr, 'l2')
       bookParticle(tr, 'l1Jet')
       bookParticle(tr, 'l2Jet')
       
       var( tr, 'nJets')
       var( tr, 'nJets20')
       bookJet(tr, 'jet1')
       bookJet(tr, 'jet2')

       # b jets
       var( tr, 'nBJets')
       bookJet(tr, 'bjet1')

       bookVBF( tr, 'VBF' )
       
       var( tr, 'weight')
       var( tr, 'vertexWeight')
       var( tr, 'embedWeight')
       var( tr, 'hqtWeight')
       
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
       fill( tr, 'NUP', event.NUP)

       fillDiLepton( tr, event.diLepton )

       # import pdb; pdb.set_trace()
       pfmet = self.handles['pfmetraw'].product()[0]
       fill(tr, 'pfmet', pfmet.pt())
       
       fillParticle(tr, 'diTau', event.diLepton)
       fillTau(tr, 'l1', event.diLepton.leg1() )
       fillMuon(tr, 'l2', event.diLepton.leg2() )
       fillParticle(tr, 'l1Jet', event.diLepton.leg1().jet )
       fillParticle(tr, 'l2Jet', event.diLepton.leg2().jet )


       fill(tr, 'nJets20', len(event.cleanJets) )
       nJets30 = len(event.cleanJets30)
       fill(tr, 'nJets', nJets30 )
       nJets = len(event.cleanJets)
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
       fill(tr, 'embedWeight', event.embedWeight)
       fill(tr, 'hqtWeight', event.higgsPtWeight)

       if hasattr( event, 'vertexWeight'): 
          fill(tr, 'vertexWeight', event.vertexWeight)
          fill(tr, 'nVert', len(event.vertices) ) 
          
       fill(tr, 'isFake', event.isFake)
       fill(tr, 'isSignal', event.isSignal)
       
       self.tree.tree.Fill()
