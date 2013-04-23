import ROOT
import os, types

class JetReCalibrator:
    def __init__(self,globalTag,jetFlavour,doResidualJECs):
        # Make base corrections
        path = "%s/src/CMGTools/TTHAnalysis/data/jec" % os.environ['CMSSW_BASE'];
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
    def correct(self,jet,rho):
        self.JetCorrector.setJetEta(jet.eta())
        self.JetCorrector.setJetPt(jet.pt() * jet.rawFactor())
        self.JetCorrector.setJetA(jet.jetArea())
        self.JetCorrector.setRho(rho)
        corr = self.JetCorrector.getCorrection()
        #print "   jet with raw pt %6.2f eta %+5.3f phi %+5.3f: previous corr %.4f, my corr %.4f " % (jet.pt()*jet.rawFactor(), jet.eta(), jet.phi(), 1./jet.rawFactor(), corr)
        jet.setP4(jet.p4() * (corr * jet.rawFactor()))
        # overload rawFactor() since we don't have setters
        jet._rawFactor = 1.0/corr
        jet.rawFactor = types.MethodType(lambda self : self._rawFactor, jet, jet.__class__)
        

