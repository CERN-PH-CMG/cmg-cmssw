from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.GenStudies.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class GenTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree

        var( tr, 'nJets')
        var( tr, 'nJets_gen')
        var(tr, 'mjj')
        var(tr, 'deta')
        var( tr, 'Nlepton')
        bookGenJet(tr, 'jet1')
        bookGenJet(tr, 'jet2')
        

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()

        nJets = len(event.cleanGenJets)

        nJets_gen = len(event.selectedGenJets)
        fill(tr, 'nJets', len(event.cleanGenJets) )
        fill(tr, 'nJets_gen', len(event.genJets) )
        fill(tr, 'Nlepton', len(event.genLeptons3))

        if nJets>1:
            fillGenJet(tr, 'jet1', event.cleanGenJets[0])
            fillGenJet(tr, 'jet2', event.cleanGenJets[1])
            fill(tr, 'mjj', event.vbf.mjj)
            fill(tr, 'deta', event.vbf.deta)

        self.tree.tree.Fill()
       
