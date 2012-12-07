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
        ## --- JETS ---
        var( tr, 'nJet20', int)
        var( tr, 'nJet30', int)
        for i in range(8):
            bookLepton(tr,"Lep%d"%(i+1))
            bookJet(tr,"Jet%d"%(i+1))
        var( tr, 'nBJetLoose20', int )
        var( tr, 'nBJetMedium20', int )
        var( tr, 'nBJetLoose30', int )
        var( tr, 'nBJetMedium30', int )

        if self.cfg_comp.isMC: 
            self.declareMCVariables()

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
        fill(tr, 'nJet30', sum([(j.pt() > 30) for j in event.cleanJets]))      
        for i in range(min(8,len(event.cleanJets))):
            fillJet(tr, "Jet%d"%(i+1), event.cleanJets[i])        
        fill(tr, 'nBJetLoose20', len(event.bjetsLoose))      
        fill(tr, 'nBJetLoose30', sum([(j.pt() > 30) for j in event.bjetsLoose]))      
        fill(tr, 'nBJetMedium20', len(event.bjetsMedium))      
        fill(tr, 'nBJetMedium30', sum([(j.pt() > 30) for j in event.bjetsMedium]))      

        if self.cfg_comp.isMC: 
            self.fillMCVariables(iEvent, event)
              
        self.tree.tree.Fill()      

    def declareMCVariables(self):
        tr = self.tree
        var( tr, 'GenHiggsDecayMode', int)
        bookGenParticle(tr, 'GenHiggs')
        bookGenParticle(tr, 'GenTop1')
        bookGenParticle(tr, 'GenTop2')
        var( tr, 'nGenLeps', int) 
        var( tr, 'nGenLepsFromTau', int) 
        var( tr, 'nGenQuarks',   int) 
        var( tr, 'nGenBQuarks',   int) 
        for i in range(6):
            bookGenParticle(tr, 'GenLep%d'%(i+1),        withSourceId=True)
            bookGenParticle(tr, 'GenLepFromTau%d'%(i+1), withSourceId=True)
            bookGenParticle(tr, 'GenQuark%d'%(i+1),      withSourceId=True)
        for i in range(2):
            bookGenParticle(tr, 'GenBQuark%d'%(i+1))
 
 
    def fillMCVariables(self, iEvent, event):
        tr = self.tree
        fill( tr, 'GenHiggsDecayMode', event.genHiggsDecayMode )
        if event.genHiggsDecayMode == 0: return
        
        fillGenParticle(tr, 'GenHiggs', event.genHiggsBoson)
        fillGenParticle(tr, 'GenTop1', event.gentopquarks[0])
        fillGenParticle(tr, 'GenTop2', event.gentopquarks[1])

        fill( tr, 'nGenLeps', len(event.genleps) )
        fill( tr, 'nGenLepsFromTau', len(event.gentauleps) )
        fill( tr, 'nGenBQuarks', len(event.genbquarks) )
        fill( tr, 'nGenQuarks', len(event.genwzquarks) )
        
        for i in range(min(6,len(event.genleps))): 
            fillGenParticle(tr, 'GenLep%d'%(i+1), event.genleps[i], withSourceId=True)
        for i in range(min(6,len(event.gentauleps))): 
            fillGenParticle(tr, 'GenLepFromTau%d'%(i+1), event.gentauleps[i], withSourceId=True)
        for i in range(min(6,len(event.genwzquarks))): 
            fillGenParticle(tr, 'GenQuark%d'%(i+1), event.genwzquarks[i], withSourceId=True)
        for i in range(min(2,len(event.genbquarks))): 
            fillGenParticle(tr, 'GenBQuark%d'%(i+1), event.genbquarks[i])
        
