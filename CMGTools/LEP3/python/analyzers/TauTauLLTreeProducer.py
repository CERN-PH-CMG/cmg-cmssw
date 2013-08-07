from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class TauTauLLTreeProducer( TreeAnalyzer ):
    '''Tree producer for the HZ -> tau tau  l+l- analysis.'''

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
            var('{pName}pdgId'.format(pName=pName))
            var('{pName}NMu'.format(pName=pName))
            var('{pName}B7'.format(pName=pName))
            
        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            
        var('tautaull')
        var('eleele')
        var('mumu')
        var('nJets')
        var('ptMiss')
        var('pMiss')
        var('eMiss')
        var('mMiss')
        var('ctMiss')


        jetVars('Tau1')
        jetVars('Tau2')
        jetVars('Lep1')
        jetVars('Lep2')

        jetVars('Tau1R')
        jetVars('Tau2R')
        jetVars('Lep1R')
        jetVars('Lep2R')

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
        fill('tautaull',subevent.tautaull)
        fill('nJets',len(subevent.tauJets))

        if len(subevent.tauJets) >= 2 : 
            fJetVars('Tau1',subevent.tauJets[0])
            fJetVars('Tau2',subevent.tauJets[1])

        if len(subevent.zLeptons) == 2 : 
            fJetVars('Lep1',subevent.zLeptons[0])
            fJetVars('Lep2',subevent.zLeptons[1])

        fJetVars('Tau1R',subevent.rescaledTauJets[0])
        fJetVars('Tau2R',subevent.rescaledTauJets[1])
        fJetVars('Lep1R',subevent.rescaledLeptonJets[0])
        fJetVars('Lep2R',subevent.rescaledLeptonJets[1])

        fParticleVars('HR',subevent.ttRescaled)
        fParticleVars('ZR',subevent.llRescaled)
        fParticleVars('Z',subevent.zPair)

        fill('ptMiss',subevent.eMiss.Pt())
        fill('pMiss',subevent.eMiss.P())
        fill('eMiss',subevent.eMiss.E())
        fill('mMiss',subevent.eMiss.M())
        ctMiss = 1.
        if subevent.eMiss.P() > 0. : 
            ctMiss = subevent.eMiss.Pz()/subevent.eMiss.P()
        fill('ctMiss',ctMiss)
        self.tree.fill()
