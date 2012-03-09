import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.tauCuts_cff import getTauCuts
# from CMGTools.H2TauTau.objects.tauSkimCuts_cff import * 

diTauCuts = cms.PSet(
    baseline = cms.PSet(
         mass = cms.string('mass()>10'),
         tau1Leg = getTauCuts('leg1','diTau').clone(),
         tau2Leg = getTauCuts('leg2','diTau').clone(),
         ),
    # this cut is kept out of the baseline for now, until it is studied.
    caloMuVeto = cms.string('leg1().eOverP()>0.2'),
    skimming = cms.PSet(
         mass = cms.string('mass()>10'),
         tau1LegSkimCuts = getTauCuts('leg1', 'diTau', skim=True).clone(),
         tau2LegSkimCuts = getTauCuts('leg2', 'diTau', skim=True).clone()
         )
    )

if __name__ == '__main__':

    print diTauCuts
