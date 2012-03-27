from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class FourLeptonTreeProducer( TreeAnalyzer ):
    '''Tree producer for the H->ZZ->4 lepton analysis.

    Some of the functions in this class should be made available to everybody.'''
    
    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))
            var('{pName}PdgId'.format(pName=pName))
            var('{pName}SIP3D'.format(pName=pName))
            var('{pName}Iso'.format(pName=pName))
            
        particleVars('L1')
        particleVars('L2')
        particleVars('L3')
        particleVars('L4')
        
        particleVars('Z1')
        particleVars('Z1L1')
        particleVars('Z1L2')
        
        particleVars('Z2')
        particleVars('Z2L1')
        particleVars('Z2L2')
        
        particleVars('H')
        
        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )
            fill('{pName}PdgId'.format(pName=pName), particle.pdgId() )
            fill('{pName}SIP3D'.format(pName=pName), particle.sip3D() )
            fill('{pName}Iso'.format(pName=pName), particle.relIso(0.5) )
           
            
        fParticleVars('Z1', event.zBoson1 )
        fParticleVars('Z1L1', event.zBoson1.leg1 )
        fParticleVars('Z1L2', event.zBoson1.leg2 )

        if hasattr( event, 'zBoson2'): 
            fParticleVars('Z2', event.zBoson2 )
            fParticleVars('Z2L1', event.zBoson2.leg1 )
            fParticleVars('Z2L2', event.zBoson2.leg2 )

        if hasattr( event, 'higgsCand' ):
            fParticleVars('H', event.higgsCand )
            fParticleVars('L1', event.theLeptons_ptSorted[0])
            fParticleVars('L2', event.theLeptons_ptSorted[1])
            fParticleVars('L3', event.theLeptons_ptSorted[2])
            fParticleVars('L4', event.theLeptons_ptSorted[3])
 
        self.tree.fill()
