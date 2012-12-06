from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.TTHAnalysis.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

    

class ttHLepTreeProducerBase( TreeAnalyzerNumpy ):

    #-----------------------------------
    # TREE PRODUCER FOR THE TTH ANALYSIS
    #-----------------------------------

    def declareVariables(self):

        tr = self.tree
        # var( tr, 'run', int)
        # var( tr, 'lumi', int)
        # var( tr, 'evt', int)
        var( tr, 'nVert')
        var( tr, 'nLepLoose', int)
        var( tr, 'nJet20', int)
        for i in range(8):
            bookLepton(tr,"Lep%d"%(i+1))
            bookJet(tr,"Jet%d"%(i+1))


    def process(self, iEvent, event):
         
        tr = self.tree
        tr.reset()

        # fill( tr, 'run', event.run) 
        # fill( tr, 'lumi',event.lumi)
        # fill( tr, 'evt', event.eventId)    
        fill( tr, 'nVert', len(event.goodVertices) )
            
        fill(tr, 'nLepLoose', len(event.looseLeptons))
        for i in range(min(8,len(event.looseLeptons))):
            fillLepton( tr, "Lep%d"%(i+1), event.looseLeptons[i])

        fill(tr, 'nJet20', len(event.cleanJets))      
        for i in range(min(8,len(event.cleanJets))):
            fillJet(tr, "Jet%d"%(i+1), event.cleanJets[i])        
              
        self.tree.tree.Fill()      

