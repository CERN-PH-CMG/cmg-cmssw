
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle,Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, matchObjectCollection2, deltaR2
from CMGTools.RootTools.utils.PileupJetHistograms import PileupJetHistograms
from CMGTools.RootTools.analyzers.SimpleJetAnalyzer import ResolutionJetHistograms, FractionJetHistograms, JetHistograms

from ROOT import TH1F, TH2F, TFile, THStack, TF1, TGraphErrors

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
            
        self.mchandles['genJets'] =  AutoHandle (
            *self.cfg_ana.genJetsCollection
           )
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
            ## Workaround: for some reason PyROOT does not bind nor PFJetIDSelectionFunctor(Jet)PFJetIDSelectionFunctor.getBitsTemplates 
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
        self.matchedCleanJetHistos_barrel = JetHistograms ('MatchedCleanJets_barrel')
        self.matchedCleanJetHistos_endtk = JetHistograms ('MatchedCleanJets_endtk')
        self.matchedCleanJetHistos_endNOtk = JetHistograms ('MatchedCleanJets_endNOtk')
        self.matchedCleanJetHistos_fwd = JetHistograms ('MatchedCleanJets_fwd')
        self.LPtmatchedCleanJetHistos = JetHistograms ('LPtMatchedCleanJets') 
        self.HPtmatchedCleanJetHistos = JetHistograms ('HPtMatchedCleanJets')
        self.unmatchedCleanJetHistos = JetHistograms ('UnmatchedCleanJets')
        self.LPtUnmatchedCleanJetHistos = JetHistograms ('LPtUnmatchedCleanJets') 
        self.HPtUnmatchedCleanJetHistos = JetHistograms ('HPtUnmatchedCleanJets')

        # histograms of the components fraction
        self.matchedCleanJetHistosComponents = FractionJetHistograms ('MatchedCleanJetsCompontents')
        self.unmatchedCleanJetHistosComponents = FractionJetHistograms ('UnmatchedCleanJetsCompontents')

        # histograms for the resolution of matched jets
        self.matchedCleanJetHistosResolution = ResolutionJetHistograms ('MatchedCleanJetsResolution', 50, 1)
        self.matchedCleanJetHistosResolution_barrel = ResolutionJetHistograms ('MatchedCleanJetsResolution_barrel', 50, 1)
        self.matchedCleanJetHistosResolution_endtk = ResolutionJetHistograms ('MatchedCleanJetsResolution_endtk', 50, 1)
        self.matchedCleanJetHistosResolution_endNOtk = ResolutionJetHistograms ('MatchedCleanJetsResolution_endNOtk', 50, 1)
        self.matchedCleanJetHistosResolution_fwd = ResolutionJetHistograms ('MatchedCleanJetsResolution_fwd', 50, 1)

        if self.cfg_ana.doJetIdHisto:
            # histograms for pileup jet identification variables
            self.vtxBins   = (0,5,10,15,20,30) ## (0,2,4,6,10,15,20,30,35)
            self.ptBins    = (20,30,50) ## (20,30,40,50,100)
            self.etaBins   = (0,1.4,2.5,3.0)
            self.puEtaLables = ["_barrel","_endtk","_endNOtk","_fwd"]
            reweight_f = TF1("f","pol2(0)+expo(3)")
            reweight_f.SetParameters(0.1955298,-0.003830591,1.944794e-05,4.649755,-0.1722024)
            self.reweight = ("pt", reweight_f)
            self.doJetIdHisto = True
            self.gluCleanHistosId = PileupJetHistograms("GluonMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight,
                                                        jetIdMva=self.cfg_ana.jetIdMva)
            self.quarkCleanHistosId = PileupJetHistograms("QuarkMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight,
                                                          jetIdMva=self.cfg_ana.jetIdMva)
            self.reweiMatchedCleanHistosId = PileupJetHistograms("ReweiMatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,reweight=self.reweight,
                                                                 jetIdMva=self.cfg_ana.jetIdMva)
            self.unmatchedCleanHistosId = PileupJetHistograms("UnmatchedCleanHistosId",self.vtxBins,self.ptBins,self.etaBins,etalabels=self.puEtaLables,
                                                              jetIdMva=self.cfg_ana.jetIdMva)
            
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
        
        # get status 2 leptons
        if 'leptons2' in self.mchandles:
            event.genLeptons = map (GenParticle, self.mchandles['leptons2'].product ())
        else:
            event.genLeptons = [ lep for lep in self.mchandles['genParticles'].product() if lep.status() == 3 and (abs(lep.pdgId()) == 11 or abs(lep.pdgId()) == 13 or abs(lep.pdgId()) == 15) ]  
#        event.selGenLeptons = [GenParticle (lep) for lep in event.genLeptons if (lep.pt ()>self.cfg_ana.ptCut and abs (lep.eta ()) < jetEtaCut)]
        
        # get genJets
        event.genJets = map (GenJet, self.mchandles['genJets'].product ())
        # filter genjets as for reco jets
        # event.selGenJets = [GenJet (jet) for jet in event.genJets if (jet.pt ()>self.cfg_ana.ptCut*.5)]
        event.selGenJets = event.genJets
        for jet in event.selGenJets : 
            self.h_genjetspt.Fill (jet.pt ())
        
        # first stats plots
        # print 'genLeptons : ' + repr (len (event.genLeptons)) + ' | genJets : ' + repr (len (event.genJets)) + ' | recoJets : ' + repr (len (event.jets))
        self.jetHistos.fillStats (len (event.selGenJets), len (event.jets))
        
        #FIXME why are there cases in which there's 4 or 6 leptons?
        if len (event.genLeptons) != 2 :
            return
        # in case I want to filter out taus
        # 11, 13, 15 : e, u, T
#        event.genOneLepton = [GenParticle (part) for part in event.genLeptons if abs (part.pdgId ()) == 15]
        # remove leptons from jets if closer than 0.2
        event.cleanJets = cleanObjectCollection (event.jets, event.genLeptons, 0.2)

        event.matchingCleanJets = matchObjectCollection2 (event.cleanJets, event.selGenJets, 0.25)
        # assign to each jet its gen match (easy life :))
        for jet in event.cleanJets :
            jet.gen = event.matchingCleanJets[ jet ]
        # FIXME next step might be to put this in the matching and remove the boolean flags

        self.cleanJetHistos.fillEvent (event.cleanJets)
        
        event.matchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None]
        event.cleanGluJets = []
        event.cleanQuarkJets = []
        for jet in event.matchedCleanJets:
            flav = abs(jet.partonFlavour()) 
            if flav == 21:
                event.cleanGluJets.append(jet)
            elif flav > 0 and flav <= 3:
                event.cleanQuarkJets.append(jet)

        event.LPtmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None and jet.pt () <= 30]
        event.HPtmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None and jet.pt () > 30]

        self.matchedCleanJetHistos.fillEvent (event.matchedCleanJets)
        self.LPtmatchedCleanJetHistos.fillEvent (event.LPtmatchedCleanJets)
        self.HPtmatchedCleanJetHistos.fillEvent (event.HPtmatchedCleanJets)

        event.unmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen == None]
        event.LPtunmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen == None and jet.pt () <= 30]
        event.HPtunmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen == None and jet.pt () > 30]
        
        self.unmatchedCleanJetHistos.fillEvent (event.unmatchedCleanJets)
        self.LPtUnmatchedCleanJetHistos.fillEvent (event.LPtunmatchedCleanJets)
        self.HPtUnmatchedCleanJetHistos.fillEvent (event.HPtunmatchedCleanJets)
        
        self.matchedCleanJetHistosComponents.fillEvent (event.matchedCleanJets)
        
        self.unmatchedCleanJetHistosComponents.fillEvent (event.unmatchedCleanJets)
        
        self.matchedCleanJetHistosResolution.fillEvent (event.matchedCleanJets, len (event.vertices))
        
        for jet in event.matchedCleanJets :
            index = int (jet.pt ()) / 20 
            if (index) > 9 : continue
            if abs (jet.gen.eta ()) < 1.4 :
                self.matchedCleanJetHistosResolution_barrel.fillJet (jet, len (event.vertices))
                self.matchedCleanJetHistos_barrel.fillJet (jet)
            elif 1.6 < abs (jet.gen.eta ()) < 2.5 :    
                self.matchedCleanJetHistosResolution_endtk.fillJet (jet, len (event.vertices))
                self.matchedCleanJetHistos_endtk.fillJet (jet)
            elif 2.6 < abs (jet.gen.eta ()) < 2.9 :    
                self.matchedCleanJetHistosResolution_endNOtk.fillJet (jet, len (event.vertices))
                self.matchedCleanJetHistos_endNOtk.fillJet (jet)
            elif 3.1 < abs (jet.gen.eta ()) :    
                self.matchedCleanJetHistosResolution_fwd.fillJet (jet, len (event.vertices))
                self.matchedCleanJetHistos_fwd.fillJet (jet)

        if self.doJetIdHisto:
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
        self.matchedCleanJetHistos_barrel.Write (self.file)
        self.matchedCleanJetHistos_endtk.Write (self.file)
        self.matchedCleanJetHistos_endNOtk.Write (self.file)
        self.matchedCleanJetHistos_fwd.Write (self.file)
        
        self.LPtmatchedCleanJetHistos.Write (self.file)
        self.HPtmatchedCleanJetHistos.Write (self.file)
        self.LPtUnmatchedCleanJetHistos.Write (self.file)
        self.HPtUnmatchedCleanJetHistos.Write (self.file)

        self.unmatchedCleanJetHistos.Write (self.file)

        self.matchedCleanJetHistosComponents.summary ()
        self.matchedCleanJetHistosComponents.Write (self.file)
        self.unmatchedCleanJetHistosComponents.summary ()
        self.unmatchedCleanJetHistosComponents.Write (self.file)

        self.matchedCleanJetHistosResolution.summary ()
        self.matchedCleanJetHistosResolution.Write (self.file)
        
        self.matchedCleanJetHistosResolution_barrel.summary ()
        self.matchedCleanJetHistosResolution_barrel.Write (self.file)
        
        self.matchedCleanJetHistosResolution_endtk.summary ()
        self.matchedCleanJetHistosResolution_endtk.Write (self.file)
        
        self.matchedCleanJetHistosResolution_endNOtk.summary ()
        self.matchedCleanJetHistosResolution_endNOtk.Write (self.file)
        
        self.matchedCleanJetHistosResolution_fwd.summary ()
        self.matchedCleanJetHistosResolution_fwd.Write (self.file)

        if self.doJetIdHisto:
            self.gluCleanHistosId.summary()
            self.gluCleanHistosId.Write(self.file)
            
            self.quarkCleanHistosId.summary()
            self.quarkCleanHistosId.Write(self.file)
            
            self.reweiMatchedCleanHistosId.summary()
            self.reweiMatchedCleanHistosId.Write(self.file)
        
            ### self.matchedCleanHistosId.Write(self.file)
            self.unmatchedCleanHistosId.Write(self.file)
        
        self.file.cd ()
        self.h_nvtx.Write ()
        self.h_genjetspt.Write ()
        
        self.file.Close()
        
