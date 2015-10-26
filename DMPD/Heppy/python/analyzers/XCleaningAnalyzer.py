from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaR2, deltaPhi
import copy
import math
import ROOT
import sys

class XCleaningAnalyzer( Analyzer ):
    '''Perform cross-cleaning'''

    def beginLoop(self,setup):
        super(XCleaningAnalyzer,self).beginLoop(setup)

    def crossCleanTaus(self, event):

        toberemoved = []

        for tau in event.selectedTaus:
            # Clean taus from tight muons
            if self.cfg_ana.cleanFromMuons:
                for mu in event.selectedMuons:
                    if mu.pt() < self.cfg_ana.mu_clean_pt: continue
                    if not mu.muonID(self.cfg_ana.mu_clean_id): continue
                    if not self.cfg_ana.mu_clean_iso: continue

                    dRm = deltaR(mu.eta(),mu.phi(),tau.eta(),tau.phi())
                    if (dRm < self.cfg_ana.mu_tau_dr):
                        toberemoved.append(tau)

            # Clean taus from tight electrons
            if self.cfg_ana.cleanFromElectrons:
                for ele in event.selectedElectrons:
                    if ele.pt() < self.cfg_ana.ele_clean_pt: continue
                    if not ele.electronID(self.cfg_ana.ele_clean_id): continue
                    if not self.cfg_ana.ele_clean_iso: continue

                    dRe = deltaR(ele.eta(),ele.phi(),tau.eta(),tau.phi())
                    if (dRe < self.cfg_ana.ele_tau_dr):
                        toberemoved.append(tau)

        event.xcleanTaus = [x for x in event.selectedTaus if x not in toberemoved]

        return True


    def crossCleanJets(self, event):

        toberemoved = []

        for jet in event.cleanJets:
            # Clean jets from tight muons
            if self.cfg_ana.cleanFromMuons:
                for mu in event.selectedMuons:
                    if mu.pt() < self.cfg_ana.mu_clean_pt: continue
                    if not mu.muonID(self.cfg_ana.mu_clean_id): continue
                    if not self.cfg_ana.mu_clean_iso: continue

                    dRm = deltaR(mu.eta(),mu.phi(),jet.eta(),jet.phi())
                    if (dRm < self.cfg_ana.mu_jet_dr):
                        toberemoved.append(jet)

            # Clean jets from tight electrons
            if self.cfg_ana.cleanFromElectrons:
                for ele in event.selectedElectrons:
                    if ele.pt() < self.cfg_ana.ele_clean_pt: continue
                    if not ele.electronID(self.cfg_ana.ele_clean_id): continue
                    if not self.cfg_ana.ele_clean_iso: continue

                    dRe = deltaR(ele.eta(),ele.phi(),jet.eta(),jet.phi())
                    if (dRe < self.cfg_ana.ele_jet_dr):
                        toberemoved.append(jet)

        event.xcleanJets = [x for x in event.cleanJets if x not in toberemoved]

        return True


    def crossCleanJetsAK8(self, event):

        toberemoved = []

        for jet in event.cleanJetsAK8:
            # Clean jets from tight muons
            if self.cfg_ana.cleanFromMuons:
                for mu in event.selectedMuons:
                    if mu.pt() < self.cfg_ana.mu_clean_pt: continue
                    if not mu.muonID(self.cfg_ana.mu_clean_id): continue
                    if not self.cfg_ana.mu_clean_iso: continue

                    dRm = deltaR(mu.eta(),mu.phi(),jet.eta(),jet.phi())
                    if (dRm < self.cfg_ana.mu_fatjet_dr):
                        toberemoved.append(jet)

            # Clean jets from tight electrons
            if self.cfg_ana.cleanFromElectrons:
                for ele in event.selectedElectrons:
                    if ele.pt() < self.cfg_ana.ele_clean_pt: continue
                    if not ele.electronID(self.cfg_ana.ele_clean_id): continue
                    if not self.cfg_ana.ele_clean_iso: continue

                    dRe = deltaR(ele.eta(),ele.phi(),jet.eta(),jet.phi())
                    if (dRe < self.cfg_ana.ele_fatjet_dr):
                        toberemoved.append(jet)

        event.xcleanJetsAK8 = [x for x in event.cleanJetsAK8 if x not in toberemoved]

        return True



    def process(self, event):
        
        if self.cfg_ana.cleanTaus:
            self.crossCleanTaus(event)
        
        if self.cfg_ana.cleanJets:
            self.crossCleanJets(event)
        
        if self.cfg_ana.cleanJetsAK8:
            self.crossCleanJetsAK8(event)

        return True
