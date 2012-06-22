from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauElectron
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Electron, GenParticle
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

        self.handles['mvametsigs'] = AutoHandle(
            'mvaMETTauEle',
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
#            print 'TEST tau iso',pydil.leg1().tauID('byRawIsoMVA')
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg2().associatedVertex = event.goodVertices[0]
#FIXME how I could work w/o the following line it's mistery 
#            pydil.mvaMetSig = mvaMetSig = self.handles['mvametsigs'].product()[index]
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
            if not self.testEleLoosePhil( pyl ):
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

#        self.bestVertex = event.goodVertices[0]
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
        """Returns True if a tau passes a set of cuts.
        
        contains the selections of the sync exercise of 17/05/12
        """
        if tau.decayMode() == 0 and \
               tau.calcEOverP() < 0.2: #reject muons faking taus in 2011B #PG FIXME should I put this in?
            return False
        if abs (tau.dz())  > 0.2   : return False
        if abs (tau.dxy()) > 0.045 : return False
        return tau.tauID("againstElectronMVA")    == True and \
               tau.tauID("againstElectronMedium") == True and \
               tau.tauID("againstMuonLoose")      == True

               #tau.tauID("byLooseIsoMVA")         == True and \
        # byLooseCombinedIsolationDeltaBetaCorr


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg1Iso(self, tau, isocut):
        '''if isocut is None, returns true if loose iso MVA is passed. <-- PG not now, why so?
        Otherwise, returns true if iso MVA > isocut.'''
#        print 'TEST tau iso',isocut
        if isocut is None:
            return tau.tauID("byRawIsoMVA")>self.cfg_ana.iso1
#        if isocut is None:   ##PG why this?
#            return tau.tauID("byLooseIsoMVA")>0.5
        else:
            return tau.tauID("byRawIsoMVA")>isocut


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg2ID(self, leg):
        leg.tightIdResult = self.testElectronTwiki_2012(leg) 
        if abs (leg.dxy())  >= 0.045                                : return False
        if abs (leg.dz())   >= 0.2                                  : return False
        if leg.pt ()        <= self.cfg_ana.pt2                     : return False # FIXME should be in kine
        if abs( leg.eta())  >= self.cfg_ana.eta2                    : return False # FIXME should be in kine
        if not self.testEleLoosePhil (leg, self.cfg_ana.pt2, 99999) : return False
        if not leg.tightIdResult                                    : return False
        return True


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testLeg2Iso(self, leg, isocut):
        if isocut is None:
           isocut = self.cfg_ana.iso2
        return leg.relIsoAllChargedDB05() < isocut


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testElectronTwiki_2012(self, leg):
        """reference numbers form the Htautau twiki, to be updated
        
        https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection
        """
        eta = abs( leg.eta() )
        if eta > 2.1 : return False
        lmvaID = -99999 # identification
        if leg.pt() < 20 :
            if   eta<0.8:   
                lmvaID = 0.925
            elif eta<1.479: 
                lmvaID = 0.915
            else :          
                lmvaID = 0.965
        else:
            if   eta<0.8:   
                lmvaID = 0.925
            elif eta<1.479: 
                lmvaID = 0.975
            else :          
                lmvaID = 0.985
        result = leg.mvaNonTrigV0()  > lmvaID
        return result

# 30.05.12, from Lorenzo
# loose, pt > 20: {0.925, 0.975, 0.985}
# tight, pt > 20: {0.925, 0.985, 0.985}
# 13.06.12, numbers from the twiki


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def testEleLoosePhil( self, ele, ptCut = 15, isoCut = 0.3 ):
        """Loose electron selection, for the lepton veto, 
        
        according to Phil sync prescription for the sync exercise 16/05/12
        """
        nInnerHits = ele.numberOfHits()
        if nInnerHits != 0 : return False
        if ele.passConversionVeto() == False   : return False 
        if ele.pt()                   < ptCut  : return False
        if ele.relIsoAllChargedDB05() > isoCut : return False
        if abs(ele.dxy())             >= 0.045 : return False
        if abs(ele.dz())              >= 0.2   : return False
        hoe = ele.hadronicOverEm()
        deta = ele.deltaEtaSuperClusterTrackAtVtx()
        dphi = ele.deltaPhiSuperClusterTrackAtVtx()
        sihih = ele.sigmaIetaIeta() 
        if ele.sourcePtr().isEB() :
            if sihih >= 0.010     : return False
            if dphi  >= 0.80      : return False 
            if deta  >= 0.007     : return False
            if hoe   >= 0.15      : return False
        elif ele.sourcePtr().isEE() :
            if sihih >= 0.030     : return False
            if dphi  >= 0.70      : return False 
            if deta  >= 0.010     : return False
#            if hoe   >= 0.07      : return False
        else : return False #PG is this correct? does this take cracks into consideration?
        return True


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


    def leptonAccept(self, leptons, isoCut = 0.3) :
        ''' returns True if the additional lepton veto is successful'''
        #PG FIXME how do I pass the isolation argument to testEleLoosePhil?
        looseLeptons = filter( self.testEleLoosePhil, leptons)
        nLeptons = len(looseLeptons)
        if nLeptons < 2 : return True
        if nLeptons > 2 : return False
        if looseLeptons[0].charge() == looseLeptons[1].charge() : return True
        if deltaR (looseLeptons[0].eta(), looseLeptons[0].phi(), looseLeptons[1].eta(), looseLeptons[1].phi()) < 0.15 : return True 
        else : return False
    
