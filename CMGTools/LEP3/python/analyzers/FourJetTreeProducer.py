from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class FourJetTreeProducer( TreeAnalyzer ):
    '''Tree producer for the HZ -> 4 jet analysis.'''

    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}B01'.format(pName=pName))
            var('{pName}B02'.format(pName=pName))
            var('{pName}B11'.format(pName=pName))
            var('{pName}B12'.format(pName=pName))
            var('{pName}B21'.format(pName=pName))
            var('{pName}B22'.format(pName=pName))
            var('{pName}B31'.format(pName=pName))
            var('{pName}B32'.format(pName=pName))
            var('{pName}B41'.format(pName=pName))
            var('{pName}B42'.format(pName=pName))
            var('{pName}B51'.format(pName=pName))
            var('{pName}B52'.format(pName=pName))
            var('{pName}B61'.format(pName=pName))
            var('{pName}B62'.format(pName=pName))
            var('{pName}B71'.format(pName=pName))
            var('{pName}B72'.format(pName=pName))

        def jetVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Nobj'.format(pName=pName))
            var('{pName}Ntrk'.format(pName=pName))
            var('{pName}Photon'.format(pName=pName))
            var('{pName}Electron'.format(pName=pName))
            
            
        particleVars('H')
        particleVars('Z')
        particleVars('W1')
        particleVars('W2')

        jetVars('Jet1')
        jetVars('Jet2')
        jetVars('Jet3')
        jetVars('Jet4')

        var('wwMin')
        var('zzMin')
        #var('wwMinKin')
        #var('zzMinKin')
        var('deltaZ')
        var('chi2')
        var('chi2fit')
        var('mvis')
        var('evis')
        var('pxvis')
        var('pyvis')
        var('pzvis')
        var('mmin')

        var('sumtet') 
        var('cijklmin')
        var('m2min') 

        var('m3min') 

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
            fill('{pName}Photon'.format(pName=pName), particle.component(4).energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )

        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}B01'.format(pName=pName), particle.leg1.btag(0) )
            fill('{pName}B02'.format(pName=pName), particle.leg2.btag(0) )
            fill('{pName}B11'.format(pName=pName), particle.leg1.btag(1) )
            fill('{pName}B12'.format(pName=pName), particle.leg2.btag(1) )
            fill('{pName}B21'.format(pName=pName), particle.leg1.btag(2) )
            fill('{pName}B22'.format(pName=pName), particle.leg2.btag(2) )
            fill('{pName}B31'.format(pName=pName), particle.leg1.btag(3) )
            fill('{pName}B32'.format(pName=pName), particle.leg2.btag(3) )
            fill('{pName}B41'.format(pName=pName), particle.leg1.btag(4) )
            fill('{pName}B42'.format(pName=pName), particle.leg2.btag(4) )
            fill('{pName}B51'.format(pName=pName), particle.leg1.btag(5) )
            fill('{pName}B52'.format(pName=pName), particle.leg2.btag(5) )
            fill('{pName}B61'.format(pName=pName), particle.leg1.btag(6) )
            fill('{pName}B62'.format(pName=pName), particle.leg2.btag(6) )
            fill('{pName}B71'.format(pName=pName), particle.leg1.btag(7) )
            fill('{pName}B72'.format(pName=pName), particle.leg2.btag(7) )

        subevent = getattr( event, self.cfg_ana.anaName )

        if len(subevent.hz) : 
            fParticleVars( 'H',subevent.hz[0] ) 
            fParticleVars( 'Z',subevent.hz[1] ) 
            fParticleVars( 'W1',subevent.ww[0] ) 
            fParticleVars( 'W2',subevent.ww[1] )

        fJetVars('Jet1',subevent.allJets[0])
        fJetVars('Jet2',subevent.allJets[1])
        fJetVars('Jet3',subevent.allJets[2])
        fJetVars('Jet4',subevent.allJets[3])

        fill('wwMin',subevent.wwMin)
        fill('zzMin',subevent.zzMin)
        #fill('wwMinKin',subevent.wwMinKin)
        #fill('zzMinKin',subevent.zzMinKin)
        fill('deltaZ',subevent.deltaZ)
        fill('mvis',subevent.mvis)
        fill('pxvis',subevent.pxvis) 
        fill('pyvis',subevent.pyvis)
        fill('pzvis',subevent.pzvis) 
        fill('evis',subevent.evis)
        fill('chi2',subevent.chi2)
        fill('chi2fit',subevent.chi2fit)
        fill('mmin',subevent.mmin)

        fill('sumtet',subevent.sumtet) 
        fill('cijklmin',subevent.cijklmin)
        fill('m2min',subevent.m2min) 

        fill('m3min',subevent.m3min) 

        self.tree.fill()
