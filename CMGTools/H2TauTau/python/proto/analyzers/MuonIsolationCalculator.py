from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer

from PhysicsTools.HeppyCore.utils.deltar import deltaR

from ROOT import heppy


class MuonIsolationCalculator(Analyzer):

    '''Calculates different muon isolation values'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(MuonIsolationCalculator, self).__init__(cfg_ana, cfg_comp, looperName)

    def attachPuppiIso(self, muon, puppi, name='puppi'):
        puppi_iso_cands = []
        puppi_iso_cands_04 = []
        puppi_iso_cands_03 = []
        muon_eta = muon.eta()
        muon_phi = muon.phi()

        for c_p in puppi:
            pdgId = c_p.pdgId()

            if abs(pdgId) not in [22, 130, 211]:
                continue

            eta = c_p.eta()
            phi = c_p.phi()
            # Neutral hadrons or photons
            inner_cone = 0.01
            if abs(pdgId) in [211]:
                inner_cone = 0.0001
            elif c_p.pt() < 0.5:
                continue

            if abs(muon_eta - eta) < 0.5:
                dr = deltaR(eta, phi, muon_eta, muon_phi)
                if inner_cone < dr:
                    if dr < 0.5:
                        puppi_iso_cands.append(c_p)
                    if dr < 0.4:
                        puppi_iso_cands_04.append(c_p)
                    if dr < 0.3:
                        puppi_iso_cands_03.append(c_p)


        setattr(muon, name+'_iso_pt', sum(c_p.pt() for c_p in puppi_iso_cands))
        setattr(muon, name+'_iso04_pt', sum(c_p.pt() for c_p in puppi_iso_cands_04))
        setattr(muon, name+'_iso03_pt', sum(c_p.pt() for c_p in puppi_iso_cands_03))

    def attachEffectiveArea(self, mu):
        aeta = abs(mu.eta())
        if   aeta < 0.800: mu.EffectiveArea03 = 0.0913
        elif aeta < 1.300: mu.EffectiveArea03 = 0.0765
        elif aeta < 2.000: mu.EffectiveArea03 = 0.0546
        elif aeta < 2.200: mu.EffectiveArea03 = 0.0728
        else:              mu.EffectiveArea03 = 0.1177
        if   aeta < 0.800: mu.EffectiveArea04 = 0.1564
        elif aeta < 1.300: mu.EffectiveArea04 = 0.1325
        elif aeta < 2.000: mu.EffectiveArea04 = 0.0913
        elif aeta < 2.200: mu.EffectiveArea04 = 0.1212
        else:              mu.EffectiveArea04 = 0.2085

    def attachMiniIsolation(self, mu):
        mu.miniIsoR = 10.0/min(max(mu.pt(), 50), 200)
        # -- version with increasing cone at low pT, gives slightly better performance for tight cuts and low pt leptons
        # mu.miniIsoR = 10.0/min(max(mu.pt(), 50),200) if mu.pt() > 20 else 4.0/min(max(mu.pt(),10),20)
        what = "mu" if (abs(mu.pdgId()) == 13) else ("eleB" if mu.isEB() else "eleE")
        if what == "mu":
            mu.miniAbsIsoCharged = self.IsolationComputer.chargedAbsIso(mu.physObj, mu.miniIsoR, {"mu": 0.0001, "eleB": 0, "eleE": 0.015}[what], 0.0)
        else:
            mu.miniAbsIsoCharged = self.IsolationComputer.chargedAbsIso(
                mu.physObj, mu.miniIsoR, {"mu": 0.0001, "eleB": 0, "eleE": 0.015}[what], 0.0, self.IsolationComputer.selfVetoNone)
        if what == "mu":
            mu.miniAbsIsoNeutral = self.IsolationComputer.neutralAbsIsoRaw(mu.physObj, mu.miniIsoR, 0.01, 0.5)
        else:
            mu.miniAbsIsoPho = self.IsolationComputer.photonAbsIsoRaw(mu.physObj, mu.miniIsoR, 0.08 if what == "eleE" else 0.0, 0.0, self.IsolationComputer.selfVetoNone)
            mu.miniAbsIsoNHad = self.IsolationComputer.neutralHadAbsIsoRaw(mu.physObj, mu.miniIsoR, 0.0, 0.0, self.IsolationComputer.selfVetoNone)
            mu.miniAbsIsoNeutral = mu.miniAbsIsoPho + mu.miniAbsIsoNHad

        if self.miniIsolationPUCorr == "rhoArea":
            mu.miniAbsIsoNeutral = max(0.0, mu.miniAbsIsoNeutral - mu.rho * mu.EffectiveArea03 * (mu.miniIsoR/0.3)**2)
        elif self.miniIsolationPUCorr != 'raw':
            raise RuntimeError, "Unsupported miniIsolationCorr name '" + \
                str(self.cfg_ana.miniIsolationCorr) + "'! For now only 'rhoArea', 'deltaBeta', 'raw', 'weights' are supported (and 'weights' is not tested)."

        mu.miniAbsIso = mu.miniAbsIsoCharged + mu.miniAbsIsoNeutral
        mu.miniRelIso = mu.miniAbsIso/mu.pt()

    def declareHandles(self):

        super(MuonIsolationCalculator, self).declareHandles()
        self.handles['puppi'] = AutoHandle(('puppi'), 'std::vector<reco::PFCandidate>')
        self.handles['puppi_no_muon'] = AutoHandle(('particleFlowNoMuonPUPPI'), 'std::vector<reco::PFCandidate>')
        self.handles['packedCandidates'] = AutoHandle('packedPFCandidates', 'std::vector<pat::PackedCandidate>')

        self.IsolationComputer = heppy.IsolationComputer()
        self.miniIsolationPUCorr = 'rhoArea'

    def beginLoop(self, setup):
        print self, self.__class__
        super(MuonIsolationCalculator, self).beginLoop(setup)

    def process(self, event):
        self.readCollections(event.input)

        puppi = self.handles['puppi'].product()
        puppi_no_muon = self.handles['puppi_no_muon'].product()

        self.IsolationComputer.setPackedCandidates(self.handles['packedCandidates'].product())
        for lep in [event.diLepton.leg1(), event.diLepton.leg2()]:
            self.IsolationComputer.addVetos(lep.physObj)

        for muon in [event.diLepton.leg1()]:
            muon.rho = event.rho
            self.attachEffectiveArea(muon)
            self.attachMiniIsolation(muon)
            self.attachPuppiIso(muon, puppi)
            self.attachPuppiIso(muon, puppi_no_muon, 'puppi_no_muon')

        return True
