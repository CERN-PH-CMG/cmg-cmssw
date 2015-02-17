from PhysicsTools.HeppyCore.utils.deltar        import matchObjectCollection
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.utils.deltar        import bestMatch

class DYJetsFakeAnalyzer( Analyzer ):
  '''Checks which kind of DYJet of Higgs event this is.
  isFake gets written to the event.
  - Z->tau tau : isFake = 0
  - Z->tau tau with matched rec hadr. tau->l: isFake = 3
  - Z->l l matched : isFake = 1
  - other : isFake = 2 
  set the lepton type as leptonType in the configuration.
  In case of VH events, only the Higgs is considered.
  '''

#   def __init__(self, event) :
#     super(DYJetsFakeAnalyzer, self).__init__(event)
#     ## flag to split DYJets into ZTT, ZL, ZJ
  
  def process(self, event) :

    event.isZtt     = False
    event.isZmt     = False
    event.isZet     = False
    event.isZee     = False
    event.isZmm     = False
    event.isZem     = False
    event.isZEE     = False
    event.isZMM     = False
    event.isZLL     = False    
    event.isFake    = -99
    event.genMass   = -99.
    event.genMet    = -99.
    event.genMex    = -99.
    event.genMey    = -99.
    event.genMetPhi = -99.
    event.hasZ      = False
    event.hasW      = False

    # gen MET as sum of the neutrino 4-momenta
    neutrinos = [ part for part in event.genParticles if abs(part.pdgId()) in (12,14,16) ]
    if len(neutrinos) == 0 : pass
    else :
      genmet = neutrinos[0].p4()
      for nu in neutrinos[1:] :
        genmet += nu.p4()
      event.genMet    = genmet.pt()
      event.genMex    = genmet.px()
      event.genMey    = genmet.py()
      event.genMetPhi = genmet.phi()
       
    if   'Higgs' in self.cfg_comp.name : theZs = [ bos for bos in event.genHiggsBosons if     bos.pdgId() in (25, 35, 36, 37)  ]
    elif 'DY'    in self.cfg_comp.name : theZs = [ bos for bos in event.genVBosons     if     bos.pdgId()  == 23               ]
    elif 'W'     in self.cfg_comp.name : theZs = [ bos for bos in event.genVBosons     if abs(bos.pdgId()) == 24               ]
    else                               : return True

    # there must always be a Z or a H boson
    # should raise an error too FIXME
    if len(theZs) != 1 :
      print 'I cannot find any H, W or Z in the sample!' 
      return False
    
    event.parentBoson = theZs[0]
    
    # check SM H associated production
    if event.parentBoson.pdgId() == 25 :
      if any( [    bos.pdgId  == 23 for bos in event.genVBosons] ) : event.hasZ = True
      if any( [abs(bos.pdgId) == 24 for bos in event.genVBosons] ) : event.hasW = True
      
    # gen mass of the Higgs or Z boson
    event.genMass = event.parentBoson.mass()
    
    ptcut = 0.
    # you can apply a pt cut on the gen leptons, electrons and muons
    # in HIG-13-004 it was 8 GeV
    if hasattr(self.cfg_ana, 'genPtCut') :
      ptcut = self.cfg_ana.genPtCut
    
    self.ptSelGentauleps = [ lep for lep in event.gentauleps if lep.pt() > ptcut ]
    self.ptSelGenleps    = [ lep for lep in event.genleps    if lep.pt() > ptcut ]
    # self.ptSelGentaus    = [ lep for lep in event.gentaus    if lep.pt() > ptcut ] # not needed

    self.l1 = event.diLepton.leg1()
    self.l2 = event.diLepton.leg2()

    self.l1.isTauHad    = False
    self.l1.isTauLep    = False
    self.l1.isPromptLep = False

    self.l2.isTauHad    = False
    self.l2.isTauLep    = False
    self.l2.isPromptLep = False

    self.genMatch(event)

    # move on if this is a W sample
    if abs(event.parentBoson.pdgId()) == 24 : return True
        
    self.getGenType(event)
    
    if self.cfg_ana.channel == 'tt' : self.isFakeTauTau(event)
    if self.cfg_ana.channel == 'et' : self.isFakeETau  (event)
    if self.cfg_ana.channel == 'mt' : self.isFakeMuTau (event)
    if self.cfg_ana.channel == 'em' : self.isFakeEMu   (event)

  def genMatch(self, event, dR = 0.3) :
    
    dR2 = dR * dR
    
    # match the tau_h leg
    # to generated had taus
    l1match, dR2best = bestMatch(self.l1, event.gentaus)
    if dR2best < dR2 :
      self.l1.genp = l1match
      self.l1.isTauHad = True

    # to generated leptons from taus
    if not self.l1.isTauHad :
      l1match, dR2best = bestMatch(self.l1, self.ptSelGentauleps)
      if dR2best < dR2 :
        self.l1.genp = l1match
        self.l1.isTauLep = True

    # to generated prompt leptons
    elif not self.l1.isTauLep :
      l1match, dR2best = bestMatch(self.l1, self.ptSelGenleps)
      if dR2best < dR2 :
        self.l1.genp = l1match
        self.l1.isPromptLep = True

    # match with any other gen particle
    elif not self.l1.isPromptLep :
      l1match, dR2best = bestMatch(self.l1, event.genParticles)
      if dR2best < dR2 :
        self.l1.genp = l1match
    
    
    # match the mu leg
    # to generated had taus
    l2match, dR2best = bestMatch(self.l2, event.gentaus)
    if dR2best < dR2 :
      self.l2.genp = l2match
      self.l2.isTauHad = True
        
    # to generated leptons from taus
    if not self.l2.isTauHad :
      l2match, dR2best = bestMatch(self.l2, self.ptSelGentauleps)
      if dR2best < dR2 :
        self.l2.genp = l2match
        self.l2.isTauLep = True

    # to generated prompt leptons
    elif not self.l2.isTauLep :
      l2match, dR2best = bestMatch(self.l2, self.ptSelGenleps)
      if dR2best < dR2 :
        self.l2.genp = l2match
        self.l2.isPromptLep = True

    # match with any other gen particle
    elif not self.l2.isPromptLep :
      l2match, dR2best = bestMatch(self.l2, event.genParticles)
      if dR2best < dR2 :
        self.l2.genp = l2match
    
  def getGenType(self, event) :
    '''Check the Z or H boson decay mode at gen level.
       Save a bunch of flags in the event.
       event.isZtt : Z/H -> tautau, fully hadronic
       event.isZmt : Z/H -> tautau, mutau
       event.isZet : Z/H -> tautau, etau
       event.isZee : Z/H -> tautau, ee
       event.isZmm : Z/H -> tautau, mm
       event.isZem : Z/H -> tautau, em
       event.isZEE : Z/H -> ee
       event.isZMM : Z/H -> mm
       event.isZLL : Z/H -> ll (ee or mm)
    ''' 
    # Z->TT
    # full hadronic first 
    if   len(event.gentauleps) == 0 and len(event.gentaus) == 2 : event.isZtt = True
    # semi leptonic 
    elif len(event.gentauleps) == 1 and len(event.gentaus) == 1 : 
      if abs(event.gentauleps[0].pdgId()) == 11 : event.isZet = True
      if abs(event.gentauleps[0].pdgId()) == 13 : event.isZmt = True
    # fully leptonic 
    elif len(event.gentauleps) == 2 and len(event.gentaus) == 0 :
      if   abs(event.gentauleps[0].pdgId()) == 11 and abs(event.gentauleps[1].pdgId()) == 11 : event.isZee = True
      elif abs(event.gentauleps[0].pdgId()) == 13 and abs(event.gentauleps[1].pdgId()) == 13 : event.isZmm = True
      else                                                                                   : event.isZem = True
    # Z->LL
    elif len(event.genleps) == 2 : 
      if   abs(event.genleps[0].pdgId()) == 11 and abs(event.genleps[1].pdgId()) == 11 : event.isZEE = True
      elif abs(event.genleps[0].pdgId()) == 13 and abs(event.genleps[1].pdgId()) == 13 : event.isZMM = True
    # should raise an error too FIXME
    else : return False
    
    event.isZLL = event.isZEE or event.isZMM

  def isFakeMuTau(self, event) :
    '''Define the criteria to label a given mt ZTT event as fake''' 
    if   self.l1.isTauHad    and self.l2.isTauLep    and event.isZmt : event.isFake = 0
    elif self.l1.isPromptLep and self.l2.isPromptLep and event.isZLL : event.isFake = 1
    elif self.l1.isTauLep    and self.l2.isTauLep                    : event.isFake = 3
    else                                                             : event.isFake = 2
  
  def isFakeETau(self, event) :
    '''Define the criteria to label a given et ZTT event as fake''' 
    if   self.l1.isTauHad    and self.l2.isTauLep    and event.isZet : event.isFake = 0
    elif self.l1.isPromptLep and self.l2.isPromptLep and event.isZLL : event.isFake = 1
    elif self.l1.isTauLep    and self.l2.isTauLep                    : event.isFake = 3
    else                                                             : event.isFake = 2

  def isFakeEMu(self, event) :
    '''Define the criteria to label a given em ZTT event as fake.
       RIC: TO BE PROPERLY DEFINED FIXME!
    ''' 
    if   self.l1.isTauLep    and self.l2.isTauLep    and event.isZem : event.isFake = 0
    elif self.l1.isPromptLep and self.l2.isPromptLep and event.isZLL : event.isFake = 1
    elif self.l1.isTauHad    and self.l2.isTauLep                    : event.isFake = 3
    else                                                             : event.isFake = 2

  def isFakeTauTau(self, event) :
    '''Define the criteria to label a given tt ZTT event as fake
       RIC: TO BE PROPERLY DEFINED FIXME!
    ''' 
    if   self.l1.isTauHad    and self.l2.isTauHad    and event.isZtt : event.isFake = 0
    elif self.l1.isPromptLep and self.l2.isPromptLep and event.isZLL : event.isFake = 1
    elif self.l1.isTauLep    and self.l2.isTauLep                    : event.isFake = 3
    else                                                             : event.isFake = 2
