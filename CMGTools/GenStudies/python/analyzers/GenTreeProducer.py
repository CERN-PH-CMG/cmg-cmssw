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
#        var( tr, 'nw')
#        var( tr, 'nb')
                
        bookGenJet(tr, 'jet1')
        bookGenJet(tr, 'jet2')
        

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()

        nJets = len(event.cleanGenJets)
#        nJets_gen = len(event.genJets)
        nJets_gen = len(event.selectedGenJets)
        fill(tr, 'nJets', len(event.cleanGenJets) )
        fill(tr, 'nJets_gen', len(event.genJets) )
        

#        fill(tr, 'nw', len(event.Nw))
#        fill(tr, 'nb', len(event.Nb))
        fill(tr, 'Nlepton', event.Nlepton)

#        print 'event_counter'
#        for a in range(5):
#            print a, event.tCounter[a], event.tCounter[a]


#        print 'nJets', nJets, len(envet.cleanGenJets)

        if nJets>1:
            fillGenJet(tr, 'jet1', event.cleanGenJets[0])
            fillGenJet(tr, 'jet2', event.cleanGenJets[1])
            fill(tr, 'mjj', event.vbf.mjj)
            fill(tr, 'deta', event.vbf.deta)
        else:
            fill(tr, 'mjj', -999)
            fill(tr, 'deta', -999)
            
#            if(event.vbf.mjj < 0):
#                print event.vbf.mjj

#            fillGenJet(tr, 'jet2', event.cleanGenJets[1])


            
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
       
