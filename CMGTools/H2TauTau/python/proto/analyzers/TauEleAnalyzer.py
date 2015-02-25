import operator

from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Muon
from PhysicsTools.Heppy.physicsobjects.Electron import Electron

from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import TauElectron


class TauEleAnalyzer(DiLeptonAnalyzer):

    DiObjectClass = TauElectron
    LeptonClass = Electron
    OtherLeptonClass = Muon

    def declareHandles(self):
        super(TauEleAnalyzer, self).declareHandles()
        self.handles['diLeptons'] = AutoHandle(
            'cmgTauEleCorSVFitFullSel',
            'std::vector<pat::CompositeCandidate>'
        )

        self.handles['otherLeptons'] = AutoHandle(
            'slimmedMuons',
            'std::vector<pat::Muon>'
        )

        self.handles['leptons'] = AutoHandle(
            'slimmedElectrons',
            'std::vector<pat::Electron>'
        )

        self.mchandles['genParticles'] = AutoHandle('prunedGenParticles',
                                                    'std::vector<reco::GenParticle>')

        self.relaxEleId = False
        self.relaxTauId = False

    def buildDiLeptons(self, cmgDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID electron.
        The electron ID selection is done so that dxy and dz can be computed
        '''
        diLeptons = []
        for index, dil in enumerate(cmgDiLeptons):
            pydil = self.__class__.DiObjectClass(dil)
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg2().associatedVertex = event.goodVertices[0]
            pydil.leg2().rho = event.rho
            pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
            diLeptons.append(pydil)
        return diLeptons

    def buildLeptons(self, cmgLeptons, event):
        '''Build electrons for veto, associate best vertex, select loose ID electrons.
        Since the electrons are used for veto, the 0.3 default isolation cut is left there, 
        as well as the pt 15 gev cut'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = self.__class__.LeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            pyl.rho = event.rho
            # if not pyl.looseIdForEleTau():
            #     continue
            leptons.append(pyl)
        return leptons

    def testMuonIDLoose(self, muon):
        '''Loose muon ID and kine, no isolation requirement, for lepton veto'''
        return muon.pt() > 15 and \
            abs(muon.eta()) < 2.4 and \
            muon.isGlobalMuon() and \
            muon.isTrackerMuon() and \
            muon.sourcePtr().userFloat('isPFMuon') and \
            abs(muon.dz()) < 0.2
        # self.testVertex( muon )

    def buildOtherLeptons(self, cmgOtherLeptons, event):
        '''Build muons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(cmgOtherLeptons):
            pyl = self.__class__.OtherLeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            # if not self.testMuonIDLoose(pyl):
            #     continue
            otherLeptons.append(pyl)
        return otherLeptons

    def process(self, event):

        # import pdb; pdb.set_trace()
        result = super(TauEleAnalyzer, self).process(event)

        event.isSignal = False

        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # and di-lepton veto must pass
            # i.e. only the iso requirement is relaxed

            # Also relax IDs for sideband studies without changing the
            # dilepton selectionSequence
            self.relaxEleId = True
            self.relaxTauId = True
            result = self.selectionSequence(event, fillCounter=False,
                                            leg1IsoCut=-9999,
                                            leg2IsoCut=9999)
            self.relaxEleId = False
            self.relaxTauId = False

            if result is False:
                # really no way to find a suitable di-lepton,
                # even in the control region
                return False
            event.isSignal = False
        else:
            event.isSignal = event.leptonAccept and event.thirdLeptonVeto

        return True

    def testLeg1ID(self, tau):
        # import pdb; pdb.set_trace()
        # Don't apply anti-e discriminator for relaxed tau ID
        if self.relaxTauId:
            return tau.tauID("againstMuonLoose") > 0.5 and \
                (tau.zImpact() > 0.5 or tau.zImpact() < -1.5) and\
                self.testTauVertex(tau)
        return tau.tauID('againstElectronMediumMVA5') and \
            tau.tauID("againstMuonLoose") > 0.5 and \
            (tau.zImpact() > 0.5 or tau.zImpact() < -1.5) and\
            self.testTauVertex(tau)

    def testTauVertex(self, lepton):
        '''Tests vertex constraints, for tau'''
        # Just checks if the primary vertex the tau was reconstructed with
        # corresponds to the one used in the analysis
        isPV = lepton.vertex().z() == lepton.associatedVertex.z()
        return isPV

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu'''
        return abs(lepton.dxy()) < 0.045 and abs(lepton.dz()) < 0.2

    def testTauVertex(self, lepton):
        '''Tests vertex constraints, for tau'''
        isPV = lepton.vertex().z() == lepton.associatedVertex.z()
        return isPV

    def testLeg1Iso(self, tau, isocut):
        '''if isocut is None, returns true if three-hit iso MVA is passed.
        Otherwise, returns true if iso MVA > isocut.'''
        if isocut is None:
            return tau.tauID('byCombinedIsolationDeltaBetaCorrRaw3Hits') < 1.5
        else:
            return tau.tauID("byIsolationMVA3oldDMwLTraw") > isocut

    def testLeg2ID(self, electron):
        '''Tight muon selection, no isolation requirement
           RIC: needs to be revised, in parallel to the heppy Electron
                currently using POG_CSA14_25ns_v1_{Loose,Tight} FIXME!
        '''
        #        print 'WARNING: USING SETUP FOR SYNC PURPOSES'
        #        return electron.looseIdForEleTau() and \
        if self.relaxEleId:
            return electron.cutBasedId('POG_CSA14_25ns_v1_Loose') and \
                self.testVertex(electron)
#             return electron.relaxedIdForEleTau() and \
#                self.testVertex( electron )
        return electron.cutBasedId('POG_CSA14_25ns_v1_Tight') and \
            self.testVertex(electron)
#         return electron.tightIdForEleTau() and \
#                self.testVertex( electron )

    def testLeg2Iso(self, leg, isocut):  # electron
        if isocut is None:
            isocut = self.cfg_ana.iso2

        return leg.relIso(dBetaFactor=0.5, allCharged=0) < isocut

    def testLooseLeg2(self, leg):  # electrons
        if leg.relIso(dBetaFactor=0.5, allCharged=0) > 0.3:
            return False
        if abs(leg.eta()) > 2.5:
            return False
        if leg.pt() < 15:
            return False
        # if leg.looseIdForEleTau()  == False : return False # RIC: ele ID
        # needs to be revised
        return True

    def testTightOtherLepton(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.tightId() and \
            self.testVertex(muon) and \
            abs(muon.eta()) < 2.4 and \
            muon.pt() > 10. and \
            muon.relIso(dBetaFactor=0.5, allCharged=0) < 0.3


    def thirdLeptonVeto(self, leptons, otherLeptons, isoCut=0.3):
        # count electrons (leg 2)
        vLeptons = [electron for electron in leptons if
                      self.testLegKine(electron, ptcut=10, etacut=2.5) and
                      # electron.looseIdForTriLeptonVeto() and
                      self.testVertex(electron) and
                      self.testLeg2Iso(electron, isoCut)]

        # count tight muons
        vOtherLeptons = [
            muon for muon in otherLeptons if self.testTightOtherLepton(muon)]
        if len(vLeptons) + len(vOtherLeptons) > 1:
            return False

        return True

    def leptonAccept(self, leptons):
        '''Returns True if the additional lepton veto is successful'''
        looseLeptons = filter(self.testLooseLeg2, leptons)
        nLeptons = len(looseLeptons)
        if nLeptons < 2:
            return True

        # Reject if OS
        if any(l.charge() > 0 for l in looseLeptons) and \
           any(l.charge() < 0 for l in looseLeptons):
            return False

        return True
    
    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (1st precedence opposite-sign, 2nd precedence
        highest pt1 + pt2).'''

        osDiLeptons = [dl for dl in diLeptons if dl.leg1().charge() != dl.leg2().charge()]
        if osDiLeptons:
            return max(osDiLeptons, key=operator.methodcaller('sumPt'))
        else:
            return max(diLeptons, key=operator.methodcaller('sumPt'))
