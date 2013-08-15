import math
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

class TauTauAnalyzer( DiLeptonAnalyzer ):

    DiObjectClass = TauTau
    LeptonClass = Tau

    def declareHandles(self):
        super(TauTauAnalyzer, self).declareHandles()
        # print 'TauTauAnalyzer.declareHandles'
        self.handles['diLeptons'] = AutoHandle(
            'cmgDiTauCorSVFitFullSel',
            'std::vector<cmg::DiObject<cmg::Tau,cmg::Tau>>'
            )
        self.handles['rawMET'] = AutoHandle( 'cmgPFMETRaw',
                                                     'std::vector<cmg::BaseMET>' )
        self.handles['triggerResults'] = AutoHandle( ('TriggerResults','','HLT'),
                                                     'edm::TriggerResults' )
	self.triggers=['HLT_LooseIsoPFTau35_Trk20_Prong1_v6',
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET70_v6',
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET75_v6',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_v2',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_Prong1_v6']

        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name or "TTJets" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
        if self.cfg_comp.isMC and "QCD" in self.cfg_comp.name:
            self.mchandles['generator'] = AutoHandle( 'generator',
                                                      'GenEventInfoProduct' )
#COLIN this collection does not exist
##         if self.cfg_comp.isEmbed:
##             self.handles['genParticles'] = AutoHandle( 'genParticles',
##                                                      'std::vector<reco::GenParticle>' )
	if "Higgsgg" in self.cfg_comp.name:
	    masspoint=self.cfg_comp.name[7:10]
            self.higgsPtWeightFile=TFile("$CMSSW_BASE/src/CMGTools/H2TauTau/data/weight_ptH_"+masspoint+".root")
            self.higgsPtWeightHistogram=self.higgsPtWeightFile.Get("powheg_weight/weight_hqt_fehipro_fit_"+masspoint)

        self.handles['electrons'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )
        
        self.handles['muons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )

        if self.cfg_comp.isMC and ("WJets" in self.cfg_comp.name or "W0Jets" in self.cfg_comp.name or "W1Jets" in self.cfg_comp.name or "W2Jets" in self.cfg_comp.name or "W3Jets" in self.cfg_comp.name or "W4Jets" in self.cfg_comp.name
	                        or "DYJets" in self.cfg_comp.name or "DY0Jets" in self.cfg_comp.name or "DY1Jets" in self.cfg_comp.name or "DY2Jets" in self.cfg_comp.name or "DY3Jets" in self.cfg_comp.name or "DY4Jets" in self.cfg_comp.name):
          self.mchandles['source'] =  AutoHandle(
            'source',
            'LHEEventProduct'
            )
        #self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol,
        #                                   'std::vector<cmg::PFJet>' )

        self.handles['jets'] = AutoHandle( 'cmgPFJetSel',
                                           'std::vector<cmg::PFJet>' )

    def beginLoop(self):
        super(TauTauAnalyzer,self).beginLoop()
        self.counters = Counters()
        self.counters.addCounter('DiLepton')
        count = self.counters.counter('DiLepton')
        count.register('all events')
        count.register('> 0 di-lepton')
        # count.register('di-lepton cut string ok')
        count.register('lepton accept')
        count.register('third lepton veto')
        count.register('leg1 trig matched')
        count.register('leg2 trig matched')
        count.register('jet trig matched')
        count.register('leg1 offline cuts passed')
        count.register('leg2 offline cuts passed')
        count.register('{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min,
                                                             max = self.cfg_ana.m_max ))
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
        
	event.rawMET=self.handles['rawMET'].product()
        
	triggerResults=self.handles['triggerResults'].product()
        triggerNames = iEvent._event.triggerNames(triggerResults)
	for trig in self.triggers:
	    index=triggerNames.triggerIndex(trig)
	    if index>=triggerNames.size():
                trigres=-1
	    else:
	        trigres=triggerResults.accept(index)
	    setattr(event,trig,trigres)

	
        # select non signal dileptons with loose cuts
        if result is False:
	  selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
                           self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
	                   self.testNonLeg( diL.leg1() ) and self.testNonLeg( diL.leg2() ) and \
	                  (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
          if len(selDiLeptons)==0:
              selDiLeptons = [ diL for diL in event.diLeptons if \
                               self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
	                       self.testNonLeg( diL.leg1() ) and self.testNonLeg( diL.leg2() ) and \
			       (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
          if len(selDiLeptons)==0:
            return False
          event.diLepton = self.bestDiLepton( selDiLeptons )
          event.leg1 = event.diLepton.leg1()
          event.leg2 = event.diLepton.leg2()
          event.isSignal = False
        else:
          event.isSignal = True


        # count muons
        event.muons = [lep for lep in self.buildMuons(self.handles['muons'].product(),event)
                       if self.testLegKine(lep, ptcut=10, etacut=2.4) and 
                       self.testLeg2ID(lep) and
                       self.testLeg2Iso(lep, 0.3) ]
        # count electrons
        event.electrons = [electron for electron in self.buildElectrons(self.handles['electrons'].product(),event)
                if self.testLegKine(electron, ptcut=10, etacut=2.5) and \
                   electron.looseIdForTriLeptonVeto()           and \
                   self.testVertex( electron )           and \
                   electron.relIsoAllChargedDB05() < 0.3]
        ####### what ID for veto means
        ####### http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/RootTools/python/physicsobjects/HTauTauElectron.py?revision=1.10&view=markup

        if self.cfg_comp.isMC and ("WJets" in self.cfg_comp.name or "W0Jets" in self.cfg_comp.name or "W1Jets" in self.cfg_comp.name or "W2Jets" in self.cfg_comp.name or "W3Jets" in self.cfg_comp.name or "W4Jets" in self.cfg_comp.name
	                        or "DYJets" in self.cfg_comp.name or "DY0Jets" in self.cfg_comp.name or "DY1Jets" in self.cfg_comp.name or "DY2Jets" in self.cfg_comp.name or "DY3Jets" in self.cfg_comp.name or "DY4Jets" in self.cfg_comp.name):
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
        if self.cfg_comp.isMC and "W" in self.cfg_comp.name:
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
		
        if self.cfg_comp.isMC and "Higgsgg" in self.cfg_comp.name:
            genParticles = self.mchandles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
	    higgsPt=-1
	    for gen in genParticles:
                if abs(gen.pdgId())==25:
		    higgsPt = gen.pt()
		    break
	    event.higgsPtWeight = self.higgsPtWeightHistogram.GetBinContent(self.higgsPtWeightHistogram.FindBin(higgsPt))
            #event.eventWeight *= event.higgsPtWeight
		
        if self.cfg_comp.isMC and "QCD" in self.cfg_comp.name:
            generator = self.mchandles['generator'].product()
            event.generatorWeight = generator.weight()
            event.eventWeight *= event.generatorWeight

        ## third lepton veto
        #if len(event.muons)+len(event.electrons) > 0:
          #import pdb ; pdb.set_trace()
          #if len(event.electrons) > 0 : print 'electron pt',event.electrons[0].pt()
          #if len(event.muons)     > 0 : print 'muon pt'    ,event.muons[0].pt()
          #return False
        
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

        matching = {}
        for trig in event.hltPaths :
                        # {trigName:leg1,leg2,jet}
          matching.update({trig:[-99,-99,-99]})
                
        event.diLeptonsTrigMatched = []
        
        for trig in event.hltPaths :
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
        
        for trig in matching.keys() :
          if fnm(trig,'HLT_DoubleMediumIsoPFTau*_Trk*_eta2p1_Jet30_v*') :
            event.l1TrigMatched_diTauJet  = matching[trig][0]
            event.l2TrigMatched_diTauJet  = matching[trig][1]
            event.jetTrigMatched_diTauJet = matching[trig][2]        
        
        # testing leg1
        selDiLeptons = [ diL for diL in selDiLeptons if \
                         self.testLeg1( diL.leg1(), leg1IsoCut ) ]
        if len(selDiLeptons) == 0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc('leg1 offline cuts passed')

        # testing leg2 
        selDiLeptons = [ diL for diL in selDiLeptons if \
                         self.testLeg2( diL.leg2(), leg2IsoCut ) ]
        if len(selDiLeptons) == 0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc('leg2 offline cuts passed')

        # mass cut 
        selDiLeptons = [ diL for diL in selDiLeptons if \
                         self.testMass(diL) ]
        if len(selDiLeptons)==0:
            return False
        else:
            if fillCounter: self.counters.counter('DiLepton').inc(
                '{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min,
                                                      max = self.cfg_ana.m_max )
                )

        # exactly one? 
        if len(selDiLeptons)==0:
            return False
        #elif len(selDiLeptons)==1:
        elif len(selDiLeptons)==1:
            if fillCounter: self.counters.counter('DiLepton').inc('exactly 1 di-lepton')
        #else :
        #    if fillCounter: self.counters.counter('DiLepton').inc('{NUM} di-lepton(s)'.format(NUM=len(selDiLeptons)))
        
        event.diLepton = self.bestDiLepton( selDiLeptons )
        event.leg1 = event.diLepton.leg1()
        event.leg2 = event.diLepton.leg2()

        return True
    
    def testLeg1(self, leg, iso=None):
        return self.testLeg(leg)

    def testLeg2(self, leg, iso=None):
        return self.testLeg(leg)

    def testLeg(self, leg):
        leg_pt  = self.cfg_ana.pt
        leg_eta = self.cfg_ana.eta
        iso     = self.cfg_ana.isolation
        
        if iso == 'mva'  : #### Tau ID MVA
          test_leg_iso = leg.tauID("byRawIsoMVA") > 0.5 # 0.884       
        if iso == 'mva2' : #### Tau ID MVA2
          test_leg_iso = leg.tauID("byIsolationMVA2raw") > 0.5 # 0.90
        if iso == 'db3h' : #### Tau ID deltaBeta 3 hits
          test_leg_iso = leg.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 15. #15.
        
        return (test_leg_iso                                 and \
                leg.pt()>leg_pt and abs(leg.eta()) < leg_eta and \
	        leg.tauID("decayModeFinding")      > 0.5     and \
	        leg.tauID("againstElectronLoose")  > 0.5     and \
	        leg.tauID("againstMuonLoose2")     > 0.5)

    def testLooseLeg(self, leg):
        leg_pt  = self.cfg_ana.pt
        leg_eta = self.cfg_ana.eta
        iso     = self.cfg_ana.isolation
        
        if iso == 'mva'  : #### Tau ID MVA
          test_leg_iso = leg.tauID("byRawIsoMVA") > 0.5        
        if iso == 'mva2' : #### Tau ID MVA2
          test_leg_iso = leg.tauID("byIsolationMVA2raw") > 0.5
        if iso == 'db3h' : #### Tau ID deltaBeta 3 hits
          test_leg_iso = leg.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 15. #15.
        
        return (test_leg_iso                                 and \
                leg.pt()>leg_pt and abs(leg.eta()) < leg_eta and \
	        leg.tauID("decayModeFinding")      > 0.5     and \
	        leg.tauID("againstElectronLoose")  > 0.5     and \
	        leg.tauID("againstMuonLoose2")     > 0.5)

    def testNonLeg(self, leg):
        leg_pt  = self.cfg_ana.pt
        leg_eta = self.cfg_ana.eta
        
        return (leg.pt()>leg_pt and abs(leg.eta()) < leg_eta and \
	        leg.tauID("decayModeFinding")      > 0.5     and \
	        leg.tauID("againstElectronLoose")  > 0.5     and \
	        leg.tauID("againstMuonLoose2")     > 0.5)

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
        if legName == 'leg1':
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
                              dR2Max=0.5*0.5,
                              pdgIds=pdgIds )

    def testJetID(self, jet):
    
        jet.puJetIdPassed = jet.puJetId()
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
