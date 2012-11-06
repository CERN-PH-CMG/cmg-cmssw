from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauMuon
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, GenParticle
from CMGTools.RootTools.physicsobjects.HTauTauElectron import HTauTauElectron as Electron

class TauMuAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauMuon
    LeptonClass = Muon
    OtherLeptonClass = Electron

    def declareHandles(self):
        super(TauMuAnalyzer, self).declareHandles()
        # print 'TauMuAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgTauMuCorSVFitFullSel', # FIXME!!
            # 'cmgTauMu',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>'
            )

        if hasattr(self.cfg_ana, 'mvametsigs'):
            self.handles['mvametsigs'] = AutoHandle(
                self.cfg_ana.mvametsigs, # 'mvaMETTauMu'
                'std::vector<cmg::METSignificance>'
                )

        self.handles['otherLeptons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        
        self.handles['leptons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
        
        # FIXME reading the genparticlespruned collection. problem elsewhere?
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

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
            if not self.testLeg2( pydil.leg2(), 99999 ):
                continue
            if hasattr(self.cfg_ana, 'mvametsigs'):
                pydil.mvaMetSig = mvaMetSig = self.handles['mvametsigs'].product()[index]
            diLeptons.append( pydil )
        return diLeptons


    def buildLeptons(self, cmgLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = self.__class__.LeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            if not self.testMuonIDLoose( pyl ):
                continue
            leptons.append( pyl )
        return leptons


    def buildOtherLeptons(self, cmgOtherLeptons, event):
        '''Build electrons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(cmgOtherLeptons):
            pyl = self.__class__.OtherLeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            #COLINTLV check ID 
            # if not self.testMuonIDLoose(pyl):
            #    continue
            otherLeptons.append( pyl )
        return otherLeptons


    def process(self, iEvent, event):


        #if event.eventId == 70370:
        #    print 'EVENT', event.eventId
        #    import pdb; pdb.set_trace()
        result = super(TauMuAnalyzer, self).process(iEvent, event)
        if self.cfg_ana.verbose and result is False:
        # if 1:
            print 'EventReport', event.run, event.lumi, event.eventId
            import pdb; pdb.set_trace()
        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # and di-lepton veto must pass
            result = self.selectionSequence(event, fillCounter=False,
                                            leg1IsoCut = -9999,
                                            leg2IsoCut = 9999)
            if result is False:
                # really no way to find a suitable di-lepton,
                # even in the control region
                return False
            event.isSignal = False
        else:
            event.isSignal = True
       
        event.genMatched = None
        if self.cfg_comp.isMC:
            # print event.eventId
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                event.genMatched = True
            else:
                event.genMatched = False
                
        return True
        

    def testLeg1ID(self, tau):
        if tau.decayMode() == 0 and \
               tau.calcEOverP() < 0.2: #reject muons faking taus in 2011B
            return False
        return tau.tauID("againstMuonTight")>0.5 and \
               tau.tauID("againstElectronLoose")>0.5 and \
               self.testVertex( tau )
        

    def testLeg1Iso(self, tau, isocut):
        '''if isocut is None, returns true if loose iso MVA is passed.
        Otherwise, returns true if iso MVA > isocut.'''
        if isocut is None:
            return tau.tauID("byLooseIsoMVA")>0.5
        else:
            return tau.tauID("byRawIsoMVA")>isocut


    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and tau'''
        return abs(lepton.dxy()) < 0.045 and \
               abs(lepton.dz()) < 0.2 


    def testLeg2ID(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.tightId() and \
               self.testVertex( muon )
               

    def testLeg2Iso(self, muon, isocut):
        '''Tight muon selection, with isolation requirement'''
        if isocut is None:
            isocut = self.cfg_ana.iso2
        return self.muonIso(muon)<isocut


    def muonIso(self, muon ):
        '''dbeta corrected pf isolation with all charged particles instead of
        charged hadrons'''
        return muon.relIsoAllChargedDB05()
    

    def testMuonIDLoose(self, muon):
        '''Loose muon ID and kine, no isolation requirement, for lepton veto'''        
        return muon.pt() > 15 and \
               abs( muon.eta() ) < 2.4 and \
               muon.isGlobalMuon() and \
               muon.isTrackerMuon() and \
               muon.sourcePtr().userFloat('isPFMuon') and \
               abs(muon.dz()) < 0.2
               # self.testVertex( muon ) 
            
    def testMuonLoose( self, muon ):
        '''Loose muon selection, with isolation requirement (for di-lepton veto)'''
        #COLIN: not sure the vertex constraints should be kept 
        return self.testMuonIDLoose(muon) and \
               self.muonIso(muon)<0.3


    def testOtherLepton(self, electron, isoCut):
        '''Electron selection for tri-lepton veto'''
        return self.testLegKine(electron, ptcut=10, etacut=2.5) and \
               electron.looseIdForTriLeptonVeto()           and \
               self.testVertex( electron )           and \
               electron.relIsoAllChargedDB05() < isoCut
    

    def thirdLeptonVeto(self, leptons, otherLeptons, isoCut = 0.3) :
        # count muons
        leptons = [lep for lep in leptons if
                   self.testLegKine(lep, ptcut=10, etacut=2.4) and 
                   self.testLeg2ID(lep) and
                   self.testLeg2Iso(lep, isoCut) ]
        # count electrons
        otherLeptons = [olep for olep in otherLeptons
                        if self.testOtherLepton(olep, isoCut)]
        if len(leptons) + len(otherLeptons)> 1:
            # import pdb; pdb.set_trace()
##             print '-'*100
##             print 'muons'
##             print map(str, leptons)
##             print 'electrons'
##             print map(str, otherLeptons)    
            return False
        else:
            return True
        

    def leptonAccept(self, leptons):
        looseLeptons = set(filter( self.testMuonLoose, leptons ))
        isPlus = False
        isMinus = False
        for lepton in looseLeptons:
            if lepton.charge()<0: isMinus=True
            elif lepton.charge()>0: isPlus=True
            else:
                raise ValueError('Impossible!')
        veto = isMinus and isPlus
        return not veto
