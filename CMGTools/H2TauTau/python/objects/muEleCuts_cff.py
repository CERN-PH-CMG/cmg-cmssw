import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.muCuts_cff import getMuCuts
from CMGTools.H2TauTau.objects.eleCuts_cff import getEleCuts

muEleCuts = cms.PSet(
    baseline = cms.PSet(
         mass = cms.string('mass()>10'),
         muLeg = getMuCuts('leg1', 'muEle').clone(),
         eleLeg = getEleCuts('leg2', 'muEle').clone()
         ),
    # this cut is kept out of the baseline for now, until it is studied.
    # caloMuVeto = cms.string('leg1().eOverP()>0.2'),
    )

if __name__ == '__main__':

    print muEleCuts
