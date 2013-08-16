import math
import re
from fnmatch import fnmatch as fnm
from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauTau
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Tau, Muon, GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2
from ROOT import TFile
from CMGTools.RootTools.physicsobjects.HTauTauElectron import HTauTauElectron as Electron
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counters

### TauID WP summer13
#           loose  medium tight
# old mva > 0.795, 0.884, 0.921  
# mva2    > 0.85 , 0.90 , 0.94
# 3hit    < 2    , 1    , 0.8

notPassed = []

class TauTauAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauTau
    LeptonClass = Tau

    def declareHandles(self):
        super(TauTauAnalyzer, self).declareHandles()

        #self.handles['diLeptons']      = AutoHandle( 'cmgDiTauCorSVFitFullSel'  , 'std::vector<cmg::DiObject<cmg::Tau,cmg::Tau> >'   ) ## old object
        self.handles['diLeptons']      = AutoHandle( 'cmgDiTauCorSVFitFullSel'  , 'std::vector<cmg::DiTauObject<cmg::Tau,cmg::Tau> >')  ## new object by Jan
        self.handles['rawMET']         = AutoHandle( 'cmgPFMETRaw'              , 'std::vector<cmg::BaseMET>'                        )
        self.handles['jets']           = AutoHandle( 'cmgPFJetSel'              , 'std::vector<cmg::PFJet>'                          )
        self.handles['electrons']      = AutoHandle( 'cmgElectronSel'           , 'std::vector<cmg::Electron>'                       )
        self.handles['muons']          = AutoHandle( 'cmgMuonSel'               , 'std::vector<cmg::Muon>'                           )
        self.handles['triggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults'                              )
        self.handles['metSig']         = AutoHandle( 'pfMetSignificance'        , 'cmg::METSignificance'                             )
        #self.handles['mvametsigs']     = AutoHandle( 'mvaMETDiTau'              , 'std::vector<cmg::METSignificance>'                )



        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name or "TTJets" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name and not "WW" in self.cfg_comp.name and not "WZ" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned','std::vector<reco::GenParticle>' )
        if self.cfg_comp.isMC and "QCD" in self.cfg_comp.name:
            self.mchandles['generator'] = AutoHandle( 'generator','GenEventInfoProduct' )
        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name and not "WW" in self.cfg_comp.name and not "WZ" in self.cfg_comp.name) :
            self.mchandles['source'] =  AutoHandle('source','LHEEventProduct')

	self.triggers = ['HLT_LooseIsoPFTau35_Trk20_Prong1_v6',
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET70_v6'        ,
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET75_v6'        ,
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2'  ,
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_v2'        ,
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6'        ,
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_Prong1_v6' ,
         ]

	self.triggersEmb = [
         'HLT_Mu17_Mu8_v16',
         'HLT_Mu17_Mu8_v17',
         'HLT_Mu17_Mu8_v18',
         'HLT_Mu17_Mu8_v19',
         'HLT_Mu17_Mu8_v21',
         'HLT_Mu17_Mu8_v21',
         'HLT_Mu17_Mu8_v22',
         ]

        self.triggers += self.triggersEmb
        
        ###### PT HIGGS REWEIGHTING
	if "HiggsGGH" in self.cfg_comp.name:
	    masspoint = re.findall(r"(\d{2,3})", self.cfg_comp.name)
	    masspoint = str(masspoint[0])
            self.higgsPtWeightFile          = TFile("$CMSSW_BASE/src/CMGTools/H2TauTau/data/weight_ptH_"+masspoint+"_8TeV.root")
            self.higgsPtWeightHistogramNom  = self.higgsPtWeightFile.Get("Nominal")
            self.higgsPtWeightHistogramUp   = self.higgsPtWeightFile.Get("Up")
            self.higgsPtWeightHistogramDown = self.higgsPtWeightFile.Get("Down")

    def beginLoop(self):
        super(TauTauAnalyzer,self).beginLoop()
        self.counters = Counters()
        self.counters.addCounter('DiLepton')
        count = self.counters.counter('DiLepton')
        count.register('all events')
        count.register('> 0 di-lepton')
        count.register('lepton accept')
        count.register('third lepton veto')
        count.register('leg1 trig matched')
        count.register('leg2 trig matched')
        count.register('jet trig matched')
        count.register('leg1 offline cuts passed')
        count.register('leg2 offline cuts passed')
        count.register('{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min, max = self.cfg_ana.m_max ))
        count.register('exactly 1 di-lepton')

    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (the one with best isolation).'''
        iso = self.cfg_ana.isolation
        if iso == 'mva'  : #### Tau ID MVA
          return max( [ (min(dilep.leg1().tauID("byRawIsoMVA"), dilep.leg2().tauID("byRawIsoMVA")), dilep) for dilep in diLeptons ] )[1]
        if iso == 'mva2' : #### Tau ID MVA2
          return max( [ (min(dilep.leg1().tauID("byIsolationMVA2raw"), dilep.leg2().tauID("byIsolationMVA2raw")), dilep) for dilep in diLeptons ] )[1]
        if iso == 'db3h' : #### Tau ID deltaBeta 3 hits
          return min( [ (max(dilep.leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits"), dilep.leg2().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")), dilep) for dilep in diLeptons ] )[1]
        
        #### highest sum pt pair
        #return max( [ (dilep.sumPt(), dilep) for dilep in diLeptons ] )[1]
    
    def process(self, iEvent, event):
        # select signal dileptons with all cuts on both legs

        self.readCollections( iEvent )
        # trigger stuff could be put in a separate analyzer
        # event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]

        #if event.eventId in notPassed :
        #  print 'before anything'
        #  import pdb ; pdb.set_trace()
          
        event.diLeptons = self.buildDiLeptons( self.handles['diLeptons'].product(), event )

        #event.leptons = self.buildLeptons( self.handles['leptons'].product(), event )
        event.leptons = []
	for diLepton in event.diLeptons:
          if not diLepton.leg1() in event.leptons:
            event.leptons += [diLepton.leg1()]
          if not diLepton.leg2() in event.leptons:
            event.leptons += [diLepton.leg2()]
        self.shiftEnergyScale(event)
 
	result = self.selectionSequence(event, fillCounter=True)
        
	event.rawMET = self.handles['rawMET'].product()
        
	triggerResults = self.handles['triggerResults'].product()
        triggerNames = iEvent._event.triggerNames(triggerResults)
	for trig in self.triggers:
	    index = triggerNames.triggerIndex(trig)
	    if index >= triggerNames.size():
                trigres = -1
	    else:
	        trigres = triggerResults.accept(index)
	    setattr(event,trig,trigres)

	  	  
        # select non signal dileptons with loose cuts
        if result is False:
	  selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
                           self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
	                   self.testLooseLeg( diL.leg1() ) and self.testLooseLeg( diL.leg2() ) and \
	                  (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]

          if len(selDiLeptons)==0:
              selDiLeptons = [ diL for diL in event.diLeptons if \
                               self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
	                       self.testLooseLeg( diL.leg1() ) and self.testLooseLeg( diL.leg2() ) and \
			       (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
          if len(selDiLeptons)==0:
            return False
          event.diLepton = self.bestDiLepton( selDiLeptons )
          event.leg1 = event.diLepton.leg1()
          event.leg2 = event.diLepton.leg2()
          event.isSignal = False
        else:
          event.isSignal = True

        #if event.eventId in notPassed :
        #  print 'diLeptons built, is Signal?', result
        #  import pdb ; pdb.set_trace()

        #if len([event.diLepton])>1 :
          #import pdb ; pdb.set_trace()
        event.leg1.NewLooseAntiEleMVA3 = self.passAntiEMVA(iCat = event.diLepton.leg1().tauID("againstElectronMVA3category"), raw = event.diLepton.leg1().tauID("againstElectronMVA3raw"), WP = 0 )
        event.leg2.NewLooseAntiEleMVA3 = self.passAntiEMVA(iCat = event.diLepton.leg2().tauID("againstElectronMVA3category"), raw = event.diLepton.leg2().tauID("againstElectronMVA3raw"), WP = 0 )
        
        # count muons
        event.muons = [lep for lep in self.buildMuons(self.handles['muons'].product(),event)
                       if self.testLegKine(lep, ptcut=10, etacut=2.4) and 
                          self.testLeg2ID(lep)                        and
                          self.testLeg2Iso(lep, 0.3) ]
        # count electrons
        event.electrons = [electron for electron in self.buildElectrons(self.handles['electrons'].product(),event)
                           if self.testLegKine(electron, ptcut=10, etacut=2.5) and
                              electron.looseIdForTriLeptonVeto()               and
                              self.testVertex( electron )                      and
                              electron.relIsoAllChargedDB05() < 0.3]
        ####### what ID for veto means
        ####### http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/RootTools/python/physicsobjects/HTauTauElectron.py?revision=1.10&view=markup

        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name and not "WW" in self.cfg_comp.name and not "WZ" in self.cfg_comp.name) :
          event.NUP = self.mchandles['source'].product().hepeup().NUP
	else:
          event.NUP = -1

        event.genMatched = None
        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name):
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            leg1DeltaR, leg2DeltaR = event.diLepton.match( event.genParticles ) 
	    event.leg1DeltaR=leg1DeltaR
            event.leg2DeltaR=leg2DeltaR
            if leg1DeltaR>-1 and leg1DeltaR < 0.1 and \
               leg2DeltaR>-1 and leg2DeltaR < 0.1:
                event.genMatched = True
            else:
                event.genMatched = False
            event.isPhoton=False
            event.isElectron=False
	    for gen in genParticles:
                if abs(gen.pdgId())==15 and abs(gen.mother().pdgId())==23 and (gen.mother().mass()<80 or gen.mother().mass()>100):
                    event.isPhoton=True
                if abs(gen.pdgId())==13 and abs(gen.mother().pdgId())==23:
                    event.isMuon=True
                if abs(gen.pdgId())==11 and abs(gen.mother().pdgId())==23:
                    event.isElectron=True
                if abs(gen.pdgId()) in [23, 25, 35, 36, 37]:
                    event.genMass=gen.mass()
                if abs(gen.pdgId()) in [23]:
                    event.hasW=True
                if abs(gen.pdgId()) in [24]:
                    event.hasZ=True
        if self.cfg_comp.isMC and "W"      in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            genTaus = []
            event.genMatched = False
            event.genMatchedElectron = False
            event.genMatchedMuon = False
            for gen in genParticles:
                if abs(gen.pdgId()) in [11,13,15] and abs(gen.mother().pdgId())==24: # W -> tau nu
                    genTaus.append( gen )
            if len(genTaus)>=1:
                dR2leg1Min, event.diLepton.leg1Gen = ( float('inf'), None)
                dR2leg2Min, event.diLepton.leg2Gen = ( float('inf'), None) 
                for genTau in genTaus:
                    dR2leg1 = deltaR2(event.diLepton.leg1().eta(), event.diLepton.leg1().phi(),
                                      genTau.eta(), genTau.phi() )
                    dR2leg2 = deltaR2(event.diLepton.leg2().eta(), event.diLepton.leg2().phi(),
                                      genTau.eta(), genTau.phi() )
                    if dR2leg1 <  dR2leg1Min:
                        dR2leg1Min, event.diLepton.leg1Gen = (dR2leg1, genTau)
                    if dR2leg2 <  dR2leg2Min:
                        dR2leg2Min, event.diLepton.leg2Gen = (dR2leg2, genTau)
                    leg1DeltaR = math.sqrt( dR2leg1Min )
                    leg2DeltaR = math.sqrt( dR2leg2Min )
                    if ((leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
                        (leg2DeltaR>-1 and leg2DeltaR < 0.1)) and abs(genTau.pdgId())==11:
                         event.genMatchedElectron = True
                    if ((leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
                        (leg2DeltaR>-1 and leg2DeltaR < 0.1)) and abs(genTau.pdgId())==13:
                         event.genMatchedMuon = True
                    if ((leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
                        (leg2DeltaR>-1 and leg2DeltaR < 0.1)) and abs(genTau.pdgId())==15:
                         event.genMatched = True
            event.isElectron=False
            event.isMuon=False
            event.isTau=False
	    for gen in genParticles:
                if abs(gen.pdgId())==11 and abs(gen.mother().pdgId())==24:
                    event.isElectron=True
                if abs(gen.pdgId())==13 and abs(gen.mother().pdgId())==24:
                    event.isMuon=True
                if abs(gen.pdgId())==15 and abs(gen.mother().pdgId())==24:
                    event.isTau=True
        if self.cfg_comp.isMC and "TTJets" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
            genTaus = []
            event.genMatched = 0
            event.genMatchedElectron = 0
            event.genMatchedMuon = 0
            for gen in genParticles:
                if abs(gen.pdgId()) in [11,13,15] and abs(gen.mother().pdgId())==24: # W -> tau nu
                    genTaus.append( gen )
            if len(genTaus)>=1:
                dR2leg1Min, event.diLepton.leg1Gen = ( float('inf'), None)
                dR2leg2Min, event.diLepton.leg2Gen = ( float('inf'), None) 
                for genTau in genTaus:
                    dR2leg1 = deltaR2(event.diLepton.leg1().eta(), event.diLepton.leg1().phi(),
                                      genTau.eta(), genTau.phi() )
                    dR2leg2 = deltaR2(event.diLepton.leg2().eta(), event.diLepton.leg2().phi(),
                                      genTau.eta(), genTau.phi() )
                    if dR2leg1 <  dR2leg1Min:
                        dR2leg1Min, event.diLepton.leg1Gen = (dR2leg1, genTau)
                    if dR2leg2 <  dR2leg2Min:
                        dR2leg2Min, event.diLepton.leg2Gen = (dR2leg2, genTau)
                    leg1DeltaR = math.sqrt( dR2leg1Min )
                    leg2DeltaR = math.sqrt( dR2leg2Min )
                    if (leg1DeltaR>-1 and leg1DeltaR < 0.1) and abs(genTau.pdgId())==11:
                        event.genMatchedElectron+=1
                    if (leg1DeltaR>-1 and leg1DeltaR < 0.1) and abs(genTau.pdgId())==13:
                        event.genMatchedMuon+=1
                    if (leg1DeltaR>-1 and leg1DeltaR < 0.1) and abs(genTau.pdgId())==15:
                        event.genMatched+=1
                    if (leg2DeltaR>-1 and leg2DeltaR < 0.1) and abs(genTau.pdgId())==11:
                        event.genMatchedElectron+=1
                    if (leg2DeltaR>-1 and leg2DeltaR < 0.1) and abs(genTau.pdgId())==13:
                        event.genMatchedMuon+=1
                    if (leg2DeltaR>-1 and leg2DeltaR < 0.1) and abs(genTau.pdgId())==15:
                        event.genMatched+=1
            event.isElectron=0
            event.isMuon=0
            event.isTau=0
	    for gen in genParticles:
                if abs(gen.pdgId())==11 and abs(gen.mother().pdgId())==24:
                    event.isElectron+=1
                if abs(gen.pdgId())==13 and abs(gen.mother().pdgId())==24:
                    event.isMuon+=1
                if abs(gen.pdgId())==15 and abs(gen.mother().pdgId())==24:
                    event.isTau+=1
        if self.cfg_comp.isMC and "QCD"    in self.cfg_comp.name:
            generator = self.mchandles['generator'].product()
            event.generatorWeight = generator.weight()
            event.eventWeight *= event.generatorWeight
		
        if self.cfg_comp.isMC and "HiggsGGH" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
	    higgsPt=-1
	    for gen in genParticles:
                if abs(gen.pdgId())==25:
		    higgsPt = gen.pt()
		    break
	    event.higgsPtWeightNom  = self.higgsPtWeightHistogramNom.GetBinContent(self.higgsPtWeightHistogramNom.FindBin(higgsPt))
	    event.higgsPtWeightUp   = self.higgsPtWeightHistogramUp.GetBinContent(self.higgsPtWeightHistogramUp.FindBin(higgsPt))
	    event.higgsPtWeightDown = self.higgsPtWeightHistogramDown.GetBinContent(self.higgsPtWeightHistogramDown.FindBin(higgsPt))
	else :
	  event.higgsPtWeightNom  = 1.
          event.higgsPtWeightUp   = 1.
          event.higgsPtWeightDown = 1.


        ## third lepton veto
        if len(event.muons)>0+len(event.electrons) > 0: ## FOR SYNCHING!!!
          return False

        if "DY" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name or self.cfg_comp.isEmbed:
          event.isRealTau = 1
        else :
          event.isRealTau = 0  
        
        if not hasattr(event,'NJetWeight') :
          event.NJetWeight = 1.
        
        #event.name = self.cfg_comp.name
        #import pdb ; pdb.set_trace()
        event.metsig    = self.handles['metSig'].product()

        return True

    def selectionSequence(self, event, fillCounter, leg1IsoCut=None, leg2IsoCut=None):

        if fillCounter: self.counters.counter('DiLepton').inc('all events')
            
        if len(event.diLeptons) == 0:
            return False
        if fillCounter: self.counters.counter('DiLepton').inc('> 0 di-lepton')

        # testing di-lepton itself
        selDiLeptons = event.diLeptons
        
        if not self.leptonAccept( event.leptons ):
            return False
        if fillCounter: self.counters.counter('DiLepton').inc('lepton accept')

        ## for embedded no hltPaths (for now)
        if not hasattr( event, 'hltPaths'):
            event.hltPaths = []

        matching = {}
        for trig in event.hltPaths :
          matching.update({trig:[-99,-99,-99]}) # {trigName:leg1,leg2,jet}
                
        event.diLeptonsTrigMatched = []
        
        for trig in event.hltPaths :
          if self.cfg_comp.isEmbed : continue   ## no matching for the embed
          selDiLeptons = event.diLeptons
          if len(self.cfg_comp.triggers)>0:
              # trigger matching leg1
              selDiLeptons = [diL for diL in selDiLeptons if self.trigMatched(event, diL.leg1(), 'leg1', trig)]
              if len(selDiLeptons) == 0:
                  matching[trig][0]=0
              else:
                  if fillCounter: self.counters.counter('DiLepton').inc('leg1 trig matched')
                  matching[trig][0]=1
  
          if len(self.cfg_comp.triggers)>0:
              # trigger matching leg2
              selDiLeptons = [diL for diL in selDiLeptons if self.trigMatched(event, diL.leg2(), 'leg2', trig)]
              if len(selDiLeptons) == 0:
                  matching[trig][1]=0
              else:
                  if fillCounter: self.counters.counter('DiLepton').inc('leg2 trig matched')
                  matching[trig][1]=1
  
          if len(self.cfg_comp.triggers)>0 and len(self.cfg_ana.triggerMap[ trig ])>2:
              # trigger matching jet
              cmgJets = self.handles['jets'].product()
	      jets=[]
              for cmgJet in cmgJets:
                  jet = Jet( cmgJet )
                  if self.testJet( jet ):
                      jets.append(jet)
  
	      selDiLeptonsNew=[]
	      for diL in selDiLeptons:
                  cleanJets, dummy = cleanObjectCollection( jets, masks = [ diL.leg1(), diL.leg2() ], deltaRMin = 0.5 )
                  if len(cleanJets)>0 and self.trigMatched(event, cleanJets[0], 'jet', trig):
                      selDiLeptonsNew += [diL]
	      selDiLeptons = selDiLeptonsNew
              if len(selDiLeptons) == 0:
                  matching[trig][2]=0
              else:
                  if fillCounter: self.counters.counter('DiLepton').inc('jet trig matched')
                  matching[trig][2]=1
              
              
          event.diLeptonsTrigMatched += selDiLeptons
        
        event.diLeptonsTrigMatched = set(event.diLeptonsTrigMatched)
        
        ### need unix style wild card to macth different trigger versions in data
        for trig in matching.keys() :
          if fnm(trig,'HLT_DoubleMediumIsoPFTau35_Trk*_eta2p1_v*') :
            event.l1TrigMatched_diTau = matching[trig][0]
            event.l2TrigMatched_diTau = matching[trig][1]      
          if fnm(trig,'HLT_DoubleMediumIsoPFTau*_Trk*_eta2p1_Jet30_v*') :
            event.l1TrigMatched_diTauJet  = matching[trig][0]
            event.l2TrigMatched_diTauJet  = matching[trig][1]
            event.jetTrigMatched_diTauJet = matching[trig][2]        
        
        # testing leg1
        selDiLeptons = [ diL for diL in selDiLeptons if self.testLeg( diL.leg1() ) ]
        if len(selDiLeptons) == 0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc('leg1 offline cuts passed')

        # testing leg2 
        selDiLeptons = [ diL for diL in selDiLeptons if self.testLeg( diL.leg2() ) ]
        if len(selDiLeptons) == 0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc('leg2 offline cuts passed')

 
        # mass cut 
        selDiLeptons = [ diL for diL in selDiLeptons if self.testMass(diL) ]
        if len(selDiLeptons)==0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc('{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min, max = self.cfg_ana.m_max ))

        # exactly one? 
        if len(selDiLeptons)==0:
            return False
        elif len(selDiLeptons)==1:
            if fillCounter: self.counters.counter('DiLepton').inc('exactly 1 di-lepton')
        
        event.diLepton = self.bestDiLepton( selDiLeptons )
        event.leg1 = event.diLepton.leg1()
        event.leg2 = event.diLepton.leg2()
        
        
        ### require trigger bit in Embedded RecHit
        if self.cfg_comp.isEmbed :
          if not event.hltPath : return False
        
        if "DY" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name or self.cfg_comp.isEmbed:
          if event.leg1.decayMode() == 0 :
            event.leg1.prongWeight = 0.88
          else :
            event.leg1.prongWeight = 1.
          if event.leg2.decayMode() == 0 :
            event.leg2.prongWeight = 0.88
          else :
            event.leg2.prongWeight = 1.
        #import pdb ; pdb.set_trace()
        return True
    
    def testLeg(self, leg):
        leg_pt  = self.cfg_ana.pt
        leg_eta = self.cfg_ana.eta
        iso     = self.cfg_ana.isolation
        
        if iso == 'mva'  : #### Tau ID MVA
          test_leg_iso = leg.tauID("byRawIsoMVA") > 0.5 # 0.884       
        if iso == 'mva2' : #### Tau ID MVA2
          test_leg_iso = leg.tauID("byIsolationMVA2raw") > 0.5 # 0.90
        if iso == 'db3h' : #### Tau ID deltaBeta 3 hits
          test_leg_iso = leg.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10. #15. 
          
        return (test_leg_iso                                 and \
                leg.pt()>leg_pt and abs(leg.eta()) < leg_eta and \
	        leg.tauID("decayModeFinding")      > 0.5     and \
	        leg.tauID("againstElectronLoose")  > -0.5     and \
	        leg.tauID("againstMuonLoose")      > -0.5)

    def testLooseLeg(self, leg):
        leg_pt  = self.cfg_ana.pt
        leg_eta = self.cfg_ana.eta
        iso     = self.cfg_ana.isolation
        
        if iso == 'mva'  : #### Tau ID MVA
          test_leg_iso = leg.tauID("byRawIsoMVA") > 0.5        
        if iso == 'mva2' : #### Tau ID MVA2
          test_leg_iso = leg.tauID("byIsolationMVA2raw") > 0.5
        if iso == 'db3h' : #### Tau ID deltaBeta 3 hits
          test_leg_iso = leg.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10. #15.
        
        return (test_leg_iso                                 and \
                leg.pt()>leg_pt and abs(leg.eta()) < leg_eta and \
	        leg.tauID("decayModeFinding")      > 0.5     and \
	        leg.tauID("againstElectronLoose")  > -0.5     and \
	        leg.tauID("againstMuonLoose")      > -0.5)

    def passAntiEMVA(self, iCat, raw, WP=0) :
    
      iCat = int(iCat)
      
      if iCat<0  : return False
      if iCat>15 : return True
    
      cutsLoose     = [0.835,0.831,0.849,0.859,0.873,0.823,0.85 ,0.855,0.816,0.861,0.862,0.847,0.893,0.82 ,0.845,0.851]
      cutsMedium    = [0.933,0.921,0.944,0.945,0.918,0.941,0.981,0.943,0.956,0.947,0.951,0.95 ,0.897,0.958,0.955,0.942]
      cutsTight     = [ 0.96,0.968,0.971,0.972,0.969,0.959,0.981,0.965,0.975,0.972,0.974,0.971,0.897,0.971,0.961,0.97 ]
      cutsVeryTight = [0.978,0.98 ,0.982,0.985,0.977,0.974,0.989,0.977,0.986,0.983,0.984,0.983,0.971,0.987,0.977,0.981]
      cut = 0
      
      if WP==0  : cut = cutsLoose[iCat]
      if WP==1  : cut = cutsMedium[iCat]
      if WP==2  : cut = cutsTight[iCat]
      if WP==3  : cut = cutsVeryTight[iCat]
      
      return (raw>cut)

    def muonIso(self, muon ):
        '''dbeta corrected pf isolation with all charged particles instead of
        charged hadrons'''
        return muon.relIsoAllChargedDB05()

    def testLeg2ID(self, muon):
        '''Tight muon selection, no isolation requirement'''
        return muon.tightId() and \
               self.testVertex( muon )
               
    def testLeg2Iso(self, muon, isocut):
        '''Tight muon selection, with isolation requirement'''
        if isocut is None:
            isocut = self.cfg_ana.iso2
        return self.muonIso(muon)<isocut

    def testVertex(self, lepton):
        '''Tests vertex constraints, for mu and tau'''
        return abs(lepton.dxy()) < 0.045 and \
               abs(lepton.dz()) < 0.2 

    def testMuonIDLoose(self, muon):
        '''Loose muon ID and kine, no isolation requirement, for lepton veto'''        
        return muon.pt() > 15 and \
               abs( muon.eta() ) < 2.4 and \
               muon.isGlobalMuon() and \
               muon.isTrackerMuon() and \
               muon.sourcePtr().userFloat('isPFMuon') and \
               abs(muon.dz()) < 0.2
            
    def buildMuons(self, cmgLeptons, event):
        '''Build muons for veto, associate best vertex, select loose ID muons.
        The loose ID selection is done to ensure that the muon has an inner track.'''
        leptons = []
        for index, lep in enumerate(cmgLeptons):
            pyl = Muon(lep)
            pyl.associatedVertex = event.goodVertices[0]
            if not self.testMuonIDLoose( pyl ):
                continue
            leptons.append( pyl )
        return leptons

    def buildElectrons(self, cmgOtherLeptons, event):
        '''Build electrons for third lepton veto, associate best vertex.
        '''
        otherLeptons = []
        for index, lep in enumerate(cmgOtherLeptons):
            pyl = Electron(lep)
            pyl.associatedVertex = event.goodVertices[0]
            #COLINTLV check ID 
            # if not self.testMuonIDLoose(pyl):
            #    continue
            otherLeptons.append( pyl )
        return otherLeptons

    def trigMatched(self, event, leg, legName, trigpath):
        '''Returns true if the leg is matched to a trigger object as defined in the
        triggerMap parameter'''
        if not hasattr( self.cfg_ana, 'triggerMap'):
            return True
        #path = event.hltPath this works if you have only one path
        path = trigpath
        #import pdb ; pdb.set_trace()
        triggerObjects = event.triggerObjects
        filters = self.cfg_ana.triggerMap[ path ]
        filter = None
        if   legName == 'leg1':
            filter = filters[0]
        elif legName == 'leg2':
            filter = filters[1]
        elif legName == 'jet':
            filter = filters[2]
        else:
            raise ValueError( 'legName should be leg1 or leg2, not {leg}'.format(
                leg=legName )  )
        # the default dR2Max value is 0.3^2
        pdgIds = None
        if len(filter) == 2:
            filter, pdgIds = filter[0], filter[1]
            
        return triggerMatched(leg, triggerObjects, path, filter,
                              dR2Max=0.5*0.5,dRMax=0.5,
                              pdgIds=pdgIds )

    def testJetID(self, jet):
        
        #jet.puJetIdPassed = jet.puJetId()
        jet.puJetIdPassed = jet.puJetId53X()
        jet.pfJetIdPassed = jet.looseJetId()

        #jet.pfJetIdPassed = jet.getSelection('cuts_looseJetId')

        if self.cfg_ana.relaxJetId:
            return True
        else:
            return jet.puJetIdPassed and jet.pfJetIdPassed
             
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt()         > self.cfg_ana.jetPt  and \
               abs( jet.eta() ) < self.cfg_ana.jetEta and \
               self.testJetID(jet)
               # jet.passPuJetId('full', 2)
