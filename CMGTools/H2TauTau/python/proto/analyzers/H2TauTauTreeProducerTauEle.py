from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle


class H2TauTauTreeProducerTauEle( TreeAnalyzerNumpy ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self, setup):

       tr = self.tree

       var( tr, 'run', int)
       var( tr, 'lumi', int)
       var( tr, 'evt', int)
       var( tr, 'NUP', int)
       
       bookDiLepton(tr)

       var( tr, 'pfmet')
       
       bookParticle(tr, 'diTau')
       bookTau(tr, 'l1')
       bookEle(tr, 'l2')
       bookParticle(tr, 'l1Jet')
       bookParticle(tr, 'l2Jet')
       
       var( tr, 'nJets')
       var( tr, 'nJets20')
       bookJet(tr, 'jet1')
       bookJet(tr, 'jet2')

       # b jets
       var( tr, 'nBJets')
       var(tr, 'nCSVLJets')
       bookJet(tr, 'bjet1')

       bookVBF( tr, 'VBF' )
       
       var( tr, 'weight')
       var( tr, 'vertexWeight')
       var( tr, 'embedWeight')
       var( tr, 'hqtWeight')
       var( tr, 'hqtWeightUp')
       var( tr, 'hqtWeightDown')
       var( tr, 'NJetWeight')
       var( tr, 'zllWeight')

       var( tr, 'tauFakeRateWeight')
       var( tr, 'tauFakeRateWeightUp')
       var( tr, 'tauFakeRateWeightDown')
       
       var( tr, 'nVert')
       
       var( tr, 'isFake')
       var( tr, 'isSignal')
       var( tr, 'leptonAccept')
       var( tr, 'thirdLeptonVeto')

       var(tr, 'genMass')

       bookGenParticle(tr, 'genW')
       bookGenParticle(tr, 'genZ')
       bookGenParticle(tr, 'genWlep')
       bookGenParticle(tr, 'genWnu')
       bookGenParticle(tr, 'genZleg1')
       bookGenParticle(tr, 'genZleg2')
       
       
    def declareHandles(self):
        super(H2TauTauTreeProducerTauEle, self).declareHandles()
        self.handles['pfmetraw'] = AutoHandle(
                'slimmedMETs',
                'std::vector<pat::MET>' 
                )
        
    def process(self, event):
       self.readCollections( event.input )
            
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
       fillEle(tr, 'l2', event.diLepton.leg2() )
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

       # JAN - FIXME - temporarily directly count CSVL
       # jets. Eventually apply SFs as for CSVM jets
       # after rewriting BTagSF module
       nCSVLJets = 0
       for jet in event.cleanJets:
          if jet.btag('combinedSecondaryVertexBJetTags') > 0.244:
              nCSVLJets += 1
       fill(tr, 'nCSVLJets', nCSVLJets)

       if hasattr( event, 'vbf'):
           fillVBF( tr, 'VBF', event.vbf )

       fill(tr, 'weight', event.eventWeight)
       fill(tr, 'embedWeight', event.embedWeight)
       fill(tr, 'hqtWeight', event.higgsPtWeight)
       fill(tr, 'hqtWeightUp', event.higgsPtWeightUp)
       fill(tr, 'hqtWeightDown', event.higgsPtWeightDown)
       
       fill(tr, 'tauFakeRateWeightUp', event.tauFakeRateWeightUp)
       fill(tr, 'tauFakeRateWeightDown', event.tauFakeRateWeightDown)
       fill(tr, 'tauFakeRateWeight', event.tauFakeRateWeight)

       if hasattr(event, 'NJetWeight'):
          fill(tr, 'NJetWeight', event.NJetWeight)
       fill(tr, 'zllWeight', event.zllWeight)

       if hasattr( event, 'vertexWeight'): 
          fill(tr, 'vertexWeight', event.vertexWeight)
          fill(tr, 'nVert', len(event.vertices) ) 
          
       fill(tr, 'isFake', event.isFake)
       fill(tr, 'isSignal', event.isSignal)
       fill(tr, 'leptonAccept',    event.leptonAccept)
       fill(tr, 'thirdLeptonVeto', event.thirdLeptonVeto)

       if hasattr(event, 'genParticles'):
         for p in event.genParticles:
            if p.pdgId() in [23, 25, 35, 36, 37]:
              fill(tr, 'genMass', p.mass())
              break

       if hasattr( event, 'genZs'):
           if len(event.genZs):
               fillGenParticle(tr, 'genZ', event.genZs[0])
               fillGenParticle(tr, 'genZleg1', event.genZs[0].leg1)
               fillGenParticle(tr, 'genZleg2', event.genZs[0].leg2)
       if hasattr( event, 'genWs'):
           if len(event.genWs):
               fillGenParticle(tr, 'genW', event.genWs[0])
               fillGenParticle(tr, 'genWlep', event.genWs[0].lep)
               fillGenParticle(tr, 'genWnu', event.genWs[0].nu)
       
       self.tree.tree.Fill()
