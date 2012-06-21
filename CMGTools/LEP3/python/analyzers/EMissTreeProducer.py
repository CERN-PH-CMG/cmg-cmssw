from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class EMissTreeProducer( TreeAnalyzer ):
    '''Tree producer for the WWH and HZ -> nunu bb analysis.'''

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
            var('{pName}B7'.format(pName=pName))
#            var('{pName}B71'.format(pName=pName))
#            var('{pName}B72'.format(pName=pName))
            
            
        var('ptMiss')
        var('pMiss')
        var('eMiss')
        var('mMiss')
        var('ctMiss')
        var('ptVis')
        var('pVis')
        var('eVis')
        var('mVis')
        var('ctVis')
        var('acol')
        var('acop')
        var('sumtet')
        var('nunubb')
        var('wwh')
        var('eleele')
        var('mumu')
        var('tautau')
        var('alpha')

        jetVars('Jet1')
        jetVars('Jet2')

        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fJetVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Nobj'.format(pName=pName), particle.nConstituents() )
            fill('{pName}Ntrk'.format(pName=pName), particle.component(1).number() )
            fill('{pName}Electron'.format(pName=pName), particle.component(2).energy() )
            fill('{pName}B7'.format(pName=pName), particle.btag(7) )
#            fill('{pName}B72'.format(pName=pName), particle.leg2.btag(7) )

        subevent = getattr( event, self.cfg_ana.anaName )

        fill('ptMiss',subevent.ptMiss)
        fill('eMiss',subevent.eMiss)
        fill('pMiss',subevent.pMiss)
        fill('mMiss',subevent.mMiss)
        fill('ctMiss',subevent.ctMiss) 
        fill('ptVis',subevent.ptVis)
        fill('eVis',subevent.eVis)
        fill('pVis',subevent.pVis)
        fill('mVis',subevent.mVis)
        fill('ctVis',subevent.ctVis) 
        fill('acol',subevent.acol)
        fill('acop',subevent.acop)
        fill('sumtet',subevent.sumtet)
        fill('nunubb',subevent.nunubb)
        fill('wwh',subevent.wwh)
        fill('eleele',subevent.eleele)
        fill('mumu',subevent.mumu)
        fill('tautau',subevent.tautau)
        fill('alpha',subevent.alpha)

        fJetVars('Jet1',subevent.allJets[0])
        fJetVars('Jet2',subevent.allJets[1])

        self.tree.fill()
