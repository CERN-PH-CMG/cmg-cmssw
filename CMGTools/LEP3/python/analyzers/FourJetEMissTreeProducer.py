from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class FourJetEMissTreeProducer( TreeAnalyzer ):
    '''Tree producer for the WWH and HZ -> nunu VV analysis.'''

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
            
            
        var('nj_ini')
        var('njet')
        var('ptMiss')
        var('pMiss')
        var('eMiss')
        var('mMiss')
        var('mMissFit')
        var('ctMiss')
        var('ptVis')
        var('pVis')
        var('eVis')
        var('mVis')
        var('mVisFit')
        var('ctVis')
        var('acol')
        var('acop')
        var('sumtet')
        var('nunuVV')
        var('wwh')
        #var('eleele')
        #var('mumu')
        #var('tautau')
        var('alpha')
        var('cross')
        var('chi2mZ')
        var('ele')
        var('muo')
        var('tau')
        
#        var('chi2partiel')
        jetVars('Jet1')
        jetVars('Jet2')
#        jetVars('Jet3')
#        jetVars('Jet4')

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

        subevent = getattr( event, self.cfg_ana.anaName )

        fill('nj_ini',subevent.njet_ini)
        fill('njet',subevent.njet)
        fill('ptMiss',subevent.ptMiss)
        fill('eMiss',subevent.eMiss)
        fill('pMiss',subevent.pMiss)
        fill('mMiss',subevent.mMiss)
        fill('mMissFit',subevent.mMissFit)
        fill('ctMiss',subevent.ctMiss) 
        fill('ptVis',subevent.ptVis)
        fill('eVis',subevent.eVis)
        fill('pVis',subevent.pVis)
        fill('mVis',subevent.mVis)
        fill('mVisFit',subevent.mVisFit)
        fill('ctVis',subevent.ctVis) 
        fill('acol',subevent.acol)
        fill('acop',subevent.acop)
        fill('sumtet',subevent.sumtet)
        fill('nunuVV',subevent.nunuVV)
        fill('wwh',subevent.wwh)
        #fill('eleele',subevent.eleele)
        #fill('mumu',subevent.mumu)
        #fill('tautau',subevent.tautau)
        fill('alpha',subevent.alpha)
        fill('cross',subevent.cross)
        fill('chi2mZ',subevent.chi2mZ)
        fill('ele',subevent.ele)
        fill('muo',subevent.muo)
        fill('tau',subevent.tau)
#        fill('chi2partiel',subevent.chi2partiel)

        fJetVars('Jet1',subevent.allJets[0])
        fJetVars('Jet2',subevent.allJets[1])
#        fJetVars('Jet3',subevent.allJets[2])
#        fJetVars('Jet4',subevent.allJets[3])

        self.tree.fill()
