from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer

from PhysicsTools.HeppyCore.utils.deltar import deltaPhi, deltaR

class TauIsolationCalculator(Analyzer):

    '''Gets tau decay mode efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(TauIsolationCalculator, self).__init__(cfg_ana, cfg_comp, looperName)
    def declareHandles(self):

        super(TauIsolationCalculator, self).declareHandles()
        self.handles['puppi'] = AutoHandle(('puppi'), 'std::vector<reco::PFCandidate>')

        self.getter = self.cfg_ana.getter if hasattr(self.cfg_ana, 'getter') else lambda event: event.selectedTaus
        
    def beginLoop(self, setup):
        print self, self.__class__
        super(TauIsolationCalculator, self).beginLoop(setup)

    def process(self, event):
        self.readCollections(event.input)

        puppi = self.handles['puppi'].product()

        for tau in self.getter(event):
            puppi_iso_cands = []
            puppi_iso_cands_04 = []
            puppi_iso_cands_03 = []
            isoPtSumOld = 0.
            tau_eta = tau.eta()
            tau_phi = tau.phi()
            # Normal loop crashes for some reason...
            for i_iso in range(len(tau.isolationCands())):
                    isoPtSumOld += tau.isolationCands()[i_iso].pt()
                    eta = tau.isolationCands()[i_iso].eta()
                    phi = tau.isolationCands()[i_iso].phi()
                    for c_p in puppi:
                        if abs(eta - c_p.eta()) < 0.00001 and \
                           abs(deltaPhi(phi, c_p.phi())) < 0.00001:
                            puppi_iso_cands.append(c_p)
                            dr = deltaR(c_p.eta(), c_p.phi(), tau_eta, tau_phi)
                            if dr < 0.4:
                                puppi_iso_cands_04.append(c_p)
                            if dr < 0.3:
                                puppi_iso_cands_03.append(c_p)

            #  = sum(iso.pt() for iso in tau.isolationCands())
            tau.puppi_iso_pt = sum(c_p.pt() for c_p in puppi_iso_cands)
            tau.puppi_iso04_pt = sum(c_p.pt() for c_p in puppi_iso_cands_04)
            tau.puppi_iso03_pt = sum(c_p.pt() for c_p in puppi_iso_cands_03)
            # Add puppi isolation

        return True
