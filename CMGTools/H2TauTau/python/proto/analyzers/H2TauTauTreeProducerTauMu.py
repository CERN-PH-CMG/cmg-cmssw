from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer
from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes

class H2TauTauTreeProducerTauMu(H2TauTauTreeProducer):

    '''Tree producer for the H->tau tau analysis.'''

    def declareVariables(self, setup):

        super(H2TauTauTreeProducerTauMu, self).declareVariables(setup)

        self.bookTau(self.tree, 'l2')
        self.bookMuon(self.tree, 'l1')

        self.bookGenParticle(self.tree, 'l2_gen')
        self.var(self.tree, 'l2_gen_lepfromtau', int)
        self.bookGenParticle(self.tree, 'l1_gen')
        self.var(self.tree, 'l1_gen_lepfromtau', int)

        self.bookParticle(self.tree, 'l2_gen_vis')
        self.var(self.tree, 'l2_gen_decaymode', int)

        self.var(self.tree, 'l2_weight_fakerate')
        self.var(self.tree, 'l2_weight_fakerate_up')
        self.var(self.tree, 'l2_weight_fakerate_down')

        if hasattr(self.cfg_ana, 'addIsoInfo') and self.cfg_ana.addIsoInfo:
            self.var(self.tree, 'l1_puppi_iso_pt')
            self.var(self.tree, 'l1_puppi_iso04_pt')
            self.var(self.tree, 'l1_puppi_iso03_pt')

            self.var(self.tree, 'l1_puppi_no_muon_iso_pt')
            self.var(self.tree, 'l1_puppi_no_muon_iso04_pt')
            self.var(self.tree, 'l1_puppi_no_muon_iso03_pt')

            self.var(self.tree, 'l2_puppi_iso_pt')
            self.var(self.tree, 'l2_puppi_iso04_pt')
            self.var(self.tree, 'l2_puppi_iso03_pt')

            self.var(self.tree, 'l1_mini_iso')
            self.var(self.tree, 'l1_mini_reliso')

    def process(self, event):

        super(H2TauTauTreeProducerTauMu, self).process(event)

        tau = event.diLepton.leg2()
        muon = event.diLepton.leg1()

        self.fillTau(self.tree, 'l2', tau)
        self.fillMuon(self.tree, 'l1', muon)

        if hasattr(tau, 'genp') and tau.genp:
            self.fillGenParticle(self.tree, 'l2_gen', tau.genp)
            self.fill(self.tree, 'l2_gen_lepfromtau', tau.isTauLep)

        if hasattr(muon, 'genp') and muon.genp:
            self.fillGenParticle(self.tree, 'l1_gen', muon.genp)
            self.fill(self.tree, 'l1_gen_lepfromtau', muon.isTauLep)

        # save the p4 of the visible tau products at the generator level
        if tau.genJet() and hasattr(tau, 'genp') and tau.genp and abs(tau.genp.pdgId()) == 15:
            self.fillParticle(self.tree, 'l2_gen_vis', tau.physObj.genJet())
            self.fill(self.tree, 'l2_gen_decaymode', tauDecayModes.translateGenModeToInt(tauDecayModes.genDecayModeFromGenJet(tau.physObj.genJet())))

        self.fill(self.tree, 'l2_weight_fakerate', event.tauFakeRateWeightUp)
        self.fill(self.tree, 'l2_weight_fakerate_up', event.tauFakeRateWeightDown)
        self.fill(self.tree, 'l2_weight_fakerate_down', event.tauFakeRateWeight)

        if hasattr(self.cfg_ana, 'addIsoInfo') and self.cfg_ana.addIsoInfo:
            self.fill(self.tree, 'l1_puppi_iso_pt', muon.puppi_iso_pt)
            self.fill(self.tree, 'l1_puppi_iso04_pt', muon.puppi_iso04_pt)
            self.fill(self.tree, 'l1_puppi_iso03_pt', muon.puppi_iso03_pt)
            self.fill(self.tree, 'l1_puppi_no_muon_iso_pt', muon.puppi_no_muon_iso_pt)
            self.fill(self.tree, 'l1_puppi_no_muon_iso04_pt', muon.puppi_no_muon_iso04_pt)
            self.fill(self.tree, 'l1_puppi_no_muon_iso03_pt', muon.puppi_no_muon_iso03_pt)
            self.fill(self.tree, 'l2_puppi_iso_pt', tau.puppi_iso_pt)
            self.fill(self.tree, 'l2_puppi_iso04_pt', tau.puppi_iso04_pt)
            self.fill(self.tree, 'l2_puppi_iso03_pt', tau.puppi_iso03_pt)
            self.fill(self.tree, 'l1_mini_iso', muon.miniAbsIso)
            self.fill(self.tree, 'l1_mini_reliso', muon.miniRelIso)

        self.fillTree(event)
