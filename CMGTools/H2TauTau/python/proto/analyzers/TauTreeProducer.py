from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes

from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerBase import H2TauTauTreeProducerBase

class TauTreeProducer(H2TauTauTreeProducerBase):
    ''' Tree producer for tau POG study.
    '''

    def __init__(self, *args):
        super(TauTreeProducer, self).__init__(*args)
        self.maxNTaus = 2

    def declareHandles(self):
        super(TauTreeProducer, self).declareHandles()

    def declareVariables(self, setup):

        for i in range(self.maxNTaus):
            self.bookTau(self.tree, 'tau{i}'.format(i=i))
            self.bookGenParticle(self.tree, 'tau{i}_gen'.format(i=i))
            self.bookGenParticle(self.tree, 'tau{i}_gen_vis'.format(i=i))
            self.var(self.tree, 'tau{i}_gen_decayMode'.format(i=i))


    def process(self, event):
        # needed when doing handle.product(), goes back to
        # PhysicsTools.Heppy.analyzers.core.Analyzer
        self.readCollections(event.input)

        self.tree.reset()

        if not eval(self.skimFunction):
            return False

        for i_tau, tau in enumerate(event.selectedTaus):
            if i_tau < self.maxNTaus:
                self.fillTau(self.tree, 'tau{i}'.format(i=i_tau), tau)
                if tau.mcTau:
                    self.fillGenParticle(self.tree, 'tau{i}_gen'.format(i=i_tau), tau.mcTau)
                    if tau.genJet():
                        self.fillGenParticle(self.tree, 'tau{i}_gen_vis'.format(i=i_tau), tau.genJet())
                        self.fill(self.tree, 'tau{i}_gen_decayMode'.format(i=i_tau), tauDecayModes.genDecayModeInt(tau.genJet()))

        if type(self) is TauTreeProducer:
            self.fillTree(event)
