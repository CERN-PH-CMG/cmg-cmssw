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
        bookJet(tr, 'jet1')
        bookJet(tr, 'jet2')
        bookJet(tr, 'jet3')
        bookGenJet(tr, 'jet1gen')
        bookGenJet(tr, 'jet2gen')
        bookGenJet(tr, 'jet3gen')
        bookGenParticle(tr, 'jet1gp')
        bookGenParticle(tr, 'jet2gp')
        bookGenParticle(tr, 'jet3gp')

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
       
        nJets = len(event.cleanJets)
        fill(tr, 'nJets', len(event.cleanJets) )

        if nJets>=1:
            fillJet(tr, 'jet1', event.cleanJets[0] )
            if event.cleanJets[0].genJet:
                fillGenJet(tr, 'jet1gen', event.cleanJets[0].genJet )
            if event.cleanJets[0].genPtc3:
                fillGenParticle(tr, 'jet1gp', event.cleanJets[0].genPtc3 )
        if nJets>=2:
            fillJet(tr, 'jet2', event.cleanJets[1] )
            if event.cleanJets[1].genJet:
                fillGenJet(tr, 'jet2gen', event.cleanJets[1].genJet )
            if event.cleanJets[1].genPtc3:
                fillGenParticle(tr, 'jet2gp', event.cleanJets[1].genPtc3 )
        if nJets>=3:
            fillJet(tr, 'jet3', event.cleanJets[2] )
            if event.cleanJets[2].genJet:
                fillGenJet(tr, 'jet3gen', event.cleanJets[2].genJet )
            if event.cleanJets[2].genPtc3:
                fillGenParticle(tr, 'jet2gp', event.cleanJets[2].genPtc3 )

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
       
