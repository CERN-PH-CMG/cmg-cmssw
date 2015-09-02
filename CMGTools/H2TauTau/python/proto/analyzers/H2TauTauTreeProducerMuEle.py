from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer

class H2TauTauTreeProducerMuEle(H2TauTauTreeProducer):
    '''Tree producer for the H->tau tau analysis'''
  
    def declareVariables(self, setup):
        super(H2TauTauTreeProducerMuEle, self).declareVariables(setup)

        self.bookEle (self.tree, 'l1')
        self.bookMuon(self.tree, 'l2')

        self.bookGenParticle(self.tree, 'l1_gen')
        self.var(self.tree, 'l1_gen_lepfromtau', int)
        self.bookGenParticle(self.tree, 'l2_gen')
        self.var(self.tree, 'l2_gen_lepfromtau', int)

  
    def process(self, event):
         
        super(H2TauTauTreeProducerMuEle, self).process(event)

        ele = event.diLepton.leg1() 
        muon = event.diLepton.leg2()

        self.fillEle(self.tree, 'l1', ele)
        self.fillMuon(self.tree, 'l2', muon)

        if muon.genp:
            self.fillGenParticle(self.tree, 'l2_gen', muon.genp)
            self.fill(self.tree, 'l2_gen_lepfromtau', muon.isTauLep)
        if ele.genp:
            self.fillGenParticle(self.tree, 'l1_gen', ele.genp)
            self.fill(self.tree, 'l1_gen_lepfromtau', ele.isTauLep)

        self.fillTree(event)
