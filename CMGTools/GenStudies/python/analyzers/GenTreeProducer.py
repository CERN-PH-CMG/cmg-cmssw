from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.GenStudies.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class GenTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree

        var(tr, 'nCleanJets')
        var(tr, 'nGeneratedJets')
        var(tr, 'mjj')
        var(tr, 'deta')
        var(tr, 'decaymode')
        var(tr, 'categoryid')
        var(tr, 'x1')
        var(tr, 'x2')
        var(tr, 'id1')
        var(tr, 'id2')
        var(tr, 'scalePdf')

        bookGenJet(tr, 'jet1')
        bookGenJet(tr, 'jet2')
        

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()
        
        fill(tr, 'nCleanJets', len(event.cleanGenJets) )
        fill(tr, 'nGeneratedJets', len(event.genJets) )
        fill(tr, 'decaymode', event.channel)
        fill(tr, 'categoryid', event.categoryid)        
        
        fill(tr, 'x1', event.generator.pdf().x.first)
        fill(tr, 'x2', event.generator.pdf().x.second)
        fill(tr, 'id1', event.generator.pdf().id.first)
        fill(tr, 'id2', event.generator.pdf().id.second)
        fill(tr, 'scalePdf', event.generator.pdf().scalePDF)
        

        if len(event.cleanGenJets) > 1:
            fillGenJet(tr, 'jet1', event.cleanGenJets[0])
            fillGenJet(tr, 'jet2', event.cleanGenJets[1])
            fill(tr, 'mjj', event.vbf.mjj)
            fill(tr, 'deta', event.vbf.deta)

        self.tree.tree.Fill()
       
