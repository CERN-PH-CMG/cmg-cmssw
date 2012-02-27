# there is 2, 4, 6 leptons, how comes there's so many sometimes? what is the frequency? make a plot
# the lepton-jet matching is always verified, ok to clean w/ gen leptons (for each lep there's almost always a jet closeby)
#    - nb almost, there's always some grass
#         - check this with lepton multiplicity... no, the grass remains
#        - check this with lepton flavour
# matching gen-reco
#    - test the DR... there's cases with big values, let's try and remove them if dR2 > 0.3
#        - removes the low mass peaks in the resolution plot
#        - how many times does this happen? 1314 / 8520 survive the selection... very few!
#        - where does this happen?
#    - the new univoque matcher implemented, I have some python failures
#        - fixed with Colin
#    - why the spikes around 2.5 do not have an associated gen jet?
#        - check this with the new matcher: at most not
#        - look at the distribution of gen jets 

# - I can have a look at jet conponents
# - remove taus


from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Average import Average
from CMGTools.RootTools.statistics.Histograms import Histograms
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle,Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, matchObjectCollection2, deltaR2

from ROOT import TH1F, TH2F, TFile, THStack, TF1, TGraphErrors


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class ResolutionJetHistograms (Histograms) :
    ''' energy resolution as a function of the jet eta for different number of vertexes'''
    def __init__ (self, name, maxVtx = 50, vtxBinning = 5) :
        self.maxVtx = maxVtx
        self.vtxBinning = vtxBinning
        self.listLen = int (self.maxVtx) / int (self.vtxBinning)
        self.histosEta = []
        self.histosPt = []
        for i in range (self.listLen) : 
            self.histosEta.append (TH2F (name + '_h_dpt_eta_' + str (i), '', 24, -6, 6, 100, -2, 2))
            self.histosPt.append (TH2F (name + '_h_dpt_pt_' + str (i), '', 10, 0, 200, 100, -2, 2))
        super (ResolutionJetHistograms, self).__init__ (name)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillJet (self, jet, nVtx) :
        if nVtx < self.maxVtx : 
            index = int (nVtx) / int (self.vtxBinning)
            self.histosEta[index].Fill (jet.gen.eta (), (jet.pt () - jet.gen.pt ()) / jet.gen.pt ())
            self.histosPt[index].Fill (jet.gen.pt (), (jet.pt () - jet.gen.pt ()) / jet.gen.pt ())
        else : print 'the vertex number: ' + str (nVtx) + ' is too high'

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillEvent (self, jets, nVtx) :
        for jet in jets :
            self.fillJet (jet, nVtx)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def summary (self) :
        '''to be run after the event loop, before saving'''
        self.ptResolutions = []
        self.etaResolutions = []
        for i in range (self.listLen) :
            self.ptResolutions.append (self.GetSigmaGraph (self.histosPt[i]))
            self.etaResolutions.append (self.GetSigmaGraph (self.histosEta[i]))

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def Write (self, dir) :
        '''overloads mother function, to save lists contents'''
        self.dir = dir.mkdir( self.name )
        self.dir.cd ()
        for i in range (len (self.ptResolutions)) : self.ptResolutions[i].Write ('pt_graph_' + str (i))
        for i in range (len (self.etaResolutions)) : self.etaResolutions[i].Write ('eta_graph_' + str (i))
        for i in range (len (self.histosPt)) : self.histosPt[i].Write ()
        for i in range (len (self.histosEta)) : self.histosEta[i].Write ()
        dir.cd ()

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def doubleFit (self, histo, k = 1.) :            
        '''double iterative gaussian fit'''
        #FIXME put in into an external module
        min = histo.GetMean () - 1.5 * k * histo.GetRMS () # FIXME 1.5 is maybe useless?
        max = histo.GetMean () + 1.5 * k * histo.GetRMS () # FIXME 1.5 is maybe useless?
        self.func = TF1 ('gauss','gaus', 0, 2) #FIXME do I want "self" here? the variable is local...
        histo.Fit (self.func, '', '', min, max)
        min = self.func.GetParameter (1) - k * self.func.GetParameter (2)
        max = self.func.GetParameter (1) + k * self.func.GetParameter (2)
        histo.Fit (self.func, '+', '', min, max)
        return [self.func.GetParameter (1), self.func.GetParameter (2), self.func.GetParError (1), self.func.GetParError (2)] #FIXME would it be better to return a tuple?

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def GetSigmaGraph (self, histo) :
        '''more sophisticated FitSlicesY'''
        # if the first gaus fit is not easy, I could try to start from the TProfile of the TH2F
        # or from the default FitSlicesY
        #FIXME put in into an external module
        # create the resulting histo
        self.resol_vs_var = TGraphErrors ()
        self.minEntriesNum = 10 #FIXME want it configurable

        # get the sigmas
        for xBin in range (1, histo.GetNbinsX ()) :
            # get a slice of the histogram
            #FIXME do I want the slide to be integrated possibly on more than a single bin?
            aSlice = histo.ProjectionY ('temp', xBin, xBin, 'e') 
            if aSlice.GetEntries () < self.minEntriesNum : 
                self.resol_vs_var.SetPoint (xBin, histo.GetXaxis ().GetBinCenter (xBin), 0.)
                self.resol_vs_var.SetPointError (xBin, 0., 0.)
            else :                
                # fit the slice w/a gaussian within the range of the gaussian
                res = self.doubleFit (aSlice)
                # get the sigma
                self.resol_vs_var.SetPoint (xBin, histo.GetXaxis ().GetBinCenter (xBin), res[1])
                self.resol_vs_var.SetPointError (xBin, 0., res[3])
        return self.resol_vs_var


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


# what the jet components are
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/AnalysisDataFormats/CMGTools/interface/PFJet.h?revision=1.4&view=markup

#    enum ParticleType {
#              X=0,     // undefined
#                    h,       // charged hadron
#                    e,       // electron
#                    mu,      // muon
#                    gamma,   // photon
#                    h0,      // neutral hadron
#                    h_HF,        // HF tower identified as a hadron
#                    egamma_HF    // HF tower identified as an EM particle
#                  };


class FractionJetHistograms (Histograms) :
    '''eta distribution of the energy fraction per component'''
    def __init__ (self, name) :
        self.histos = []
        for i in range (8) : # NB here we start from 0 on purpose, for simplicity
            self.histos.append (TH2F (name + '_' + str (len (self.histos)), '', 240, -6, 6, 100, 0, 1))
        super (FractionJetHistograms, self).__init__ (name)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillJet (self, jet) :
        for i in range (1, 8) :
            self.histos[i].Fill (jet.eta (), jet.component (i).fraction ())

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillEvent (self, jets) :
        for jet in jets:
            self.fillJet (jet)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def summary (self) :
        '''to be run after the event loop, before saving'''
        self.summ = THStack (self.name + '_summ', 'total energy')
        self.mean = []
        for i in range (1, 8) :
#            self.mean.append (self.histos[i].ProfileX ())
            self.mean.append (self.fromProfileToHisto (self.histos[i].ProfileX (), 10 + i * 2))
            self.summ.Add (self.mean[len (self.mean) - 1])

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fromProfileToHisto (self, profile, color = 0) :
        thename = profile.GetName ().replace('pfx', 'ave')
        histo = TH1F (thename, '', profile.GetNbinsX (), profile.GetXaxis ().GetXmin (), profile.GetXaxis ().GetXmax ())
        histo.SetFillColor (color)
        for iBin in range (1, profile.GetNbinsX () + 1) :
            histo.SetBinContent (iBin, profile.GetBinContent (iBin))
            histo.SetBinError (iBin, profile.GetBinError (iBin))
        return histo

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def Write (self, dir) :
        '''overloads mother function, to save lists contents'''
        self.dir = dir.mkdir( self.name )
        self.dir.cd ()
        self.summ.Write ()
        for i in range (1, 8) :
            self.histos[i].Write ()
            self.mean[i-1].Write ()
        dir.cd ()
        # FIXME will the THStack be saved?      


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class JetHistograms (Histograms):
    '''general histograms for jets'''
    def __init__ (self, name) :
        self.h_pt = TH1F (name + '_h_pt', '', 100, 0, 200)
        self.h_genpt = TH1F (name + '_h_genpt', '', 100, 0, 200)
        self.h_geneta = TH1F (name + '_h_geneta', '', 240, -6, 6)
        self.h_dpt = TH1F (name + '_h_dpt', '', 100, -2, 2)
        self.h_eta = TH1F (name + '_h_eta', '', 240, -6, 6)
        self.h_comp = TH1F (name + '_h_comp', '', 10, 0, 10)
        self.h_deltaEleMatch = TH1F (name + '_h_deltaEleMatch', '', 1000, 0, 6)
        self.h_deltaJetMatch = TH1F (name + '_h_deltaJetMatch', '', 1000, 0, 6)
        self.h_numGen_numReco = TH2F (name + '_h_numGen_numReco', '', 20, 0, 20, 20, 0, 20)
        self.h_dpt_pt = TH2F (name + '_h_dpt_pt', '', 100, 0, 200, 100, -2, 2)
        self.h_dpt_eta = TH2F (name + '_h_dpt_eta', '', 240, -6, 6, 100, -2, 2)
        self.h_phi_eta = TH2F (name + '_h_phi_eta', '', 240, -6, 6, 360, -3.14, 3.14)
        self.h_dpt_dR2 = TH2F (name + '_h_dpt_dR2', '', 100, 0, 6, 100, -2, 2)
        self.h_ptr_ptg = TH2F (name + '_h_ptr_ptg', '', 100, 0, 200, 100, 0, 200)
        self.h_dR2_ptr = TH2F (name + '_h_dR2_ptr', '', 100, 0, 200, 100, 0, 6)
        self.h_dR2_eta = TH2F (name + '_h_dR2_eta', '', 240, -6, 6, 100, 0, 6)
        self.h_frac_com = TH2F (name + '_h_frac_com', '', 8, 0, 8, 10, 0, 1) # fraction, component
        super (JetHistograms, self).__init__ (name) #FIXME check that the super has to be called within __init__

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillJet (self, jet) :
        # import pdb
        # pdb.set_trace ()
        self.fillFrac (jet)
        self.h_pt.Fill (jet.pt ())
        self.h_eta.Fill (jet.eta ())
        self.h_phi_eta.Fill (jet.eta (), jet.phi ()) 
        if hasattr (jet, 'gen') and jet.gen is not None:
            dR2 = deltaR2 (jet.gen.eta (), jet.gen.phi (), jet.eta (), jet.phi ())
            self.h_deltaJetMatch.Fill (dR2)
            self.h_dpt_dR2.Fill (dR2, (jet.pt () - jet.gen.pt ()) / jet.gen.pt ())
            self.h_dR2_ptr.Fill (jet.gen.pt (), dR2)
            self.h_dR2_eta.Fill (jet.gen.eta (), dR2)
            if dR2 < 0.3 :
                self.h_genpt.Fill (jet.gen.pt ())
                self.h_geneta.Fill (jet.gen.pt ())
                self.h_dpt.Fill ((jet.pt () - jet.gen.pt ()) / jet.gen.pt ())
                self.h_dpt_pt.Fill (jet.gen.pt (), (jet.pt () - jet.gen.pt ()) / jet.gen.pt ()) 
                self.h_dpt_eta.Fill (jet.gen.eta (), (jet.pt () - jet.gen.pt ()) / jet.gen.pt ()) 
                self.h_ptr_ptg.Fill (jet.gen.pt (), jet.pt ()) 

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillFrac (self, jet) :
        for i in range (1, 8) :
            self.h_frac_com.Fill (i, jet.component (i).fraction ())

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillEvent (self, jets) :
        for jet in jets :
            self.fillJet (jet)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillStats (self, ngj, nrj) :
        self.h_numGen_numReco.Fill (ngj, nrj)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def fillEleMatch (self, delta) :
        self.h_deltaEleMatch.Fill (delta)

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def summary (self) :
        pass


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class SimpleJetAnalyzer (Analyzer) :
    '''A simple jet analyzer for Pietro.'''

    def declareHandles (self) :
        super (SimpleJetAnalyzer, self).declareHandles ()
        self.handles['jets'] =  AutoHandle (
            'cmgPFJetSel',
            'std::vector<cmg::PFJet>'
           )
        self.mchandles['leptons2'] =  AutoHandle (
            'genLeptonsStatus2',
            'std::vector<reco::GenParticle>'
           )
        self.mchandles['genJets'] =  AutoHandle (
            'genJetSel',
            'std::vector< cmg::PhysicsObjectWithPtr< edm::Ptr< reco::GenJet > > >'
           )
        self.handles['vertices'] =  AutoHandle (
            'offlinePrimaryVertices',
            'std::vector<reco::Vertex>'
          )

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def beginLoop (self) :
        super (SimpleJetAnalyzer,self).beginLoop ()
        self.file = TFile ('/'.join ([self.looperName, 'testJets.root']),
                           'recreate')
        # general histograms
        self.jetHistos = JetHistograms ('Jets')
        self.cleanJetHistos = JetHistograms ('CleanJets')
        self.matchedCleanJetHistos = JetHistograms ('MatchedCleanJets')
        self.LPtmatchedCleanJetHistos = JetHistograms ('LPtMatchedCleanJets') # low pt (< 40 GeV)
        self.HPtmatchedCleanJetHistos = JetHistograms ('HPtMatchedCleanJets') # high pt (> 40 GeV)
        self.matchedCleanJetHistosList = [JetHistograms ('MatchedCleanJets_'+ str (i)) for i in range (10)]
        self.LPtmatchedCleanJetHistosList = [JetHistograms ('LPtMatchedCleanJets_'+ str (i)) for i in range (10)]
        self.HPtmatchedCleanJetHistosList = [JetHistograms ('HPtMatchedCleanJets_'+ str (i)) for i in range (10)]
        self.unmatchedCleanJetHistos = JetHistograms ('UnmatchedCleanJets')

        self.matchedCleanJetHistosSuperList         = tuple (tuple (JetHistograms ('MatchedCleanJets_%d_%d' % (j,i)) for i in range (10)) for j in range (10))
        self.matchedCleanJetHistosSuperList_barrel  = tuple (tuple (JetHistograms ('MatchedCleanJets_barrel_%d_%d' % (j,i)) for i in range (10)) for j in range (10))
        self.matchedCleanJetHistosSuperList_endtk   = tuple (tuple (JetHistograms ('MatchedCleanJets_endtk_%d_%d' % (j,i)) for i in range (10)) for j in range (10))
        self.matchedCleanJetHistosSuperList_endNOtk = tuple (tuple (JetHistograms ('MatchedCleanJets_endNOtk_%d_%d' % (j,i)) for i in range (10)) for j in range (10))
        self.matchedCleanJetHistosSuperList_fwd     = tuple (tuple (JetHistograms ('MatchedCleanJets_fwd_%d_%d' % (j,i)) for i in range (10)) for j in range (10))

        # histograms of the components fraction
        self.matchedCleanJetHistosComponents = FractionJetHistograms ('MatchedCleanJetsCompontents')
        self.matchedCleanJetHistosComponentsList = [FractionJetHistograms ('MatchedCleanJetsCompontents_'+str (i)) for i in range (10)]

        self.unmatchedCleanJetHistosComponents = FractionJetHistograms ('UnmatchedCleanJetsCompontents')
        self.unmatchedCleanJetHistosComponentsList = [FractionJetHistograms ('UnmatchedCleanJetsCompontents_'+str (i)) for i in range (10)]

        # histograms for the resolution of matched jets
        self.matchedCleanJetHistosResolution = ResolutionJetHistograms ('MatchedCleanJetsResolution', 50, 5)

        self.h_nvtx = TH1F ("h_nvtx", "" ,50, 0, 50) ; 

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def process (self, iEvent, event) :
        #read all the handles defined beforehand
        self.readCollections (iEvent)
        
        jetEtaCut = 4.5 
        
        # get the vertexes
        event.vertices = self.handles['vertices'].product ()
        self.h_nvtx.Fill (len (event.vertices))
        event.vertexBin = int (len (event.vertices)) / 5
        
        # get the jets in the jets variable
        jets = self.handles['jets'].product ()
        # filter jets with some selections
        event.jets = [ Jet (jet) for jet in jets if (abs (jet.eta ()) < jetEtaCut)]
        self.jetHistos.fillEvent (event.jets)
        
        # get status 2 leptons
        event.genLeptons = map (GenParticle, self.mchandles['leptons2'].product ())
#        event.selGenLeptons = [GenParticle (lep) for lep in event.genLeptons if (lep.pt ()>self.cfg_ana.ptCut and abs (lep.eta ()) < jetEtaCut)]
        
        # get genJets
        event.genJets = map (GenJet, self.mchandles['genJets'].product ())
        # filter genjets as for reco jets
        event.selGenJets = [GenJet (jet) for jet in event.genJets if (jet.pt ()>self.cfg_ana.ptCut)]
        # event.selGenJets = event.genJets
        
        # first stats plots
        # print 'genLeptons : ' + repr (len (event.genLeptons)) + ' | genJets : ' + repr (len (event.genJets)) + ' | recoJets : ' + repr (len (event.jets))
        self.jetHistos.fillStats (len (event.selGenJets), len (event.jets))
        
        #FIXME why are there cases in which there's 4 or 6 leptons?
        if len (event.genLeptons) != 2 :
            return
        # in case I want to filter out taus
        # 11, 13, 15 : e, u, T
#        event.genOneLepton = [GenParticle (part) for part in event.genLeptons if abs (part.pdgId ()) == 15]
        
        # almost debugging: check that each gen lepton has a jet nearby
        for lep in event.genLeptons :
            deltaR2Min = float ('+inf')
            bm = None
            for jet in event.jets:
                dR2 = deltaR2 (lep.eta (), lep.phi (), jet.eta (), jet.phi ())
                if dR2 < deltaR2Min :
                    deltaR2Min = dR2
            self.jetHistos.fillEleMatch (deltaR2Min)
        
        # remove leptons from jets if closer than 0.2
        event.cleanJets = cleanObjectCollection (event.jets, event.genLeptons, 0.2)

#       event.matchedCleanJets = matchObjectCollection (event.cleanJets, event.selGenJets, 0.3)
        event.matchingCleanJets = matchObjectCollection2 (event.cleanJets, event.selGenJets, 0.3)
#       print 'matched jets num: ' + repr (len (event.matchingCleanJets))
            
        # assign to each jet its gen match (easy life :))
        for jet in event.cleanJets :
            jet.gen = event.matchingCleanJets[ jet ]
        # FIXME next step might be to put this in the matching and remove the boolean flags

        self.cleanJetHistos.fillEvent (event.cleanJets)
        
        event.matchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None]
        event.LPtmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None and jet.pt () <= 40]
        event.HPtmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen != None and jet.pt () > 40]

        event.unmatchedCleanJets = [jet for jet in event.matchingCleanJets if jet.gen == None]
        
        self.matchedCleanJetHistos.fillEvent (event.matchedCleanJets)
        self.LPtmatchedCleanJetHistos.fillEvent (event.LPtmatchedCleanJets)
        self.HPtmatchedCleanJetHistos.fillEvent (event.HPtmatchedCleanJets)
        self.matchedCleanJetHistosList[event.vertexBin].fillEvent (event.matchedCleanJets)
        self.LPtmatchedCleanJetHistosList[event.vertexBin].fillEvent (event.LPtmatchedCleanJets)
        self.HPtmatchedCleanJetHistosList[event.vertexBin].fillEvent (event.HPtmatchedCleanJets)

        for jet in event.matchedCleanJets :
            index = int (jet.pt ()) / 20 
            if (index) > 9 : continue
            self.matchedCleanJetHistosSuperList[event.vertexBin][index].fillJet (jet)
            if abs (jet.gen.eta ()) < 1.4 :
                self.matchedCleanJetHistosSuperList_barrel[event.vertexBin][index].fillJet (jet)
            elif 1.6 < abs (jet.gen.eta ()) < 2.5 :    
                self.matchedCleanJetHistosSuperList_endtk[event.vertexBin][index].fillJet (jet)
            elif 2.6 < abs (jet.gen.eta ()) < 2.9 :    
                self.matchedCleanJetHistosSuperList_endNOtk[event.vertexBin][index].fillJet (jet)
            elif 3.1 < abs (jet.gen.eta ()) :    
                self.matchedCleanJetHistosSuperList_fwd[event.vertexBin][index].fillJet (jet)
        
        self.unmatchedCleanJetHistos.fillEvent (event.unmatchedCleanJets)

        self.matchedCleanJetHistosComponents.fillEvent (event.matchedCleanJets)
        self.matchedCleanJetHistosComponentsList[event.vertexBin].fillEvent (event.matchedCleanJets)
        
        self.unmatchedCleanJetHistosComponents.fillEvent (event.unmatchedCleanJets)
        self.unmatchedCleanJetHistosComponentsList[event.vertexBin].fillEvent (event.unmatchedCleanJets)
        self.matchedCleanJetHistosResolution.fillEvent (event.matchedCleanJets, len (event.vertices))

        # check that the same gen jet is not matched to more than one reco
#        for jet1 in event.cleanJets:
#            for jet2 in event.cleanJets:
#                if jet1.gen == jet2.gen and jet1.gen != None and jet1 != jet2:
#                    print 'same matched ' + repr (jet1.gen.pt ())
        

# .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... .... ....

    def write (self):
        self.jetHistos.Write (self.file)
        self.cleanJetHistos.Write (self.file)
        self.matchedCleanJetHistos.Write (self.file)
        self.LPtmatchedCleanJetHistos.Write (self.file)
        self.HPtmatchedCleanJetHistos.Write (self.file)
        for i in range (10) : self.matchedCleanJetHistosList[i].Write (self.file)
        for i in range (10) : self.LPtmatchedCleanJetHistosList[i].Write (self.file)
        for i in range (10) : self.HPtmatchedCleanJetHistosList[i].Write (self.file)

        for line in self.matchedCleanJetHistosSuperList :
            for cell in line :
                cell.Write (self.file)

        for line in self.matchedCleanJetHistosSuperList_barrel :
            for cell in line :
                cell.Write (self.file)

        for line in self.matchedCleanJetHistosSuperList_endtk :
            for cell in line :
                cell.Write (self.file)

        for line in self.matchedCleanJetHistosSuperList_endNOtk :
            for cell in line :
                cell.Write (self.file)

        for line in self.matchedCleanJetHistosSuperList_fwd :
            for cell in line :
                cell.Write (self.file)


        self.unmatchedCleanJetHistos.Write (self.file)

        self.matchedCleanJetHistosComponents.summary ()
        self.matchedCleanJetHistosComponents.Write (self.file)
        for i in range (10) :
            self.matchedCleanJetHistosComponentsList[i].summary ()
            self.matchedCleanJetHistosComponentsList[i].Write (self.file)
        self.unmatchedCleanJetHistosComponents.summary ()
        self.unmatchedCleanJetHistosComponents.Write (self.file)
        for i in range (10) :
            self.unmatchedCleanJetHistosComponentsList[i].summary ()
            self.unmatchedCleanJetHistosComponentsList[i].Write (self.file)

        self.matchedCleanJetHistosResolution.summary ()
        self.matchedCleanJetHistosResolution.Write (self.file)
        
        self.file.cd ()
        self.h_nvtx.Write ()
        
