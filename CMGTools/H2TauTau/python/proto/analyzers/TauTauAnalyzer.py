from PhysicsTools.HeppyCore.statistics.counter              import Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle           import AutoHandle
from PhysicsTools.Heppy.analyzers.examples.DiLeptonAnalyzer import DiLeptonAnalyzer
from PhysicsTools.Heppy.physicsutils.DiObject               import TauTau ## RIC: what DiObject should we import? Heppy, HTauTau?
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects       import Tau, Muon, Jet, GenParticle
from PhysicsTools.Heppy.physicsobjects.HTauTauElectron      import HTauTauElectron as Electron

class TauTauAnalyzer( DiLeptonAnalyzer ) :

  DiObjectClass    = TauTau
  LeptonClass      = Muon 
  OtherLeptonClass = Electron 

  def declareHandles( self ) :
    super(TauTauAnalyzer, self).declareHandles()
    self.handles['diLeptons' ] = AutoHandle( 'cmgDiTauCorSVFitFullSel', 'vector<pat::CompositeCandidate>' ) 
    self.handles['electrons' ] = AutoHandle( 'slimmedElectronss'      , 'vector<pat::Electron>'           )
    self.handles['muons'     ] = AutoHandle( 'slimmedMuons'           , 'vector<pat::Muon>'               )
    self.handles['jets'      ] = AutoHandle( 'slimmedJets'            , 'vector<pat::Jet>'                )
    self.handles['mvametsigs'] = AutoHandle( 'mvaMETTauMu'            , 'vector<cmg::METSignificance>'    )
  
  def process( self, event ):
    
    # build di-tau pairs
    event.diLeptons = self.buildDiLeptons( self.handles['diLeptons'].product(), event )
        
    # method inherited from parent class DiLeptonAnalyzer
    # asks for at least on di-tau pair
    # applies the third lepton veto
    # tests leg1 and leg2
    # cleans by dR the two signal leptons
    # applies the trigger matching to the two signal leptons
    # choses the best di-tau pair, with the bestDiLepton method implemented here
    self.selectionSequence(event, fillCounter = True, 
                           leg1IsoCut = self.cfg_ana.isolation, 
                           leg2IsoCut = self.cfg_ana.isolation)
    
    # make sure that the legs are sorted by pt
    if event.leg1.pt < event.leg2.pt :
      event.leg1 = event.diLepton.leg2()
      event.leg2 = event.diLepton.leg1()
      event.selectedLeptons = [event.leg2, event.leg1]

    return True
      
  def bestDiLepton( self, diLeptons) :
    '''Returns the most isolated di-tau candidate:
       OS if present, else SS.'''
    iso = self.cfg_ana.isolation 
    myDiLeptons = [ dilep for dilep in diLeptons if dilep.leg1().charge() * dilep.leg1().charge() < 0 ]
    if len(myDiLeptons) < 1 : myDiLeptons = [ dilep for dilep in diLeptons ]     
    return sorted(myDiLeptons, key=lambda dilep: max(dilep.leg1().tauID(iso), \
                                                     dilep.leg2().tauID(iso)), reverse=True)[0]

  def muonVeto ( self, pt = 10., eta = 2.4, iso = 0.3 ) :
    event.muons = [ muon for muon in self.buildMuons(self.handles['muons'].product(),event)
                    if self.testLegKine(muon, ptcut=pt, etacut=eta) and 
                       self.testVertex(muon)                        and
                       self.testMuonID(muon)                        and
                       self.testMuonIso(muon, iso) ]
    return len(event.muons) == 0
                      
  def electronVeto ( self, pt = 10., eta = 2.4, iso = 0.3 ) :
    event.electrons = [ electron for electron in self.buildElectrons(self.handles['electrons'].product(),event)
                        if self.testLegKine(electron, ptcut=pt, etacut=eta) and
                           self.testVertex(electron)                        and
                           electron.looseIdForTriLeptonVeto()               and
                           electron.relIsoAllChargedDB05() < iso]
    return len(event.electrons) == 0

  def thirdLeptonVeto ( self ) :
    return self.muonVeto and self.electronVeto

  def testLeg( self, leg, leg_pt, leg_eta, iso, isocut ) :      
    '''requires loose isolation, pt, eta and minimal tauID cuts'''
    return ( self.testVertex(leg)                        and 
             leg.tauID(iso)                    < isocut  and 
             leg.pt()                          > leg_pt  and  
             abs(leg.eta())                    < leg_eta and 
             leg.tauID('decayModeFinding')     > 0.5     and 
             leg.tauID('againstElectronLoose') > 0.5     and 
             leg.tauID('againstMuonLoose')     > 0.5       )

  def testLeg1(self, leg) :
    leg_pt  = self.cfg_ana.pt1       
    leg_eta = self.cfg_ana.eta1      
    iso     = self.cfg_ana.isolation 
    isocut  = self.cfg_ana.iso1           
    return self.testLeg(leg, leg_pt, leg_eta, iso, isocut)    

  def testLeg2(self, leg) :
    leg_pt  = self.cfg_ana.pt2       
    leg_eta = self.cfg_ana.eta2      
    iso     = self.cfg_ana.isolation 
    isocut  = self.cfg_ana.iso2           
    return self.testLeg(leg, leg_pt, leg_eta, iso, isocut)    

  def muonIso( self, muon ) :
    '''dbeta corrected pf isolation with all charged particles instead of
    charged hadrons'''
    return muon.relIsoAllChargedDB05()

  def testMuonID( self, muon ) :
    '''Tight muon selection, no isolation requirement'''
    return muon.tightId()
             
  def testMuonIso( self, muon, isocut ):
    '''Tight muon selection, with isolation requirement'''
    return self.muonIso(muon)<isocut

  def testVertex( self, lepton, dxy = 0.045, dz = 0.2 ) :
    '''Tests vertex constraints, for mu, e and tau'''
    return abs(lepton.dxy()) < dxy and \
           abs(lepton.dz())  < dz 

  def testMuonIDLoose( self, muon ):
    '''Loose muon ID and kine, no isolation requirement, for lepton veto'''        
    return muon.isGlobalMuon()                    and \
           muon.isTrackerMuon()                   and \
           muon.sourcePtr().userFloat('isPFMuon')
        
  def buildMuons( self, cmgLeptons, event ):
    '''Build muons for veto, associate best vertex, select loose ID muons.
    The loose ID selection is done to ensure that the muon has an inner track.'''
    muons = []
    for index, lep in enumerate(cmgLeptons):
      pyl = Muon(lep)
      pyl.associatedVertex = event.goodVertices[0]
      if not self.testMuonIDLoose( pyl ) : continue
      muons.append( pyl )
    return muons

  def buildElectrons( self, cmgOtherLeptons, event ):
   '''Build electrons for third lepton veto, associate best vertex'''
   electrons = []
   for index, lep in enumerate(cmgOtherLeptons):
     pyl = Electron(lep)
     pyl.associatedVertex = event.goodVertices[0]
     electrons.append( pyl )
   return electrons

  def testJetID( self, jet ) :
    #jet.puJetIdPassed = jet.puJetId()
    jet.puJetIdPassed = jet.puJetId53X()
    jet.pfJetIdPassed = jet.looseJetId()
    #jet.pfJetIdPassed = jet.getSelection('cuts_looseJetId')
    if self.cfg_ana.relaxJetId : return True
    else                       : return jet.puJetIdPassed and jet.pfJetIdPassed
           
  def testJet( self, jet ) :
    # 2 is loose pile-up jet id
    return jet.pt()         > self.cfg_ana.jetPt  and \
           abs( jet.eta() ) < self.cfg_ana.jetEta and \
           self.testJetID(jet)
        
