import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix
import os

HiggsMass = cms.vstring(['90','95','100','105','110','115','120','125','130','135','140','145','150','155','160'])
Higgsmass = [90,95,100,105,110,115,120,125,130,135,140,145,150,155,160]
#SUSYMass = cms.vstring(['90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYMass = cms.vstring(['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000'])
SUSYmass = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000]

#from CMGTools.Utilities.metRecoilCorrection.rootfile_dir import rootfile_dir
#recoilCorr_dir = rootfile_dir

recoilCorr_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Utilities/data/metRecoilCorrection/'
httjson_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/json/'
json_dir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'


#cmgtag = 'PAT_CMG_V5_14_0'
#cmgtagVBF = 'CMG_2011_V5_14_0_Test1'
#cmgtagData = 'PAT_CMG_2011_V5_14_0_Test1'

cmgtag = 'PAT_CMG_V5_15_0'


####--------------------------------------muTau 2012--------------------------------------------
def configureFlatNtpSampleTauMu2011(module,sampleAlias):
    module.dataPeriodFlag = 2011
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm42X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm42X_20pv_njet.root'
    module.pupWeightNames1 = 'vertexWeightFall112invfb'
    module.pupWeightNames2 = 'vertexWeight2011B'
    module.pupWeightNames3 = 'vertexWeightFall112011AB'
    module.pujetidname = 'full'
    module.muPtCut = 17.
    module.tauPtCut = 20.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonTight'
    module.tauAntiEDisc = 'againstElectronLoose'

    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5/{0}".format(cmgtag)
        module.dataType = 1
        module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
        module.jsonfile = json_dir + '/Collisions11/7TeV/Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt'

    if sampleAlias == 'Embedded2012A' : 
        module.path = "/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/{0}".format(cmgtag)
        module.dataType = 2
        module.jsonfile = httjson_dir + 'Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt'
        module.correctTauES = 0
        module.genParticlesTag =  'genParticles'
        module.sampleGenMassMin = 50.

    if sampleAlias == 'ZToTauTau' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 5
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 1
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_zmm53X_20pv_njet.root'
        module.saveLHENUP = 1
        module.correctTauES = 0
        
    if sampleAlias == 'ZToMuMu' : 
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 3
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.saveLHENUP = 1
        
    if sampleAlias == 'ZToLJet' :
        module.path = "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.sampleGenEventType = 3
        module.sampleTruthEventType = 6
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.saveLHENUP = 1
        
    if sampleAlias == 'WJetsToLNu' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1

    if sampleAlias == 'WJetsToLNu2' :
        module.path = "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W1JetsToLNu' :
        module.path = "/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W2JetsToLNu' :
        module.path = "/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W3JetsToLNu' :
        module.path = "/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'W4JetsToLNu' :
        module.path = "/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag) 
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'Wbb' :
        module.path = "/WbbJetsToLNu_Massive_TuneZ2star_8TeV-madgraph-pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root'
        module.saveLHENUP = 1

    if sampleAlias == 'Wg' :
        module.path = "/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 
        module.recoilCorrection = 2
        module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_wjets53X_20pv_njet.root' 
        module.saveLHENUP = 1
        
    if sampleAlias == 'TTJets' :
        #module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.path = "/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 


    if sampleAlias == 'WW2L2Nu' :
        module.path = "/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ3LNu' :
        module.path = "/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.pupWeightName = 'vertexWeightSummer12MC53XHCPData'
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'WZ2L2Q' :
        module.path = "/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ4L' :
        module.path = "/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Nu' :
        module.path = "/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'ZZ2L2Q' :
        module.path = "/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    if sampleAlias == 'TopTW' :        
        module.path = "/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 

    if sampleAlias == 'TopBTW' :        
        module.path = "/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/{0}".format(cmgtag)
        module.dataType = 0
        module.trigPath1 = cms.InputTag("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2","hltOverlapFilterIsoMu17LooseIsoPFTau20","hltOverlapFilterIsoMu17LooseIsoPFTau20") 



    for i in range(0,15):


        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-{0}_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs42X_20pv_njet.root'

        if sampleAlias == "HiggsGG"+HiggsMass[i] :
            module.path = "/GluGluToHToTauTau_M-{0}_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.trigPath1 = cms.InputTag("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15")
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs42X_20pv_njet.root'
            ##missing higgs pT weights 


            


  #for i in range(0,21):



####--------------------------------------e-Tau 2011--------------------------------------------

def configureFlatNtpSampleTauEle2011(module,sampleAlias):
    module.dataPeriodFlag = 2011
    module.fileZmmData = recoilCorr_dir + 'recoilfit_datamm42X_20pv_njet.root'
    module.fileZmmMC = recoilCorr_dir + 'recoilfit_zmm42X_20pv_njet.root'
    module.pupWeightNames1 = 'vertexWeightFall112invfb'
    module.pupWeightNames2 = 'vertexWeight2011B'
    module.pupWeightNames3 = 'vertexWeightFall112011AB'
    module.pujetidname = 'full'
    module.muPtCut = 20.
    module.tauPtCut = 20.
    module.muEtaCut = 2.1
    module.tauEtaCut = 2.3
    module.tauAntiMuDisc = 'againstMuonLoose'
    module.tauAntiEDisc = 'againstElectronTightMVA2'

    if sampleAlias == 'TauPlusXAug' : 
        module.path = "/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5/{0}".format(cmgtag)
        module.dataType = 1
        #module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","hltOverlapFilterIsoEle15TightIsoPFTau20","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")
        module.trigPath1 = cms.InputTag("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2","hltOverlapFilterIsoEle15TightIsoPFTau20","hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter")
        module.jsonfile = json_dir + '/Collisions11/7TeV/Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt'

    for i in range(0,15):


        if sampleAlias == "HiggsVBF"+HiggsMass[i] :
            module.path = "/VBF_HToTauTau_M-{0}_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/{1}".format(HiggsMass[i],cmgtag)
            module.dataType = 0
            module.pupWeightName = 'vertexWeightFall112011AB'
            module.trigPath1 = cms.InputTag("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1","hltPFTauMediumIso20TrackMediumIso","hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter") 
            module.recoilCorrection = 1
            module.fileCorrectTo =  recoilCorr_dir + 'recoilfit_higgs42X_20pv_njet.root'

 
    #for i in range(0,21):

