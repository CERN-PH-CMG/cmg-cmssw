import operator

from PhysicsTools.Heppy.analyzers.core.AutoHandle       import AutoHandle
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects   import Muon, GenParticle
from PhysicsTools.Heppy.physicsobjects.HTauTauElectron  import HTauTauElectron as Electron

from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.physicsobjects.DiObject    import MuonElectron

class EMuAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass    = MuonElectron
    LeptonClass      = Muon
    OtherLeptonClass = Electron

    def declareHandles(self):
        super(EMuAnalyzer, self).declareHandles()
        self.handles  ['diLeptons'   ] = AutoHandle('cmgMuEleCorSVFitFullSel', 'std::vector<pat::CompositeCandidate>')
        self.handles  ['otherLeptons'] = AutoHandle('slimmedElectrons'       , 'std::vector<pat::Electron>'          )
        self.handles  ['leptons'     ] = AutoHandle('slimmedMuons'           , 'std::vector<pat::Muon>'              )
        self.mchandles['genParticles'] = AutoHandle('prunedGenParticles'     , 'std::vector<reco::GenParticle>'      )

    def buildDiLeptons(self, cmgDiLeptons, event):
        '''Build di-leptons, associate best vertex to both legs,
        select di-leptons with a tight ID muon.
        The tight ID selection is done so that dxy and dz can be computed
        (the muon must not be standalone).
        '''
        diLeptons = []
        for index, dil in enumerate(cmgDiLeptons):
            # pydil = self.__class__.DiObjectClass(dil)
            pydil = MuonElectron(dil)
            pydil.leg1().associatedVertex = event.goodVertices[0]
            pydil.leg2().associatedVertex = event.goodVertices[0]
            #if not self.testLeg2( pydil.leg2(), 99999 ):
            #    continue
            # pydil.mvaMetSig = pydil.met().getSignificanceMatrix()
            diLeptons.append( pydil )
        return diLeptons

    def buildLeptons(self, cmgLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            # pyl = self.__class__.LeptonClass(lep)
            pyl = Muon(lep)
            pyl.associatedVertex = event.goodVertices[0]
            leptons.append( pyl )
        return leptons

    def buildOtherLeptons(self, cmgOtherLeptons, event):
        '''Build electrons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(cmgOtherLeptons):
            # pyl = self.__class__.OtherLeptonClass(lep)
            pyl = Electron(lep)
            pyl.associatedVertex = event.goodVertices[0]
            pyl.rho = event.rho
            otherLeptons.append( pyl )
        return otherLeptons

    def process(self, event):

        result = super(EMuAnalyzer, self).process(event)
                
        if result is False:
            # trying to get a dilepton from the control region.
            # it must have well id'ed and trig matched legs,
            # di-lepton and tri-lepton veto must pass
            result = self.selectionSequence(event, fillCounter = False,
                                            leg1IsoCut = -9999,
                                            leg2IsoCut =  9999)
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
        # RIC: this leg is the muon, I believe
        # needs to be implemented here 
        return True
        
    def testLeg1Iso(self, tau, isocut):
        '''if isocut is None, returns true if three-hit iso cut is passed.
        Otherwise, returns true if iso MVA > isocut.'''
        # RIC: this leg is the muon, I believe
        # needs to be implemented here 
        return True

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and tau'''
        return abs(lepton.dxy()) < 0.045 and abs(lepton.dz ()) < 0.2 

    def testLeg2ID(self, muon):
        '''Electron ID. To be implemented'''
        # RIC: this leg is the electron, I believe
        # needs to be implemented here 
        return True
               
    def testLeg2Iso(self, muon, isocut):
        '''Electron ID. To be implemented'''
        # RIC: this leg is the electron, I believe
        # needs to be implemented here 
        return True

    def thirdLeptonVeto(self, leptons, otherLeptons, ptcut = 10, isocut = 0.3) :
        '''The tri-lepton veto. To be implemented'''
#         vleptons = [ lep for lep in leptons if self.testLegKine(lep, ptcut=ptcut, etacut=2.4) and 
#                                                self.testLeg2ID (lep)                          and
#                                                self.testLeg2Iso(lep, isocut)                      ]
#         # count electrons
#         votherLeptons = [ olep for olep in otherLeptons if self.testLegKine(olep, ptcut=ptcut, etacut=2.5) and
#                                                            olep.mvaIDLoose()                               and
#                                                            self.testVertex( olep )                         and
#                                                            olep.relIsoAllChargedDB05() < isocut                ]
#         if len(vleptons) + len(votherLeptons) > 0 : return False
#         else                                      : return True
        return True        

    def leptonAccept(self, leptons):
        '''The di-lepton veto, returns false if > one lepton.
        e.g. > 1 mu in the mu tau channel.
        To be implemented.'''
#         looseLeptons = [muon for muon in leptons if
#                         self.testLegKine(muon, ptcut=15, etacut=2.4) and
#                         muon.isGlobalMuon() and
#                         muon.isTrackerMuon() and
#                         muon.userFloat('isPFMuon') and
#                         #COLIN Not sure this vertex cut is ok... check emu overlap
#                         #self.testVertex(muon) and
#                         # JAN: no dxy cut
#                         abs(muon.dz()) < 0.2 and
#                         self.testLeg2Iso(muon, 0.3)
#                         ]
#         isPlus = False
#         isMinus = False
#         # import pdb; pdb.set_trace()
#         for lepton in looseLeptons:
#             if lepton.charge()<0: isMinus=True
#             elif lepton.charge()>0: isPlus=True
#             else:
#                 raise ValueError('Impossible!')
#         veto = isMinus and isPlus
#         return not veto
        return True
        
    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (1st precedence opposite-sign, 
        2nd precedence highest pt1 + pt2).'''
        osDiLeptons = [dl for dl in diLeptons if dl.leg1().charge() != dl.leg2().charge()]
        if osDiLeptons : return max( osDiLeptons, key=operator.methodcaller( 'sumPt' ) )
        else           : return max(   diLeptons, key=operator.methodcaller( 'sumPt' ) )

