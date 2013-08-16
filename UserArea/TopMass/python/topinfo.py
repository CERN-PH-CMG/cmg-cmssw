
## this file contains the general informations about the ful analysis
## all samples, number of events, cross-sections etc.
## and luminosity

lumi = 5814.139 ## FIXME: this has been calculated using lumiclac (i.e. HF method) 

samples = { 
    '/MuHad/Run2012A-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'              :[7855190  ,1.],
    '/MuHad/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'      :[778240   ,1.],
    '/SingleMu/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'           :[59538958 ,1.],
    '/SingleMu/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'           :[6076746  ,1.],
    '/SingleMu/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_V5_10_0'    :[1619573  ,1.],
    '/SingleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_10_0'               :[81770645 ,1.],
    '/SingleMu/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_10_0'               :[95637290 ,1.],   # 95641409 is the total in dbs (99.9957 %)
    '/ElectronHad/Run2012A-recover-06Aug2012-v1/AOD/V5_B/PAT_CMG_V5_10_0':[779825   ,1.],
    '/ElectronHad/Run2012A-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'        :[8092832  ,1.],
    '/SingleElectron/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'     :[67946476 ,1.],
    '/SingleElectron/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'     :[7466918  ,1.],
    '/SingleElectron/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_V5_10_0' :[1806578   ,1.],
    '/SingleElectron/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_10_0'            :[94646865  ,1.],    
    '/SingleElectron/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_10_0'            :[106614966 ,1.],   
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[6923750 , 225.2], 
    '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[30434389 , 3503.7], 
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'     :[75449843 , 36257.2], 
    ## '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'     :[18269870 , 36257.2], 
    ## '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_10_0'     :[57179973 , 36257.2], 
    '/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'       :[259961   , 3.79], 
    '/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'       :[3758227  , 56.4], 
    '/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'   :[497658   , 11.1], 
    '/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'    :[139974   , 1.76], 
    '/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'    :[1935072  , 30.7],
    '/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[493460   , 11.1], 
    '/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_10_0':[21484602, 3.64E8*3.7E-4],
    '/QCD_Pt_20_30_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'       :[1740229 , 2.886E8*5.8E-4],
    '/QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'       :[2048152 , 7.424E7*0.00225],
    '/QCD_Pt_80_170_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'      :[1945525 , 1191000.0*0.0109],
    '/QCD_Pt_20_30_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'  :[35040695, 2.886E8 * 0.0101],
    '/QCD_Pt_30_80_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'  :[33088888, 7.433E7 * 0.0621],
    '/QCD_Pt_80_170_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0' :[34542763, 1191000.0*0.1539],
    '/TTJets_mass161_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5369214  , 225.2],
    '/TTJets_mass163_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5365348  , 225.2],
    '/TTJets_mass166_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[4469095  , 225.2],
    '/TTJets_mass169_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5202817  , 225.2],
    '/TTJets_mass175_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5186494  , 225.2],
    '/TTJets_mass178_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[4733483  , 225.2],
    '/TTJets_mass181_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5145140  , 225.2],
    '/TTJets_mass184_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5249525  , 225.2],

    '/TTJets_scaledown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'   :[5387181 , 225.2], 
    '/TTJets_scaleup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'     :[5009488 , 225.2], 
    '/TTJets_matchingdown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0':[5476728 , 225.2], 
    '/TTJets_matchingup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'  :[5415010 , 225.2], 

    '/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'  : [6433355  , 225.2],
    '/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_10_0'  : [21634275 , 225.2], 
    '/TT_8TeV-mcatnlo/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'                        : [32852589 , 225.2], 
    }

 

## QCD cross sections from Enrique
## /QCD_Pt_20_MuEnrichedPt_15:       3.64E8 * 3.7E-4
## /QCD_Pt_20_30_BCtoE:                 2.886E8 * 5.8E-4
## /QCD_Pt_30_80_BCtoE :                7.424E7 * 0.00225
## /QCD_Pt_80_170_BCtoE:           1191000.0 * 0.0109
## /QCD_Pt_20_30_EMEnriched:        2.886E8 * 0.0101
## /QCD_Pt_30_80_EMEnriched :       7.433E7 * 0.0621
## /QCD_Pt_80_170_EMEnriched:  1191000.0 * 0.1539
