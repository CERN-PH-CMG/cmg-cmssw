import ROOT

from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerBase import H2TauTauTreeProducerBase

from CMGTools.H2TauTau.proto.analyzers.DYJetsFakeAnalyzer import DYJetsFakeAnalyzer

class TauTreeProducer(H2TauTauTreeProducerBase):
    ''' Tree producer for tau POG study.
    '''

    def __init__(self, *args):
        super(TauTreeProducer, self).__init__(*args)
        self.maxNTaus = 5

    def declareHandles(self):
        super(TauTreeProducer, self).declareHandles()

        self.mchandles['genJets'] = AutoHandle('slimmedGenJets', 'std::vector<reco::GenJet>')
        self.handles['puppi_met'] = AutoHandle('pfMetPuppi', 'std::vector<reco::PFMET>')
        self.handles['pfmet'] = AutoHandle('slimmedMETs', 'std::vector<pat::MET>')

    def declareVariables(self, setup):

        self.bookTau(self.tree, 'tau')
        self.bookGenParticle(self.tree, 'tau_gen')
        self.bookGenParticle(self.tree, 'tau_gen_vis')
        self.var(self.tree, 'tau_gen_decayMode')
        self.var(self.tree, 'tau_puppi_iso_pt')
        self.var(self.tree, 'tau_puppi_iso04_pt')
        self.var(self.tree, 'tau_puppi_iso03_pt')

        self.declareJetMETVars()


    def declareJetMETVars(self):
        self.var(self.tree, 'genmet_pt')
        self.var(self.tree, 'genmet_phi')

        self.var(self.tree, 'pfmet_pt')
        self.var(self.tree, 'pfmet_phi')

        self.var(self.tree, 'puppimet_pt')
        self.var(self.tree, 'puppimet_phi')

        self.var(self.tree, 'njets20')
        self.var(self.tree, 'njets30')

        self.var(self.tree, 'nbjets20')
        self.var(self.tree, 'nbjets30')

    def fillJetMETVars(self, event):
        neutrinos = [
            p for p in event.genParticles if abs(p.pdgId()) in (12, 14, 16)]

        genmet = ROOT.math.XYZTLorentzVectorD()
        for nu in neutrinos:
            genmet += nu.p4()

        self.fill(self.tree, 'genmet_pt', genmet.pt())
        self.fill(self.tree, 'genmet_phi', genmet.phi())

        met = self.handles['pfmet'].product()[0]
        self.fill(self.tree, 'pfmet_pt', met.pt())
        self.fill(self.tree, 'pfmet_phi', met.phi())

        puppimet = self.handles['puppi_met'].product()[0]
        self.fill(self.tree, 'puppimet_pt', puppimet.pt())
        self.fill(self.tree, 'puppimet_phi', puppimet.phi())

        self.fill(self.tree, 'njets20', len(event.jets))
        self.fill(self.tree, 'njets30', len([j for j in event.jets if j.pt() > 30.]))

        self.fill(self.tree, 'nbjets20', len(event.bJets))
        self.fill(self.tree, 'nbjets30', len([j for j in event.bJets if j.pt() > 30.]))

    def process(self, event):
        # needed when doing handle.product(), goes back to
        # PhysicsTools.Heppy.analyzers.core.Analyzer
        self.readCollections(event.input)

        if not eval(self.skimFunction):
            return False

        event.genJets = self.mchandles['genJets'].product()

        ptcut = 8.
        ptSelGentauleps = [lep for lep in event.gentauleps if lep.pt() > ptcut]
        ptSelGenleps = [lep for lep in event.genleps if lep.pt() > ptcut]
        ptSelGenSummary = [p for p in event.generatorSummary if p.pt() > ptcut and abs(p.pdgId()) not in [6, 23, 24, 25, 35, 36, 37]]

        for tau in event.selectedTaus:
            DYJetsFakeAnalyzer.genMatch(event, tau, ptSelGentauleps,
                                        ptSelGenleps, ptSelGenSummary)


        for i_tau, tau in enumerate(event.selectedTaus):
            
            if i_tau < self.maxNTaus:
                self.tree.reset()
                self.fillJetMETVars(event)
                self.fillTau(self.tree, 'tau', tau)
                self.fill(self.tree, 'tau_puppi_iso_pt', tau.puppi_iso_pt)
                self.fill(self.tree, 'tau_puppi_iso04_pt', tau.puppi_iso04_pt)
                self.fill(self.tree, 'tau_puppi_iso03_pt', tau.puppi_iso03_pt)
                if tau.genp:
                    self.fillGenParticle(self.tree, 'tau_gen', tau.genp)
                    if tau.genJet():
                        self.fillGenParticle(self.tree, 'tau_gen_vis', tau.genJet())
                        self.fill(self.tree, 'tau_gen_decayMode', tauDecayModes.genDecayModeInt(tau.genJet()))

                self.fillTree(event)
