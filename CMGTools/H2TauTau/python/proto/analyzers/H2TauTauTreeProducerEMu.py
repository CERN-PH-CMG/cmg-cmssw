from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer

class H2TauTauTreeProducerEMu( H2TauTauTreeProducer ):
  '''Tree producer for the H->tau tau analysis'''
  
  def declareVariables(self, setup):

    super(H2TauTauTreeProducerEMu, self).declareVariables(setup)

    self.bookMuon(self.tree, 'l1')
    self.bookEle (self.tree, 'l2')

    self.bookGenParticle(self.tree, 'l1_gen')
    self.bookGenParticle(self.tree, 'l2_gen')
  
  def process(self, event):
     
    super(H2TauTauTreeProducerEMu, self).process(event)

    muon = event.diLepton.leg1() 
    ele  = event.diLepton.leg2()

    self.fillMuon(self.tree, 'l1', muon )
    self.fillEle (self.tree, 'l2', ele  )

    if hasattr(muon, 'genp') : self.fillGenParticle(self.tree, 'l1_gen', muon.genp )
    if hasattr(ele , 'genp') : self.fillGenParticle(self.tree, 'l2_gen', ele .genp )

    self.tree.tree.Fill() 
