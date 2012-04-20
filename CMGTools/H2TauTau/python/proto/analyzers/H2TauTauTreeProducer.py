from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
from math import *

def deltaPhi(phi1, phi2):
   p=abs(phi1-phi2)
   if p<pi:
      return p
   else:
      return 2*pi-p

class H2TauTauTreeProducer( TreeAnalyzer ):
    '''Tree producer for the H->tau tau analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def particleVars( pName ):
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))
            var('{pName}Iso'.format(pName=pName))
            
        var('visMass')
        var('svfitMass')
        var('mt')
        
	var('met')
        var('mttj')
        var('dRtt')

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

        var('l1MedIso')
        var('l2MedIso')
        var('l1TigIso')
        var('l2TigIso')

        var('genTauVisMass')
        var('genJetVisMass')

        var('nJets')
        particleVars('jet1')
        particleVars('jet2')

        var('weight')
        var('vertexWeight')
        var('l1EffData')
        var('l1EffMC')
        var('l1Weight')
        var('l2EffData')
        var('l2EffMC')
        var('l2Weight')

        var('isFake')
        var('isPhoton')
        
        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )
            if hasattr( particle, 'relIso' ):
                fill('{pName}Iso'.format(pName=pName), particle.relIso(0.5) )
           
        fill('visMass', event.diLepton.mass())
        fill('svfitMass', event.diLepton.massSVFit())
        fill('mt', event.diLepton.mTLeg2())

        fill('met', event.diLepton.met().pt())
        fill('dRtt', sqrt(pow(deltaPhi(event.diLepton.leg1().phi(),event.diLepton.leg2().phi()),2)+pow(fabs(event.diLepton.leg1().eta()-event.diLepton.leg2().eta()),2)))
            
        fParticleVars('diTau', event.diLepton)
        fParticleVars('l1', event.diLepton.leg1() )
        fParticleVars('l2', event.diLepton.leg2() )

        fill('l1MedIso', event.diLepton.leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l2MedIso', event.diLepton.leg2().tauID("byMediumCombinedIsolationDeltaBetaCorr") )
        fill('l1TigIso', event.diLepton.leg1().tauID("byTightCombinedIsolationDeltaBetaCorr") )
        fill('l2TigIso', event.diLepton.leg2().tauID("byTightCombinedIsolationDeltaBetaCorr") )

        if event.diLepton.leg1().genTaup4() and event.diLepton.leg2().genTaup4():
          fill('genTauVisMass', sqrt(
	   pow(event.diLepton.leg1().genTaup4().energy()+event.diLepton.leg2().genTaup4().energy(),2)
	   -pow(event.diLepton.leg1().genTaup4().px()+event.diLepton.leg2().genTaup4().px(),2)
	   -pow(event.diLepton.leg1().genTaup4().py()+event.diLepton.leg2().genTaup4().py(),2)
	   -pow(event.diLepton.leg1().genTaup4().pz()+event.diLepton.leg2().genTaup4().pz(),2)
	   ))

        if event.diLepton.leg1().genJetp4() and event.diLepton.leg2().genJetp4():
          fill('genJetVisMass', sqrt(
	   pow(event.diLepton.leg1().genJetp4().energy()+event.diLepton.leg2().genJetp4().energy(),2)
	   -pow(event.diLepton.leg1().genJetp4().px()+event.diLepton.leg2().genJetp4().px(),2)
	   -pow(event.diLepton.leg1().genJetp4().py()+event.diLepton.leg2().genJetp4().py(),2)
	   -pow(event.diLepton.leg1().genJetp4().pz()+event.diLepton.leg2().genJetp4().pz(),2)
	   ))

        nJets = len(event.cleanJets)
        fill('nJets', nJets )
        if nJets>=1:
            fParticleVars('jet1', event.cleanJets[0] )
	    fill('mttj', sqrt(pow(event.diLepton.energy()+event.cleanJets[0].energy(),2) - pow(event.diLepton.px()+event.cleanJets[0].px(),2) - pow(event.diLepton.py()+event.cleanJets[0].py(),2) - pow(event.diLepton.pz()+event.cleanJets[0].pz(),2)))
        if nJets>=2:
            fParticleVars('jet2', event.cleanJets[1] )

        fill('weight', event.eventWeight)
        fill('vertexWeight', event.vertexWeight)
        fill('l1EffData', event.leg1_eff)
        fill('l1EffMC', event.leg1_effMC)
        fill('l1Weight', event.leg1_weight)
        fill('l2EffData', event.leg1_eff)
        fill('l2EffMC', event.leg1_effMC)
        fill('l2Weight', event.leg1_weight)

        isFake = 1
        if hasattr(event,'genMatched') and event.genMatched == 1: isFake = 0
        fill('isFake', isFake)

        isPhoton = 0
        if hasattr(event,'isPhoton') and event.isPhoton == 1: isPhoton = 1
        fill('isPhoton', isPhoton)

        self.tree.fill()
