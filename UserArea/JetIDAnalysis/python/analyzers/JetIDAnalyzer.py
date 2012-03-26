
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle,Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, matchObjectCollection2, deltaR2, deltaR

## This will load libCMGToolsExternal
## import CMGTools.External.External
import CMG.JetIDAnalysis.JetIDAnalysis
from ROOT import PileupJetIdNtupleAlgo 

## python 
from CMGTools.RootTools.utils.PileupJetHistograms import PileupJetHistograms, mkBinLabels, findBin
from CMGTools.RootTools.analyzers.SimpleJetAnalyzer import ResolutionJetHistograms, FractionJetHistograms, JetHistograms

from ROOT import TH1F, TH2F, TFile, THStack, TF1, TGraphErrors, TTree, gDirectory, AddressOf, gROOT
from array import array

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....
class JetIDAnalyzer (Analyzer) :
    '''A simple jet analyzer.'''
    
    def declareHandles (self) :
        super (JetIDAnalyzer, self).declareHandles ()
        self.handles['jets'] =  AutoHandle (
            *self.cfg_ana.jetCollection
            )
        if self.cfg_ana.useGenLeptons: 
            self.mchandles['leptons2'] =  AutoHandle (
                'genLeptonsStatus2',
                'std::vector<reco::GenParticle>'
                )
        else:
            self.mchandles['genParticles'] =  AutoHandle (
                'prunedGen',
                'std::vector<reco::GenParticle>'
                )
            
        ### self.mchandles['genJets'] =  AutoHandle (
        ###     *self.cfg_ana.genJetsCollection
        ###    )
        self.handles['vertices'] =  AutoHandle (
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
          )

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def beginLoop (self) :
        super (JetIDAnalyzer,self).beginLoop ()
        self.file = TFile ('/'.join ([self.looperName, 'testJets.root']),
                           'recreate')

        if self.cfg_ana.applyPFLooseId:
            from ROOT import PFJetIDSelectionFunctor 
            self.isPFLooseFunc = PFJetIDSelectionFunctor(0,PFJetIDSelectionFunctor.LOOSE)
            ## Workaround: for some reason PyROOT does not bind PFJetIDSelectionFunctor(Jet) nor PFJetIDSelectionFunctor.getBitsTemplates 
            from ROOT import pat        
            self.isPFLooseFunc.bits = pat.strbitset()
            for i in "CHF","NHF","CEF","NEF","NCH","nConstituents": self.isPFLooseFunc.bits.push_back(i) 
            ## /Workaround
            self.isPFLoose = lambda x : self.isPFLooseFunc(x,self.isPFLooseFunc.bits)
        else:
            self.isPFLoose = lambda x : True

        # general histograms
        self.jetHistos = JetHistograms ('Jets')
        self.cleanJetHistos = JetHistograms ('CleanJets')
        self.matchedCleanJetHistos = JetHistograms ('MatchedCleanJets')
        self.unmatchedCleanJetHistos = JetHistograms ('UnmatchedCleanJets')
        
        ## Pileup JetID
        if self.cfg_ana.jetIdMva:
            self.puidalgo = PileupJetIdNtupleAlgo(*self.cfg_ana.jetIdMva)
            self.runMva = True
        else:
            self.puidalgo = PileupJetIdNtupleAlgo()
            self.runMva = False
        
        self.vtxBins   = (0,10,15,20,30) ## (0,2,4,6,10,15,20,30,35)
        self.vtxBinLabels = mkBinLabels(self.vtxBins)
        self.ptBins    = (20,30,50) ## (20,30,40,50,100)
        self.etaBins   = (0,1.4,2.5,3.0)
        self.puEtaLables = ["_barrel","_endtk","_endNOtk","_fwd"]
        reweight_f = TF1("f","pol2(0)+expo(3)")
        reweight_f.SetParameters(0.1955298,-0.003830591,1.944794e-05,4.649755,-0.1722024)
        self.reweight = ("pt", reweight_f)

        ## PileUp histograms
        if self.cfg_ana.doJetIdHisto:
            # histograms for pileup jet identification variables
            
            self.gluCleanHistosId = PileupJetHistograms("GluonMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight)
            self.quarkCleanHistosId = PileupJetHistograms("QuarkMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight)
            self.reweiMatchedCleanHistosId = PileupJetHistograms("ReweiMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight)
            self.unmatchedCleanHistosId = PileupJetHistograms("UnmatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables)

            
        ### self.jetKin = []
        ### for vtx in self.vtxBinLabels:
        ###     self.jetKin.append([])
        ###     for eta in self.puEtaLables:
        ###         self.jetKin[-1].append( JetHistograms('Jets%s%s' % (vtx,eta) ) )
        
        if self.cfg_ana.dumpTree:
            self.tree = TTree("tree","tree")
            self.tree.SetDirectory(self.file)

            self.puidalgo.bookBranches(self.tree)
            
            gROOT.ProcessLine("""struct MyTreeVars{ Int_t nvtx; Bool_t isMatched; Int_t jetFlavour; Float_t partonPt; Float_t genPt; Float_t genDr; }""")
            from ROOT import MyTreeVars
            self.treevars = MyTreeVars()
            
            for i,j in ("nvtx","I"),("isMatched","O"),("jetFlavour","I"),("partonPt","F"),("genPt","F"),("genDr","F"):
                self.tree.Branch(i,AddressOf(self.treevars,i),"%s/%s"%(i,j))
            
        self.h_nvtx = TH1F ("h_nvtx", "" ,50, 0, 50)
        self.h_genjetspt = TH1F ("h_genjetspt", "" ,500, 0, 500) ; 
        

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....
    def process (self, iEvent, event) :
        #read all the handles defined beforehand
        self.readCollections (iEvent)
        
        jetEtaCut = 4.5 
        
        # get the vertexes
        event.vertices = self.handles['vertices'].product ()
        self.h_nvtx.Fill (len (event.vertices))
        event.vertexBin = int (len (event.vertices))
        
        # get the jets in the jets variable
        jets = self.handles['jets'].product ()
        # filter jets with some selections
        event.jets = [ jet for jet in jets if ( abs(jet.eta()) < jetEtaCut and jet.pt()>self.cfg_ana.ptCut and self.isPFLoose(jet) ) ]
        self.jetHistos.fillEvent (event.jets)
        
        try:
            # get status 2 leptons
            if 'leptons2' in self.mchandles:
                event.genLeptons = map (GenParticle, self.mchandles['leptons2'].product ())
            else:
                event.genLeptons = [ lep for lep in self.mchandles['genParticles'].product() if lep.status() == 3 and (abs(lep.pdgId()) == 11 or abs(lep.pdgId()) == 13 or abs(lep.pdgId()) == 15) ]  
        except:
            event.genLeptons = None

        if event.genLeptons:
        #FIXME why are there cases in which there's 4 or 6 leptons?
            if len (event.genLeptons) != 2 :
                return
            # in case I want to filter out taus
            # 11, 13, 15 : e, u, T
            # remove leptons from jets if closer than 0.2
            event.cleanJets = cleanObjectCollection (event.jets, event.genLeptons, 0.2)
        else:
            event.cleanJets = event.jets
            
        ### event.matchingCleanJets = matchObjectCollection2 (event.cleanJets, event.selGenJets, 0.25)
        # assign to each jet its gen match (easy life :))
        event.selGenJets = []
        for jet in event.cleanJets :
            try:
                jet.gen = jet.genJet()
                jet.gen.pt()
                event.selGenJets.append(jet.gen) 
            except:
                jet.gen = None

        for jet in event.selGenJets : 
            self.h_genjetspt.Fill (jet.pt ())
        self.jetHistos.fillStats (len (event.selGenJets), len (event.jets))
        
        ### try:
        ###     # get genJets
        ###     event.genJets = map (GenJet, self.mchandles['genJets'].product ())
        ###     event.selGenJets = event.genJets
        ###     for jet in event.selGenJets : 
        ###         self.h_genjetspt.Fill (jet.pt ())
        ###         
        ###     # first stats plots
        ###     self.jetHistos.fillStats (len (event.selGenJets), len (event.jets))
        ### except:
        ###     pass
           

        ## run the PileupJetIdAlgo and attach the id to the jet
        ijet = 0
        for jet in event.cleanJets:
            try:
                jet.puIdentifier = self.puidalgo.computeIdVariables(jet.sourcePtr().get(),0.,event.vertices[0],self.runMva)
            except:
                jet.puIdentifier = self.puidalgo.computeIdVariables(jet,0.,event.vertices[0],self.runMva)
            if self.cfg_ana.dumpTree:
                ## self.puidalgo.fillJet(jet.puIdentifier,ijet,0)
                self.puidalgo.setIJetIEvent(ijet,0)
                self.treevars.nvtx = len(event.vertices)
                self.treevars.isMatched = False
                self.treevars.jetFlavour = -999
                self.treevars.genPt = 0.
                self.treevars.partonPt = 0.
                self.treevars.genDr = -100.
                if jet.gen:
                    self.treevars.isMatched  = True
                    self.treevars.jetFlavour = jet.partonFlavour()
                    self.treevars.genPt = jet.gen.pt()
                    self.treevars.genDr = deltaR(jet.gen.eta(),jet.gen.phi(),jet.eta(),jet.phi())
                    try:
                        self.treevars.partonPt = jet.genParton.pt()
                    except:
                        pass
                self.tree.Fill()
            ijet += 1

        self.cleanJetHistos.fillEvent (event.cleanJets)
        
        event.matchedCleanJets = [jet for jet in event.cleanJets if jet.gen != None]

        event.unmatchedCleanJets = [jet for jet in event.cleanJets if jet.gen == None]
        self.matchedCleanJetHistos.fillEvent (event.matchedCleanJets)
        self.unmatchedCleanJetHistos.fillEvent (event.unmatchedCleanJets)

        event.cleanGluJets = []
        event.cleanQuarkJets = []
        vtxbin = findBin(self.vtxBinLabels,len(event.vertices))
        for jet in event.matchedCleanJets:
            flav = abs(jet.partonFlavour()) 
            if flav == 21:
                event.cleanGluJets.append(jet)
            elif flav > 0 and flav <= 3:
                event.cleanQuarkJets.append(jet)
            jet.weight = self.reweight[1](getattr(jet,self.reweight[0])())
            etabin = findBin(self.etaBins,jet.eta())
            ### self.jetKin[vtxbin][etabin].fillJet(jet)

        if self.cfg_ana.doJetIdHisto:
            self.gluCleanHistosId.fillEvent(event.cleanGluJets,event.vertices)
            self.quarkCleanHistosId.fillEvent(event.cleanQuarkJets,event.vertices)
            ### self.matchedCleanHistosId.fillEvent(event.matchedCleanJets,event.vertices)
            self.reweiMatchedCleanHistosId.fillEvent(event.matchedCleanJets,event.vertices)
            self.unmatchedCleanHistosId.fillEvent(event.unmatchedCleanJets,event.vertices)
        
        
            

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def write (self):
        from ROOT import gROOT
        gROOT.SetBatch(True)

        self.jetHistos.Write (self.file)
        self.cleanJetHistos.Write (self.file)
        
        self.matchedCleanJetHistos.Write (self.file)
        self.unmatchedCleanJetHistos.Write (self.file)

        if self.cfg_ana.doJetIdHisto:
            self.gluCleanHistosId.summary()
            self.gluCleanHistosId.Write(self.file)
            
            self.quarkCleanHistosId.summary()
            self.quarkCleanHistosId.Write(self.file)
            
            self.reweiMatchedCleanHistosId.summary()
            self.reweiMatchedCleanHistosId.Write(self.file)
        
            ### self.matchedCleanHistosId.Write(self.file)
            self.unmatchedCleanHistosId.Write(self.file)
        
        ### for j in self.jetKin:
        ###     for h in j:
        ###         h.Write(self.file)
                
        self.file.cd ()
        self.h_nvtx.Write ()
        self.h_genjetspt.Write ()
        
        if self.cfg_ana.dumpTree:
            self.tree.Write()
        
        self.file.Close()
        
