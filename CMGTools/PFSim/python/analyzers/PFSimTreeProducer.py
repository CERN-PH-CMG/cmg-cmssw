from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.PFSim.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class PFSimTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree

        var( tr, 'nJets')
        var( tr, 'nJets20')
        bookParticle(tr, 'jet1')
        bookParticle(tr, 'jet2')
        bookParticle(tr, 'jet1gen')
        bookParticle(tr, 'jet2gen')

        var( tr, 'nEles')
        bookParticle(tr, 'ele1')
        bookParticle(tr, 'ele2')
        bookParticle(tr, 'ele1gen')
        bookParticle(tr, 'ele2gen')
        
        var( tr, 'nMus')
        bookParticle(tr, 'mu1')
        bookParticle(tr, 'mu2')
        bookParticle(tr, 'mu1gen')
        bookParticle(tr, 'mu2gen')
        

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()
       
        fill(tr, 'nJets20', len(event.cleanJets) )
        nJets30 = len(event.cleanJets30)
        fill(tr, 'nJets', nJets30 )
        nJets = len(event.cleanJets)

        if nJets>=1:
            fillParticle(tr, 'jet1', event.cleanJets[0] )
            fillParticle(tr, 'jet1gen', event.cleanJets[0].genJet )
        if nJets>=2:
            fillParticle(tr, 'jet2', event.cleanJets[1] )
            fillParticle(tr, 'jet2gen', event.cleanJets[1].genJet )

        nEles = len(event.electrons)
        fill(tr, 'nEles', nEles )        
        if nEles>=1:
            fillParticle(tr, 'ele1', event.electrons[0] )
            fillParticle(tr, 'ele1gen', event.electrons[0].gen )
        if nEles>=2:
            fillParticle(tr, 'ele2', event.electrons[1] )
            fillParticle(tr, 'ele2gen', event.electrons[1].gen )

        nMus = len(event.muons)
        fill(tr, 'nMus', nMus )        
        if nMus>=1:
            fillParticle(tr, 'mu1', event.muons[0] )
            fillParticle(tr, 'mu1gen', event.muons[0].gen )
        if nMus>=2:
            fillParticle(tr, 'mu2', event.muons[1] )
            fillParticle(tr, 'mu2gen', event.muons[1].gen )
                    
        self.tree.tree.Fill()
       
