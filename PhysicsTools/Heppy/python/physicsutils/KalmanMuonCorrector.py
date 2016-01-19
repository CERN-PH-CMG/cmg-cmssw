import PhysicsTools.Heppy.loadlibs
import ROOT

class KalmanMuonCorrector:
    def __init__(self, calibration, isMC, isSync=False, smearMode="none"):
        self.kamuca = ROOT.KalmanMuonCalibrator(calibration, 0, isSync)
        self.isMC = isMC
        self.smearMode = smearMode
    def correct(self, mu, run):
        newPt = self.kamuca.getCorrectedPt(mu.pt(), mu.eta(), mu.phi(), mu.charge())
        newPtErr = newPt * self.kamuca.getCorrectedError(newPt, mu.eta(), mu.ptErr()/newPt)
        if self.isMC: # new we do the smearing
            if self.smearMode == "none":
                pass
            elif self.smearMode == "basic":
                newPt = self.kamuca.smear(newPt, mu.eta())
                newPtErr = newPt * self.kamuca.getCorrectedErrorAfterSmearing(newPt, mu.eta(), newPtErr/newPt)
            else:
                newPt = self.kamuca.smearUsingEbE(newPt, mu.eta(), newPtErr/newPt)
                newPtErr = newPt * self.kamuca.getCorrectedErrorAfterSmearing(newPt, mu.eta(), newPtErr/newPt)
        newP4 = ROOT.math.PtEtaPhiMLorentzVector(newPt, mu.eta(), mu.phi(), mu.mass())
        mu.setP4(newP4)
        mu._ptErr = newPtErr

    def correct_all(self, mus, run):
        for mu in mus:
            self.correct(mu, run)

class KalmanMuonCorrector2:
    def __init__(self, calibrationData, cailbrationMC, isMC, isSync=False, smearMode="ebeDiff"):
        self.kamuca = ROOT.KalmanMuonCalibrator(calibrationMC if isMC else calibrationData, 0, isSync)
        self.isMC = isMC
        self.smearMode = smearMode
        if self.isMC and smearMode =="ebeDiff":
            self.kamucaData = ROOT.KalmanMuonCalibrator(calibrationData, 0, isSync)

    def correct(self, mu, run):
        newPt = self.kamuca.getCorrectedPt(mu.pt(), mu.eta(), mu.phi(), mu.charge())
        newPtErr = newPt * self.kamuca.getCorrectedError(newPt, mu.eta(), mu.ptErr()/newPt)
        if self.isMC: # new we do the smearing
            if self.smearMode == "none":
                pass
            elif self.smearMode == "basic":
                newPt = self.kamuca.smear(newPt, mu.eta())
                newPtErr = newPt * self.kamuca.getCorrectedErrorAfterSmearing(newPt, mu.eta(), newPtErr/newPt)
            elif self.smearMode == "ebeDiff":
                newPtErrData = newPt * self.kamucaData.getCorrectedError(newPt, mu.eta(), mu.ptErr()/newPt)
                sigmaDiff2 = newPtErrData**2 - newPtErr**2
                if (sigmaDiff2 > 0):
                    newPt += ROOT.gRandom.Gaus(0, sqrt(sigmaDiff2))
                    newPtErr = newPtErrData
            else:
                newPt = self.kamuca.smearUsingEbE(newPt, mu.eta(), newPtErr/newPt)
                newPtErr = newPt * self.kamuca.getCorrectedErrorAfterSmearing(newPt, mu.eta(), newPtErr/newPt)
        newP4 = ROOT.math.PtEtaPhiMLorentzVector(newPt, mu.eta(), mu.phi(), mu.mass())
        mu.setP4(newP4)
        mu._ptErr = newPtErr

    def correct_all(self, mus, run):
        for mu in mus:
            self.correct(mu, run)


if __name__ == '__main__':
    kamuka = KalmanMuonCorrector("MC_74X_13TeV", True)
