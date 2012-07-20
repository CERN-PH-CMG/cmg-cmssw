from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauElectron
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.physicsobjects.HTauTauElectron import HTauTauElectron as Electron

from CMGTools.RootTools.utils.DeltaR import deltaR

class TauEleAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauElectron
    LeptonClass = Electron

    def declareHandles(self):
        super(TauEleAnalyzer, self).declareHandles()
        # print 'TauEleAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgTauEleCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Electron>>'
            )

        if hasattr(self.cfg_ana, 'mvametsigs'):
            self.handles['mvametsigs'] = AutoHandle(
                self.cfg_ana.mvametsigs, # 'mvaMETTauMu'
                'std::vector<cmg::METSignificance>'
                )
        
        self.handles['leptons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


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
            if not self.testLeg2( pydil.leg2(), 999999 ):
                continue
            if hasattr(self.cfg_ana, 'mvametsigs'):
                pydil.mvaMetSig = mvaMetSig = self.handles['mvametsigs'].product()[index]
            diLeptons.append( pydil )
        return diLeptons


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def buildLeptons(self, cmgLeptons, event):
        '''Build electrons for veto, associate best vertex, select loose ID electrons.
        Since the electrons are used for veto, the 0.3 default isolation cut is left there, 
        as well as the pt 15 gev cut'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = self.__class__.LeptonClass(lep)
            pyl.associatedVertex = event.goodVertices[0]
            if not pyl.looseIdForEleTau():
                continue
            leptons.append( pyl )
        return leptons


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def process(self, iEvent, event):
        
        #        import pdb; pdb.set_trace()
        #        if event.eventId == 20990 : 
        #            print 'STOPPING'
        #            import pdb
        #            pdb.set_trace()
        
        result = super(TauEleAnalyzer, self).process(iEvent, event)

        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # and di-lepton veto must pass
            # i.e. only the iso requirement is relaxed
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

#        for dil in event.diLeptons :
#            print 'TEST tau iso',result,dil.leg1().tauID('byRawIsoMVA')

        return True


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg1ID(self, tau):
        return tau.tauID("againstElectronMVA") >0.5 and \
               tau.tauID("againstElectronMedium") > 0.5 and \
               tau.tauID("againstMuonLoose")>0.5 and \
               self.testVertex( tau )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and tau'''
        return abs(lepton.dxy()) < 0.045 and \
               abs(lepton.dz()) < 0.2 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg1Iso(self, tau, isocut):
        '''if isocut is None, returns true if loose iso MVA is passed.
        Otherwise, returns true if iso MVA > isocut.'''
        if isocut is None:
            return tau.tauID("byLooseIsoMVA")>0.5
        else:
            return tau.tauID("byRawIsoMVA")>isocut


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg2ID(self, electron):
        '''Tight muon selection, no isolation requirement'''
#        print 'WARNING: USING SETUP FOR SYNC PURPOSES'
#        return electron.looseIdForEleTau() and \
        return electron.tightIdForEleTau() and \
               self.testVertex( electron )


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg2Iso(self, leg, isocut):
        if isocut is None:
           isocut = self.cfg_ana.iso2
        return leg.relIsoAllChargedDB05() < isocut


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLooseLeg2 (self, leg):
        if leg.relIsoAllChargedDB05() > 0.3 : return False
        if abs( leg.eta() )           > 2.5 : return False
        if leg.pt()                   < 15  : return False
        if leg.looseIdForEleTau()  == False : return False
        return True


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


#PG eleID parameters (implemented in Electron)
# 30.05.12, from Lorenzo
# loose, pt > 20: {0.925, 0.975, 0.985}
# tight, pt > 20: {0.925, 0.985, 0.985}
# 13.06.12, numbers from the twiki


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def leptonAccept(self, leptons, isoCut = 0.3) :
        ''' returns True if the additional lepton veto is successful'''
        #PG FIXME how do I pass the isolation argument to testLooseLeg2?
        looseLeptons = filter( self.testLooseLeg2, leptons)
        nLeptons = len(looseLeptons)
#        return nLeptons < 2 #PG according to AN 2012/150 of 12/07/2012
        if nLeptons < 2 :
            return True
        else :
            chargeSum = 0
            for i in range(len(looseLeptons)) : chargeSum = chargeSum + looseLeptons[i].charge()
            if abs(chargeSum) < len(looseLeptons) : return False
        return True

#        elif nLeptons > 2 :
#            return False
#        else: # 2 leptons
#            if looseLeptons[0].charge() == looseLeptons[1].charge() :
#                return True
#            elif deltaR (looseLeptons[0].eta(), looseLeptons[0].phi(), 
#                         looseLeptons[1].eta(), looseLeptons[1].phi()) < 0.15 :
#                return True 
#            else :
#                return False
    
