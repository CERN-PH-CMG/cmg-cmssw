from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class ttHLepTreeProducerBase( TreeAnalyzer ):

    #-----------------------------------
    # TREE PRODUCER FOR THE TTH ANALYSIS
    #-----------------------------------
    

    def declareVariables(self):

        def ivar( varName ):
            self.tree.addVar('int', varName)
            
        def var( varName ):
            self.tree.addVar('float', varName)

        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))


        def leptonVars( pName ):
            particleVars(pName)
            var('{pName}PdgId'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))


        ivar("nLep")
        for i in range(4):
            leptonVars("Lep%d"%(i+1))

    
        self.tree.book()

    def process(self, iEvent, event):
        def fill( varName, value ):
            setattr( self.tree.s, varName, value )
            
        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )

        def fLeptonVars( pName, particle ):
            fParticleVars(pName, particle)
            fill('{pName}PdgId'.format(pName=pName), particle.pdgId() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )

        subevent = getattr(event, self.cfg_ana.anaName)

        fill('nLep', len(subevent.looseLeptons))
        for i in range(min(4,len(subevent.looseLeptons))):
             fLeptonVars("Lep%d"%(i+1), subevent.looseLeptons[i])

        self.tree.fill()
