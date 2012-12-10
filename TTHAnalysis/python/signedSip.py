import ROOT

#ROOT.gSystem.Load("libCMGToolsTTHAnalysis")
SignedImpactParameterComputer = ROOT.SignedImpactParameter()

def signedSip3D(lepton, vertex=None):
    if vertex is None:
        vertex = lepton.associatedVertex
    dir   = lepton.jet.momentum()         if hasattr(lepton,'jet')      else lepton.momentum()
    track = lepton.sourcePtr().gsfTrack() if abs(lepton.pdgId()) == 11  else lepton.sourcePtr().track()
    meas = SignedImpactParameterComputer.signedIP3D(track.get(), vertex, dir)
    return meas.significance()


def signedIp3D(lepton, vertex=None):
    if vertex is None:
        vertex = lepton.associatedVertex
    dir   = lepton.jet.momentum()         if hasattr(lepton,'jet')      else lepton.momentum()
    track = lepton.sourcePtr().gsfTrack() if abs(lepton.pdgId()) == 11  else lepton.sourcePtr().track()
    meas = SignedImpactParameterComputer.signedIP3D(track.get(), vertex, dir)
    return meas.value()
