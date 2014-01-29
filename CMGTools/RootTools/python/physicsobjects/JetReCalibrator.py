import ROOT
import os, types

class JetReCalibrator:
    def __init__(self,globalTag,jetFlavour,doResidualJECs):
        """Create a corrector object that reads the payloads from the text dumps of a global tag under
            CMGTools/RootTools/data/jec  (see the getJec.py there to make the dumps).
           It will apply the L1,L2,L3 and possibly the residual corrections to the jets."""
        # Make base corrections
        path = "%s/src/CMGTools/RootTools/data/jec" % os.environ['CMSSW_BASE'];
        self.L1JetPar  = ROOT.JetCorrectorParameters("%s/%s_L1FastJet_%s.txt" % (path,globalTag,jetFlavour));
        self.L2JetPar  = ROOT.JetCorrectorParameters("%s/%s_L2Relative_%s.txt" % (path,globalTag,jetFlavour));
        self.L3JetPar  = ROOT.JetCorrectorParameters("%s/%s_L3Absolute_%s.txt" % (path,globalTag,jetFlavour));
        self.vPar = ROOT.vector(ROOT.JetCorrectorParameters)()
        self.vPar.push_back(self.L1JetPar);
        self.vPar.push_back(self.L2JetPar);
        self.vPar.push_back(self.L3JetPar);
        # Add residuals if needed
        if doResidualJECs : 
            self.ResJetPar = ROOT.JetCorrectorParameters("%s/%s_L2L3Residual_%s.txt" % (path,globalTag,jetFlavour))
            self.vPar.push_back(self.ResJetPar);
        #Step3 (Construct a FactorizedJetCorrector object) 
        self.JetCorrector = ROOT.FactorizedJetCorrector(self.vPar)
        self.JetUncertainty = ROOT.JetCorrectionUncertainty("%s/%s_Uncertainty_%s.txt" % (path,globalTag,jetFlavour));
    def correctAll(self,jets,rho,delta=0,metShift=[0,0]):
        """Applies 'correct' to all the jets, discard the ones that have bad corrections (corrected pt <= 0)"""
        badJets = []
        for j in jets:
            ok = self.correct(j,rho,delta,metShift)
            if not ok: badJets.append(j)
        if len(badJets) > 0:
            print "Warning: %d out of %d jets flagged bad by JEC." % (len(badJets), len(jets))
        for bj in badJets:
            jets.remove(bj)
    def correct(self,jet,rho,delta=0,metShift=[0,0]):
        """Corrects a jet energy (optionally shifting it also by delta times the JEC uncertainty)
           If a two-component list is passes as 'metShift', it will be modified adding to the first and second
           component the change to the MET along x and y due to the JEC, defined as the negative difference 
           between the new and old jet 4-vectors, for jets with corrected pt > 10."""
        self.JetCorrector.setJetEta(jet.eta())
        self.JetCorrector.setJetPt(jet.pt() * jet.rawFactor())
        self.JetCorrector.setJetA(jet.jetArea())
        self.JetCorrector.setRho(rho)
        corr = self.JetCorrector.getCorrection()
        self.JetUncertainty.setJetEta(jet.eta())
        self.JetUncertainty.setJetPt(corr * jet.pt() * jet.rawFactor())
        try:
            jet.jetEnergyCorrUncertainty = self.JetUncertainty.getUncertainty(True) 
        except RuntimeError, r:
            print "Caught %s when getting uncertainty for jet of pt %.1f, eta %.2f\n" % (r,corr * jet.pt() * jet.rawFactor(),jet.eta())
            jet.jetEnergyCorrUncertainty = 0.5
        if delta != 0:
            #print "   jet with corr pt %6.2f has an uncertainty %.2f " % (jet.pt()*jet.rawFactor()*corr, jet.jetEnergyCorrUncertainty)
            corr *= max(0, 1+delta*jet.jetEnergyCorrUncertainty)
        #print "   jet with raw pt %6.2f eta %+5.3f phi %+5.3f: previous corr %.4f, my corr %.4f " % (jet.pt()*jet.rawFactor(), jet.eta(), jet.phi(), 1./jet.rawFactor(), corr)
        if corr <= 0:
            return False
        if jet.pt() > 10:
            metShift[0] += jet.px()
            metShift[1] += jet.py()
        jet.setP4(jet.p4() * (corr * jet.rawFactor()))
        jet.setRawFactor(1.0/corr)
        if jet.pt() > 10:
            metShift[0] -= jet.px()
            metShift[1] -= jet.py()
        return True
        

