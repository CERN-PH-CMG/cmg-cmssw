from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
from math import *

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
            #var('{pName}Iso'.format(pName=pName))
            #var('{pName}Id'.format(pName=pName))
            
            
        var('visMass')
        var('svfitMass')
        var('mt')
	var('pThiggs')
        
	var('mex')
	var('mey')
	var('met')
	var('mttj')

	var('mjj')
	var('deltaEta')
	varInt('nCentralJets')

        var('dRtt')
        var('dPhitt')

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

        var('l1DecayMode')
        #varInt('l1LooIso')
        #varInt('l1MedIso')
        #varInt('l1TigIso')
        var('l1RawMVAIso')
        varInt('l1MedMVAIso')
        varInt('l1TigMVAIso')
        #varInt('l1LooseEle')
        varInt('l1MVAEle')
        #varInt('l1LooseMu')

        var('l2DecayMode')
        #varInt('l2LooIso')
        #varInt('l2MedIso')
        #varInt('l2TigIso')
        var('l2RawMVAIso')
        varInt('l2MedMVAIso')
        varInt('l2TigMVAIso')
        #varInt('l2LooseEle')
        varInt('l2MVAEle')
        #varInt('l2LooseMu')

        var('genTauVisMass')
        var('genJetVisMass')

        varInt('nJets')
        particleVars('jet1')
        particleVars('jet2')

        var('weight')
        var('vertexWeight')

        varInt('nVert')
        
        var('l1EffData')
        var('l1EffMC')
        var('l1Weight')
        var('l2EffData')
        var('l2EffMC')
        var('l2Weight')

        varInt('isFake')
        varInt('isPhoton')
        varInt('isElectron')
	
        self.tree.book()


    def process(self, iEvent, event):

        scale=1.0

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Px'.format(pName=pName), particle.px()*scale )
            fill('{pName}Py'.format(pName=pName), particle.py()*scale )
            fill('{pName}Pz'.format(pName=pName), particle.pz()*scale )
            fill('{pName}E'.format(pName=pName), particle.energy()*scale )
            fill('{pName}Pt'.format(pName=pName), particle.pt()*scale )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )
            #if hasattr( particle, 'relIso' ):
            #    fill('{pName}Iso'.format(pName=pName), particle.relIso(0.5) )
            #if abs( particle.pdgId() )==11:
            #    fill('{pName}Id'.format(pName=pName), particle.mvaDaniele() )

        # SKIMMING here:
        #if not (event.diLepton.leg1().pt>20 and \
	#   event.diLepton.leg2().pt>20 and \
	#   event.diLepton.leg1().tauID("decayModeFinding")>0.5 and \
	#   event.diLepton.leg2().tauID("decayModeFinding")>0.5 and \
	#   (event.diLepton.leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5 or \
	#    event.diLepton.leg2().tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5) and \
	#   event.diLepton.leg1().tauID("againstElectronLoose")>0.5 and \
	#   event.diLepton.leg2().tauID("againstElectronLoose")>0.5 and \
	#   event.diLepton.leg1().tauID("againstMuonLoose")>0.5 and \
	#   event.diLepton.leg2().tauID("againstMuonLoose")>0.5): return
                
        #if "TTJets" in self.cfg_comp.name and not (event.diLepton.leg1().tauID("byMediumIsoMVA")>0.5 and \
	#   event.diLepton.leg2().tauID("byMediumIsoMVA")>0.5): return
                
        fill('visMass', event.diLepton.mass()*scale)
        fill('svfitMass', event.diLepton.massSVFit()*scale)
        fill('mt', event.diLepton.mTLeg2()*scale)
        fill('pThiggs', sqrt(pow(event.diLepton.met().px()+event.diLepton.px(),2)+pow(event.diLepton.met().py()+event.diLepton.py(),2))*scale)

        fill('mex', event.diLepton.met().px()*scale)
        fill('mey', event.diLepton.met().py()*scale)
        fill('met', event.diLepton.met().pt()*scale)
        fill('dRtt', sqrt(pow(deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()),2)+pow(fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()),2)))
        fill('dPhitt', deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()))
            
        fParticleVars('diTau', event.diLepton)
	
	#if event.diLepton.leg1().pt()>event.diLepton.leg2().pt():
	#    leg1=event.diLepton.leg1()
	#    leg2=event.diLepton.leg2()
        #else:
	#    leg1=event.diLepton.leg2()
	#    leg2=event.diLepton.leg1()
	leg1=event.diLepton.leg1()
	leg2=event.diLepton.leg2()
	
        fParticleVars('l1', leg1 )
        fParticleVars('l2', leg2 )

        fill('l1DecayMode', leg1.decayMode() )
        #fill('l1LooIso', leg1.tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        #fill('l1MedIso', leg1.tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        #fill('l1TigIso', leg1.tauID("byTightCombinedIsolationDeltaBetaCorr") )
        fill('l1RawMVAIso', leg1.tauID("byRawIsoMVA") )
        fill('l1MedMVAIso', leg1.tauID("byMediumIsoMVA") )
        fill('l1TigMVAIso', leg1.tauID("byTightIsoMVA") )
        #fill('l1LooseEle', leg1.tauID("againstElectronLoose") )
        fill('l1MVAEle', leg1.tauID("againstElectronMVA") )
        #fill('l1LooseMu', leg1.tauID("againstMuonLoose") )

        fill('l2DecayMode', leg2.decayMode() )
        #fill('l2LooIso', leg2.tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        #fill('l2MedIso', leg2.tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        #fill('l2TigIso', leg2.tauID("byTightCombinedIsolationDeltaBetaCorr") )
        fill('l2RawMVAIso', leg2.tauID("byRawIsoMVA") )
        fill('l2MedMVAIso', leg2.tauID("byMediumIsoMVA") )
        fill('l2TigMVAIso', leg2.tauID("byTightIsoMVA") )
        #fill('l2LooseEle', leg2.tauID("againstElectronLoose") )
        fill('l2MVAEle', leg2.tauID("againstElectronMVA") )
        #fill('l2LooseMu', leg2.tauID("againstMuonLoose") )

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
	    fill('mttj', sqrt(pow(event.diLepton.energy()+event.cleanJets[0].energy(),2) - pow(event.diLepton.px()+event.cleanJets[0].px(),2) - pow(event.diLepton.py()+event.cleanJets[0].py(),2) - pow(event.diLepton.pz()+event.cleanJets[0].pz(),2)))
        if nJets>=2:
            fParticleVars('jet2', event.cleanJets[1] )
	    fill('mjj', event.vbf.mjj)
	    fill('deltaEta', event.vbf.deta)
	    fill('nCentralJets', len(event.vbf.centralJets))
	    #fill('mjj', sqrt(pow(event.cleanJets[0].energy()+event.cleanJets[1].energy(),2) -
	    #                 pow(event.cleanJets[0].px()+event.cleanJets[1].px(),2) - 
		#	     pow(event.cleanJets[0].py()+event.cleanJets[1].py(),2) - 
		#	     pow(event.cleanJets[0].pz()+event.cleanJets[1].pz(),2)))

        fill('weight', event.eventWeight)
        if hasattr( event, 'vertexWeight'): 
            fill('vertexWeight', event.vertexWeight)
            fill('nVert', len(event.vertices) ) 
        if hasattr( event, 'leg1_eff'): 
            fill('l1EffData', event.leg1_eff)
            fill('l1EffMC', event.leg1_effMC)
            fill('l1Weight', event.leg1_weight)
        if hasattr( event, 'leg2_eff'): 
            fill('l2EffData', event.leg2_eff)
            fill('l2EffMC', event.leg2_effMC)
            fill('l2Weight', event.leg2_weight)

        isFake = 1
        if hasattr( event, 'genMatched'): 
            if event.genMatched == 1:
                isFake = 0
        fill('isFake', isFake)

        isPhoton = 0
        if hasattr(event,'isPhoton') and event.isPhoton == 1: isPhoton = 1
        fill('isPhoton', isPhoton)

        isElectron = 0
        if hasattr(event,'isElectron') and event.isElectron == 1: isElectron = 1
        fill('isElectron', isElectron)

        self.tree.fill()
