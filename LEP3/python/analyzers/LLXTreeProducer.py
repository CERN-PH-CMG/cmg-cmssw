from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class LLXTreeProducer( TreeAnalyzer ):
    '''Tree producer for the HZ -> l+l- X analysis.'''

    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def jetVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Nobj'.format(pName=pName))
            var('{pName}Ntrk'.format(pName=pName))
            var('{pName}NEle'.format(pName=pName))
            var('{pName}fEle'.format(pName=pName))
            var('{pName}fGamma'.format(pName=pName))
            var('{pName}pdgId'.format(pName=pName))
            var('{pName}NMu'.format(pName=pName))
            var('{pName}B7'.format(pName=pName))
            
        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            
        var('llX')
        var('eleele')
        var('mumu')
        var('nJets')
        var('ptMiss')
        var('pMiss')
        var('eMiss')
        var('mMiss')
        var('ctMiss')
        var('acol')
        var('acop')
        var('cross')


        jetVars('Jet1')
        jetVars('Jet2')
        jetVars('Lep1')
        jetVars('Lep2')

        #jetVars('Tau1R')
        #jetVars('Tau2R')
        #jetVars('Lep1R')
        #jetVars('Lep2R')

        particleVars('Z')
        particleVars('ZR')
        particleVars('HR')

        

        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fJetVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Nobj'.format(pName=pName), particle.nConstituents() )
            fill('{pName}Ntrk'.format(pName=pName), particle.component(1).number() )
            fill('{pName}NEle'.format(pName=pName), particle.component(2).number() )
            fill('{pName}NMu'.format(pName=pName), particle.component(3).number() )
            fill('{pName}fEle'.format(pName=pName), particle.component(2).fraction() )
            fill('{pName}fGamma'.format(pName=pName), particle.component(4).fraction() )
            fill('{pName}pdgId'.format(pName=pName), particle.pdgId() )
            fill('{pName}B7'.format(pName=pName), particle.btag(7) )

        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )

        subevent = getattr( event, self.cfg_ana.anaName )

        fill('eleele',subevent.eleele)
        fill('mumu',subevent.mumu)
        fill('llX',subevent.llX)
        fill('nJets',len(subevent.jets))

        if len(subevent.jets) >= 1 : 
            fJetVars('Jet1',subevent.jets[0])
            if len(subevent.jets) >= 2 : 
                fJetVars('Jet2',subevent.jets[1])

        if len(subevent.zLeptons) == 2 : 
            fJetVars('Lep1',subevent.zLeptons[0])
            fJetVars('Lep2',subevent.zLeptons[1])

        fParticleVars('Z',subevent.zPair)

        fill('ptMiss',subevent.eMiss.Pt())
        fill('pMiss',subevent.eMiss.P())
        fill('eMiss',subevent.eMiss.E())
        fill('mMiss',subevent.eMiss.M())
        ctMiss = 1.
        if subevent.eMiss.P() > 0. : 
            ctMiss = subevent.eMiss.Pz()/subevent.eMiss.P()
        fill('ctMiss',ctMiss)

        fill('acol',subevent.acol)
        fill('acop',subevent.acop)
        fill('cross',subevent.cross)

        self.tree.fill()
