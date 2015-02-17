from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer

class H2TauTauTreeProducerTauEle( H2TauTauTreeProducer ):
    '''Tree producer for the H->tau tau analysis.'''
    
    def declareVariables(self, setup):

        super(H2TauTauTreeProducerTauEle, self).declareVariables(setup)

        self.bookTau(self.tree, 'l1')
        self.bookMuon(self.tree, 'l2')

        self.bookGenParticle(self.tree, 'l1_gen')
        self.bookGenParticle(self.tree, 'l2_gen')

        self.bookParticle(self.tree, 'l1_gen_vis')

        self.var( self.tree, 'tauFakeRateWeight')
        self.var( self.tree, 'tauFakeRateWeightUp')
        self.var( self.tree, 'tauFakeRateWeightDown')

        self.var( self.tree, 'zllWeight')

        self.bookGenParticle(self.tree, 'genW')
        self.bookGenParticle(self.tree, 'genZ')
        self.bookGenParticle(self.tree, 'genWlep')
        self.bookGenParticle(self.tree, 'genWnu')
        self.bookGenParticle(self.tree, 'genZleg1')
        self.bookGenParticle(self.tree, 'genZleg2')
       
               
    def process(self, event):
        
        super(H2TauTauTreeProducerTauEle, self).process(event)

        tau = event.diLepton.leg1() 
        ele = event.diLepton.leg2()

        self.fillTau(self.tree, 'l1', tau )
        self.fillEle(self.tree, 'l2', ele )
        
        if hasattr(tau , 'genp') : self.fillGenParticle(self.tree, 'l1_gen', tau.genp )
        if hasattr(ele , 'genp') : self.fillGenParticle(self.tree, 'l2_gen', ele.genp )
    
        taupo = tau.physObj    
        # save the p4 of the visible tau products at the generator level
        # make sure that the reco tau matches with a gen tau that decays into hadrons
        if hasattr(taupo, 'genJet') and hasattr(tau,'genp') and abs(tau.genp.pdgId()) == 15 : 
            self.fillParticle(self.tree, 'l1_gen_vis', tau.physObj.genJet() )

        self.fill(self.tree, 'tauFakeRateWeightUp'  , event.tauFakeRateWeightUp  )
        self.fill(self.tree, 'tauFakeRateWeightDown', event.tauFakeRateWeightDown)
        self.fill(self.tree, 'tauFakeRateWeight'    , event.tauFakeRateWeight    )
       
        self.fill(self.tree, 'zllWeight', event.zllWeight)

        if hasattr( event, 'genZs'):
            if len(event.genZs):
                self.fillGenParticle(self.tree, 'genZ'    , event.genZs[0]     )
                self.fillGenParticle(self.tree, 'genZleg1', event.genZs[0].leg1)
                self.fillGenParticle(self.tree, 'genZleg2', event.genZs[0].leg2)
        if hasattr( event, 'genWs'):
            if len(event.genWs):
                self.fillGenParticle(self.tree, 'genW'   , event.genWs[0]    )
                self.fillGenParticle(self.tree, 'genWlep', event.genWs[0].lep)
                self.fillGenParticle(self.tree, 'genWnu' , event.genWs[0].nu )
       
        self.tree.tree.Fill()
