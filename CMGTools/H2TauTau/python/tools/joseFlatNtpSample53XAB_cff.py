import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix

HiggsMass = cms.vstring(['110','115','120','125','130','135','140','145'])
#SUSYMass = cms.vstring(['90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYMass = cms.vstring(['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])

import os 

from CMGTools.Utilities.metRecoilCorrection.rootfile_dir import rootfile_dir
recoilCorr_dir = rootfile_dir

httjson_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/json/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'

####--------------------------------------muTau 2012--------------------------------------------
    
def configureFlatNtpSampleTauMu2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.muPtCut = 20.
    module.tauPtCut = 20.
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')


    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.trigPath3 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath2 = cms.InputTag("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5","hltOverlapFilterIsoMu18LooseIsoPFTau20","hltOverlapFilterIsoMu18LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'Embedded2012A2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'Embedded2012B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 11
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'

    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        
    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
 
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 11
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.dataType = 0
            module.path = "/VBF_HToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
            module.recoilCorrection = 11
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if i == 5 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
            module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    
  

####--------------------------------------e-Tau 2012--------------------------------------------

def configureFlatNtpSampleTauEle2012(module,sampleAlias):
    module.dataPeriodFlag = 2012
    module.muPtCut = 24.
    module.tauPtCut = 20.
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm53X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
    module.mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_52X.weights.xml')
        
    if sampleAlias == 'TauPlusX2012A' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-13Jul2012-v1/AOD/V5/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'TauPlusX2012A2' : #DAS range: 190450 - 193686
        module.path = "/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath2 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.trigPath3 = cms.InputTag("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6","hltOverlapFilterIsoEle20LooseIsoPFTau20","hltOverlapFilterIsoEle20LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'TauPlusX2012B' : #DAS range: 193752 - 197044
        module.path = "/TauPlusX/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'


    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

    if sampleAlias == 'Embedded2012A2' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
        
    if sampleAlias == 'Embedded2012B' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0"
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'

 

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 11
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'


    if sampleAlias == 'ZToEE' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 1
        module.sampleTruthEventType = 1
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        
 
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 

    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" 
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
        module.recoilCorrection = 12
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 


    if sampleAlias == 'TTJets' :
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0"
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
        module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")



    for i in range(0,8):

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path ="/GluGluToHToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 11
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-%s_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'                                             
           #hltOverlapFilterEle20LooseIsoPFTau20, hltIsoElePFTau20TrackLooseIso, hltOverlapFilterIsoEle20WP90LooseIsoPFTau20, hltOverlapFilterEle20LooseIsoPFTau20
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")
            module.recoilCorrection = 11
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root' 

            
        if sampleAlias == "HiggsVH"+HiggsMass[i] :
            if i == 5 :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            else :
                module.path = "/WH_ZH_TTH_HToTauTau_M-%s_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0" % HiggsMass[i]
            module.dataType = 0
            module.pupWeightName = 'vertexWeightSummer12MC53XICHEPData'
            module.trigPath1 = cms.InputTag("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20","hltOverlapFilterIsoEle20WP90LooseIsoPFTau20")


