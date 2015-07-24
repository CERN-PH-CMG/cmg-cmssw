import ROOT

from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Muon import Muon
from PhysicsTools.Heppy.physicsobjects.Electron import Electron
from PhysicsTools.Heppy.physicsobjects.Tau import Tau

from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import TauMuon, DirectDiTau



class TauMuAnalyzer(DiLeptonAnalyzer):

    DiObjectClass = TauMuon
    LeptonClass = Muon
    OtherLeptonClass = Electron

    def declareHandles(self):
        super(TauMuAnalyzer, self).declareHandles()

        if hasattr(self.cfg_ana, 'from_single_objects') and self.cfg_ana.from_single_objects:
            self.handles['taus'] = AutoHandle(
                'slimmedTaus',
                'std::vector<pat::Tau>'
            )
            self.handles['met'] = AutoHandle(
                'slimmedMETs',
                'std::vector<pat::MET>'
            )
        else:
            self.handles['diLeptons'] = AutoHandle(
                'cmgTauMuCorSVFitFullSel',
                'std::vector<pat::CompositeCandidate>'
            )

        self.handles['otherLeptons'] = AutoHandle(
            'slimmedElectrons',
            'std::vector<pat::Electron>'
        )

        self.handles['leptons'] = AutoHandle(
            'slimmedMuons',
            'std::vector<pat::Muon>'
        )

        self.mchandles['genParticles'] = AutoHandle(
            'prunedGenParticles',
            'std::vector<reco::GenParticle>'
        )

    def buildDiLeptons(self, patDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID muon.
        The tight ID selection is done so that dxy and dz can be computed
        (the muon must not be standalone).
        '''
        diLeptons = []
        for index, dil in enumerate(patDiLeptons):
            pydil = self.__class__.DiObjectClass(dil)
            pydil.leg2().associatedVertex = event.goodVertices[0]
            pydil.leg1().associatedVertex = event.goodVertices[0]
            if not self.testLeg1(pydil.leg1(), 99999):
                continue
            # JAN: This crashes. Waiting for idea how to fix this; may have
            # to change data format otherwise, though we don't yet strictly
            # need the MET significance matrix here since we calculate SVFit
            # before
            pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
            diLeptons.append(pydil)
        return diLeptons

    def buildDiLeptonsSingle(self, leptons, event):
        di_leptons = []
        met = self.handles['met'].product()[0]
        for pat_mu in leptons:
            muon = self.__class__.LeptonClass(pat_mu)
            for pat_tau in self.handles['taus'].product():
                tau = Tau(pat_tau)
                di_tau = DirectDiTau(muon, tau, met)
                di_tau.leg2().associatedVertex = event.goodVertices[0]
                di_tau.leg1().associatedVertex = event.goodVertices[0]
                if not self.testLeg1(di_tau.leg1(), 99999):
                    continue

                di_tau.mvaMetSig = None
                di_leptons.append(di_tau)
        return di_leptons

    def buildLeptons(self, patLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(patLeptons):
            pyl = self.__class__.LeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            leptons.append(pyl)
        return leptons

    def buildOtherLeptons(self, patOtherLeptons, event):
        '''Build electrons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(patOtherLeptons):
            pyl = self.__class__.OtherLeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            pyl.rho = event.rho
            otherLeptons.append(pyl)
        return otherLeptons

    def process(self, event):
        # FIXME - JAN - for current 2015 sync, but shall we really discard
        # the vertex cuts?
        event.goodVertices = event.vertices

        result = super(TauMuAnalyzer, self).process(event)

        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # di-lepton and tri-lepton veto must pass
            result = self.selectionSequence(event, fillCounter=True,
                                            leg1IsoCut=self.cfg_ana.looseiso1,
                                            leg2IsoCut=self.cfg_ana.looseiso2)
            if result is False:
                # really no way to find a suitable di-lepton,
                # even in the control region
                return False
            event.isSignal = False
        else:
            event.isSignal = True

        return True

    def testLeg2ID(self, tau):
        return ((tau.tauID('decayModeFinding') > 0.5 or
                 tau.tauID('decayModeFindingNewDMs') > 0.5) and
                # tau.tauID('againstElectronVLooseMVA5') > 0.5  and
                # tau.tauID('againstMuonTight3')         > 0.5  and
                abs(tau.charge()) == 1. and
                self.testTauVertex(tau))
        # https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendation13TeV

    def testLeg2Iso(self, tau, isocut):
        '''if isocut is None, returns true if three-hit iso cut is passed.
        Otherwise, returns true if iso MVA > isocut.'''
        if isocut is None:
            return tau.tauID('byLooseCombinedIsolationDeltaBetaCorr3Hits') > 0.5
        else:
            # JAN FIXME - placeholder, as of now only used to define passing cuts
            # return tau.tauID("byIsolationMVA3newDMwLTraw") > isocut
            # RIC: 9 March 2015
            return tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < isocut

    def testTauVertex(self, tau):
        '''Tests vertex constraints, for tau'''
        # Just checks if the primary vertex the tau was reconstructed with
        # corresponds to the one used in the analysis
        # isPV = abs(tau.vertex().z() - tau.associatedVertex.z()) < 0.2
        isPV = abs(tau.leadChargedHadrCand().dz()) < 0.2
        return isPV

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu'''
        return abs(lepton.dxy()) < 0.045 and abs(lepton.dz()) < 0.2

    def testLeg1ID(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.muonID('POG_ID_Medium') and self.testVertex(muon)

    def testLeg1Iso(self, muon, isocut):
        '''Tight muon selection, with isolation requirement'''
        if isocut is None:
            isocut = self.cfg_ana.iso2

        return muon.relIso(dBetaFactor=0.5, allCharged=0) < isocut

    def thirdLeptonVeto(self, leptons, otherLeptons, isoCut=0.3):
        # count tight muons
        vLeptons = [muon for muon in leptons if
                    muon.muonID('POG_ID_Medium') and
                    self.testVertex(muon) and
                    self.testLegKine(muon, ptcut=10, etacut=2.4) and
                    muon.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]

        if len(vLeptons) > 1:
            return False

        return True


    def testElectronID(self, electron):
        mva = electron.mvaRun2('NonTrigPhys14')
        eta = abs(electron.superCluster().eta())
        if eta < 0.8:
            return mva > 0.933
        elif eta < 1.479:
            return mva > 0.825
        return mva > 0.337

    def tempEleIso(self, ele):
        iso_sum = ele.chargedHadronIsoR(R=0.3) + max(ele.photonIsoR(R=0.3) + ele.neutralHadronIsoR(R=0.3) - 0.5 * ele.puChargedHadronIsoR(R=0.3), 0.)
        return iso_sum/ele.pt()

    def otherLeptonVeto(self, leptons, otherLeptons, isoCut=0.3):
        # count electrons
        vOtherLeptons = [electron for electron in otherLeptons if
                         self.testLegKine(electron, ptcut=10, etacut=2.5) and
                         self.testVertex(electron) and
                         self.testElectronID(electron) and
                         electron.passConversionVeto() and
                         electron.physObj.gsfTrack().hitPattern().numberOfHits(ROOT.reco.HitPattern.MISSING_INNER_HITS) <= 1 and
                         # electron.cutBasedId('POG_PHYS14_25ns_v1_Veto') and
                         # electron.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]
                         self.tempEleIso(electron) < 0.3]

        if len(vOtherLeptons) > 0:
            return False

        return True

    def leptonAccept(self, leptons, event):
        '''Di-lepton veto: returns false if >= 1 OS same flavour lepton pair,
        e.g. >= 1 OS mu pair in the mu tau channel'''
        looseLeptons = [muon for muon in leptons if
                        self.testLegKine(muon, ptcut=15, etacut=2.4) and
                        muon.isGlobalMuon() and
                        muon.isTrackerMuon() and
                        muon.isPFMuon() and
                        abs(muon.dz()) < 0.2 and
                        self.testLeg1Iso(muon, 0.3)
                        ]

        if event.leg1 not in looseLeptons:
            looseLeptons.append(event.leg1)

        if any(l.charge() > 0 for l in looseLeptons) and \
           any(l.charge() < 0 for l in looseLeptons):
            return False

        return True

    def trigMatched(self, event, diL, requireAllMatched=False):
        
        matched = super(TauMuAnalyzer, self).trigMatched(event, diL, requireAllMatched=requireAllMatched)

        if matched and len(diL.matchedPaths) == 1 and diL.leg1().pt() < 25. and 'IsoMu24' in list(diL.matchedPaths)[0]:
            matched = False

        return matched

    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (1st precedence opposite-sign, 2nd precedence
        highest pt1 + pt2).'''

        if len(diLeptons) == 1:
            return diLeptons[0]

        minRelIso = min(d.leg1().relIso(dBetaFactor=0.5, allCharged=0) for d in diLeptons)

        diLeps = [dil for dil in diLeptons if dil.leg1().relIso(dBetaFactor=0.5, allCharged=0) == minRelIso]

        if len(diLeps) == 1:
            return diLeps[0]

        maxPt = max(d.leg1().pt() for d in diLeps)

        diLeps = [dil for dil in diLeps if dil.leg1().pt() == maxPt]

        if len(diLeps) == 1:
            return diLeps[0]

        minIso = min(d.leg2().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") for d in diLeps)

        diLeps = [dil for dil in diLeps if dil.leg2().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") == minIso]

        if len(diLeps) == 1:
            return diLeps[0]

        maxPt = max(d.leg2().pt() for d in diLeps)

        diLeps = [dil for dil in diLeps if dil.leg2().pt() == maxPt]

        if len(diLeps) != 1:
            print 'ERROR in finding best dilepton', diLeps
            import pdb
            pdb.set_trace()

        return diLeps[0]

        # osDiLeptons = [dl for dl in diLeptons if dl.leg2().charge() != dl.leg1().charge()]
        # if osDiLeptons:
        #     return max(osDiLeptons, key=operator.methodcaller('sumPt'))
        # else:
        #     return max(diLeptons, key=operator.methodcaller('sumPt'))
