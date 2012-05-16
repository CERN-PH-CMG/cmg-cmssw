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

        def particleVars( pName ):
            var('{pName}Px'.format(pName=pName))
            var('{pName}Py'.format(pName=pName))
            var('{pName}Pz'.format(pName=pName))
            var('{pName}E'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))
            var('{pName}Iso'.format(pName=pName))
            var('{pName}Id'.format(pName=pName))
            
            
        var('visMass')
        var('svfitMass')
        var('mt')
	var('pThiggs')
        
	var('mex')
	var('mey')
	var('met')
	var('mttj')
	var('mjj')

        var('dRtt')
        var('dPhitt')

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

        var('l1DecayMode')
        var('l1Prongs')
        var('l1LooIso')
        var('l1MedIso')
        var('l1TigIso')
        var('l1RawMVAIso')
        var('l1MedMVAIso')
        var('l1TigMVAIso')
        var('l1LooseEle')
        var('l1MVAEle')
        var('l1LooseMu')

        var('l2DecayMode')
        var('l2Prongs')
        var('l2LooIso')
        var('l2MedIso')
        var('l2TigIso')
        var('l2RawMVAIso')
        var('l2MedMVAIso')
        var('l2TigMVAIso')
        var('l2LooseEle')
        var('l2MVAEle')
        var('l2LooseMu')

        var('genTauVisMass')
        var('genJetVisMass')

        var('nJets')
        particleVars('jet1')
        particleVars('jet2')

        var('weight')
        var('vertexWeight')

        var('nVert')
        
        var('l1EffData')
        var('l1EffMC')
        var('l1Weight')
        var('l2EffData')
        var('l2EffMC')
        var('l2Weight')

        var('isFake')
        var('isPhoton')
        var('isElectron')
        
        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Px'.format(pName=pName), particle.px() )
            fill('{pName}Py'.format(pName=pName), particle.py() )
            fill('{pName}Pz'.format(pName=pName), particle.pz() )
            fill('{pName}E'.format(pName=pName), particle.energy() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )
            if hasattr( particle, 'relIso' ):
                fill('{pName}Iso'.format(pName=pName), particle.relIso(0.5) )
            if abs( particle.pdgId() )==11:
                fill('{pName}Id'.format(pName=pName), particle.mvaDaniele() )
                
        fill('visMass', event.diLepton.mass())
        fill('svfitMass', event.diLepton.massSVFit())
        fill('mt', event.diLepton.mTLeg2())
        fill('pThiggs', sqrt(pow(event.diLepton.met().px()+event.diLepton.px(),2)+pow(event.diLepton.met().py()+event.diLepton.py(),2)))

        fill('mex', event.diLepton.met().px())
        fill('mey', event.diLepton.met().py())
        fill('met', event.diLepton.met().pt())
        fill('dRtt', sqrt(pow(deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()),2)+pow(fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()),2)))
        fill('dPhitt', deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()))
            
        fParticleVars('diTau', event.diLepton)
        fParticleVars('l1', event.diLepton.leg1() )
        fParticleVars('l2', event.diLepton.leg2() )

        fill('l1DecayMode', event.diLepton.leg1().decayMode() )
        fill('l1Prongs', len(event.diLepton.leg1().signalPFCands()) )
        fill('l1LooIso', event.diLepton.leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        fill('l1MedIso', event.diLepton.leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l1TigIso', event.diLepton.leg1().tauID("byTightCombinedIsolationDeltaBetaCorr") )
        fill('l1RawMVAIso', event.diLepton.leg1().tauID("byRawIsoMVA") )
        fill('l1MedMVAIso', event.diLepton.leg1().tauID("byMediumIsoMVA") )
        fill('l1TigMVAIso', event.diLepton.leg1().tauID("byTightIsoMVA") )
        fill('l1LooseEle', event.diLepton.leg1().tauID("againstElectronLoose") )
        fill('l1MVAEle', event.diLepton.leg1().tauID("againstElectronMVA") )
        fill('l1LooseMu', event.diLepton.leg1().tauID("againstMuonLoose") )

        fill('l2DecayMode', event.diLepton.leg2().decayMode() )
        fill('l2Prongs', len(event.diLepton.leg2().signalPFCands()) )
        fill('l2LooIso', event.diLepton.leg2().tauID("byLooseCombinedIsolationDeltaBetaCorr") )
        fill('l2MedIso', event.diLepton.leg2().tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l2TigIso', event.diLepton.leg2().tauID("byTightCombinedIsolationDeltaBetaCorr") )
        fill('l2RawMVAIso', event.diLepton.leg2().tauID("byRawIsoMVA") )
        fill('l2MedMVAIso', event.diLepton.leg2().tauID("byMediumIsoMVA") )
        fill('l2TigMVAIso', event.diLepton.leg2().tauID("byTightIsoMVA") )
        fill('l2LooseEle', event.diLepton.leg2().tauID("againstElectronLoose") )
        fill('l2MVAEle', event.diLepton.leg2().tauID("againstElectronMVA") )
        fill('l2LooseMu', event.diLepton.leg2().tauID("againstMuonLoose") )

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
	    fill('mjj', sqrt(pow(event.cleanJets[0].energy()+event.cleanJets[1].energy(),2) -
	                     pow(event.cleanJets[0].px()+event.cleanJets[1].px(),2) - 
			     pow(event.cleanJets[0].py()+event.cleanJets[1].py(),2) - 
			     pow(event.cleanJets[0].pz()+event.cleanJets[1].pz(),2)))

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

        # SKIMMING here:
        if event.diLepton.leg1().pt>20 and \
	   event.diLepton.leg2().pt>20 and \
	   event.diLepton.leg1().tauID("decayModeFinding")>0.5 and \
	   event.diLepton.leg2().tauID("decayModeFinding")>0.5 and \
	   event.diLepton.leg1().tauID("againstElectronLoose")>0.5 and \
	   event.diLepton.leg2().tauID("againstElectronLoose")>0.5 and \
	   event.diLepton.leg1().tauID("againstMuonLoose")>0.5 and \
	   event.diLepton.leg2().tauID("againstMuonLoose")>0.5:
            self.tree.fill()
