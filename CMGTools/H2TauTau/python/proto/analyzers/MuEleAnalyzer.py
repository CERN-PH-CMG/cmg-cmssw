import operator

from PhysicsTools.Heppy.analyzers.core.AutoHandle       import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects   import Muon, GenParticle
# RIC: 16/2/15 need to fix the Electron object first
# from PhysicsTools.Heppy.physicsobjects.HTauTauElectron  import HTauTauElectron as Electron
from PhysicsTools.Heppy.physicsobjects.Electron         import Electron

from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.physicsobjects.DiObject    import MuonElectron, DirectDiTau
import ROOT

class MuEleAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass    = MuonElectron
    LeptonClass      = Electron
    OtherLeptonClass = Muon

    def declareHandles(self):
        super(MuEleAnalyzer, self).declareHandles()

        if hasattr(self.cfg_ana, 'from_single_objects') and self.cfg_ana.from_single_objects:
            self.handles['met'] = AutoHandle(
                'slimmedMETs',
                'std::vector<pat::MET>'
            )
        else:
            self.handles['diLeptons'] = AutoHandle(
                'cmgMuEleCorSVFitFullSel', 
                'std::vector<pat::CompositeCandidate>'
                )


        self.handles['leptons'] = AutoHandle(
            'slimmedElectrons',
            'std::vector<pat::Electron>'          
            )

        self.handles['otherLeptons'] = AutoHandle(
            'slimmedMuons',
            'std::vector<pat::Muon>'              
            )

        self.mchandles['genParticles'] = AutoHandle(
            'prunedGenParticles',
            'std::vector<reco::GenParticle>'      
            )

    def buildDiLeptons(self, cmgDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID muon.
        The tight ID selection is done so that dxy and dz can be computed
        (the muon must not be standalone).
        '''
        diLeptons = []
        for index, dil in enumerate(cmgDiLeptons):
            pydil = self.__class__.DiObjectClass(dil)
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg2().associatedVertex = event.goodVertices[0]
#            pydil.leg2().rho = event.rho
            pydil.leg1().rho = event.rho
#            if not self.testLeg2( pydil.leg2(), 999999 ):
            if not self.testLeg1( pydil.leg1(), 999999 ):
                continue
            # pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
            diLeptons.append( pydil )
            pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
        return diLeptons

    def buildDiLeptonsSingle(self, leptons, event):
        di_leptons = []
        met = self.handles['met'].product()[0]

#        import pdb; pdb.set_trace()
        for pat_e in leptons:
            electron = self.__class__.LeptonClass(pat_e)
            for pat_mu in self.handles['otherLeptons'].product():
                muon = self.__class__.OtherLeptonClass(pat_mu)
                di_tau = DirectDiTau(electron, muon, met)
                di_tau.leg1().associatedVertex = event.goodVertices[0]
                di_tau.leg2().associatedVertex = event.goodVertices[0]
                di_tau.leg1().rho = event.rho

                if not self.testLeg1(di_tau.leg1(), 99999):
                    continue

                di_tau.mvaMetSig = None
                di_leptons.append(di_tau)
        return di_leptons


    def buildLeptons(self, cmgLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = self.__class__.LeptonClass(lep)
            #pyl = Muon(lep)
            pyl.associatedVertex = event.goodVertices[0]
            leptons.append( pyl )
        return leptons

    def buildOtherLeptons(self, cmgOtherLeptons, event):
        '''Build electrons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(cmgOtherLeptons):
            pyl = self.__class__.OtherLeptonClass(lep)
            #import pdb ; pdb.set_trace()
            #pyl = Electron(lep)
            pyl.associatedVertex = event.goodVertices[0]
            pyl.rho = event.rho
            otherLeptons.append( pyl )
        return otherLeptons

    def process(self, event):

        event.goodVertices = event.vertices

#        import pdb; pdb.set_trace()
        result = super(MuEleAnalyzer, self).process(event)

        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # di-lepton and tri-lepton veto must pass
            result = self.selectionSequence(event, fillCounter = False,
                                            leg1IsoCut = self.cfg_ana.looseiso1,
                                            leg2IsoCut = self.cfg_ana.looseiso2)
            if result is False:
                # really no way to find a suitable di-lepton,
                # even in the control region
                return False
            event.isSignal = False
        else:
            event.isSignal = True

        return True

    def testLeg2ID(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.muonID('POG_ID_Medium') and self.testVertex(muon)

    def testLeg2Iso(self, muon, isocut):
        '''Muon isolation to be implemented'''
        if isocut is None:
            isocut = self.cfg_ana.iso2

        return muon.relIso(dBetaFactor=0.5, allCharged=0)<isocut

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and electron'''
        return abs(lepton.dxy()) < 0.045 and abs(lepton.dz ()) < 0.2

    def testLeg1ID(self, electron):
        '''Electron ID. To be implemented'''

        cVeto = electron.passConversionVeto()
        mHits = electron.gsfTrack().hitPattern().numberOfHits(ROOT.reco.HitPattern.MISSING_INNER_HITS) <= 1

        return self.testElectronID(electron) and self.testVertex(electron) and (cVeto and mHits)

#    electron.electronID('POG_MVA_ID_Run2_NonTrig_Tight') and \


    def testLeg1Iso(self, electron, isocut):
        '''Electron Isolation. Relative isolation
           dB corrected factor 0.5
           all charged aprticles
        '''
        if isocut is None:
            isocut = self.cfg_ana.iso2
        return electron.relIso(dBetaFactor=0.5, allCharged=0) < isocut

    def thirdLeptonVeto(self, leptons, otherLeptons, ptcut = 10, isocut = 0.3) :
        '''The tri-lepton veto. To be implemented'''

        # count electrons (leg 2)
        vOtherLeptons = [electron for electron in leptons if
                         self.testLegKine(electron, ptcut=10, etacut=2.5) and
                         self.testVertex(electron) and
                         electron.cutBasedId('POG_PHYS14_25ns_v1_Veto') and
                         electron.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]

        # count tight muons
        vLeptons = [muon for muon in otherLeptons if
                    muon.muonID('POG_ID_Medium') and
                    self.testVertex(muon) and
                    self.testLegKine(muon, ptcut=10, etacut=2.4) and
                    muon.relIso(dBetaFactor=0.5, allCharged=0) < 0.3]

        if len(vLeptons) + len(vOtherLeptons) > 1:
            return False

        return True

    def testElectronID(self, electron):
        mva = electron.mvaRun2('NonTrigPhys14')
        eta = abs(electron.superCluster().eta())

        if eta < 0.8:
            return mva > 0.965
        elif eta < 1.479:
            return mva > 0.917
        return mva > 0.683


    def leptonAccept(self, leptons, event):
        '''The di-lepton veto, returns false if > one lepton.
        e.g. > 1 mu in the mu tau channel.
        To be implemented.'''
        return True

    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (1st precedence opposite-sign,
        2nd precedence highest pt1 + pt2).'''
        osDiLeptons = [dl for dl in diLeptons if dl.leg1().charge() != dl.leg2().charge()]
        if osDiLeptons : return max( osDiLeptons, key=operator.methodcaller( 'sumPt' ) )
        else           : return max(   diLeptons, key=operator.methodcaller( 'sumPt' ) )


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


            if self.cfg_ana.verbose:
                print '[DBG] HLT_path = ', info.name

            matchedIds = set()
            allMatched = True
            for to in info.objects:

                if self.cfg_ana.verbose:
                    print '[DBG] \t match =', self.trigObjMatched(to, legs)

                    for ipath in to.pathNames(True, True):
                        print '[DBG] \t\t pathNames(True, True) = ', ipath

                    for ipath in to.pathNames(True, False):
                        print '[DBG] \t\t pathNames(True, False) = ', ipath

                    for ipath in to.pathNames(False, True):
                        print '[DBG] \t\t pathNames(False, True) = ', ipath

                    for ipath in to.pathNames(False, False):
                        print '[DBG] \t\t pathNames(False, False) = ', ipath

                    for ipath in to.filterLabels():
                        print '[DBG] \t\t filter name = ', ipath


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
        
        Mu23_flag = any([mp.find('Mu23')!=-1 for mp in event.matchedPaths])
        Ele23_flag = any([mp.find('Ele23')!=-1 for mp in event.matchedPaths])

#        print event.matchedPaths, diL.leg1().pt(), diL.leg2().pt(), Mu23_flag, Ele23_flag, matched

        if all([Mu23_flag, Ele23_flag]): 
#            print '--> all', matched and (diL.leg1().pt() > 24 or diL.leg2().pt() > 24)
            return matched and (diL.leg1().pt() > 24 or diL.leg2().pt() > 24)
        elif Ele23_flag and not Mu23_flag:
#            print '--> e-only', matched and diL.leg1().pt() > 24
            return matched and diL.leg1().pt() > 24
        elif Mu23_flag and not Ele23_flag:
#            print '--> mu-only', matched and diL.leg2().pt() > 24
            return matched and diL.leg2().pt() > 24
        else:
            print 'This maens, no trigger fired'
            return matched
