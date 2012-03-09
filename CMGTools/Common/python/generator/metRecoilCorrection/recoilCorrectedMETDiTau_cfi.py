'''By default, this module is configured for correcting
W+jets -> l + fake tau + jets events.

To configure it for Z + jets -> l + tau + jets events,
you need to change "fileCorrectTo", and to change leptonLeg to 0,
as we need to add the momentum of both leptons to the recoil to get the MET.
'''
import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'

recoilCorrectedMETDiTau = cms.EDProducer(
    "RecoilCorrectedMETProducerDiTau",
    metSrc = cms.InputTag('cmgPFMET'),
    # the tau is on the first leg and the muon on the second leg
    recBosonSrc = cms.InputTag('cmgDiTauSel'),
    genBosonSrc = cms.InputTag('genWorZ'),
    jetSrc = cms.InputTag('cmgPFJetForRecoil'),
    # 1: lepton is on leg1; 2: lepton is on leg2;
    # 0: take both legs as leptons, and sum them up 
    leptonLeg = cms.int32(0),
    # 1: type 1; 2 : type 2; 0 : all (use 1)
    correctionType = cms.int32(1),
    # fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_zjets_ltau_njet.root'),
    fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_wjets_njet.root'),
    # you should not have to change the files below
    fileZmmData = cms.string(rootfile_dir + 'recoilfit_datamm_njet.root'),
    fileZmmMC = cms.string(rootfile_dir + 'recoilfit_zmm42X_njet.root'),
    enable = cms.bool(True),
    verbose = cms.untracked.bool( False )
    #COLIN: make delta R a parameter
    )

