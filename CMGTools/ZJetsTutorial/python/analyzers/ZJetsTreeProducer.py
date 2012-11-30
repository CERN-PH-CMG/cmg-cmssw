from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.ZJetsTutorial.analyzers.ntuple import *

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class ZJetsTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree
        var( tr, 'run', int)
        var( tr, 'lumi', int)
        var( tr, 'evt', int)

        bookParticle(tr, 'diL')
        var(tr, 'diL_mass')

        bookParticle(tr, 'diLRaw')
        var(tr, 'diLRaw_mass')

        bookLepton(tr, 'leg1')
        bookLepton(tr, 'leg2')
##         bookLepton(tr, 'leg1Raw')
##         bookLepton(tr, 'leg2Raw')
        
        var( tr, 'nJets')
        var( tr, 'nJets20')
        bookJet(tr, 'jet1')
        bookJet(tr, 'jet2')

        bookVBF( tr, 'VBF' )

        var( tr, 'nVert')
        var( tr, 'weight')
        

    def process(self, iEvent, event):
        
        tr = self.tree
        tr.reset()

        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)
       
        fillParticle(tr, 'diL', event.diLepton)
        fill(tr, 'diL_mass', event.diLepton.mass())
        fillParticle(tr, 'diLRaw', event.diLeptonRaw)
        fill(tr, 'diLRaw_mass', event.diLeptonRaw.mass())
        fillLepton(tr, 'leg1', event.diLepton.leg1())
        fillLepton(tr, 'leg2', event.diLepton.leg2())
##         fillLepton(tr, 'leg1Raw', event.diLeptonRaw.leg1())
##         fillLepton(tr, 'leg2Raw', event.diLeptonRaw.leg2())


        fill(tr, 'nJets20', len(event.cleanJets) )
        nJets30 = len(event.cleanJets30)
        fill(tr, 'nJets', nJets30 )
        nJets = len(event.cleanJets)
        if nJets>=1:
            fillJet(tr, 'jet1', event.cleanJets[0] )
        if nJets>=2:
            fillJet(tr, 'jet2', event.cleanJets[1] )
            
        if hasattr( event, 'vbf'):
            fillVBF( tr, 'VBF', event.vbf )

        fill(tr, 'nVert', len(event.goodVertices) )
        fill(tr, 'weight', event.eventWeight )
        
        self.tree.tree.Fill()
       
