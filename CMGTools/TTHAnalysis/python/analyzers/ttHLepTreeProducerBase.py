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

        isMC = self.cfg_comp.isMC 

        tr = self.tree
        var( tr, 'run', int)
        var( tr, 'lumi', int)
        var( tr, 'evt', int)
        var( tr, 'nVert')
        
        ## --- LEPTONS ---
        var( tr, 'nLepGood', int)
        for i in range(8):
            bookLepton(tr,"LepGood%d"%(i+1), isMC)
        ## --- LOOSE LEPTONS (OPTIONAL) ---
        if self.cfg_ana.doLooseLeptons:
            var( tr, 'nLepLoose', int)
            for i in range(8):
                bookLepton(tr,"LepLoose%d"%(i+1))
        ## --- PHOTONS ---
        for i in range(8):            
            bookPhoton(tr,"Photon%d"%(i+1))     
        ## --- JETS ---
        var( tr, 'nJet25', int)
        var( tr, 'nJet30', int)       
        var( tr, 'nJet25Fwd', int)
        var( tr, 'nJet30Fwd', int)       
        self.saveJetId = (self.cfg_ana.saveJetId if hasattr(self.cfg_ana,'saveJetId') else False)
        for i in range(8):
            bookJet(tr,"Jet%d"%(i+1), isMC, saveID=self.saveJetId)
        for i in range(6):
            bookJet(tr,"FwdJet%d"%(i+1), isMC, saveID=self.saveJetId)
        if hasattr(self.cfg_ana, 'doJetsFailId') and self.cfg_ana.doJetsFailId:
            for i in range(8):
                bookJet(tr,"JetFailId%d"%(i+1), isMC, saveID=True)
        var( tr, 'nBJetLoose25', int )
        var( tr, 'nBJetMedium25', int )
        var( tr, 'nBJetLoose30', int )
        var( tr, 'nBJetMedium30', int )
        ## --- MET, HT, MHT ---
        var( tr, 'met' )
        var( tr, 'met_phi' )
        var( tr, 'metNoPU' )
        var( tr, 'metNoPU_phi' )
        #var( tr, 'metSignificance' )
        #var( tr, 'projMetAll1S' )
        #var( tr, 'projMetAll2S' )
        #var( tr, 'projMetJet1S' )
        #var( tr, 'projMetJet2S' )
        ## --- HT, MHT ---
        var( tr, 'htJet30' )
        var( tr, 'htJet25' )
        var( tr, 'mhtJet30' )
        var( tr, 'mhtJet25' )
        #var( tr, 'htJet30A' )
        var( tr, 'htJet25A' )
        #var( tr, 'mhtJet30A' )
        var( tr, 'mhtJet25A' )
        ## --- DILEPTON MASSES ---
        var( tr, 'mZ1' )
        var( tr, 'mZ1SFSS' )
        var( tr, 'mZ2' )
        var( tr, 'minMllSFOS' )
        var( tr, 'minMllAFOS' )
        var( tr, 'minMllAFSS' )
        var( tr, 'minMllAFAS' )
        var( tr, 'maxMllAFOS' )
        var( tr, 'maxMllAFSS' )
        var( tr, 'minDrllAFOS' )
        var( tr, 'maxDrllAFOS' )
        var( tr, 'minDrllAFSS' )
        var( tr, 'maxDrllAFSS' )
        var( tr, 'minPtllAFOS' )
        var( tr, 'maxPtllAFOS' )
        var( tr, 'minPtllAFSS' )
        var( tr, 'maxPtllAFSS' )
        var( tr, 'q3l', 'I' )
        var( tr, 'q4l', 'I' )
        var( tr, 'm2l' )
        var( tr, 'm3l' )
        var( tr, 'm4l' )
        var( tr, 'pt2l' )
        var( tr, 'pt3l' )
        var( tr, 'pt4l' )
        var( tr, 'ht3l' )
        var( tr, 'ht4l' )
         
        var( tr, 'minMWjj' )
        var( tr, 'minMWjjPt' )
        var( tr, 'bestMWjj' )
        var( tr, 'bestMWjjPt' )
        var( tr, 'bestMTopHad' )
        var( tr, 'bestMTopHadPt' )
        

        ## --- LEP EFFICIENCY WEIGHT ---
        if self.cfg_comp.isMC:
            var(tr, 'Eff_4lep')
            var(tr, 'EffUp_4lep')
            var(tr, 'EffDwn_4lep')
            var(tr, 'Eff_3lep')
            var(tr, 'EffUp_3lep')
            var(tr, 'EffDwn_3lep')
            var(tr, 'Eff_2lep')
            var(tr, 'EffUp_2lep')
            var(tr, 'EffDwn_2lep')    
        

        if self.cfg_comp.isMC: 
            var( tr, 'puWeight' )
            self.declareMCVariables()


    def process(self, iEvent, event):
         
        tr = self.tree
        tr.reset()

        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)    
        fill( tr, 'nVert', len(event.goodVertices) )

        ## --- LEPTONS ---    
        fill(tr, 'nLepGood', len(event.selectedLeptons))
        for i in range(min(8,len(event.selectedLeptons))):
            fillLepton( tr, "LepGood%d"%(i+1), event.selectedLeptons[i])

        ## --- LOOSE LEPTONS (OPTIONAL) ---
        if self.cfg_ana.doLooseLeptons:
            fill(tr, 'nLepLoose', len(event.looseLeptons))
            for i in range(min(8,len(event.looseLeptons))):
                fillLepton( tr, "LepLoose%d"%(i+1), event.looseLeptons[i])

        ## --- PHOTONS ---
        for i in range(min(8,len(event.allphotons))):
            fillPhoton(tr,"Photon%d"%(i+1), event.allphotons[i])         

        ## --- JETS ---
        #ordering the jets
        event.cleanJets.sort(key = lambda j : j.btag('combinedSecondaryVertexBJetTags'), reverse = True)
            
        fill(tr, 'nJet25', len(event.cleanJets))      
        fill(tr, 'nJet30', sum([(j.pt() > 30) for j in event.cleanJets]))      
        fill(tr, 'nJet25Fwd', len(event.cleanJetsFwd))      
        fill(tr, 'nJet30Fwd', sum([(j.pt() > 30) for j in event.cleanJetsFwd]))      
        for i in range(min(8,len(event.cleanJets))):
            fillJet(tr, "Jet%d"%(i+1), event.cleanJets[i], saveID=self.saveJetId)        
        for i in range(min(6,len(event.cleanJetsFwd))):
            fillJet(tr, "FwdJet%d"%(i+1), event.cleanJetsFwd[i], saveID=self.saveJetId)        

        if hasattr(self.cfg_ana, 'doJetsFailId') and self.cfg_ana.doJetsFailId:
            event.jetsFailId.sort(key = lambda j : j.pt(), reverse = True)
            for i in range(min(8,len(event.jetsFailId))):
                fillJet(tr,"JetFailId%d"%(i+1), event.jetsFailId[i], saveID=True)

        fill(tr, 'nBJetLoose25', len(event.bjetsLoose))      
        fill(tr, 'nBJetLoose30', sum([(j.pt() > 30) for j in event.bjetsLoose]))      
        fill(tr, 'nBJetMedium25', len(event.bjetsMedium))      
        fill(tr, 'nBJetMedium30', sum([(j.pt() > 30) for j in event.bjetsMedium]))      

        ## --- MET ---
        fill( tr, 'met', event.met.pt() )
        fill( tr, 'met_phi', event.met.phi() )
        fill( tr, 'metNoPU', event.metNoPU.pt() )
        fill( tr, 'metNoPU_phi', event.metNoPU.phi() )
        #fill( tr, 'metSignificance', event.metSignificance )
        #fill( tr, 'projMetAll1S', event.projMetAll1S )
        #fill( tr, 'projMetAll2S', event.projMetAll2S )
        #fill( tr, 'projMetJet1S', event.projMetJets1S )
        #fill( tr, 'projMetJet2S', event.projMetJets2S )

        ## --- MHT, HT ---
        fill( tr, 'htJet30', event.htJet30 )
        fill( tr, 'htJet25', event.htJet25 )
        fill( tr, 'htJet25A', event.htJet25a )
        fill( tr, 'mhtJet30', event.mhtJet30 )
        fill( tr, 'mhtJet25', event.mhtJet25 )
        fill( tr, 'mhtJet25A', event.mhtJet25a )

        ## --- DILEPTON MASSES ---
        fill( tr, 'mZ1', event.bestZ1[0] )
        fill( tr, 'mZ2', event.bestZ2[3] )
        fill( tr, 'mZ1SFSS', event.bestZ1sfss[0] )
        fill( tr, 'minMllSFOS', event.minMllSFOS )
        fill( tr, 'minMllAFOS', event.minMllAFOS )
        fill( tr, 'minMllAFSS', event.minMllAFSS )
        fill( tr, 'minMllAFAS', event.minMllAFAS )
        fill( tr, 'maxMllAFOS', event.maxMllAFOS )
        fill( tr, 'maxMllAFSS', event.maxMllAFSS )
        fill( tr, 'minPtllAFOS', event.minPtllAFOS )
        fill( tr, 'minPtllAFSS', event.minPtllAFSS )
        fill( tr, 'maxPtllAFOS', event.maxPtllAFOS )
        fill( tr, 'maxPtllAFSS', event.maxPtllAFSS )
        fill( tr, 'minDrllAFOS', event.minDrllAFOS )
        fill( tr, 'minDrllAFSS', event.minDrllAFSS )
        fill( tr, 'maxDrllAFOS', event.maxDrllAFOS )
        fill( tr, 'maxDrllAFSS', event.maxDrllAFSS )
        fill( tr, 'm2l', event.m2l )
        fill( tr, 'pt2l', event.pt2l )
        fill( tr, 'm3l', event.m3l )
        fill( tr, 'q3l', event.q3l )
        fill( tr, 'ht3l', event.ht3l )
        fill( tr, 'pt3l', event.pt3l )
        fill( tr, 'm4l', event.m4l )
        fill( tr, 'q4l', event.q4l )
        fill( tr, 'ht4l', event.ht4l )
        fill( tr, 'pt4l', event.pt4l )
         
        fill( tr, 'minMWjj', event.minMWjj )
        fill( tr, 'minMWjjPt', event.minMWjjPt )
        fill( tr, 'bestMWjj', event.bestMWjj )
        fill( tr, 'bestMWjjPt', event.bestMWjjPt )
        fill( tr, 'bestMTopHad', event.bestMTopHad )
        fill( tr, 'bestMTopHadPt', event.bestMTopHadPt )

        ## --- LEP EFFICIENCY WEIGHT ---
        if self.cfg_comp.isMC:
            ### changed: now if there are less than <n> leptons, Eff_<n>lep is the product of the efficiency corrections for the available leptons 
            eff, effUp, effDwn = [1.],[1.],[1.]
            for l in event.selectedLeptons:
                eff.append(eff[-1]*l.eff)
                effUp.append(effUp[-1]*l.effUp)
                effDwn.append(effDwn[-1]*l.effDwn)
            for i in 2,3,4:
                fill( tr, 'Eff_%dlep'%i,      eff[min(i,len(eff)-1)])
                fill( tr, 'EffUp_%dlep'%i,  effUp[min(i,len(eff)-1)])
                fill( tr, 'EffDwn_%dlep'%i, effUp[min(i,len(eff)-1)])

        if self.cfg_comp.isMC: 
            fill( tr, 'puWeight', event.eventWeight )
            self.fillMCVariables(iEvent, event)

        self.tree.tree.Fill()      

    def declareMCVariables(self):
        tr = self.tree
        var( tr, 'GenHiggsDecayMode', int)
        var( tr, 'GenHeaviestQCDFlavour', int)
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
 
        var( tr, 'nGoodLepsMatchId',  int) 
        var( tr, 'nGoodLepsMatchAny', int) 

        var( tr, 'nGenJets25', int)
        var( tr, 'nGenJets25Cen', int)
        var( tr, 'nGenJets25Fwd', int)
        
        self.pdfWeights = []
        if hasattr(self.cfg_ana, "PDFWeights") and len(self.cfg_ana.PDFWeights) > 0:
            self.pdfWeights = self.cfg_ana.PDFWeights
            for (pdf,nvals) in self.pdfWeights:
                for i in range(nvals):
                    var( tr, 'pdfWeight_%s_%d' % (pdf,i))
 
    def fillMCVariables(self, iEvent, event):
        tr = self.tree
        fill( tr, 'GenHiggsDecayMode', event.genHiggsDecayMode )
        fill( tr, 'GenHeaviestQCDFlavour', event.heaviestQCDFlavour )

        if event.genHiggsBoson: 
            fillGenParticle(tr, 'GenHiggs', event.genHiggsBoson)

        fill( tr, 'nGenLeps', len(event.genleps) )
        fill( tr, 'nGenLepsFromTau', len(event.gentauleps) )
        fill( tr, 'nGenBQuarks', len(event.genbquarks) )
        fill( tr, 'nGenQuarks', len(event.genwzquarks) )
        
        for i in range(min(2,len(event.gentopquarks))):
            fillGenParticle(tr, 'GenTop%d'%(i+1), event.gentopquarks[i])
        for i in range(min(6,len(event.genleps))): 
            fillGenParticle(tr, 'GenLep%d'%(i+1), event.genleps[i], withSourceId=True)
        for i in range(min(6,len(event.gentauleps))): 
            fillGenParticle(tr, 'GenLepFromTau%d'%(i+1), event.gentauleps[i], withSourceId=True)
        for i in range(min(6,len(event.genwzquarks))): 
            fillGenParticle(tr, 'GenQuark%d'%(i+1), event.genwzquarks[i], withSourceId=True)
        for i in range(min(2,len(event.genbquarks))): 
            fillGenParticle(tr, 'GenBQuark%d'%(i+1), event.genbquarks[i])

        fill( tr, 'nGoodLepsMatchId',  sum([x.mcMatchId  > 0 for x in event.selectedLeptons]) )
        fill( tr, 'nGoodLepsMatchAny', sum([x.mcMatchAny > 0 for x in event.selectedLeptons]) )

        fill( tr, 'nGenJets25', event.nGenJets25)
        fill( tr, 'nGenJets25Cen', event.nGenJets25Cen)
        fill( tr, 'nGenJets25Fwd', event.nGenJets25Fwd)
         
        for (pdf,nvals) in self.pdfWeights:
            if len(event.pdfWeights[pdf]) != nvals:
                raise RuntimeError, "PDF lenght mismatch for %s, declared %d but the event has %d" % (pdf,nvals,event.pdfWeights[pdf])
            for i,w in enumerate(event.pdfWeights[pdf]):
                fill(tr, 'pdfWeight_%s_%d' % (pdf,i), w)
 
        
