import ROOT

from PhysicsTools.Heppy.physicsutils.TauDecayModes import tauDecayModes

from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerBase import H2TauTauTreeProducerBase

class TauGenTreeProducer(H2TauTauTreeProducerBase):
    ''' Tree producer for generator tau study.
    '''

    def __init__(self, *args):
        super(TauGenTreeProducer, self).__init__(*args)

    def declareHandles(self):
        super(TauGenTreeProducer, self).declareHandles()

    @staticmethod
    def finalDaughters(gen, daughters=None):
        if daughters is None:
            daughters = []
        for i in range(gen.numberOfDaughters()):
            daughter = gen.daughter(i)
            if daughter.numberOfDaughters() == 0:
                daughters.append(daughter)
            else:
                TauGenTreeProducer.finalDaughters(daughter, daughters)

        return daughters

    @staticmethod
    def visibleP4(gen):
        final_ds = TauGenTreeProducer.finalDaughters(gen)

        p4 = sum((d.p4() for d in final_ds if abs(d.pdgId()) not in [12, 14, 16]), ROOT.math.XYZTLorentzVectorD())

        return p4

    def declareVariables(self, setup):

        self.bookTau(self.tree, 'tau1')
        self.bookGenParticle(self.tree, 'tau1_gen')
        self.bookParticle(self.tree, 'tau1_gen_vis')
        self.var(self.tree, 'tau1_gen_decayMode')

        self.bookTau(self.tree, 'tau2')
        self.bookGenParticle(self.tree, 'tau2_gen')
        self.bookParticle(self.tree, 'tau2_gen_vis')
        self.var(self.tree, 'tau2_gen_decayMode')

        self.var(self.tree, 'n_gen_taus')


    def process(self, event):
        # needed when doing handle.product(), goes back to
        # PhysicsTools.Heppy.analyzers.core.Analyzer
        self.readCollections(event.input)


        if not eval(self.skimFunction):
            return False

        self.tree.reset()
        self.fill(self.tree, 'n_gen_taus', len(event.gentaus))

        for i_tau, gen_tau in enumerate(event.gentaus):
            if i_tau >= 2:
                print 'More than two generated hadronic taus!'
                continue

            self.fillGenParticle(self.tree, 'tau{i}_gen'.format(i=i_tau+1), gen_tau)
            self.fillParticle(self.tree, 'tau{i}_gen_vis'.format(i=i_tau+1), self.visibleP4(gen_tau))
            self.fill(self.tree, 'tau{i}_gen_decayMode'.format(i=i_tau+1), tauDecayModes.genDecayModeInt([d for d in TauGenTreeProducer.finalDaughters(gen_tau) if abs(d.pdgId()) not in [12, 14, 16]]))

            for tau in event.selectedTaus:
                if tau.mcTau == gen_tau:
                    # import pdb; pdb.set_trace()
                    self.fillTau(self.tree, 'tau{i}'.format(i=i_tau+1), tau)
                    # if tau.genJet():
                        # self.fillGenParticle(self.tree, 'tau{i}_gen_vis'.format(i=i_tau), tau.genJet())
                        

        self.fillTree(event)
