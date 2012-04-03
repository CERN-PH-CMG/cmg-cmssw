from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

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

        particleVars('diTau')
        particleVars('l1')
        particleVars('l2')

        var('l2Iso')

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
            
        fParticleVars('diTau', event.diLepton)
        fParticleVars('l1', event.diLepton.leg1() )
        fParticleVars('l2', event.diLepton.leg2() )

        fill('l2Iso', event.diLepton.leg2().relIso(0.5) )

        nJets = len(event.cleanJets)
        fill('nJets', nJets )
        if nJets>=1:
            fParticleVars('jet1', event.cleanJets[0] )
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
        if event.genMatched == 1: isFake = 0
        fill('isFake', isFake)

        self.tree.fill()
