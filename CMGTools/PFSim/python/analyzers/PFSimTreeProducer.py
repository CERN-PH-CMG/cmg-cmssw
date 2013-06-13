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
        bookGenJet(tr, 'jet1')

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()
       
        nJets = len(event.cleanGenJets)
        fill(tr, 'nJets', len(event.cleanGenJets) )

        if nJets>0:
            fillGenJet(tr, 'jet1', event.cleanGenJets[0] )

        
            
# fill that with gen as a pivot. 
##         nEles = len(event.electrons)
##         fill(tr, 'nEles', nEles )        
##         if nEles>=1:
##             fillParticle(tr, 'ele1', event.electrons[0] )
##             fillParticle(tr, 'ele1gen', event.electrons[0].gen )
##         if nEles>=2:
##             fillParticle(tr, 'ele2', event.electrons[1] )
##             fillParticle(tr, 'ele2gen', event.electrons[1].gen )

##         nMus = len(event.muons)
##         fill(tr, 'nMus', nMus )        
##         if nMus>=1:
##             fillParticle(tr, 'mu1', event.muons[0] )
##             fillParticle(tr, 'mu1gen', event.muons[0].gen )
##         if nMus>=2:
##             fillParticle(tr, 'mu2', event.muons[1] )
##             fillParticle(tr, 'mu2gen', event.muons[1].gen )
                    
        self.tree.tree.Fill()
       
