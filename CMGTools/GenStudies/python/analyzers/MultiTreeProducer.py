from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.GenStudies.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class MultiTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree
        var(tr, 'cutflow')
#        bookGenParticle(tr, 'cutflow')


    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()

        for io in range(len(event.tCounter)):

#            print 'Yuta = ', io, event.tCounter[io]

            if(event.tCounter[io]==-1):
                continue
            else:
                fill(tr, 'cutflow', io)
                self.tree.tree.Fill()

#        for io in range(len(event.genParticles)):
#            fillGenParticle(tr, 'pdgId', event.genParticles[io])
#            fill(tr, 'pdgId', event.genParticles[io])
#            self.tree.tree.Fill()
       
