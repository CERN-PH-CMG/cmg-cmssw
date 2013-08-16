from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer

class htttreeproducer( TreeAnalyzer ):
    '''Tree producer for the HZ, H->tt  analysis.'''

    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def genparticleVars( pName ):
            var('g_{pName}Mass'.format(pName=pName))
            var('g_{pName}Pt'.format(pName=pName))
            var('g_{pName}Energy'.format(pName=pName))
            var('g_{pName}Eta'.format(pName=pName))
            var('g_{pName}Phi'.format(pName=pName))

        def jetVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            #var('{pName}Nobj'.format(pName=pName))
            var('{pName}Ntrk'.format(pName=pName))
            #var('{pName}Photon'.format(pName=pName))
            #var('{pName}Electron'.format(pName=pName))
            #var('{pName}NeutralHadron'.format(pName=pName))
            var('{pName}ChFraction'.format(pName=pName))
            var('{pName}PFraction'.format(pName=pName))
            var('{pName}EFraction'.format(pName=pName))
            var('{pName}NHFraction'.format(pName=pName))
        
        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
        
            
        genparticleVars('H')
        genparticleVars('Z')
        genparticleVars('t1')
        genparticleVars('t2')
        jetVars('t1genjet')
        jetVars('t2genjet')

        particleVars('recH')
        particleVars('recZ')

        particleVars('leadingMuon')
        particleVars('subleadingMuon')
        particleVars('leadingElectron')
        particleVars('subleadingElectron')

        jetVars('nontgenjet1')
        jetVars('nontgenjet2')
        jetVars('nontgenjet3')
        jetVars('nontgenjet4')

        jetVars('t1rec')
        jetVars('t2rec')
        jetVars('j1rec')
        jetVars('j2rec')

        var('g_isHZ')
        var('g_isHZqq')
        var('g_isHZbb')
        var('g_ishtt')

        var('step')
        #var('nontgenjetiso1')
        #var('nontgenjetiso2')
        #var('nontgenjetiso3')
        #var('nontgenjetiso4')
        
        
        var('btag_tt')
        var('btag_jj')

        #var('t1cosjet')
        #var('t2cosjet')
        #var('t1iso')
        #var('t2iso')
        #var('t2cosjet')
        var('njets')
        var('genRecDistance1')
        var('genRecDistance2')
        var('genRecDistance3')
        var('genRecDistance4')
        var('tau1matchingjetindex')
        var('tau2matchingjetindex')


        var('mvis')
        var('px') 
        var('py') 
        var('pz') 
        var('evis') 
        var('chi2')
        var('mmin')
        
        var('t1_px')
        var('t1_py')
        var('t1_pz')
        var('t1_en')
        var('t2_px')
        var('t2_py')
        var('t2_pz')
        var('t2_en')
#        var('t1_charge')
#        var('t2_charge')
        var('j1_px')
        var('j1_py')
        var('j1_pz')
        var('j1_en')
        var('j2_px')
        var('j2_py')
        var('j2_pz')
        var('j2_en')

        var('t1candiso')
        var('t2candiso')

        var('t1s_px')
        var('t1s_py')
        var('t1s_pz')
        var('t1s_en')
        var('t2s_px')
        var('t2s_py')
        var('t2s_pz')
        var('t2s_en')
        var('j1s_px')
        var('j1s_py')
        var('j1s_pz')
        var('j1s_en')
        var('j2s_px')
        var('j2s_py')
        var('j2s_pz')
        var('j2s_en')
        var('wwMin')
        var('zzMin')
        
        var('tt_acoll')
        var('jj_acoll')
        var('ttmet_acopl')




#       var('deltaZ')
#       var('chi2')
#       var('mvis')
#       var('evis')
#       var('pxvis')
#       var('pyvis')
#       var('pzvis')
#       var('mmin')
#
#       var('sumtet') 
#       var('cijklmin')
#       var('m2min') 
#
#       var('m3min') 

        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fJetVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            #fill('{pName}Nobj'.format(pName=pName), particle.nConstituents() )
            fill('{pName}Ntrk'.format(pName=pName), particle.component(1).number() + particle.component(2).number() + particle.component(3).number())
            #fill('{pName}Electron'.format(pName=pName), particle.component(2).energy() )
            #fill('{pName}Photon'.format(pName=pName), particle.component(4).energy() )
            #fill('{pName}NeutralHadron'.format(pName=pName), particle.component(5).energy() )
            fill('{pName}ChFraction'.format(pName=pName), particle.component(1).fraction() + particle.component(2).fraction() + particle.component(3).fraction())
            fill('{pName}EFraction'.format(pName=pName), particle.component(2).fraction() )
            fill('{pName}PFraction'.format(pName=pName), particle.component(4).fraction() )
            fill('{pName}NHFraction'.format(pName=pName), particle.component(5).fraction() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
#
        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )

        def fgenParticleVars( pName, particle ):
            fill('g_{pName}Mass'.format(pName=pName), particle.mass() )
            fill('g_{pName}Pt'.format(pName=pName), particle.pt() )
            fill('g_{pName}Energy'.format(pName=pName), particle.energy() )
            fill('g_{pName}Eta'.format(pName=pName), particle.eta() )
            fill('g_{pName}Phi'.format(pName=pName), particle.phi() )
#            fill('{pName}B01'.format(pName=pName), particle.leg1.btag(0) )
#            fill('{pName}B02'.format(pName=pName), particle.leg2.btag(0) )
#            fill('{pName}B11'.format(pName=pName), particle.leg1.btag(1) )
#            fill('{pName}B12'.format(pName=pName), particle.leg2.btag(1) )
#            fill('{pName}B21'.format(pName=pName), particle.leg1.btag(2) )
#            fill('{pName}B22'.format(pName=pName), particle.leg2.btag(2) )
#            fill('{pName}B31'.format(pName=pName), particle.leg1.btag(3) )
#            fill('{pName}B32'.format(pName=pName), particle.leg2.btag(3) )
#            fill('{pName}B41'.format(pName=pName), particle.leg1.btag(4) )
#            fill('{pName}B42'.format(pName=pName), particle.leg2.btag(4) )
#            fill('{pName}B51'.format(pName=pName), particle.leg1.btag(5) )
#            fill('{pName}B52'.format(pName=pName), particle.leg2.btag(5) )
#            fill('{pName}B61'.format(pName=pName), particle.leg1.btag(6) )
#            fill('{pName}B62'.format(pName=pName), particle.leg2.btag(6) )
#            fill('{pName}B71'.format(pName=pName), particle.leg1.btag(7) )
#            fill('{pName}B72'.format(pName=pName), particle.leg2.btag(7) )
#
        subevent = getattr( event, self.cfg_ana.anaName )
        fill('step',subevent.step)
        
        fill('g_ishtt',subevent.ishtt)
        fill('g_isHZ',subevent.isHZ)
        fill('g_isHZqq',subevent.isHZqq)
        fill('g_isHZbb',subevent.isHZbb)

        #fill leading muons/electrons
        if (len(subevent.leadingMuons)>0):
          fParticleVars('leadingMuon', subevent.leadingMuons[0])
          if (len(subevent.leadingMuons)>1):
            fParticleVars('subleadingMuon', subevent.leadingMuons[1])
        if (len(subevent.leadingElectrons)>0):
          fParticleVars('leadingElectron', subevent.leadingElectrons[0])
          if (len(subevent.leadingElectrons)>1):
            fParticleVars('subleadingElectron', subevent.leadingElectrons[1])     

        
        if subevent.isHZ==1 : 
            fgenParticleVars( 'H',subevent.H ) 
            fgenParticleVars( 'Z',subevent.Z )
        if subevent.ishtt==1 :
            fgenParticleVars( 't1',subevent.tau1 ) 
            fgenParticleVars( 't2',subevent.tau2 )
            if subevent.njets>=2:
                fJetVars( 't1genjet',subevent.tau1genjet )
                fJetVars( 't2genjet',subevent.tau2genjet )
                fill( 't1cosjet',subevent.tau1cosjet)
                fill( 't2cosjet',subevent.tau2cosjet)
                fill( 'tau1matchingjetindex',subevent.tau1matchingjetindex)
                fill( 'tau2matchingjetindex',subevent.tau2matchingjetindex)
                fill( 't1iso',subevent.tau1iso)
                fill( 't2iso',subevent.tau2iso)
            if subevent.isHZqq==1:
              if (len(subevent.matchedRecGenDistances)>0):
                fill( 'genRecDistance1', subevent.matchedRecGenDistances[0] )   
              if (len(subevent.matchedRecGenDistances)>1):
                fill( 'genRecDistance2', subevent.matchedRecGenDistances[1] )  
              if (len(subevent.matchedRecGenDistances)>2):
                fill( 'genRecDistance3', subevent.matchedRecGenDistances[2] )   
              if (len(subevent.matchedRecGenDistances)>3):
                fill( 'genRecDistance4', subevent.matchedRecGenDistances[3] )  
        if len(subevent.hz):
            fParticleVars('recH', subevent.hz[0])
            fParticleVars('recZ', subevent.hz[1])


        fill('njets',subevent.njets)
        if subevent.step>=4:
            # fill rest of the variables
                fill('mvis', subevent.mvis)
                fill('px'  , subevent.px  )
                fill('py'  , subevent.py  )
                fill('pz'  , subevent.pz  )
                fill('evis', subevent.evis)
                fill('chi2', subevent.chi2)
                fill('mmin', subevent.mmin)

                fill('t1_px',subevent.t1_px)
                fill('t1_py',subevent.t1_py)
                fill('t1_pz',subevent.t1_pz)
                fill('t1_en',subevent.t1_en)
                fill('t2_px',subevent.t2_px)
                fill('t2_py',subevent.t2_py)
                fill('t2_pz',subevent.t2_pz)
                fill('t2_en',subevent.t2_en)
#                fill('t1_charge', subevent.taucandcharge[0])
#                fill('t2_charge', subevent.taucandcharge[1])
                fill('j1_px',subevent.j1_px)
                fill('j1_py',subevent.j1_py)
                fill('j1_pz',subevent.j1_pz)
                fill('j1_en',subevent.j1_en)
                fill('j2_px',subevent.j2_px)
                fill('j2_py',subevent.j2_py)
                fill('j2_pz',subevent.j2_pz)
                fill('j2_en',subevent.j2_en)
                fill('tt_acoll',subevent.acoll)
                fill('jj_acoll',subevent.jcoll)
                fill('ttmet_acopl',subevent.acopl)

                fill('t1s_px',subevent.t1s_px)
                fill('t1s_py',subevent.t1s_py)
                fill('t1s_pz',subevent.t1s_pz)
                fill('t1s_en',subevent.t1s_en)
                fill('t2s_px',subevent.t2s_px)
                fill('t2s_py',subevent.t2s_py)
                fill('t2s_pz',subevent.t2s_pz)
                fill('t2s_en',subevent.t2s_en)
                fill('j1s_px',subevent.j1s_px)
                fill('j1s_py',subevent.j1s_py)
                fill('j1s_pz',subevent.j1s_pz)
                fill('j1s_en',subevent.j1s_en)
                fill('j2s_px',subevent.j2s_px)
                fill('j2s_py',subevent.j2s_py)
                fill('j2s_pz',subevent.j2s_pz)
                fill('j2s_en',subevent.j2s_en)
                fill('t1candiso',subevent.taucandiso[0])
                fill('t2candiso',subevent.taucandiso[1])

                fJetVars('t1rec',subevent.taucand[0])
                fJetVars('t2rec',subevent.taucand[1])
                fJetVars('j1rec',subevent.nontaucand[0])
                fJetVars('j2rec',subevent.nontaucand[1])
                fill('btag_tt',subevent.btag_tt)
                fill('btag_jj',subevent.btag_jj)

#        if subevent.step>=5:
                fill('wwMin',subevent.wwMin)
                fill('zzMin',subevent.zzMin)
                
        

#        for n_j in range(0,4):
#            if len(subevent.nontaugenjet)>n_j:
#                fJetVars('nontgenjet%d'%(n_j+1),subevent.nontaugenjet[n_j])
#                fill('nontgenjetiso%d'%(n_j+1),subevent.nontaugenjetiso[n_j])
                     
#        fJetVars('Jet1',subevent.allJets[0])
#        fJetVars('Jet2',subevent.allJets[1])
#        fJetVars('Jet3',subevent.allJets[2])
#        fJetVars('Jet4',subevent.allJets[3])
#
#        fill('deltaZ',subevent.deltaZ)
#        fill('mvis',subevent.mvis)
#        fill('pxvis',subevent.pxvis) 
#        fill('pyvis',subevent.pyvis)
#        fill('pzvis',subevent.pzvis) 
#        fill('evis',subevent.evis)
#        fill('chi2',subevent.chi2)
#        fill('mmin',subevent.mmin)
#
#        fill('sumtet',subevent.sumtet) 
#        fill('cijklmin',subevent.cijklmin)
#        fill('m2min',subevent.m2min) 
#
#        fill('m3min',subevent.m3min) 
#
        
        self.tree.fill()
