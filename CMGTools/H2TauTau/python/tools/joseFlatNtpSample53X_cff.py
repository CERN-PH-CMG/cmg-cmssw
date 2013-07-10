import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix
import os

HiggsMass = cms.vstring(['90','95','100','105','110','115','120','125','130','135','140','145','150','155','160'])
Higgsmass = [90,95,100,105,110,115,120,125,130,135,140,145,150,155,160]
#SUSYMass = cms.vstring(['90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYMass = cms.vstring(['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYmass = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000]

from CMGTools.Utilities.metRecoilCorrection.rootfile_dir import rootfile_dir
recoilCorr_dir = rootfile_dir

httjson_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/json/'
json_dir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'

#cmgtag = 'PAT_CMG_V5_8_0'

#cmgtag = cms.untracked.string('PAT_CMG_V5_13_0_BleedingEdge')
#cmgtag = cms.untracked.string('PAT_CMG_V5_13_0_BleedingEdge_eConvFix')
#cmgtag = 'PAT_CMG_V5_13_0_BleedingEdge_MET53X'

##fixed vertex collection in inputs
#cmgtag = 'PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar27'

##fixed PF jet id in Mva met
#cmgtag = 'PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar29'

##fixed muon iso veto cone to 1e-3
#cmgtag = 'PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar31'

##fixed tau track ref to recover gsf tracks
#cmgtag = 'PAT_CMG_V5_13_0_BleedingEdge_MET53X_Apr2'

#fixed pf cand dZ in TKMet
#cmgtag = 'PAT_CMG_V5_14_0'
cmgtag = 'PAT_CMG_V5_16_0'
cmgtagEmb = 'PAT_CMG_V5_16_0_B'


####--------------------------------------muTau 2012--------------------------------------------
def configureFlatNtpSampleTauMu2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53XRR_2012_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53XRR_2012_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')
    module.pupWeightNames1 = 'vertexWeightSummer12MC53XICHEPData'
    module.pupWeightNames2 = 'vertexWeightSummer12MC53XHCPData'
    module.pupWeightNames3 = 'vertexWeightSummer12MC53X2012D6fbData'
    module.pupWeightNames4 = 'vertexWeightSummer12MC53X2012ABCDData'
    module.muPtCut = 20.
    module.tauPtCut = 20.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonTight'
    module.tauAntiEDisc = 'againstElectronLoose'

    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath3 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv1' : #DAS range: 197770 - 198913
        module.path = "/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv2' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-PromptReco-v2/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'

    if sampleAlias == 'TauPlusX2012D' : #DAS range: 203773 -  
        module.path = "/TauPlusX/Run2012D-PromptReco-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'


    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
                
    if sampleAlias == 'Embedded2012A2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012Cv1' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012Cv2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'  
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012D' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012D_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.

    ####New ReReco samples
        
    if sampleAlias == 'TauPlusX2012ReRecoA' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath3 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    
    if sampleAlias == 'TauPlusX2012ReRecoB' :
        module.path = "/TauPlusX/Run2012B-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012ReRecoC' :
        module.path = "/TauPlusX/Run2012C-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012ReRecoD' : 
        module.path = "/TauPlusX/Run2012D-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'



    if sampleAlias == 'RHEmbedded2012A' : 
        module.path = "/DoubleMu/StoreResults-Run2012A_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")
  
    if sampleAlias == 'RHEmbedded2012B' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012B_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")
        
    if sampleAlias == 'RHEmbedded2012C' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012C_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")
        
    if sampleAlias == 'RHEmbedded2012D' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012D_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")
        
        
    if sampleAlias == 'ZToTauTau' or sampleAlias == 'Z1ToTauTau' or sampleAlias == 'Z2ToTauTau' or sampleAlias == 'Z3ToTauTau' or sampleAlias == 'Z4ToTauTau': 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.saveLHENUP = 1
        module.correctTauES = 0

    if sampleAlias == 'ZToMuMu'  or sampleAlias == 'Z1ToMuMu' or sampleAlias == 'Z2ToMuMu' or sampleAlias == 'Z3ToMuMu' or sampleAlias == 'Z4ToMuMu' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.saveLHENUP = 1

    if sampleAlias == 'ZToLJet'  or sampleAlias == 'Z1ToLJet' or sampleAlias == 'Z2ToLJet' or sampleAlias == 'Z3ToLJet' or sampleAlias == 'Z4ToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.saveLHENUP = 1

    ##DY+njet samples, note they must be place after the above configs
    if sampleAlias == 'Z1ToTauTau' or sampleAlias == 'Z1ToMuMu'  or sampleAlias == 'Z1ToLJet' : 
        module.path = "/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
    if sampleAlias == 'Z2ToTauTau' or sampleAlias == 'Z2ToMuMu'  or sampleAlias == 'Z2ToLJet' : 
        module.path = "/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
    if sampleAlias == 'Z3ToTauTau' or sampleAlias == 'Z3ToMuMu'  or sampleAlias == 'Z3ToLJet' : 
        module.path = "/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)     
    if sampleAlias == 'Z4ToTauTau' or sampleAlias == 'Z4ToMuMu'  or sampleAlias == 'Z4ToLJet' : 
        module.path = "/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        
        
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'Wbb' :
        module.path = "/WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root'
        module.saveLHENUP = 1

    if sampleAlias == 'Wg' :
        module.path = "/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'TTJets' :
        #module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    for i in range(0,15):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            if Higgsmass[i] < 110  :
                module.path ="/GluGluToHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            else:
                module.path ="/GluGluToHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root'
            module.correctTauES = 0
            module.signalWeightMass = "{0}_8TeV".format(HiggsMass[i])
            

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
           
            if Higgsmass[i] < 110 :                
                module.path = "/VBFHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 120 :
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 125 :
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            else:
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
                
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root'
            module.correctTauES = 0            
            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if Higgsmass[i] < 110 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 135 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
            module.correctTauES = 0


    for i in range(0,21):
        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            if SUSYmass[i] == 300 :
                module.path ="/SUSYBBHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            else:
                module.path ="/SUSYBBHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root'
            module.correctTauES = 0
            module.sampleGenMassMin = SUSYmass[i] * 0.7
            module.sampleGenMassMax = SUSYmass[i] * 1.3

        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root' 
            module.correctTauES = 0
            module.sampleGenMassMin = SUSYmass[i] * 0.7
            module.sampleGenMassMax = SUSYmass[i] * 1.3

####Trigger measurements
def configureFlatNtpSampleTauMu2012Trig(module,sampleAlias):
    ##Remember tauPtCut should be loose > 15 GeV
    module.dataPeriodFlag = 2012
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_2012_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_2012_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')
    module.pupWeightNames1 = 'vertexWeightSummer12MC53XICHEPData'
    module.pupWeightNames2 = 'vertexWeightSummer12MC53XHCPData'
    module.pupWeightNames3 = 'vertexWeightSummer12MC53X2012D6fbData'
    module.pupWeightNames4 = 'vertexWeightSummer12MC53X2012ABCDData'
    module.muPtCut = 20.
    module.tauPtCut = 15.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonTight'
    module.tauAntiEDisc = 'againstElectronLoose'
        
    if sampleAlias == 'TauPlusX2012ReRecoA' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v1","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v2","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPath3 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v3","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPath4 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v4","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPath5 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPathTest2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPathTest3 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'


    if sampleAlias == 'TauPlusX2012ReRecoB' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012ReRecoC' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v6","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPath2 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v7","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.trigPathTest2 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.trigPathTest3 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012ReRecoD' : 
        module.path = "/TauPlusX/Run2012D-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v7","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 5
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        module.correctTauES = 0
        
    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
                
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
                
    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root'
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'TTJets' :
        #module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_eta2p1_L1ETM20_v5","","hltL3crIsoL1sMu12Eta2p1L1f0L2f12QL3f15QL3crIsoRhoFiltered0p15")
        module.trigPathTest1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")


####--------------------------------------e-Tau 2012--------------------------------------------

def configureFlatNtpSampleTauEle2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53XRR_2012_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53XRR_2012_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')
    module.pupWeightNames1 = 'vertexWeightSummer12MC53XICHEPData'
    module.pupWeightNames2 = 'vertexWeightSummer12MC53XHCPData'
    module.pupWeightNames3 = 'vertexWeightSummer12MC53X2012D6fbData'
    module.pupWeightNames4 = 'vertexWeightSummer12MC53X2012ABCDData'
    module.muPtCut = 24.
    module.tauPtCut = 20.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonLoose'
    module.tauAntiEDisc = 'againstElectronLooseMVA3'
    
    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv1' : #DAS range: 197770 - 198913
        module.path = "/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012Cv2' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-PromptReco-v2/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath6 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath7 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'


    if sampleAlias == 'TauPlusX2012D' : 
        module.path = "/TauPlusX/Run2012D-PromptReco-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        #module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20")
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v9","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        #module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-206539_8TeV_PromptReco_Collisions12_JSON.txt'
        #module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-207898_8TeV_PromptReco_Collisions12_JSON.txt'
        module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'




    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012A2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.        
        
    if sampleAlias == 'Embedded2012Cv1' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012Cv2' : 
        #module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.path = "/DoubleMu/StoreResults-Run2012C_PromptReco_v2_embedded_trans1_tau115_ptelec1_17had1_17_v2-5ef1c0fd428eb740081f19333520fdc8/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        
    if sampleAlias == 'Embedded2012D' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_2012D_PromptReco_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
        module.correctTauES = 0 
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.


    ####New Rereco samples
    if sampleAlias == 'TauPlusX2012ReRecoA' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012ReRecoB' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012ReRecoC' : #DAS range: 198934 - 203755
        module.path = "/TauPlusX/Run2012C-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath6 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath7 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'


    if sampleAlias == 'TauPlusX2012ReRecoD' : 
        module.path = "/TauPlusX/Run2012D-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v9","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'



    if sampleAlias == 'RHEmbedded2012A' : 
        module.path = "/DoubleMu/StoreResults-Run2012A_22Jan2013_v1_RHembedded_trans1_tau115_ptelec1_20had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")

        
    if sampleAlias == 'RHEmbedded2012B' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012B_22Jan2013_v1_RHembedded_trans1_tau115_ptelec1_20had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")

    if sampleAlias == 'RHEmbedded2012C' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012C_22Jan2013_v1_RHembedded_trans1_tau115_ptelec1_20had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")                                                                          

    if sampleAlias == 'RHEmbedded2012D' : 
        module.path = "/DoubleMuParked/StoreResults-Run2012D_22Jan2013_v1_RHembedded_trans1_tau115_ptelec1_20had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtagEmb)
        module.dataType = 2
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.
        module.embeddedWeightType = 2
        module.embeddedKinWeightFile = 'TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root'
        module.trigPath1 = cms.InputTag("HLT_Mu17_Mu8_v16","","")
        module.trigPath2 = cms.InputTag("HLT_Mu17_Mu8_v17","","")
        module.trigPath3 = cms.InputTag("HLT_Mu17_Mu8_v18","","")
        module.trigPath4 = cms.InputTag("HLT_Mu17_Mu8_v19","","")
        module.trigPath5 = cms.InputTag("HLT_Mu17_Mu8_v21","","")
        module.trigPath6 = cms.InputTag("HLT_Mu17_Mu8_v22","","")
        
    if sampleAlias == 'ZToTauTau'  or sampleAlias == 'Z1ToTauTau' or sampleAlias == 'Z2ToTauTau' or sampleAlias == 'Z3ToTauTau' or sampleAlias == 'Z4ToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.saveLHENUP = 1
        module.correctTauES = 0
        
    if sampleAlias == 'ZToEE'  or sampleAlias == 'Z1ToEE' or sampleAlias == 'Z2ToEE' or sampleAlias == 'Z3ToEE' or sampleAlias == 'Z4ToEE' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.saveLHENUP = 1
        
    if sampleAlias == 'ZToLJet'  or sampleAlias == 'Z1ToLJet' or sampleAlias == 'Z2ToLJet' or sampleAlias == 'Z3ToLJet' or sampleAlias == 'Z4ToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.saveLHENUP = 1


    ##DY+njet samples, note they must be place after the above configs
    if sampleAlias == 'Z1ToTauTau' or sampleAlias == 'Z1ToEE'  or sampleAlias == 'Z1ToLJet' : 
        module.path = "/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
    if sampleAlias == 'Z2ToTauTau' or sampleAlias == 'Z2ToEE'  or sampleAlias == 'Z2ToLJet' : 
        module.path = "/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
    if sampleAlias == 'Z3ToTauTau' or sampleAlias == 'Z3ToEE'  or sampleAlias == 'Z3ToLJet' : 
        module.path = "/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)     
    if sampleAlias == 'Z4ToTauTau' or sampleAlias == 'Z4ToEE'  or sampleAlias == 'Z4ToLJet' : 
        module.path = "/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        

 
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1

    if sampleAlias == 'Wg' :
        module.path = "/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1

    if sampleAlias == 'TTJets' :
        #module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")



    for i in range(0,15):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            if Higgsmass[i] < 110 :
                module.path ="/GluGluToHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            else:                
                module.path ="/GluGluToHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root'
            module.correctTauES = 0
            module.signalWeightMass = "{0}_8TeV".format(HiggsMass[i])

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            if Higgsmass[i] < 110 :                
                module.path = "/VBFHToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 120 :
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 125 :
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            else:
                module.path = "/VBF_HToTauTau_M-{0}_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'                                             
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root' 
            module.correctTauES = 0
            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if Higgsmass[i] < 110 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            elif Higgsmass[i] == 135 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.correctTauES = 0


    for i in range(0,21):
        if sampleAlias == "SUSYBB"+SUSYMass[i] :
            if SUSYmass[i] == 300 :
                module.path ="/SUSYBBHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            else:
                module.path ="/SUSYBBHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root' 
            module.correctTauES = 0
            module.sampleGenMassMin = SUSYmass[i] * 0.7
            module.sampleGenMassMax = SUSYmass[i] * 1.3

        if sampleAlias == "SUSYGG"+SUSYMass[i] :
            module.path ="/SUSYGluGluToHToTauTau_M-{0}_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{1}".format(SUSYMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs53X_20pv_njet.root' 
            module.correctTauES = 0
            module.sampleGenMassMin = SUSYmass[i] * 0.7
            module.sampleGenMassMax = SUSYmass[i] * 1.3

def configureFlatNtpSampleTauEle2012Trig(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
    module.pupWeightNames1 = 'vertexWeightSummer12MC53XICHEPData'
    module.pupWeightNames2 = 'vertexWeightSummer12MC53XHCPData'
    module.pupWeightNames3 = 'vertexWeightSummer12MC53X2012D6fbData'
    module.pupWeightNames4 = 'vertexWeightSummer12MC53X2012ABCDData'
    module.muPtCut = 24.
    module.tauPtCut = 15.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonLoose'
    module.tauAntiEDisc = 'againstElectronTightMVA3'
        
    if sampleAlias == 'SingleElectron2012ReRecoA' : #DAS range: 190450 - 193686
        module.path = "/SingleElectron/Run2012A-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")
        module.trigPath2 = cms.InputTag("HLT_Ele27_WP80_v11","","hltEle27WP80TrackIsoFilter")
        module.trigPathTest1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPathTest2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPathTest3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

        
    if sampleAlias == 'SingleElectron2012ReRecoB' : #DAS range: 193752 - 197044
        module.path = "/SingleElectron/Run2012B-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")
        module.trigPath2 = cms.InputTag("HLT_Ele27_WP80_v11","","hltEle27WP80TrackIsoFilter")
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'


    if sampleAlias == 'SingleElectron2012ReRecoC' : #DAS range: 198934 - 203755
        module.path = "/SingleElectron/Run2012C-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")
        module.trigPath2 = cms.InputTag("HLT_Ele27_WP80_v11","","hltEle27WP80TrackIsoFilter")
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest6 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest7 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'


    if sampleAlias == 'SingleElectron2012ReRecoD' : 
        module.path = "/SingleElectron/Run2012D-22Jan2013-v1/AOD/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")
        module.trigPath2 = cms.InputTag("HLT_Ele27_WP80_v11","","hltEle27WP80TrackIsoFilter")
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest2 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v3","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest3 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest4 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest5 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest6 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v7","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.trigPathTest7 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v8","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = json_dir + 'Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.saveLHENUP = 1
        module.correctTauES = 0        
        
    if sampleAlias == 'ZToEE' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.saveLHENUP = 1
        
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_Ele27_WP80_v10","","hltEle27WP80TrackIsoFilter")        
        module.trigPathTest1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
