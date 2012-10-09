from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )


class WTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):
        tr = self.tree
        bookParticle(tr, 'l1')
        bookParticle(tr, 'l2')

    def process(self, iEvent, event):

        # import pdb; pdb.set_trace()
        tr = self.tree
        tr.reset()
        if len(event.selLeptons)>0:
            fillParticle(tr, 'l1', event.selLeptons[0])
        if len(event.selLeptons)>1:
            fillParticle(tr, 'l2', event.selLeptons[1])
        
        self.tree.tree.Fill()
       
