
## this file contains the general informations about the ful analysis
## all samples, number of events, cross-sections etc.
## and luminosity

lumi = 19.6 ## FIXME: this has to be properly calculated 

samples = { 
    '/SingleElectron/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_12_0'              :[ 16443264, 1.],   #  99.8136 %
    '/SingleElectron/Run2012A-recover-06Aug2012-v1/AOD/PAT_CMG_V5_12_0'      :[  4755186, 1.],
    '/SingleElectron/Run2012B-13Jul2012-v1/AOD/PAT_CMG_V5_12_0'              :[ 67892360, 1.],   #  99.9204 %
    '/SingleElectron/Run2012C-24Aug2012-v1/AOD/PAT_CMG_V5_12_0'              :[  7437942, 1.],   #  99.6119 %
    '/SingleElectron/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_12_0'             :[ 94516267, 1.],   #  99.862 %
    '/SingleElectron/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_V5_13_0'  :[  1806578, 1.],
    '/SingleElectron/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_12_0'             :[106493758, 1.],   #  99.8863 %
    '/SingleMu/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_12_0'                    :[ 16910666, 1.],   #  99.8269 %
    '/SingleMu/Run2012A-recover-06Aug2012-v1/AOD/PAT_CMG_V5_12_0'            :[  2845333, 1.],
    '/SingleMu/Run2012B-13Jul2012-v1/AOD/PAT_CMG_V5_12_0'                    :[ 59432796, 1.],   #  99.8217 %
    '/SingleMu/Run2012C-24Aug2012-v1/AOD/PAT_CMG_V5_12_0'                    :[  6061046, 1.],   #  99.7416 %
    '/SingleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_12_0'                   :[ 81689451, 1.],   #  99.9007 %
    '/SingleMu/Run2012C-EcalRecover_11Dec2012-v1/AOD/PAT_CMG_V5_13_0'        :[  1619573, 1.],
    '/SingleMu/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_12_0'                   :[ 95519747, 1.],   #  99.8728 %
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0' :[ 6923750,   225.2 ], 
    '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[30306769,  3503.7 ], 
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_13_0'            :[57141661, 36257.2 ], 
    '/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'              :[  259961,     3.79], 
    '/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'              :[ 3748227,    56.4 ], 
    '/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'          :[  497658,    11.1 ], 
    '/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'           :[  139974,     1.76], 
    '/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'           :[ 1935072,    30.7 ],
    '/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[  493460,    11.1 ], 
    '/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/V5_B/PAT_CMG_V5_13_0'      :[21484602, 3.64E8  * 3.7E-4 ],
    '/QCD_Pt_20_30_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'             :[ 1740229, 2.886E8 * 5.8E-4 ],
    '/QCD_Pt_30_80_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'             :[ 2048152, 7.424E7 * 0.00225],
    '/QCD_Pt_80_170_BCtoE_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'            :[ 1945525, 1191000.* 0.0109 ],
    '/QCD_Pt_20_30_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'        :[35028474, 2.886E8 * 0.0101 ],
    '/QCD_Pt_30_80_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'        :[33088888, 7.433E7 * 0.0621 ],
    '/QCD_Pt_80_170_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[34504567, 1191000.* 0.1539 ],
    '/TTJets_mass161_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5358593, 225.2],
    '/TTJets_mass163_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5365348, 225.2],
    '/TTJets_mass166_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 4469095, 225.2],
    '/TTJets_mass169_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5202817, 225.2],
    '/TTJets_mass175_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5186494, 225.2],
    '/TTJets_mass178_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 4723379, 225.2],
    '/TTJets_mass181_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5126484, 225.2],
    '/TTJets_mass184_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5249525, 225.2],
    '/TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'                  :[ 4216687, 225.2],	#99.2992 % 
    '/TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_13_0'                  :[12087658, 225.2],      #99.7413 % 
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'         :[ 7285914, 225.2],	#99.756  % 
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0' :[ 6923750, 225.2],
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_13_0' :[ 6897493, 225.2],	#99.6222 % 
    '/TTJets_matchingdown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'    :[ 5468633, 225.2],	#99.8522 % 
    '/TTJets_matchingup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'      :[ 5393645, 225.2],	#99.6054 % 
    '/TTJets_scaledown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'       :[ 5377388, 225.2],	#99.8182 % 
    '/TTJets_scaleup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'         :[ 5009488, 225.2],
    '/TT_8TeV-mcatnlo/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0'                                        :[32733574, 225.2],      #99.6377 %

   }
