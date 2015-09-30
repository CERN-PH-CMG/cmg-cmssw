from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import os, copy, math
import ROOT
from array import array

class XZhAnalyzer( Analyzer ):
    '''Analyzer for the X -> Zh -> llbb analysis'''

    def beginLoop(self, setup):
        super(XZhAnalyzer, self).beginLoop(setup)
        # define hist dictionary
        self.Hist = {}
        # define labels
        Z2LLlabels = ["Trigger", "Lep Acc", "Lep Id", "Lep Iso", "Z cand", "Z mass", "Z p_{T}", "Jet p_{T}", "h mass", "b-tag 1", "b-tag 2"]
        Z2NNlabels = ["Trigger", "e/#mu veto", "Jet p_{T}", "#slash{E}_{T}", "#Delta #varphi > 2.5", "h mass", "b-tag 1", "b-tag 2"]
        HEEPlabels = ["isEcalDriven", "#Delta #eta_{in}^{seed}", "#Delta #varphi_{in}", "H/E", "E^{2x5}/E^{5x5}", "Lost Hits", "|d_{xy}|", "All"]
        CstTrklabels = ["isTracker", "Matched Stations", "#delta p_{T}/p_{T}", "|d_{xy}|", "|d_{z}|", "Pixel Hits", "Trk Lay w/ Meas", "All"]
        # define binning
        pTbins = [0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 60., 70., 80., 90., 100., 110., 120., 130., 150., 175., 200., 225., 250., 300., 350., 400., 500., 750., 1000., 1250., 1500., 2000., 2500.]
        dRbins = [0., 0.025, 0.05, 0.075, 0.10, 0.15, 0.20, 0.25, 0.30, 0.40, 0.50, 0.75, 1.0]
        
        if "outputfile" in setup.services:
            # set counter histos
            setup.services["outputfile"].file.cd("Counters")
            self.Hist["Z2EECounter"] = ROOT.TH1F("ZtoEECounter", "", len(Z2LLlabels), 0, len(Z2LLlabels))
            self.Hist["Z2MMCounter"] = ROOT.TH1F("ZtoMMCounter", "", len(Z2LLlabels), 0, len(Z2LLlabels))
            for i, l in enumerate(Z2LLlabels):
                self.Hist["Z2EECounter"].GetXaxis().SetBinLabel(i+1, l)
                self.Hist["Z2MMCounter"].GetXaxis().SetBinLabel(i+1, l)
            setup.services["outputfile"].file.cd("..")
            # set lepton histos
            setup.services["outputfile"].file.mkdir("Leptons")
            setup.services["outputfile"].file.cd("Leptons")
            #   electron HEEP
            self.Hist["ElecBarrelHEEP"] = ROOT.TH1F("ElecBarrelHEEP", ";;Events", len(HEEPlabels), 0, len(HEEPlabels))
            self.Hist["ElecEndcapHEEP"] = ROOT.TH1F("ElecEndcapHEEP", ";;Events", len(HEEPlabels), 0, len(HEEPlabels))
            for i, l in enumerate(HEEPlabels):
                self.Hist["ElecBarrelHEEP"].GetXaxis().SetBinLabel(i+1, l)
                self.Hist["ElecEndcapHEEP"].GetXaxis().SetBinLabel(i+1, l)
            self.Hist["ElecEndcapHEEP"].GetXaxis().SetBinLabel(5, "#sigma_{i#eta i#eta}")
            #   muon CustomTracker
            self.Hist["MuonCustomTracker"] = ROOT.TH1F("MuonCustomTracker", ";;Events", len(CstTrklabels), 0, len(CstTrklabels))
            for i, l in enumerate(CstTrklabels):
                self.Hist["MuonCustomTracker"].GetXaxis().SetBinLabel(i+1, l)
            #   electron/muon Pt/Eta/ZdR/Zpt
            for i, n in enumerate(["ElecPt", "MuonPt"]):
                self.Hist[n] = ROOT.TH1F(n, ";Lepton p_{T} (GeV);Events", len(pTbins)-1, array('f', pTbins))
            for i, n in enumerate(["ElecEta", "MuonEta"]):
                self.Hist[n] = ROOT.TH1F(n, ";Lepton #eta;Events", 60, -3., 3.)
            for i, n in enumerate(["ElecZdR", "MuonZdR"]):
                self.Hist[n] = ROOT.TH1F(n, ";Gen #Delta R;Events", len(dRbins)-1, array('f', dRbins))
            for i, n in enumerate(["ElecZpt", "MuonZpt"]):
                self.Hist[n] = ROOT.TH1F(n, ";Z p_{T} (GeV);Events", len(pTbins)-1, array('f', pTbins))
            setup.services["outputfile"].file.cd("..")
            # set efficiency histos
            setup.services["outputfile"].file.mkdir("Eff")
            setup.services["outputfile"].file.cd("Eff")
            #   electron HEEP
            self.Hist["EffElecBarrelHEEP"] = ROOT.TH1F("EffElecBarrelHEEP", ";;Efficiency", len(HEEPlabels), 0, len(HEEPlabels))
            self.Hist["EffElecEndcapHEEP"] = ROOT.TH1F("EffElecEndcapHEEP", ";;Efficiency", len(HEEPlabels), 0, len(HEEPlabels))
            for i, l in enumerate(HEEPlabels):
                self.Hist["EffElecBarrelHEEP"].GetXaxis().SetBinLabel(i+1, l)
                self.Hist["EffElecEndcapHEEP"].GetXaxis().SetBinLabel(i+1, l)
            self.Hist["EffElecEndcapHEEP"].GetXaxis().SetBinLabel(5, "#sigma_{i#eta i#eta}")
            #   muon CustomTracker
            self.Hist["EffMuonCustomTracker"] = ROOT.TH1F("EffMuonCustomTracker", ";;Efficiency", len(CstTrklabels), 0, len(CstTrklabels))
            for i, l in enumerate(CstTrklabels):
                self.Hist["EffMuonCustomTracker"].GetXaxis().SetBinLabel(i+1, l)
            #   electron/muon Pt/Eta/ZdR/Zpt
            for i, n in enumerate(["EffElecPt_HEEP", "EffMuonPt_Highpt", "EffMuonPt_CustomTracker"]):
                self.Hist[n] = ROOT.TH1F(n, ";Lepton p_{T} (GeV);Efficiency", len(pTbins)-1, array('f', pTbins))
            for i, n in enumerate(["EffElecEta_HEEP", "EffMuonEta_Highpt", "EffMuonEta_CustomTracker"]):
                self.Hist[n] = ROOT.TH1F(n, ";Lepton #eta;Efficiency", 60, -3., 3.)
            #   Z ee mm
            for i, n in enumerate(["EffElecZdR_Loose", "EffElecZdR_Loose_pfIso", "EffElecZdR_Loose_miniIso", "EffElecZdR_Tight", "EffElecZdR_HEEP", "EffElecZdR_HEEP_pfIso", "EffElecZdR_HEEP_miniIso", "EffMuonZdR_TrackerTracker", "EffMuonZdR_TrackerTracker_pfIso", "EffMuonZdR_TrackerTracker_miniIso", "EffMuonZdR_LooseLoose", "EffMuonZdR_LooseLoose_pfIso", "EffMuonZdR_LooseLoose_miniIso", "EffMuonZdR_HighptTracker", "EffMuonZdR_HighptTracker_pfIso", "EffMuonZdR_HighptTracker_miniIso", "EffMuonZdR_HighptCustomTracker", "EffMuonZdR_HighptCustomTracker_pfIso", "EffMuonZdR_HighptCustomTracker_miniIso", "EffMuonZdR_HighptLoose", "EffMuonZdR_HighptLoose_pfIso", "EffMuonZdR_HighptLoose_miniIso", "EffMuonZdR_HighptHighpt", "EffMuonZdR_TightTight"]):
                self.Hist[n] = ROOT.TH1F(n, ";Gen #Delta R;Efficiency", len(dRbins)-1, array('f', dRbins))
            for i, n in enumerate(["EffElecZpt_Loose", "EffElecZpt_Loose_pfIso", "EffElecZpt_Loose_miniIso", "EffElecZpt_Tight", "EffElecZpt_HEEP", "EffElecZpt_HEEP_pfIso", "EffElecZpt_HEEP_miniIso", "EffMuonZpt_TrackerTracker", "EffMuonZpt_TrackerTracker_pfIso", "EffMuonZpt_TrackerTracker_miniIso", "EffMuonZpt_LooseLoose", "EffMuonZpt_LooseLoose_pfIso", "EffMuonZpt_LooseLoose_miniIso", "EffMuonZpt_HighptTracker", "EffMuonZpt_HighptTracker_pfIso", "EffMuonZpt_HighptTracker_miniIso", "EffMuonZpt_HighptCustomTracker", "EffMuonZpt_HighptCustomTracker_pfIso", "EffMuonZpt_HighptCustomTracker_miniIso", "EffMuonZpt_HighptLoose", "EffMuonZpt_HighptLoose_pfIso", "EffMuonZpt_HighptLoose_miniIso", "EffMuonZpt_HighptHighpt", "EffMuonZpt_TightTight"]):
                self.Hist[n] = ROOT.TH1F(n, ";Z p_{T} (GeV);Efficiency", len(pTbins)-1, array('f', pTbins))
            # Set Sumw2
            for n, h in self.Hist.iteritems():
                h.Sumw2()
        
#        # Jet Mass Recalibration
#        if self.cfg_ana.recalibrateMass:
#            path = os.path.expandvars(self.cfg_ana.jecPath)
#            globalTag = self.cfg_ana.mcGT if self.cfg_comp.isMC else self.cfg_ana.dataGT
#            jetFlavour = self.cfg_ana.recalibrationType
#            self.vPar = ROOT.vector(ROOT.JetCorrectorParameters)()
#            #self.L1JetPar  = ROOT.JetCorrectorParameters("%s/%s_L1FastJet_%s.txt" % (path,globalTag,jetFlavour),"");
#            self.L2JetPar  = ROOT.JetCorrectorParameters("%s/%s_L2Relative_%s.txt" % (path,globalTag,jetFlavour),"");
#            self.L3JetPar  = ROOT.JetCorrectorParameters("%s/%s_L3Absolute_%s.txt" % (path,globalTag,jetFlavour),"");
#            #self.vPar.push_back(self.L1JetPar);
#            self.vPar.push_back(self.L2JetPar);
#            self.vPar.push_back(self.L3JetPar);
#            # Add residuals if needed
#            if self.cfg_comp.isMC: 
#                self.ResJetPar = ROOT.JetCorrectorParameters("%s/%s_L2L3Residual_%s.txt" % (path,globalTag,jetFlavour))
#                self.vPar.push_back(self.ResJetPar);
#            #Step3 (Construct a FactorizedJetCorrector object) 
#            self.JetCorrector = ROOT.FactorizedJetCorrector(self.vPar)
            

            
    def endLoop(self, setup):
        for n, h in self.Hist.iteritems():
            if n.startswith("Eff"): h.Divide(self.Hist[ n.split("_", 1)[0].replace("Eff", "") ])
        
        
        
    def fillGenPlots(self, event):
        if not hasattr(event, "genleps"): return True
        if not len(event.genleps) >= 2: return True
        i1, i2 = [0, 1] if event.genleps[0].pt() > event.genleps[1].pt() else [1, 0]
        l1, l2 = -1, -1
        genZdR = deltaR(event.genleps[i1].eta(), event.genleps[i1].phi(), event.genleps[i2].eta(), event.genleps[i2].phi())
        if hasattr(event, "genVBosons") and len(event.genVBosons) >= 1:
            genZpt = event.genVBosons[0].pt()
        else:
            genZpt = (event.genleps[i1].p4() + event.genleps[i2].p4()).pt()
        
        # Electrons
        if abs(event.genleps[0].pdgId()) == 11:
            # gen matching
            for i, l in enumerate(event.inclusiveLeptons):
                if l.isElectron() and deltaR(l.eta(), l.phi(), event.genleps[i1].eta(), event.genleps[i1].phi())<0.1 and abs(1-l.pt()/event.genleps[i1].pt()) < 0.3: l1 = i
                elif l.isElectron() and deltaR(l.eta(), l.phi(), event.genleps[i2].eta(), event.genleps[i2].phi())<0.1 and abs(1-l.pt()/event.genleps[i2].pt()) < 0.3: l2 = i
            # single lepton 1
            if l1 >= 0 and event.inclusiveLeptons[l1].pt() > self.cfg_ana.elec1pt:
                self.Hist["ElecPt"].Fill(event.genleps[i1].pt())
                self.Hist["ElecEta"].Fill(event.genleps[i1].eta())
                if event.inclusiveLeptons[l1].isHEEP:
                    self.Hist["EffElecPt_HEEP"].Fill(event.genleps[i1].pt())
                    self.Hist["EffElecEta_HEEP"].Fill(event.genleps[i1].eta())
            # single lepton 2
            if l2 >= 0 and event.inclusiveLeptons[l2].pt() > self.cfg_ana.elec2pt:
                self.Hist["ElecPt"].Fill(event.genleps[i2].pt())
                self.Hist["ElecEta"].Fill(event.genleps[i2].eta())
                if event.inclusiveLeptons[l2].isHEEP:
                    self.Hist["EffElecPt_HEEP"].Fill(event.genleps[i2].pt())
                    self.Hist["EffElecEta_HEEP"].Fill(event.genleps[i2].eta())
            # both leptons
            if l1 >= 0 and l2 >= 0 and event.inclusiveLeptons[l1].pt() > self.cfg_ana.elec1pt and event.inclusiveLeptons[l2].pt() > self.cfg_ana.elec2pt:
                Zmass = (event.inclusiveLeptons[l1].p4() + event.inclusiveLeptons[l2].p4()).mass()
                pfIso = event.inclusiveLeptons[l1].relIso03<0.15 and event.inclusiveLeptons[l2].relIso03<0.15
                miniIso = event.inclusiveLeptons[l1].miniRelIso<0.1 and event.inclusiveLeptons[l2].miniRelIso<0.1
                # Z
                if Zmass > self.cfg_ana.Z_mass_low and Zmass < self.cfg_ana.Z_mass_high and event.inclusiveLeptons[l1].charge()!=event.inclusiveLeptons[l2].charge():
                    self.Hist["ElecZdR"].Fill(genZdR)
                    self.Hist["ElecZpt"].Fill(genZpt)
                    # HEEP / HEEP
                    if event.inclusiveLeptons[l1].isHEEP and event.inclusiveLeptons[l2].isHEEP:
                        self.Hist["EffElecZdR_HEEP"].Fill(genZdR)
                        self.Hist["EffElecZpt_HEEP"].Fill(genZpt)
                        # HEEP+PFiso / HEEP+PFiso
                        if pfIso:
                            self.Hist["EffElecZdR_HEEP_pfIso"].Fill(genZdR)
                            self.Hist["EffElecZpt_HEEP_pfIso"].Fill(genZpt)
                        # HEEP+miniiso / HEEP+miniiso
                        if miniIso:
                            self.Hist["EffElecZdR_HEEP_miniIso"].Fill(genZdR)
                            self.Hist["EffElecZpt_HEEP_miniIso"].Fill(genZpt)
                    # Loose / Loose (PHYS14)
                    if event.inclusiveLeptons[l1].electronID('POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Loose') and event.inclusiveLeptons[l2].electronID('POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Loose'):
                        self.Hist["EffElecZdR_Loose"].Fill(genZdR)
                        self.Hist["EffElecZpt_Loose"].Fill(genZpt)
                        # Loose+PFiso / Loose+PFiso
                        if pfIso:
                            self.Hist["EffElecZdR_Loose_pfIso"].Fill(genZdR)
                            self.Hist["EffElecZpt_Loose_pfIso"].Fill(genZpt)
                        # Loose+miniiso / Loose+miniiso
                        if miniIso:
                            self.Hist["EffElecZdR_Loose_miniIso"].Fill(genZdR)
                            self.Hist["EffElecZpt_Loose_miniIso"].Fill(genZpt)
                    # Tight / Tight (PHYS14)
                    if event.inclusiveLeptons[l1].electronID('POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Tight') and event.inclusiveLeptons[l2].electronID('POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Tight'):
                        self.Hist["EffElecZdR_Tight"].Fill(genZdR)
                        self.Hist["EffElecZpt_Tight"].Fill(genZpt)
                
        # Muons
        if abs(event.genleps[0].pdgId()) == 13:
            # gen matching
            for i, l in enumerate(event.inclusiveLeptons):
                if l.isMuon() and deltaR(l.eta(), l.phi(), event.genleps[i1].eta(), event.genleps[i1].phi())<0.1 and abs(1-l.pt()/event.genleps[i1].pt()) < 0.3: l1 = i
                elif l.isMuon() and deltaR(l.eta(), l.phi(), event.genleps[i2].eta(), event.genleps[i2].phi())<0.1 and abs(1-l.pt()/event.genleps[i2].pt()) < 0.3: l2 = i
            # single lepton 1
            if l1 >= 0 and event.inclusiveLeptons[l1].pt() > self.cfg_ana.muon1pt:
                self.Hist["MuonPt"].Fill(event.genleps[i1].pt())
                self.Hist["MuonEta"].Fill(event.genleps[i1].eta())
                if event.inclusiveLeptons[l1].muonID("POG_ID_HighPt"):
                    self.Hist["EffMuonPt_Highpt"].Fill(event.genleps[i1].pt())
                    self.Hist["EffMuonEta_Highpt"].Fill(event.genleps[i1].eta())
                if event.inclusiveLeptons[l1].isCustomTracker:
                    self.Hist["EffMuonPt_CustomTracker"].Fill(event.genleps[i1].pt())
                    self.Hist["EffMuonEta_CustomTracker"].Fill(event.genleps[i1].eta())
            # single lepton 2
            if l2 >= 0 and event.inclusiveLeptons[l2].pt() > self.cfg_ana.muon2pt:
                self.Hist["MuonPt"].Fill(event.genleps[i2].pt())
                self.Hist["MuonEta"].Fill(event.genleps[i2].eta())
                if event.inclusiveLeptons[l2].muonID("POG_ID_HighPt"):
                    self.Hist["EffMuonPt_Highpt"].Fill(event.genleps[i2].pt())
                    self.Hist["EffMuonEta_Highpt"].Fill(event.genleps[i2].eta())                
                if event.inclusiveLeptons[l2].isCustomTracker:
                    self.Hist["EffMuonPt_CustomTracker"].Fill(event.genleps[i2].pt())
                    self.Hist["EffMuonEta_CustomTracker"].Fill(event.genleps[i2].eta())                
            # both leptons
            if l1 >= 0 and l2 >= 0 and event.inclusiveLeptons[l1].pt() > self.cfg_ana.muon1pt and event.inclusiveLeptons[l2].pt() > self.cfg_ana.muon2pt:
                Zmass = (event.inclusiveLeptons[l1].p4() + event.inclusiveLeptons[l2].p4()).mass()
                pfIso = event.inclusiveLeptons[l1].relIso04<0.20 and event.inclusiveLeptons[l2].relIso04<0.20
                miniIso = event.inclusiveLeptons[l1].miniRelIso<0.1 and event.inclusiveLeptons[l2].miniRelIso<0.1
                # Z
                if Zmass > self.cfg_ana.Z_mass_low and Zmass < self.cfg_ana.Z_mass_high and event.inclusiveLeptons[l1].charge()!=event.inclusiveLeptons[l2].charge():
                    self.Hist["MuonZdR"].Fill(genZdR)
                    self.Hist["MuonZpt"].Fill(genZpt)
                    # Tracker / Tracker
                    if event.inclusiveLeptons[l1].isTrackerMuon() and event.inclusiveLeptons[l2].isTrackerMuon():
                        self.Hist["EffMuonZdR_TrackerTracker"].Fill(genZdR)
                        self.Hist["EffMuonZpt_TrackerTracker"].Fill(genZpt)
                        # Tracker+PFiso / Tracker+PFiso
                        if pfIso:
                            self.Hist["EffMuonZdR_TrackerTracker_pfIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_TrackerTracker_pfIso"].Fill(genZpt)
                        # Tracker+miniiso / Tracker+miniiso
                        if miniIso:
                            self.Hist["EffMuonZdR_TrackerTracker_miniIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_TrackerTracker_miniIso"].Fill(genZpt)
                    # Loose / Loose
                    if event.inclusiveLeptons[l1].muonID("POG_ID_Loose") and event.inclusiveLeptons[l2].muonID("POG_ID_Loose"):
                        self.Hist["EffMuonZdR_LooseLoose"].Fill(genZdR)
                        self.Hist["EffMuonZpt_LooseLoose"].Fill(genZpt)
                        # Loose+PFiso / Loose+PFiso
                        if pfIso:
                            self.Hist["EffMuonZdR_LooseLoose_pfIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_LooseLoose_pfIso"].Fill(genZpt)
                        # Loose+miniiso / Loose+miniiso
                        if miniIso:
                            self.Hist["EffMuonZdR_LooseLoose_miniIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_LooseLoose_miniIso"].Fill(genZpt)
                    # HighPt / Tracker
                    if (event.inclusiveLeptons[l1].muonID("POG_ID_HighPt") and event.inclusiveLeptons[l2].isTrackerMuon) or (event.inclusiveLeptons[l1].isTrackerMuon and event.inclusiveLeptons[l2].muonID("POG_ID_HighPt")):
                        self.Hist["EffMuonZdR_HighptTracker"].Fill(genZdR)
                        self.Hist["EffMuonZpt_HighptTracker"].Fill(genZpt)
                        # HighPt+PFiso / Tracker+PFiso
                        if pfIso:
                            self.Hist["EffMuonZdR_HighptTracker_pfIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptTracker_pfIso"].Fill(genZpt)
                        # HighPt+miniiso / Tracker+miniiso
                        if miniIso:
                            self.Hist["EffMuonZdR_HighptTracker_miniIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptTracker_miniIso"].Fill(genZpt)
                    # HighPt / CustomTracker 
                    if (event.inclusiveLeptons[l1].muonID("POG_ID_HighPt") and event.inclusiveLeptons[l2].isCustomTracker) or (event.inclusiveLeptons[l1].isCustomTracker and event.inclusiveLeptons[l2].muonID("POG_ID_HighPt")):
                        self.Hist["EffMuonZdR_HighptCustomTracker"].Fill(genZdR)
                        self.Hist["EffMuonZpt_HighptCustomTracker"].Fill(genZpt)
                        # HighPt+PFiso / Tracker+PFiso
                        if pfIso:
                            self.Hist["EffMuonZdR_HighptCustomTracker_pfIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptCustomTracker_pfIso"].Fill(genZpt)
                        # HighPt+miniiso / Tracker+miniiso
                        if miniIso:
                            self.Hist["EffMuonZdR_HighptCustomTracker_miniIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptCustomTracker_miniIso"].Fill(genZpt)
                    # HighPt / Loose 
                    if (event.inclusiveLeptons[l1].muonID("POG_ID_HighPt") and event.inclusiveLeptons[l2].muonID("POG_ID_Loose")) or (event.inclusiveLeptons[l1].muonID("POG_ID_Loose") and event.inclusiveLeptons[l2].muonID("POG_ID_HighPt")):
                        self.Hist["EffMuonZdR_HighptLoose"].Fill(genZdR)
                        self.Hist["EffMuonZpt_HighptLoose"].Fill(genZpt)
                        # HighPt+PFiso / Loose+PFiso
                        if pfIso:
                            self.Hist["EffMuonZdR_HighptLoose_pfIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptLoose_pfIso"].Fill(genZpt)
                        # HighPt+miniiso / Loose+miniiso
                        if miniIso:
                            self.Hist["EffMuonZdR_HighptLoose_miniIso"].Fill(genZdR)
                            self.Hist["EffMuonZpt_HighptLoose_miniIso"].Fill(genZpt)
                    # HighPt / HighPt
                    if event.inclusiveLeptons[l1].muonID("POG_ID_HighPt") and event.inclusiveLeptons[l2].muonID("POG_ID_HighPt"):
                        self.Hist["EffMuonZdR_HighptHighpt"].Fill(genZdR)
                        self.Hist["EffMuonZpt_HighptHighpt"].Fill(genZpt)
                    # Tight / Tight
                    if event.inclusiveLeptons[l1].muonID("POG_ID_Tight") and event.inclusiveLeptons[l2].muonID("POG_ID_Tight"):
                        self.Hist["EffMuonZdR_TightTight"].Fill(genZdR)
                        self.Hist["EffMuonZpt_TightTight"].Fill(genZpt)
    
    
    def addFakeMet(self, event, particles):
        # Copy regular met
        event.fakemet = copy.deepcopy(event.met)
        px, py = event.met.px(), event.met.py()
        for i, p in enumerate(particles):
            if not p:
                continue
            else:
                px += p.px()
                py += p.py()
        
        event.fakemet.setP4(ROOT.reco.Particle.LorentzVector(px, py, 0, math.hypot(px, py)))
        return True
    
    
    
    def addHEEP(self, event, e, doPlot=False):
        e.isHEEP = False        
        if not e.isElectron(): return False

        # define electron variables
        scTheta             = 2*math.atan( math.exp( -e.superCluster().eta() ) )
        scE                 = e.superCluster().energy()
        scEt                = e.superCluster().energy()*scTheta        
        scEta               = e.superCluster().eta()
        isEcalDriven        = e.ecalDrivenSeed()
        deltaEtaSeedIn      = e.deltaEtaSeedClusterTrackAtVtx() - e.superCluster().eta() + e.superCluster().seed().eta() 
        #deltaEtaSeedIn      =  e.deltaEtaSuperClusterTrackAtVtx() - e.superCluster().eta() + e.superCluster().seed().eta() if ( e.superCluster().isNonnull() and e.superCluster().seed().isNonnull() ) else 1e99
        deltaPhiIn          = e.deltaPhiSuperClusterTrackAtVtx()
        HoE                 = e.hadronicOverEm()
        full5x5sigmaIetaIeta= e.full5x5_sigmaIetaIeta()
        full5x5e2x5e5x5     = e.full5x5_e2x5Max()/e.full5x5_e5x5()
        full5x5e1x5e5x5     = e.full5x5_e1x5()/e.full5x5_e5x5()
        inLayLostHits       = e.gsfTrack().hitPattern().numberOfHits(1)
        dxy                 = e.gsfTrack().dxy(event.goodVertices[0].position())
                
        if not scEt > 35.: return False
                
        # Plot
        if doPlot:
          if abs(scEta) < 1.4442:
              for i in range(self.Hist["ElecBarrelHEEP"].GetNbinsX()): self.Hist["ElecBarrelHEEP"].AddBinContent(i+1)            
              if isEcalDriven: self.Hist["EffElecBarrelHEEP"].AddBinContent(1)
              if abs(deltaEtaSeedIn) < 0.004: self.Hist["EffElecBarrelHEEP"].AddBinContent(2)
              if abs(deltaPhiIn) < 0.06: self.Hist["EffElecBarrelHEEP"].AddBinContent(3)
              if HoE < 1./scE + 0.05: self.Hist["EffElecBarrelHEEP"].AddBinContent(4)
              if (full5x5e2x5e5x5 > 0.94 or full5x5e1x5e5x5 > 0.83): self.Hist["EffElecBarrelHEEP"].AddBinContent(5)
              if inLayLostHits <= 1: self.Hist["EffElecBarrelHEEP"].AddBinContent(6)
              if abs(dxy) < 0.02: self.Hist["EffElecBarrelHEEP"].AddBinContent(7)
          elif abs(scEta) > 1.566 and abs(scEta) < 2.5:
              for i in range(self.Hist["ElecEndcapHEEP"].GetNbinsX()): self.Hist["ElecEndcapHEEP"].AddBinContent(i+1)            
              if isEcalDriven: self.Hist["EffElecEndcapHEEP"].AddBinContent(1)
              if abs(deltaEtaSeedIn) < 0.006: self.Hist["EffElecEndcapHEEP"].AddBinContent(2)
              if abs(deltaPhiIn) < 0.06: self.Hist["EffElecEndcapHEEP"].AddBinContent(3)
              if HoE < 5./scE + 0.05: self.Hist["EffElecEndcapHEEP"].AddBinContent(4)
              if full5x5sigmaIetaIeta < 0.03: self.Hist["EffElecEndcapHEEP"].AddBinContent(5)
              if inLayLostHits <= 1: self.Hist["EffElecEndcapHEEP"].AddBinContent(6)
              if abs(dxy) < 0.05: self.Hist["EffElecEndcapHEEP"].AddBinContent(7)
                        
        if abs(scEta) < 1.4442:
            if not isEcalDriven: return False
            if not abs(deltaEtaSeedIn) < 0.004: return False
            if not abs(deltaPhiIn) < 0.06: return False
            if not HoE < 1./scE + 0.05: return False
            if not (full5x5e2x5e5x5 > 0.94 or full5x5e1x5e5x5 > 0.83): return False
            if not inLayLostHits <= 1: return False
            if not abs(dxy) < 0.02: return False
        elif abs(scEta) > 1.566 and abs(scEta) < 2.5:
            if not isEcalDriven: return False
            if not abs(deltaEtaSeedIn) < 0.006: return False
            if not abs(deltaPhiIn) < 0.06: return False
            if not HoE < 5./scE + 0.05: return False
            if not full5x5sigmaIetaIeta < 0.03: return False
            if not inLayLostHits <= 1: return False
            if not abs(dxy) < 0.05: return False
        else:
            return False        
        
        if doPlot:
            if abs(scEta) < 1.4442:
                self.Hist["EffElecBarrelHEEP"].AddBinContent(8)
            elif abs(scEta) > 1.566 and abs(scEta) < 2.5:
                self.Hist["EffElecEndcapHEEP"].AddBinContent(8)
        
        e.isHEEP = True
        return True
    
    def addCustomTracker(self, event, m, doPlot=False):
        m.isCustomTracker = False
        if not m.isMuon(): return False
        
        # define muon variables
        isTrk       = m.isTrackerMuon()
        nMatchSt    = m.numberOfMatchedStations()
        sigmaPt     = m.bestTrack().ptError()/m.bestTrack().pt()       
        dxy         = m.bestTrack().dxy(event.goodVertices[0].position())
        dz          = m.bestTrack().dz(event.goodVertices[0].position())
        validTrk    = m.innerTrack().isNonnull()
        nPixHits    = m.innerTrack().hitPattern().numberOfValidPixelHits() 
        nTrkLayMeas = m.innerTrack().hitPattern().trackerLayersWithMeasurement()

        # Plot
        if doPlot:
          for i in range(self.Hist["MuonCustomTracker"].GetNbinsX()): self.Hist["MuonCustomTracker"].AddBinContent(i+1)
          if isTrk: self.Hist["EffMuonCustomTracker"].AddBinContent(1)
          if nMatchSt > 1: self.Hist["EffMuonCustomTracker"].AddBinContent(2)
          if sigmaPt < 0.3: self.Hist["EffMuonCustomTracker"].AddBinContent(3)
          if abs(dxy) < 0.2: self.Hist["EffMuonCustomTracker"].AddBinContent(4)
          if abs(dz) < 0.5: self.Hist["EffMuonCustomTracker"].AddBinContent(5)
          if validTrk and nPixHits > 0: self.Hist["EffMuonCustomTracker"].AddBinContent(6)
          if nTrkLayMeas > 5: self.Hist["EffMuonCustomTracker"].AddBinContent(7)
        
        if not isTrk: return False
        if not nMatchSt > 1: return False
        if not sigmaPt < 0.3: return False
        if not abs(dxy) < 0.2: return False
        if not abs(dz) < 0.5: return False
        if not validTrk: return False
        if not nPixHits > 0: return False
        if not nTrkLayMeas > 5: return False
        
        if doPlot:
            self.Hist["EffMuonCustomTracker"].AddBinContent(8)
        
        m.isCustomTracker = True
        return True
    
#    def addCorrectedJetMass(self, event, jet):
#        if self.cfg_ana.recalibrateMass:
#            self.JetCorrector.setJetPt(jet.pt() * jet.rawFactor())
#            self.JetCorrector.setJetEta(jet.eta())
#            self.JetCorrector.setJetE(jet.energy() * jet.rawFactor());
#            self.JetCorrector.setJetA(jet.jetArea())
#            self.JetCorrector.setRho(event.rho)
#            self.JetCorrector.setNPV( len(event.vertices) )
#            corr = self.JetCorrector.getCorrection()
#        else:
#            corr = 1.

#        jet.addUserFloat("ak8PFJetsCHSPrunedMassCorr", corr*jet.userFloat("ak8PFJetsCHSPrunedMass"))
#        jet.addUserFloat("ak8PFJetsCHSSoftDropMassCorr", corr*jet.userFloat("ak8PFJetsCHSSoftDropMass"))
        
    
    
    def process(self, event):
        event.isXZh = False
        event.isZ2EE = False
        event.isZ2MM = False
        event.isGenZ2EE = False
        event.isGenZ2MM = False
        event.highptFatJets = [] #ROOT.pat.Jet()
        event.Z = ROOT.reco.Particle.LorentzVector(0, 0, 0, 0)
        event.X = ROOT.reco.Particle.LorentzVector(0, 0, 0, 0)
        
         # All
        self.Hist["Z2EECounter"].AddBinContent(0, event.eventWeight)
        self.Hist["Z2MMCounter"].AddBinContent(0, event.eventWeight)
        
        # Attach electron HEEP Id
        for i, l in enumerate(event.inclusiveLeptons): self.addHEEP(event, l, i==0)
        
        # Attach muon Custom Tracker Id
        for i, l in enumerate(event.inclusiveLeptons): self.addCustomTracker(event, l, i==0)

        ### Preliminary operations ###
        # Gen info
        if self.cfg_comp.isMC:
            if hasattr(event, "genleps") and len(event.genleps) >= 2:
                if abs(event.genleps[0].pdgId()) == 11: event.isGenZ2EE = True
                elif abs(event.genleps[0].pdgId()) == 13: event.isGenZ2MM = True
            #
            self.fillGenPlots(event)
        
        
        # Trigger
        elecTrigger = event.HLT_BIT_HLT_Ele105_CaloIdVT_GsfTrkIdT_v
        muonTrigger = event.HLT_BIT_HLT_Mu45_eta2p1_v
        
        if not elecTrigger and not muonTrigger:
            return True
        if elecTrigger: self.Hist["Z2EECounter"].AddBinContent(1, event.eventWeight)
        if muonTrigger: self.Hist["Z2MMCounter"].AddBinContent(1, event.eventWeight)
        
        #########################
        #    Part 1: Leptons    #
        #########################
        
        # Lepton collections
        if len([x for x in event.inclusiveLeptons if x.isElectron()]) < 2 and len([x for x in event.inclusiveLeptons if x.isMuon()]) < 2:
            return True
        
        elecAcc = len([x for x in event.inclusiveLeptons if x.isElectron()]) >= 2 and event.inclusiveLeptons[0].pt() > self.cfg_ana.elec1pt and event.inclusiveLeptons[1].pt() > self.cfg_ana.elec2pt
        muonAcc =  len([x for x in event.inclusiveLeptons if x.isMuon()]) >= 2 and event.inclusiveLeptons[0].pt() > self.cfg_ana.muon1pt and event.inclusiveLeptons[1].pt() > self.cfg_ana.muon2pt
        
        if not elecAcc and not muonAcc:
            return True
        if elecTrigger and elecAcc: self.Hist["Z2EECounter"].AddBinContent(2, event.eventWeight)
        if muonTrigger and muonAcc: self.Hist["Z2MMCounter"].AddBinContent(2, event.eventWeight)
        
        # Id
        event.highptIdElectrons = [x for x in event.inclusiveLeptons if x.isElectron() and self.addHEEP(event, x)] # and self.addHEEP(x) and x.miniRelIso<0.1  and x.electronID('POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Loose')
        #event.highptIdMuons = [x for x in event.inclusiveLeptons if x.isMuon() and x.isTrackerMuon()] #x.isTrackerMuon() and x.miniRelIso<0.1  and x.muonID("POG_ID_Loose")
        event.highptIdMuons = [x for x in event.inclusiveLeptons if x.isMuon() and self.addCustomTracker(event, x)] #x.isTrackerMuon() and x.miniRelIso<0.1  and x.muonID("POG_ID_Loose")
        
        event.highptIdElectrons.sort(key = lambda l : l.pt(), reverse = True)
        event.highptIdMuons.sort(key = lambda l : l.pt(), reverse = True)
        
        elecId = len(event.highptIdElectrons) >= 2 and event.highptIdElectrons[0].pt() > self.cfg_ana.elec1pt and event.highptIdElectrons[1].pt() > self.cfg_ana.elec2pt
        muonId = len(event.highptIdMuons) >= 2 and event.highptIdMuons[0].pt() > self.cfg_ana.muon1pt and event.highptIdMuons[1].pt() > self.cfg_ana.muon2pt and (event.highptIdMuons[0].muonID("POG_ID_HighPt") or event.highptIdMuons[1].muonID("POG_ID_HighPt"))
        
        if not elecId and not muonId:
            return True
        if elecTrigger and elecId: self.Hist["Z2EECounter"].AddBinContent(3, event.eventWeight)
        if muonTrigger and muonId: self.Hist["Z2MMCounter"].AddBinContent(3, event.eventWeight)
        
        # Iso
        event.highptIdIsoElectrons = [x for x in event.highptIdElectrons if x.miniRelIso<0.1]
        event.highptIdIsoMuons = [x for x in event.highptIdMuons if x.miniRelIso<0.1]
        
        event.highptIdIsoElectrons.sort(key = lambda l : l.pt(), reverse = True)
        event.highptIdIsoMuons.sort(key = lambda l : l.pt(), reverse = True)
        
        elecIdIso = len(event.highptIdIsoElectrons) >= 2 and event.highptIdIsoElectrons[0].pt() > self.cfg_ana.elec1pt and event.highptIdIsoElectrons[1].pt() > self.cfg_ana.elec2pt
        muonIdIso = len(event.highptIdIsoMuons) >= 2 and event.highptIdIsoMuons[0].pt() > self.cfg_ana.muon1pt and event.highptIdIsoMuons[1].pt() > self.cfg_ana.muon2pt and (event.highptIdIsoMuons[0].muonID("POG_ID_HighPt") or event.highptIdIsoMuons[1].muonID("POG_ID_HighPt"))
        
        if not elecIdIso and not muonIdIso:
            return True
        if elecTrigger and elecIdIso: self.Hist["Z2EECounter"].AddBinContent(4, event.eventWeight)
        if muonTrigger and muonIdIso: self.Hist["Z2MMCounter"].AddBinContent(4, event.eventWeight)
        
        # Categorization
        if elecIdIso and muonIdIso:
            if elecTrigger and event.highptIdIsoElectrons[0].pt() > event.highptIdIsoMuons[0].pt():
                event.isZ2EE = True
            elif muonTrigger and event.highptIdIsoElectrons[0].pt() < event.highptIdIsoMuons[0].pt():
                event.isZ2EE = False
            else:
                event.isZ2EE = event.highptIdIsoElectrons[0].pt() > event.highptIdIsoMuons[0].pt()
        elif elecIdIso and not muonIdIso and elecTrigger:
            event.isZ2EE = True
        elif muonIdIso and not elecIdIso and muonTrigger:
            event.isZ2EE = False
        else:
            return True
        event.isZ2MM = not event.isZ2EE
        
        #for i, m in enumerate(event.highptMuons):
        #    if m.muonID("POG_ID_HighPt"): m.setP4( m.tunePMuonBestTrack().get().p4() )
        
        event.highptLeptons = event.highptIdIsoElectrons if event.isZ2EE else event.highptIdIsoMuons
        
        self.addFakeMet(event, [event.highptLeptons[0], event.highptLeptons[1]])
        
        # Z candidate
        event.Z = event.highptLeptons[0].p4() + event.highptLeptons[1].p4()
        event.Z.charge = event.highptLeptons[0].charge() + event.highptLeptons[1].charge()
        event.Z.deltaR = deltaR(event.highptLeptons[0].eta(), event.highptLeptons[0].phi(), event.highptLeptons[1].eta(), event.highptLeptons[1].phi())
        event.Z.deltaEta = abs(event.highptLeptons[0].eta() - event.highptLeptons[1].eta())
        event.Z.deltaPhi = deltaPhi(event.highptLeptons[0].phi(), event.highptLeptons[1].phi())
        # Z candidate
        if event.Z.mass() < 50. or event.highptLeptons[0].charge() == event.highptLeptons[1].charge():
            return True
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(5, event.eventWeight)
        
        # Z mass
        if event.Z.mass() < self.cfg_ana.Z_mass_low or event.Z.mass() > self.cfg_ana.Z_mass_high:
            return True
        # Z pt
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(6, event.eventWeight)
        if event.Z.pt() < self.cfg_ana.Z_pt:
            return True
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(7, event.eventWeight)
        
        # Fill tree
        event.isXZh = True
        
        #########################
        #    Part 2: Jets       #
        #########################
        
        for i, j in enumerate(event.cleanJetsAK8):
            if j.pt() >= self.cfg_ana.fatjet_pt:
                if deltaR(event.highptLeptons[0].eta(), event.highptLeptons[0].phi(), j.eta(), j.phi()) > self.cfg_ana.jetlep_dR and deltaR(event.highptLeptons[1].eta(), event.highptLeptons[1].phi(), j.eta(), j.phi()) > self.cfg_ana.jetlep_dR:
                    #self.addCorrectedJetMass(event, j)
                    event.highptFatJets.append(j)
        event.highptFatJets.sort(key = lambda j : j.pt(), reverse = True) #j.userFloat(self.cfg_ana.jetAlgo)
            

        if len(event.highptFatJets) < 1:
            return True
        
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(8, event.eventWeight)
        
        #########################
        #   Part 3: Candidates  #
        #########################
        
        # h candidate with pseudo-kin fit
        kH = event.highptFatJets[0].p4()
        k = 125.0/event.highptFatJets[0].mass() if event.highptFatJets[0].mass() > 0 else 0. #.userFloat(self.cfg_ana.jetAlgo)
        kH = ROOT.reco.Particle.LorentzVector(event.highptFatJets[0].px()*k, event.highptFatJets[0].py()*k, event.highptFatJets[0].pz()*k, event.highptFatJets[0].energy()*k)
        
        # A/Z' candidate
        event.X = event.Z + event.highptFatJets[0].p4()
        event.X.mT = event.X.mass()
        event.X.mC = event.X.mass()
        event.X.mK = (event.Z + kH).mass()
        event.X.deltaR = deltaR(event.Z.eta(), event.Z.phi(), event.highptFatJets[0].eta(), event.highptFatJets[0].phi())
        event.X.deltaEta = abs(event.Z.eta() - event.highptFatJets[0].eta())
        event.X.deltaPhi = abs(deltaPhi(event.Z.phi(), event.highptFatJets[0].phi()))
        
#        elif len(event.highptLeptons) == 1:
#            event.X = event.highptLeptons[0].p4() + event.met.p4() + event.highptFatJets[0].p4()
#            pz = 0.
#            a = 80.4**2 - event.highptLeptons[0].mass()**2 + 2.*event.highptLeptons[0].px()*event.met.px() + 2.*event.highptLeptons[0].py()*event.met.py()
#            A = 4*( event.highptLeptons[0].energy()**2 - event.highptLeptons[0].pz()**2 )
#            B = -4*a*event.highptLeptons[0].pz()
#            C = 4*event.highptLeptons[0].energy()**2 * (event.met.px()**2  + event.met.py()**2) - a**2
#            D = B**2 - 4*A*C
#            if D>0:
#                pz = min((-B+math.sqrt(D))/(2*A), (-B-math.sqrt(D))/(2*A))
#            else:
#                pz = -B/(2*A)
#            kmet = event.met.p4()
#            kmet.SetPz(pz)
#            event.X.mT = (event.highptLeptons[0].p4() + kmet + event.highptFatJets[0].p4()).mass()
#            cmet = event.met.p4()
#            cmet.SetPz(event.highptLeptons[0].pz())
#            event.X.mC = (event.highptLeptons[0].p4() + cmet + event.highptFatJets[0].p4()).mass()
#            event.X.mK = (event.highptLeptons[0].p4() + kmet + kH).mass()
        
        if event.Z.pt() < self.cfg_ana.Z_pt:
            return True
        
        # Fill tree
        event.isXZh = True
        
        # ---------- Estimate cuts ----------
        if event.highptFatJets[0].userFloat(self.cfg_ana.jetAlgo) < 95 or event.highptFatJets[0].userFloat(self.cfg_ana.jetAlgo) > 130:
            return True
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(9, event.eventWeight) if event.isZ2EE else self.Hist["Z2MMCounter"].AddBinContent(9, event.eventWeight)
        if len(event.highptFatJets[0].subjets('SoftDrop')) < 2 or not (event.highptFatJets[0].subjets('SoftDrop')[0].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605 or event.highptFatJets[0].subjets('SoftDrop')[1].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605):
            return True
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(10, event.eventWeight) if event.isZ2EE else self.Hist["Z2MMCounter"].AddBinContent(10, event.eventWeight)
        if len(event.highptFatJets[0].subjets('SoftDrop')) < 2 or not (event.highptFatJets[0].subjets('SoftDrop')[0].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605 and event.highptFatJets[0].subjets('SoftDrop')[1].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') > 0.605):
            return True
        self.Hist["Z2EECounter" if event.isZ2EE else "Z2MMCounter"].AddBinContent(11, event.eventWeight)
        
        return True

