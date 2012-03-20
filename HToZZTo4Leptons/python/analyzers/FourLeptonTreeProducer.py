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
            # var('{pName}Iso'.format(pName=pName))
            
        particleVars('l1')
        particleVars('l2')
        particleVars('l3')
        particleVars('l4')
        
        particleVars('Z1')
        # particleVars('Z2')
        
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
            # fill('{pName}Iso'.format(pName=pName), particle.relIso(0.5) )
           
            
        fParticleVars('Z1', event.zBoson1 )

        #if hasattr( event, 'zBoson2'): 
        #    fParticleVars('Z2', event.zBoson2 )
 
        #fParticleVars('l1', event.l1 )
        #fParticleVars('l2', event.l2 )
        #fParticleVars('l3', event.l3 )
        #fParticleVars('l4', event.l4 )
 
        self.tree.fill()
