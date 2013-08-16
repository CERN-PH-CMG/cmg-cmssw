from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

def bookParticle( tree, pName ):
    var(tree, '{pName}_m'.format(pName=pName))
    var(tree, '{pName}_p'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_theta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_m'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_p'.format(pName=pName), particle.p() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_theta'.format(pName=pName), particle.theta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )



class SixJetTreeProducer( TreeAnalyzerNumpy ):
    def declareVariables(self):
        tr = self.tree
        bookParticle(tr, 'j0')
        bookParticle(tr, 'j1')
        bookParticle(tr, 'j2')
        bookParticle(tr, 'j3')
        bookParticle(tr, 'j4')
        bookParticle(tr, 'j5')
        bookParticle(tr, 'Z1')
        bookParticle(tr, 'Z2')
        bookParticle(tr, 'H')
        
        var(tr, 'nJets')
        var(tr, 'chi2_4jet')
        var(tr, 'delta_WW')
        var(tr, 'delta_ZZ')
        
    def process(self, iEvent, event):       
        tr = self.tree
        tr.reset()

        fillParticle(tr, 'j0', event.sixjets[0])
        fillParticle(tr, 'j1', event.sixjets[1])
        fillParticle(tr, 'j2', event.sixjets[2])
        fillParticle(tr, 'j3', event.sixjets[3])
        fillParticle(tr, 'j4', event.sixjets[4])
        fillParticle(tr, 'j5', event.sixjets[5])
        fillParticle(tr, 'Z1', event.Z1)
        if hasattr(event, 'Z2'):
            fillParticle(tr, 'Z2', event.Z2)
        
        fillParticle(tr, 'H', event.higgs)
        
        fill(tr, 'nJets', len(event.jets))
        fill(tr, 'chi2_4jet', event.chi2_4jet )
        fill(tr, 'delta_WW', event.delta_WW )
        fill(tr, 'delta_ZZ', event.delta_ZZ )
        
        self.tree.tree.Fill()
       
