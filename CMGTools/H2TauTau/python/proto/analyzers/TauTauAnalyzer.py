import math
from CMGTools.RootTools.analyzers.DiLeptonAnalyzer import DiLeptonAnalyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.DiObject import TauTau
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Tau, GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2
from ROOT import TFile

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

        #self.handles['leptons'] = AutoHandle(
        #    'cmgTauSel',
        #    'std::vector<cmg::Tau>'
        #    )
        if self.cfg_comp.isMC and ("DY" in self.cfg_comp.name or "W" in self.cfg_comp.name or "Higgs" in self.cfg_comp.name):
            self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
        if self.cfg_comp.isMC and "QCD" in self.cfg_comp.name:
            self.mchandles['generator'] = AutoHandle( 'generator',
                                                     'GenEventInfoProduct' )
        if self.cfg_comp.isEmbed:
            self.handles['genParticles'] = AutoHandle( 'genParticles',
                                                     'std::vector<reco::GenParticle>' )
	if "Higgsgg" in self.cfg_comp.name:
	    masspoint=self.cfg_comp.name[7:10]
            self.higgsPtWeightFile=TFile("$CMSSW_BASE/src/CMGTools/H2TauTau/data/weight_ptH_"+masspoint+".root")
            self.higgsPtWeightHistogram=self.higgsPtWeightFile.Get("powheg_weight/weight_hqt_fehipro_fit_"+masspoint)

    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (the one with best isolation).'''
        return max( [ (min(dilep.leg1().tauID("byRawIsoMVA"), dilep.leg2().tauID("byRawIsoMVA")), dilep) for dilep in diLeptons ] )[1]
    
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
        # import pdb; pdb.set_trace()
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
            # Post-Preapproval version
	    selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
                             self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
			     self.testNonLeg( diL.leg1() ) and self.testNonLeg( diL.leg2() ) and \
			     (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
            if len(selDiLeptons)==0:
                selDiLeptons = [ diL for diL in event.diLeptons if \
                             self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
			     self.testNonLeg( diL.leg1() ) and self.testNonLeg( diL.leg2() ) and \
			     (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
            # loose reference version
            #selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
            #                 self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
		#	     self.testLooseLeg( diL.leg1() ) and self.testLooseLeg( diL.leg2() ) and \
		#	     (self.testLeg( diL.leg1() ) or self.testLeg( diL.leg2() )) ]
            # std. medium iso version
            #selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
            #                 self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
		#	     self.testNonLeg( diL.leg1() ) and self.testNonLeg( diL.leg2() ) and \
		#	     (self.testStdMediumLeg( diL.leg1() ) or self.testStdMediumLeg( diL.leg2() )) ]
            # std. tight iso version
            #selDiLeptons = [ diL for diL in event.diLeptonsTrigMatched if \
            #                 self.cfg_ana.m_min < diL.mass() and diL.mass() < self.cfg_ana.m_max and \
		#	     self.testLooseStdLeg( diL.leg1() ) and self.testLooseStdLeg( diL.leg2() ) and \
		#	     (self.testStdTightLeg( diL.leg1() ) or self.testStdTightLeg( diL.leg2() )) ]
            # relax cuts completely
	    #if len(selDiLeptons)==0:
	    #    selDiLeptons=event.diLeptons
            if len(selDiLeptons)==0:
                return False
            event.diLepton = self.bestDiLepton( selDiLeptons )
            event.leg1 = event.diLepton.leg1()
            event.leg2 = event.diLepton.leg2()
            event.isSignal = False
        else:
            event.isSignal = True

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
                if abs(gen.pdgId())==15 and gen.mother().pdgId()==23 and (gen.mother().mass()<80 or gen.mother().mass()>100):
                    event.isPhoton=True
                if abs(gen.pdgId())==11 and gen.mother().pdgId()==23:
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
            for gen in genParticles:
                if abs(gen.pdgId())==15 and gen.mother().pdgId()==24: # W -> tau nu_tau
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
                if (leg1DeltaR>-1 and leg1DeltaR < 0.1) or \
                   (leg2DeltaR>-1 and leg2DeltaR < 0.1):
                     event.genMatched = True
		
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

        if self.cfg_comp.isEmbed:
            genParticles = self.handles['genParticles'].product()
            event.genParticles = map( GenParticle, genParticles)
	    for gen in genParticles:
                if abs(gen.pdgId()) in [23]:
                    event.genMass=gen.mass()
		
        return True


    def selectionSequence(self, event, fillCounter, leg1IsoCut=None, leg2IsoCut=None):

        if fillCounter: self.counters.counter('DiLepton').inc('all events')
        # if not self.triggerList.triggerPassed(event.triggerObject):
        #    return False
        # self.counters.counter('DiLepton').inc('trigger passed ')

        # if event.eventId == 155035:
        #    import pdb; pdb.set_trace()
            
        if len(event.diLeptons) == 0:
            return False
        if fillCounter: self.counters.counter('DiLepton').inc('> 0 di-lepton')

        # import pdb; pdb.set_trace()
        # testing di-lepton itself
        selDiLeptons = event.diLeptons
        # selDiLeptons = self.selectDiLeptons( selDiLeptons ) 
        
        if not self.leptonAccept( event.leptons ):
            return False
        if fillCounter: self.counters.counter('DiLepton').inc('lepton accept')

        if len(self.cfg_comp.triggers)>0:
            # trigger matching leg1
            selDiLeptons = [diL for diL in selDiLeptons if \
                            self.trigMatched(event, diL.leg1(), 'leg1')]
            if len(selDiLeptons) == 0:
                event.l1TrigMatched=False
            else:
                if fillCounter: self.counters.counter('DiLepton').inc('leg1 trig matched')
                event.l1TrigMatched=True

        if len(self.cfg_comp.triggers)>0:
            # trigger matching leg2
            selDiLeptons = [diL for diL in selDiLeptons if \
                            self.trigMatched(event, diL.leg2(), 'leg2')]
            if len(selDiLeptons) == 0:
                event.l2TrigMatched=False
            else:
                if fillCounter: self.counters.counter('DiLepton').inc('leg2 trig matched')
                event.l2TrigMatched=True

        event.diLeptonsTrigMatched = selDiLeptons

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
        elif len(selDiLeptons)==1:
            if fillCounter: self.counters.counter('DiLepton').inc('exactly 1 di-lepton')
        
        event.diLepton = self.bestDiLepton( selDiLeptons )
        event.leg1 = event.diLepton.leg1()
        event.leg2 = event.diLepton.leg2()

        return True
    


    def testLeg1(self, leg, iso=None):
        #return True
        #return self.testNonLeg(leg)
        return self.testLeg(leg)
        #return self.testStdMediumLeg(leg)
        #return self.testStdTightLeg(leg)
    def testLeg2(self, leg, iso=None):
        #return True
        #return self.testNonLeg(leg)
        return self.testLeg(leg)
        #return self.testStdMediumLeg(leg)
        #return self.testStdTightLeg(leg)

    def testLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("byMediumIsoMVA")>0.5 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)

    def testStdMediumLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("byMediumCombinedIsolationDeltaBetaCorr")>0.5 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)

    def testStdTightLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("byTightCombinedIsolationDeltaBetaCorr")>0.5 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)

    #def selectDiLeptons(self, diLeptons, cutString=None):
    #    selDiLeptons = [ diL for diL in diLeptons if \
    #                     (diL.leg1().tauID("byTightIsoMVA")>0.5 or diL.leg2().tauID("byTightIsoMVA")>0.5)]
    #    if len(selDiLeptons) > 0:
    #        self.counters.counter('DiLepton').inc( 'di-lepton cut string ok')
    #    return selDiLeptons

    def testLooseLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("byRawIsoMVA")>0.5 and \
	   #leg.tauID("byRawIsoMVA")>0.795 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)

    def testLooseStdLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)

    def testNonLeg(self, leg):
        return (leg.pt()>35 and abs(leg.eta())<2.1 and \
	   leg.tauID("decayModeFinding")>0.5 and \
	   leg.tauID("againstElectronLoose")>0.5 and \
	   leg.tauID("againstMuonLoose")>0.5)
