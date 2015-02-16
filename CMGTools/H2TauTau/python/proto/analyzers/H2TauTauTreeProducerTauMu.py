from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer

class H2TauTauTreeProducerTauMu( H2TauTauTreeProducer ):
    '''Tree producer for the H->tau tau analysis.'''
    
    def declareVariables(self, setup):
    
        super(H2TauTauTreeProducerTauMu, self).declareVariables(setup)

        self.bookTau(self.tree, 'l1')
        self.bookMuon(self.tree, 'l2')

        self.bookGenParticle(self.tree, 'l1_gen')
        self.bookGenParticle(self.tree, 'l2_gen')

        self.bookParticle(self.tree, 'l1_gen_vis')

        self.var( self.tree, 'tauFakeRateWeight')
        self.var( self.tree, 'tauFakeRateWeightUp')
        self.var( self.tree, 'tauFakeRateWeightDown')

                 
    def process(self, event):

        super(H2TauTauTreeProducerTauMu, self).process(event)

        tau  = event.diLepton.leg1() 
        muon = event.diLepton.leg2()

        self.fillTau(self.tree, 'l1', tau )
        self.fillMuon(self.tree, 'l2', muon )
        
        if hasattr(tau , 'genp') : self.fillGenParticle(self.tree, 'l1_gen', tau.genp )
        if hasattr(muon, 'genp') : self.fillGenParticle(self.tree, 'l2_gen', muon.genp )
    
        # RIC: hasattr has depth=1, but this would be nicer 
        # http://code.activestate.com/recipes/577346-getattr-with-arbitrary-depth/
        # may think of putting it into some utils
        taupo = tau.physObj
    
        # save the p4 of the visible tau products at the generator level
        # make sure that the reco tau matches with a gen tau that decays into hadrons
        if hasattr(taupo, 'genJet') and hasattr(tau,'genp') and abs(tau.genp.pdgId()) == 15 : 
            self.fillParticle(self.tree, 'l1_gen_vis', tau.physObj.genJet() )

        self.fill(self.tree, 'tauFakeRateWeightUp', event.tauFakeRateWeightUp)
        self.fill(self.tree, 'tauFakeRateWeightDown', event.tauFakeRateWeightDown)
        self.fill(self.tree, 'tauFakeRateWeight', event.tauFakeRateWeight)
            
        self.tree.tree.Fill()
