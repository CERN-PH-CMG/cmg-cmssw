
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle,Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, matchObjectCollection2, deltaR2
from CMGTools.RootTools.utils.PileupJetHistograms import PileupJetHistograms
## from CMGTools.RootTools.RootTools import loadLibs

from ROOT import TNtuple, TH1F, TH2F, TFile, THStack, TF1, TGraphErrors


class SimpleJetPlots (Analyzer) :
    '''dump very few quantities into a TNtuple for jet resolution studies.'''
    ### def __init__(self,cfg_ana, cfg_comp, looperName):
    ###     loadLibs()
    ###     super (SimpleJetPlots, self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles (self) :
        super (SimpleJetPlots, self).declareHandles ()
        self.handles['YeCHSjets'] =  AutoHandle (
            *self.cfg_ana.YeCHSjetCollection
            )
        self.handles['NoCHSjets'] =  AutoHandle (
            *self.cfg_ana.NoCHSjetCollection
            )
        if self.cfg_ana.useGenLeptons: 
            self.mchandles['genParticlesPruned'] =  AutoHandle (
                'genParticlesPruned',
                'std::vector<reco::GenParticle>'
                )
        else:
            self.mchandles['genParticles'] =  AutoHandle (
                'prunedGen',
                'std::vector<reco::GenParticle>'
                )
            
        self.mchandles['NoCHSgenJets'] =  AutoHandle (
            *self.cfg_ana.NoCHSgenJetsCollection
           )
        self.mchandles['YeCHSgenJets'] =  AutoHandle (
            *self.cfg_ana.YeCHSgenJetsCollection
           )
        self.handles['vertices'] =  AutoHandle (
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
          )

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def beginLoop (self) :
        super (SimpleJetPlots,self).beginLoop ()
        self.file = TFile ('/'.join ([self.looperName, 'testJetsPlot.root']),
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

        self.h_NoCHS_genjetspt        = TH1F ("h_NoCHS_genjetspt", "" ,500, 0, 500) ; 
        self.h_NoCHS_genjetseta       = TH1F ("h_NoCHS_genjetseta", "" ,48, -6, 6) ; 
        self.h_YeCHS_genjetspt        = TH1F ("h_YeCHS_genjetspt", "" ,500, 0, 500) ; 
        self.h_YeCHS_genjetseta       = TH1F ("h_YeCHS_genjetseta", "" ,48, -6, 6) ; 
        
        self.h_NoCHS_genjetspt_match  = TH1F ("h_NoCHS_genjetspt_match", "" ,500, 0, 500) ; 
        self.h_NoCHS_genjetseta_match = TH1F ("h_NoCHS_genjetseta_match", "" ,48, -6, 6) ;
        self.h_YeCHS_genjetspt_match  = TH1F ("h_YeCHS_genjetspt_match", "" ,500, 0, 500) ; 
        self.h_YeCHS_genjetseta_match = TH1F ("h_YeCHS_genjetseta_match", "" ,48, -6, 6) ;
        
        self.h_NoCHS_recjetspt        = TH1F ("h_NoCHS_recjetspt", "" ,500, 0, 500) ; 
        self.h_NoCHS_recjetspt_match  = TH1F ("h_NoCHS_recjetspt_match", "" ,500, 0, 500) ; 
        self.h_NoCHS_recjetseta       = TH1F ("h_NoCHS_recjetseta", "" ,48, -6, 6) ; 
        self.h_NoCHS_recjetseta_match = TH1F ("h_NoCHS_recjetseta_match", "" ,48, -6, 6) ;
        
        self.h_YeCHS_recjetspt        = TH1F ("h_YeCHS_recjetspt", "" ,500, 0, 500) ; 
        self.h_YeCHS_recjetspt_match  = TH1F ("h_YeCHS_recjetspt_match", "" ,500, 0, 500) ; 
        self.h_YeCHS_recjetseta       = TH1F ("h_YeCHS_recjetseta", "" ,48, -6, 6) ; 
        self.h_YeCHS_recjetseta_match = TH1F ("h_YeCHS_recjetseta_match", "" ,48, -6, 6) ;
        
        self.h_YeCHS_response = TH1F ("h_YeCHSresponse", "" ,100, -2, 2) ;
        self.h_NoCHS_response = TH1F ("h_NoCHSresponse", "" ,100, -2, 2) ;

        self.h_NoCHS_correction = TH2F ("h_NoCHS_correction", "", 48, -6, 6, 1000, 0, 100) ;
        self.h_YeCHS_correction = TH2F ("h_YeCHS_correction", "", 48, -6, 6, 1000, 0, 100) ;



# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....
    def process (self, iEvent, event) :
        #read all the handles defined beforehand
        self.readCollections (iEvent)
        
        jetEtaCut = 4.5 
        
        event.vertices = self.handles['vertices'].product ()
        
        YeCHSjets = self.handles['YeCHSjets'].product ()
        event.YeCHSjets = [ jet for jet in YeCHSjets if ( abs(jet.eta()) < jetEtaCut and jet.pt()>self.cfg_ana.ptCut and self.isPFLoose(jet) ) ]
        NoCHSjets = self.handles['NoCHSjets'].product ()
        event.NoCHSjets = [ jet for jet in NoCHSjets if ( abs(jet.eta()) < jetEtaCut and jet.pt()>self.cfg_ana.ptCut and self.isPFLoose(jet) ) ]
        
        # get status 2 leptons
        if 'genParticlesPruned' in self.mchandles:
            event.genLeptons = [ lep for lep in self.mchandles['genParticlesPruned'].product() if lep.status() == 2 and (abs(lep.pdgId()) == 11 or abs(lep.pdgId()) == 13 or abs(lep.pdgId()) == 15) ]  
        else:
            event.genLeptons = [ lep for lep in self.mchandles['genParticles'].product() if lep.status() == 3 and (abs(lep.pdgId()) == 11 or abs(lep.pdgId()) == 13 or abs(lep.pdgId()) == 15) ]  
        
        event.NoCHSgenJets = map (GenJet, self.mchandles['NoCHSgenJets'].product ())
        event.selNoCHSGenJets = [GenJet (jet) for jet in event.NoCHSgenJets if (jet.pt ()>self.cfg_ana.genPtCut)]
        event.YeCHSgenJets = map (GenJet, self.mchandles['YeCHSgenJets'].product ())
        event.selYeCHSGenJets = [GenJet (jet) for jet in event.YeCHSgenJets if (jet.pt ()>self.cfg_ana.genPtCut)]
        
        if len (event.genLeptons) != 2 :
            return
        
        for jet in event.selNoCHSGenJets:
            self.h_NoCHS_genjetspt.Fill (jet.pt ()) ;
            self.h_NoCHS_genjetseta.Fill (jet.eta ()) ;
        for jet in event.selYeCHSGenJets:
            self.h_YeCHS_genjetspt.Fill (jet.pt ()) ;
            self.h_YeCHS_genjetseta.Fill (jet.eta ()) ;

        event.cleanYeCHSJets = cleanObjectCollection (event.YeCHSjets, event.genLeptons, 0.2)
        event.cleanNoCHSJets = cleanObjectCollection (event.NoCHSjets, event.genLeptons, 0.2)
        for jet in event.cleanYeCHSJets :
            self.h_YeCHS_recjetspt       .Fill (jet.pt ()) ;
            self.h_YeCHS_recjetseta      .Fill (jet.eta ()) ; 
            if (jet.pt () > 20 and len(event.vertices) > 10):
                self.h_YeCHS_correction.Fill (jet.eta (), jet.jecFactor(0)) ;
                
        for jet in event.cleanNoCHSJets:
            self.h_NoCHS_recjetspt       .Fill (jet.pt ()) ;
            self.h_NoCHS_recjetseta      .Fill (jet.eta ()) ; 
            if (jet.pt () > 20 and len(event.vertices) > 10):
                self.h_NoCHS_correction.Fill (jet.eta (), jet.jecFactor(0)) ;
                
        
        event.matchingCleanYeCHSJets = matchObjectCollection2 (event.cleanYeCHSJets, event.selYeCHSGenJets, 0.25)
        for jet in event.cleanYeCHSJets :
            jet.gen = event.matchingCleanYeCHSJets[ jet ]
        event.matchedCleanYeCHSJets = [jet for jet in event.matchingCleanYeCHSJets if jet.gen != None]

        for jet in event.matchedCleanYeCHSJets:
            self.h_YeCHS_recjetspt_match .Fill (jet.pt ()) ; 
            self.h_YeCHS_recjetseta_match.Fill (jet.eta ()) ;
            self.h_YeCHS_genjetspt_match .Fill (jet.gen.pt ()) ;
            self.h_YeCHS_genjetseta_match.Fill (jet.gen.eta ()) ;

        event.matchingCleanNoCHSJets = matchObjectCollection2 (event.cleanNoCHSJets, event.selNoCHSGenJets, 0.25)
        for jet in event.cleanNoCHSJets :
            jet.gen = event.matchingCleanNoCHSJets[ jet ]
        event.matchedCleanNoCHSJets = [jet for jet in event.matchingCleanNoCHSJets if jet.gen != None]
        
        for jet in event.matchedCleanNoCHSJets:
            self.h_NoCHS_recjetspt_match .Fill (jet.pt ()) ; 
            self.h_NoCHS_recjetseta_match.Fill (jet.eta ()) ;
            self.h_NoCHS_genjetspt_match .Fill (jet.gen.pt ()) ;
            self.h_NoCHS_genjetseta_match.Fill (jet.gen.eta ()) ;
        
        if len (event.vertices) != 10 :
            return ;
        for jet in event.matchedCleanNoCHSJets:
            if (jet.gen.pt () < 20 or jet.gen.pt () > 30 or abs (jet.gen.eta ()) > 1.4) : continue ; 
            self.h_NoCHS_response.Fill ((jet.pt () - jet.gen.pt ()) / jet.gen.pt ()) ;
        for jet in event.matchedCleanYeCHSJets:
            if (jet.gen.pt () < 20 or jet.gen.pt () > 30 or abs (jet.gen.eta ()) > 1.4) : continue ; 
            self.h_YeCHS_response.Fill ((jet.pt () - jet.gen.pt ()) / jet.gen.pt ()) ;


# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def write (self):
        from ROOT import gROOT
        gROOT.SetBatch(True)
        self.file.cd ()
        self.h_NoCHS_genjetspt        .Write () ;
        self.h_NoCHS_genjetseta       .Write () ;
        self.h_YeCHS_genjetspt        .Write () ;
        self.h_YeCHS_genjetseta       .Write () ;
        
        self.h_NoCHS_genjetspt_match  .Write () ;
        self.h_NoCHS_genjetseta_match .Write () ;
        self.h_YeCHS_genjetspt_match  .Write () ;
        self.h_YeCHS_genjetseta_match .Write () ;
        
        self.h_NoCHS_recjetspt        .Write () ;
        self.h_NoCHS_recjetspt_match  .Write () ;
        self.h_NoCHS_recjetseta       .Write () ;
        self.h_NoCHS_recjetseta_match .Write () ;
        
        self.h_YeCHS_recjetspt        .Write () ;
        self.h_YeCHS_recjetspt_match  .Write () ;
        self.h_YeCHS_recjetseta       .Write () ;
        self.h_YeCHS_recjetseta_match .Write () ;
        
        self.h_YeCHS_response         .Write () ;
        self.h_NoCHS_response         .Write () ;
        
        self.h_YeCHS_correction       .Write () ;
        self.h_NoCHS_correction       .Write () ;
        
        self.file.Close()
        
