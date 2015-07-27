from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Muon import Muon
from PhysicsTools.Heppy.physicsobjects.Electron import Electron
from PhysicsTools.Heppy.physicsobjects.Tau import Tau

from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import TauElectron, DirectDiTau
import ROOT

class TauEleAnalyzer(DiLeptonAnalyzer):

    DiObjectClass = TauElectron
    LeptonClass = Electron
    OtherLeptonClass = Muon

    def declareHandles(self):
        super(TauEleAnalyzer, self).declareHandles()

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

    def buildDiLeptons(self, cmgDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID electron.
        The electron ID selection is done so that dxy and dz can be computed
        '''
        diLeptons = []
        for index, dil in enumerate(cmgDiLeptons):
            pydil = self.__class__.DiObjectClass(dil)
            pydil.leg2().associatedVertex = event.goodVertices[0]
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg1().rho = event.rho
            pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
            diLeptons.append(pydil)
        return diLeptons

    def buildDiLeptonsSingle(self, leptons, event):
        di_leptons = []
        met = self.handles['met'].product()[0]
        for pat_ele in leptons:
            ele = self.__class__.LeptonClass(pat_ele)
            for pat_tau in self.handles['taus'].product():
                tau = Tau(pat_tau)
                di_tau = DirectDiTau(ele, tau, met)
                di_tau.leg2().associatedVertex = event.goodVertices[0]
                di_tau.leg1().associatedVertex = event.goodVertices[0]
                di_tau.leg1().rho = event.rho

                di_tau.mvaMetSig = None
                di_leptons.append(di_tau)
        return di_leptons


    def relIsop3(self, electron, dbeta=0.5):

#        import pdb; pdb.set_trace()
        iso = (electron.pfIsolationVariables().sumChargedHadronPt + max(
                electron.pfIsolationVariables().sumNeutralHadronEt +
                electron.pfIsolationVariables().sumPhotonEt - 
                dbeta * electron.pfIsolationVariables().sumPUPt, 0.0)) / electron.pt()
        
        return iso

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
#            if pyl.relIso(dBetaFactor=0.5, allCharged=0) > 0.3:

            if self.relIsop3(pyl) > 0.3:
                continue
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

#        import pdb; pdb.set_trace()

        # FIXME - JAN - for current 2015 sync, but shall we really discard
        # the vertex cuts?
        event.goodVertices = event.vertices
        
        result = super(TauEleAnalyzer, self).process(event)

        event.isSignal = False
        if result:
            event.isSignal = True
        # if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # and di-lepton veto must pass
            # i.e. only the iso requirement is relaxed

        result = self.selectionSequence(event, fillCounter=True,
                                        leg2IsoCut=self.cfg_ana.looseiso1,
                                        leg1IsoCut=self.cfg_ana.looseiso2)
        if result is False:
            # really no way to find a suitable di-lepton,
            # even in the control region
            return False

        event.isSignal = event.isSignal and event.leptonAccept and event.thirdLeptonVeto

#        import pdb; pdb.set_trace()

        return True

    def testTauVertex(self, tau):
        '''Tests vertex constraints, for tau'''
        # Just checks if the primary vertex the tau was reconstructed with
        # corresponds to the one used in the analysis
        # isPV = tau.vertex().z() == lepton.associatedVertex.z()
        isPV = abs(tau.leadChargedHadrCand().dz()) < 0.2
        return isPV

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu'''
        return abs(lepton.dxy()) < 0.045 and abs(lepton.dz()) < 0.2

    def testLeg2ID(self, tau):
        # Don't apply anti-e discriminator for relaxed tau ID
        # RIC: 9 March 2015
        return ( (tau.tauID('decayModeFinding')         > 0.5  or
                  tau.tauID('decayModeFindingNewDMs')   > 0.5) and
                 # tau.tauID('againstElectronTightMVA5')  > 0.5  and
                 # tau.tauID('againstMuonLoose3')         > 0.5  and
                 # (tau.zImpact() > 0.5 or tau.zImpact() < -1.5) and
                 self.testTauVertex(tau) )

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

    def testTightElectronID(self, electron):
        mva = electron.mvaRun2('NonTrigPhys14')
        eta = abs(electron.superCluster().eta())
        if eta < 0.8:
            return mva > 0.965
        elif eta < 1.479:
            return mva > 0.917
        return mva > 0.683

    def testElectronID(self, electron):
        mva = electron.mvaRun2('NonTrigPhys14')
        eta = abs(electron.superCluster().eta())
        if eta < 0.8:
            return mva > 0.933
        elif eta < 1.479:
            return mva > 0.825
        return mva > 0.337

    def testLeg1ID(self, electron):
        '''Tight electron selection, no isolation requirement.
           Electron ID: NonTrigPhys14, Tight
        '''
        # return electron.electronID('POG_MVA_ID_Run2_NonTrig_Tight') and \

        cVeto = electron.passConversionVeto()
        mHits = electron.gsfTrack().hitPattern().numberOfHits(ROOT.reco.HitPattern.MISSING_INNER_HITS) <= 1

        return self.testTightElectronID(electron) and self.testVertex(electron) and (cVeto and mHits)


    def testLeg1Iso(self, leg, isocut):  # electron
        if isocut is None:
            isocut = self.cfg_ana.iso2
#        return leg.relIso(dBetaFactor=0.5, allCharged=0) < isocut
        return self.relIsop3(leg) < isocut

    def testLooseleg1(self, leg):  # electrons
        ''' pt, eta and isolation selection for electrons
            used in the di-electron veto.
            POG_PHYS14_25ns_v1_Veto
            pt 15, eta 2.5, dB relIso 0.3
        '''
#        if (leg.relIso(dBetaFactor=0.5, allCharged=0) > 0.3 or
        if (self.relIsop3(leg) > 0.3 or
            abs(leg.eta()) > 2.5 or
            leg.pt() < 15 or
            leg.cutBasedId('POG_PHYS14_25ns_v1_Veto') == False or
            self.testVertex(leg) == False):
            return False
        return True

    def testTightOtherLepton(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.muonID('POG_ID_Medium') and \
            self.testVertex(muon) and \
            abs(muon.eta()) < 2.4 and \
            muon.pt() > 10. and \
            muon.relIso(dBetaFactor=0.5, allCharged=0) < 0.3


    def otherLeptonVeto(self, leptons, otherLeptons, isoCut=0.3):
        # count tight muons
        vOtherLeptons = [muon for muon in otherLeptons if
                    muon.muonID('POG_ID_Medium') and
                    self.testVertex(muon) and
                    self.testLegKine(muon, ptcut=10, etacut=2.4) and
                    muon.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]

        if len(vOtherLeptons) > 0:
            return False

        return True


    def thirdLeptonVeto(self, leptons, otherLeptons, isoCut=0.3):
        # count electrons
        vLeptons = [electron for electron in leptons if
                         self.testLegKine(electron, ptcut=10, etacut=2.5) and
                         self.testVertex(electron) and
                         self.testElectronID(electron) and
                         # electron.cutBasedId('POG_PHYS14_25ns_v1_Veto') and
                         self.relIsop3(electron) < 0.3]
#                         electron.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]

        if len(vLeptons) > 1:
            return False

        return True

    def leptonAccept(self, leptons, event):
        '''Returns True if the additional lepton veto is successful'''
        looseLeptons = filter(self.testLooseleg1, leptons)
        nLeptons = len(looseLeptons)

        if event.leg1 not in looseLeptons:
            looseLeptons.append(event.leg1)

        if nLeptons < 2:
            return True

        # Reject if OS
        if any(l.charge() > 0 for l in looseLeptons) and \
           any(l.charge() < 0 for l in looseLeptons):
            return False

        return True

    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton according to Andrew's prescription.'''

        if len(diLeptons) == 1:
            return diLeptons[0]

        minRelIso = min(self.relIsop3(d.leg1()) for d in diLeptons)

        diLeps = [dil for dil in diLeptons if self.relIsop3(dil.leg1()) == minRelIso]

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
            import pdb; pdb.set_trace()

        return diLeps[0]
        


    def trigMatched(self, event, diL, requireAllMatched=False):
        '''Check that at least one trigger object per pgdId from a given trigger 
        has a matched leg with the same pdg ID. If requireAllMatched is True, 
        requires that each single trigger object has a match.'''

        matched = False
        legs = [diL.leg1(), diL.leg2()]
        event.matchedPaths = set()

        for info in event.trigger_infos:
            if not info.fired:
                continue

            print '[DBG] HLT_path = ', info.name

            matchedIds = set()
            allMatched = True
            for to in info.objects:

                print '[DBG] \t match =', self.trigObjMatched(to, legs)

                for yuta in to.pathNames(True, True):
                    print '[DBG] \t\t pathNames(True, True) = ', yuta

                for yuta in to.pathNames(True, False):
                    print '[DBG] \t\t pathNames(True, False) = ', yuta

                for yuta in to.pathNames(False, True):
                    print '[DBG] \t\t pathNames(False, True) = ', yuta

                for yuta in to.pathNames(False, False):
                    print '[DBG] \t\t pathNames(False, False) = ', yuta

                for yuta in to.filterLabels():
                    print '[DBG] \t\t filter name = ', yuta

                if self.trigObjMatched(to, legs):
                    matchedIds.add(abs(to.pdgId()))
                else:
                    allMatched = False

            if matchedIds == info.objIds:
                if requireAllMatched and not allMatched:
                    matched = False
                else:
                    matched = True
                    event.matchedPaths.add(info.name)
        

        flag = any([(mp.find('Ele22')!=-1 and mp.find('PFTau20')!=-1) for mp in event.matchedPaths])
        flag_single = any([mp.find('Ele32')!=-1 for mp in event.matchedPaths])

#        print event.matchedPaths, flag, flag_single, matched, diL.leg1().pt()

        if flag:
#            print 'This is not single trigger ... passed'
            return matched
        elif flag_single:
#            print 'This is only single trigger ... require 33 GeV'
            return matched and diL.leg1().pt() > 33
        else:
#            print 'This maens, no trigger fired'
            return matched
