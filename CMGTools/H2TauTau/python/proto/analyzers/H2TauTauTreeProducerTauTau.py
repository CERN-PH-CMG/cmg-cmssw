from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
from CMGTools.RootTools.utils.DeltaR import bestMatch
from math import *
import inspect

def lineno():
    '''Returns the current line number in our program.'''
    return inspect.currentframe().f_back.f_lineno

def deltaPhi(phi1, phi2):
   p=abs(phi1-phi2)
   if p<pi:
      return p
   else:
      return 2*pi-p

class H2TauTauTreeProducerTauTau( TreeAnalyzer ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def varInt( varName ):
            self.tree.addVar('int', varName)

        def particleVars( pName ):
            var('{pName}Px'.format(pName=pName))
            var('{pName}Py'.format(pName=pName))
            var('{pName}Pz'.format(pName=pName))
            var('{pName}E'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))
            var('{pName}Mass'.format(pName=pName))
            #var('{pName}Iso'.format(pName=pName))
            #var('{pName}Id'.format(pName=pName))
            
            
        varInt('run')
        varInt('lumi')
        varInt('event')
	
        var('visMass')
        var('svfitMass')
        var('mt1')
        var('mt2')
	var('pThiggs')
        
	var('metPhi')
	var('mex')
	var('mey')
	var('met')
	var('mttj')

	var('rawMET')

	var('mjj')
	var('dEtajj')
	var('dPhijj')
	var('dPhittjj')
	var('dEtattjj')
	varInt('nCentralJets')
	var('vbfMVA')

        var('dRtt')
        var('dEtatt')
        var('dPhitt')

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

        var('l1DecayMode')
        varInt('l1LooIso')
        varInt('l1MedIso')
        varInt('l1TigIso')
        var('l1RawMVAIso')
        var('l1RawMVA2Iso')
        var('l1RawDB3HIso')
        varInt('l1MedMVAIso')
        varInt('l1TigMVAIso')
        varInt('l1LooseDB3HIso')
        varInt('l1MediumDB3HIso')
        varInt('l1TightDB3HIso')
        varInt('l1LooseEle')
        varInt('l1MedEle')
        varInt('l1MVAEle')
        varInt('l1LooseMu')
        var('l1jetMass')
        var('l1jetPt')
        var('l1jetWidth')
        var('l1jetBtag')
        var('l1againstElectronMVA3raw')
        var('l1againstElectronLooseMVA3')
        var('l1againstElectronMediumMVA3')
        var('l1againstElectronTightMVA3')
        var('l1againstElectronVTightMVA3')
        var('l1againstMuonLoose2')
        var('l1againstMuonMedium2')
        var('l1againstMuonTight2')

        var('l2DecayMode')
        varInt('l2LooIso')
        varInt('l2MedIso')
        varInt('l2TigIso')
        var('l2RawMVAIso')
        var('l2RawMVA2Iso')
        var('l2RawDB3HIso')
        varInt('l2MedMVAIso')
        varInt('l2TigMVAIso')
        varInt('l2LooseDB3HIso')
        varInt('l2MediumDB3HIso')
        varInt('l2TightDB3HIso')
        
        
        varInt('l2LooseEle')
        varInt('l2MedEle')
        varInt('l2MVAEle')
        varInt('l2LooseMu')
        var('l2jetMass')
        var('l2jetPt')
        var('l2jetWidth')
        var('l2jetBtag')
        var('l2againstElectronMVA3raw')
        var('l2againstElectronLooseMVA3')
        var('l2againstElectronMediumMVA3')
        var('l2againstElectronTightMVA3')
        var('l2againstElectronVTightMVA3')

        var('l2againstMuonLoose2')
        var('l2againstMuonMedium2')
        var('l2againstMuonTight2')


        var('l1match')
        var('l2match')
        var('l1VertexZ')
        var('l2VertexZ')

        var('genMass')
        var('genMassSmeared')
        var('svfitMassSmeared')
        var('l1JetInvMass')
        var('l2JetInvMass')
        varInt('l1TrigMatched')
        varInt('l2TrigMatched')
        varInt('jetTrigMatched')

        #var('genTauVisMass')
        #var('genJetVisMass')

        varInt('nJets')
        particleVars('jet1')
        particleVars('jet2')
        var('jet1Btag')
        var('jet2Btag')
        var('jet1Bmatch')
        var('jet2Bmatch')
        ## Riccardo
        var('sumJetPt')
        var('allJetPt')

        varInt('nbJets')
        particleVars('bjet1')
        particleVars('bjet2')
        ## Riccardo
        var('sumbJetPt')
        var('allbJetPt')

        var('weight')
        var('vertexWeight')
        var('embedWeight')
        var('triggerWeight')
        var('triggerEffData')
        var('triggerEffMC')
        
        varInt('nVert')
        
        varInt('genMatched')
        varInt('isFake')
        varInt('isPhoton')
        varInt('isElectron')
        varInt('isMuon')

        varInt('hasW')
        varInt('hasZ')

        var('electron1Pt')
        var('muon1Pt')
        var('electron1Eta')
        var('muon1Eta')
        var('electron1Phi')
        var('muon1Phi')

        var('NUP')

	self.triggers=['HLT_LooseIsoPFTau35_Trk20_Prong1_v6',
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET70_v6',
         'HLT_LooseIsoPFTau35_Trk20_Prong1_MET75_v6',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2',
         'HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_v2',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6',
         'HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_Prong1_v6']
        for trig in self.triggers:
            varInt(trig)
	
        self.tree.book()

    def process(self, iEvent, event):

        scale=1.0

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Px'.format(pName=pName)    , particle.px()*scale     )
            fill('{pName}Py'.format(pName=pName)    , particle.py()*scale     )
            fill('{pName}Pz'.format(pName=pName)    , particle.pz()*scale     )
            fill('{pName}E'.format(pName=pName)     , particle.energy()*scale )
            fill('{pName}Pt'.format(pName=pName)    , particle.pt()*scale     )
            fill('{pName}Eta'.format(pName=pName)   , particle.eta()          )
            fill('{pName}Phi'.format(pName=pName)   , particle.phi()          )
            fill('{pName}Charge'.format(pName=pName), particle.charge()       )
            fill('{pName}Mass'.format(pName=pName)  , particle.mass()*scale   )

        if hasattr(event,"run"):
            fill('run', event.run)
        if hasattr(event,"lumi"):
            fill('lumi', event.lumi)
        if hasattr(event,"eventId"):
            fill('event', event.eventId)
        
        fill('visMass'  , event.diLepton.mass()*scale)
        fill('svfitMass', event.diLepton.massSVFit()*scale)
        ### import pdb ; pdb.set_trace() ## SVFit pT is missingin tautau cmgtuples
        ### need to hack http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h?revision=1.11&view=markup
        #fill('svfitPt' , event.diLepton.massSVFit()*scale)
        fill('mt1'      , event.diLepton.mTLeg1()*scale)
        fill('mt2'      , event.diLepton.mTLeg2()*scale)
        fill('pThiggs'  , sqrt(pow(event.diLepton.met().px()+event.diLepton.leg1().px()+event.diLepton.leg2().px(),2)+pow(event.diLepton.met().py()+event.diLepton.leg1().py()+event.diLepton.leg2().py(),2))*scale)

        #mvametsig = event.diLepton.mvaMetSig.significance()
        #fill( tr, 'mvacov00', mvametsig(0,0))
        #fill( tr, 'mvacov01', mvametsig(0,1))
        #fill( tr, 'mvacov10', mvametsig(1,0))
        #fill( tr, 'mvacov11', mvametsig(1,1))
        fill('metPhi', event.diLepton.met().phi()*scale)
        fill('mex'   , event.diLepton.met().px()*scale)
        fill('mey'   , event.diLepton.met().py()*scale)
        fill('met'   , event.diLepton.met().pt()*scale)
        fill('rawMET', event.rawMET[0].pt())
        fill('dRtt'  , sqrt(pow(deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()),2)+pow(fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()),2)))
        fill('dPhitt', deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()))
        fill('dEtatt', fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()))
            
        fParticleVars('diTau', event.diLepton)

        leg1=event.diLepton.leg1()
        leg2=event.diLepton.leg2()
	
        fParticleVars('l1', leg1 )
        fParticleVars('l2', leg2 )

        l1jet = bestMatch( leg1, event.jets )[0]
        l2jet = bestMatch( leg2, event.jets )[0]
	
        fill('l1DecayMode', leg1.decayMode() )
        fill('l1LooIso'    , leg1.tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        fill('l1MedIso'    , leg1.tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l1TigIso'    , leg1.tauID("byTightCombinedIsolationDeltaBetaCorr") )


        fill('l1RawMVAIso' , leg1.tauID("byRawIsoMVA") )
        fill('l1RawMVA2Iso', leg1.tauID("byIsolationMVA2raw") )
        fill('l1RawDB3HIso', leg1.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") )
        fill('l1MedMVAIso' , leg1.tauID("byMediumIsoMVA") )
        fill('l1TigMVAIso' , leg1.tauID("byTightIsoMVA") )
        fill('l1LooseDB3HIso' , leg1.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits"))
        fill('l1MediumDB3HIso', leg1.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"))
        fill('l1TightDB3HIso' , leg1.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits"))


        fill('l1LooseEle'  , leg1.tauID("againstElectronLoose") )
        fill('l1MedEle'    , leg1.tauID("againstElectronMedium") )
        fill('l1MVAEle'    , leg1.tauID("againstElectronMVA") )
        fill('l1LooseMu'   , leg1.tauID("againstMuonLoose2") )
        fill('l1jetMass'   , leg1.jetRefp4().mass() )
        fill('l1jetPt'     , leg1.jetRefp4().pt() )
        if l1jet:
          fill('l1jetWidth', l1jet.rms() )
          fill('l1jetBtag' , l1jet.btag("combinedSecondaryVertexBJetTags") )
        fill('l1againstElectronMVA3raw'   , leg1.tauID("againstElectronMVA3raw"))
        fill('l1againstElectronLooseMVA3' , leg1.tauID("againstElectronLooseMVA3"))
        fill('l1againstElectronMediumMVA3', leg1.tauID("againstElectronMediumMVA3"))
        fill('l1againstElectronTightMVA3' , leg1.tauID("againstElectronTightMVA3"))
        fill('l1againstElectronVTightMVA3', leg1.tauID("againstElectronVTightMVA3"))
        fill('l1againstMuonLoose2'        , leg1.tauID("againstMuonLoose2"))
        fill('l1againstMuonMedium2'       , leg1.tauID("againstMuonMedium2"))
        fill('l1againstMuonTight2'        , leg1.tauID("againstMuonTight2"))
        
#         import pdb ; pdb.set_trace()


        fill('l2DecayMode', leg2.decayMode() )
        fill('l2LooIso'   , leg2.tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        fill('l2MedIso'   , leg2.tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l2TigIso'   , leg2.tauID("byTightCombinedIsolationDeltaBetaCorr") )
        
        fill('l2RawMVAIso'    , leg2.tauID("byRawIsoMVA") )
        fill('l2RawMVA2Iso'   , leg2.tauID("byIsolationMVA2raw") )
        fill('l2RawDB3HIso'   , leg2.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") )        
        fill('l2MedMVAIso'    , leg2.tauID("byMediumIsoMVA") )
        fill('l2TigMVAIso'    , leg2.tauID("byTightIsoMVA") )
        fill('l2LooseDB3HIso' , leg2.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits"))
        fill('l2MediumDB3HIso', leg2.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits"))
        fill('l2TightDB3HIso' , leg2.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits"))

        fill('l2LooseEle', leg2.tauID("againstElectronLoose") )
        fill('l2MedEle'  , leg2.tauID("againstElectronMedium") )
        fill('l2MVAEle'  , leg2.tauID("againstElectronMVA") )
        fill('l2LooseMu' , leg2.tauID("againstMuonLoose2") )
        fill('l2jetMass' , leg2.jetRefp4().mass() )
        fill('l2jetPt'   , leg2.jetRefp4().pt() )
        if l2jet:
          fill('l2jetWidth', l2jet.rms() )
          fill('l2jetBtag' , l1jet.btag("combinedSecondaryVertexBJetTags") )

        fill('l2againstElectronMVA3raw'   , leg2.tauID("againstElectronMVA3raw"))
        fill('l2againstElectronLooseMVA3' , leg2.tauID("againstElectronLooseMVA3"))
        fill('l2againstElectronMediumMVA3', leg2.tauID("againstElectronMediumMVA3"))
        fill('l2againstElectronTightMVA3' , leg2.tauID("againstElectronTightMVA3"))
        fill('l2againstElectronVTightMVA3', leg2.tauID("againstElectronVTightMVA3"))
        fill('l2againstMuonLoose2'        , leg2.tauID("againstMuonLoose2"))
        fill('l2againstMuonMedium2'       , leg2.tauID("againstMuonMedium2"))
        fill('l2againstMuonTight2'        , leg2.tauID("againstMuonTight2"))

        fill('l1VertexZ', leg1.tau.vz() )
        fill('l2VertexZ', leg2.tau.vz() )

        if hasattr(event,"leg1DeltaR"):
            fill('l1match', event.leg1DeltaR )
            fill('l2match', event.leg2DeltaR )
	else:
            fill('l1match', -1 )
            fill('l2match', -1 )

	if hasattr(event,"genMass"):
            fill('genMass', event.genMass )
#	    from ROOT import TFile,TH1F
#            f = TFile("/afs/cern.ch/user/h/hinzmann/workspace/stable2012/CMGTools/CMSSW_5_2_5/src/CMGTools/H2TauTau/python/proto/embeddedWeights.root")
#            hEmbed = f.Get("hemb")
#            shift=hEmbed.GetRandom()/event.genMass
#            fill('genMassSmeared', event.genMass*shift )
#            fill('svfitMassSmeared', event.diLepton.massSVFit()*scale*shift )
	else:
            fill('genMass'         , -1 )
            fill('genMassSmeared'  , -1 )
            fill('svfitMassSmeared', -1 )

	if hasattr(event,"l1TrigMatched"):
            fill('l1TrigMatched', event.l1TrigMatched )
	else:
            fill('l1TrigMatched', -1 )

	if hasattr(event,"l2TrigMatched"):
            fill('l2TrigMatched', event.l2TrigMatched )
	else:
            fill('l2TrigMatched', -1 )

	if hasattr(event,"jetTrigMatched"):
            fill('jetTrigMatched', event.jetTrigMatched )
	else:
            fill('jetTrigMatched', -1 )

        #if event.diLepton.leg1().genTaup4() and event.diLepton.leg2().genTaup4():
        #  fill('genTauVisMass', sqrt(
	#   pow(event.diLepton.leg1().genTaup4().energy()+event.diLepton.leg2().genTaup4().energy(),2)
	#   -pow(event.diLepton.leg1().genTaup4().px()+event.diLepton.leg2().genTaup4().px(),2)
	#   -pow(event.diLepton.leg1().genTaup4().py()+event.diLepton.leg2().genTaup4().py(),2)
	#   -pow(event.diLepton.leg1().genTaup4().pz()+event.diLepton.leg2().genTaup4().pz(),2)
	#   ))

        #if event.diLepton.leg1().genJetp4() and event.diLepton.leg2().genJetp4():
        #  fill('genJetVisMass', sqrt(
	#   pow(event.diLepton.leg1().genJetp4().energy()+event.diLepton.leg2().genJetp4().energy(),2)
	#   -pow(event.diLepton.leg1().genJetp4().px()+event.diLepton.leg2().genJetp4().px(),2)
	#   -pow(event.diLepton.leg1().genJetp4().py()+event.diLepton.leg2().genJetp4().py(),2)
	#   -pow(event.diLepton.leg1().genJetp4().pz()+event.diLepton.leg2().genJetp4().pz(),2)
	#   ))

        nJets = len(event.cleanJets)
        fill('nJets', nJets )
        if nJets>=1:
            fParticleVars('jet1', event.cleanJets[0] )
            fill('jet1Btag'     , event.cleanJets[0].btag("combinedSecondaryVertexBJetTags") )
            fill('jet1Bmatch'   , event.cleanJets[0].matchGenParton )
	    fill('mttj'         , sqrt(pow(event.diLepton.energy()+event.cleanJets[0].energy(),2) - pow(event.diLepton.px()+event.cleanJets[0].px(),2) - pow(event.diLepton.py()+event.cleanJets[0].py(),2) - pow(event.diLepton.pz()+event.cleanJets[0].pz(),2)))
	    fill('l1JetInvMass' , sqrt(pow(leg1.energy()+event.cleanJets[0].energy(),2) - pow(leg1.px()+event.cleanJets[0].px(),2) - pow(leg1.py()+event.cleanJets[0].py(),2) - pow(leg1.pz()+event.cleanJets[0].pz(),2)))
	    fill('l2JetInvMass' , sqrt(pow(leg2.energy()+event.cleanJets[0].energy(),2) - pow(leg2.px()+event.cleanJets[0].px(),2) - pow(leg2.py()+event.cleanJets[0].py(),2) - pow(leg2.pz()+event.cleanJets[0].pz(),2)))
        if nJets>=2:
            fParticleVars('jet2', event.cleanJets[1] )
            fill('jet2Btag'     , event.cleanJets[1].btag("combinedSecondaryVertexBJetTags") )
            fill('jet2Bmatch'   , event.cleanJets[1].matchGenParton )
	    fill('mjj'          , event.vbf.mjj)
	    fill('dEtajj'       , event.vbf.deta)
	    fill('dPhijj'       , deltaPhi(event.cleanJets[0].phi(), event.cleanJets[1].phi()))
	    fill('dEtattjj'     , fabs(event.cleanJets[0].eta()+event.cleanJets[1].eta()-event.diLepton.eta()))
	    fill('dPhittjj'     , deltaPhi(event.cleanJets[0].phi()+event.cleanJets[1].phi(),event.diLepton.phi()))
	    fill('nCentralJets' , len(event.vbf.centralJets))
	    fill('vbfMVA'       , event.vbf.mva)

        ## Riccardo
        sumJetPt = 0.
        for j in range(nJets) :
          sumJetPt += event.cleanJets[j].pt()
          fill('allJetPt', event.cleanJets[j].pt())
        if nJets == 0 : sumJetPt = -99.
        fill('sumJetPt', sumJetPt)

        nbJets = len(event.cleanBJets)
        fill('nbJets', nbJets )
        if nbJets>=1:
            fParticleVars('bjet1', event.cleanBJets[0] )
        if nbJets>=2:
            fParticleVars('bjet2', event.cleanBJets[1] )

        ## Riccardo
        sumbJetPt = 0.
        for j in range(nbJets) :
          sumbJetPt += event.cleanBJets[j].pt()
          fill('allbJetPt', event.cleanBJets[j].pt())
        if nbJets == 0 : sumbJetPt = -99.
        fill('sumbJetPt', sumbJetPt)
        
          


        fill('weight', event.eventWeight)
        if hasattr( event, 'vertexWeight'): 
            fill('vertexWeight', event.vertexWeight  )
            fill('nVert'       , len(event.vertices) ) 
        if hasattr( event, 'embedWeight'): 
            fill('embedWeight', event.embedWeight)
        if hasattr( event, 'triggerWeight'): 
            fill('triggerWeight', event.triggerWeight)

        if hasattr( event, 'triggerEffData'): 
            fill('triggerEffData', event.triggerEffData)
        if hasattr( event, 'triggerEffMC'): 
            fill('triggerEffMC', event.triggerEffMC)

        isFake = 1
        if hasattr( event, 'genMatched'): 
            if event.genMatched == 1:
                isFake = 0
        fill('isFake', isFake)

        genMatched = 0
        if hasattr(event,'genMatched') and event.genMatched > 0: genMatched = event.genMatched
        fill('genMatched', genMatched)

        isPhoton = 0
        if hasattr(event,'isPhoton') and event.isPhoton == 1: isPhoton = 1
        fill('isPhoton', isPhoton)

        isElectron = 0
        if hasattr(event,'isElectron') and event.isElectron > 0: isElectron = event.isElectron
        fill('isElectron', isElectron)

        isMuon = 0
        if hasattr(event,'isMuon') and event.isMuon > 0: isMuon = event.isMuon
        fill('isMuon', isMuon)

        hasW = 0
        if hasattr(event,'hasW') and event.hasW == 1: hasW = 1
        fill('hasW', hasW)

        hasZ = 0
        if hasattr(event,'hasZ') and event.hasZ == 1: hasZ = 1
        fill('hasZ', hasZ)

        if len(event.muons)>0:
            fill('muon1Pt' , event.muons[0].pt()  )
            fill('muon1Eta', event.muons[0].eta() )
            fill('muon1Phi', event.muons[0].phi() )
	else:
            fill('muon1Pt' , -1 )
            fill('muon1Eta', -1 )
            fill('muon1Phi', -1 )

        if len(event.electrons)>0:
            fill('electron1Pt' , event.electrons[0].pt()  )
            fill('electron1Eta', event.electrons[0].eta() )
            fill('electron1Phi', event.electrons[0].phi() )
	else:
            fill('electron1Pt' , -1 )
            fill('electron1Eta', -1 )
            fill('electron1Phi', -1 )

        fill('NUP', event.NUP )

        for trig in self.triggers:
            fill(trig, getattr(event,trig))

        self.tree.fill()
