# investigating an event in the DYJet sample for which
# the 2 lepton veto techniques give different results 

from CMGTools.H2TauTau.macros.PhysicsObjects import DiTau, Lepton, Jet, bestDiTau

allTmp = loop.handles['cmgTauMu'].product()
allDiTaus = [ DiTau(diTau) for diTau in allTmp ]

for diTau in allDiTaus:
    print diTau
    mu = Lepton( diTau.leg2() )
    tau = Lepton( diTau.leg1() )
    print '\ttau leg: ', tau
    print '\tmu  leg: ', mu
    print '\tmu  cuts     ', diTau.getSelection('cuts_baseline_muLeg')
    print '\ttau cuts     ', diTau.getSelection('cuts_baseline_tauLeg')
    print '\ttau mass cut ', diTau.getSelection('cuts_skimming_mass')

print '\n'.join( map(str, loop.leptons) ) 
