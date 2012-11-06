
# 2011

may10 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt'
aug05 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt'
pr2011 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'

# 2012

pr2012AB = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt'
# the good one:
pr2012C = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'
# the old one 
# pr2012C = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt'
aug6_2012 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
aug24_2012 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'
jul13_2012 = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt'


jsonMap = {
    #
    # 2011A 10May
    # 
    '/TauPlusX/Run2011A-May10ReReco-v1/AOD':may10,	
    '/Tau/Run2011A-May10ReReco-v1/AOD':may10,	        
    '/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':may10,    
    '/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':may10,    
    '/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':may10,
    #
    # 2011A PR V4
    #
    '/TauPlusX/Run2011A-PromptReco-v4/AOD':pr2011,
    '/Tau/Run2011A-PromptReco-v4/AOD':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    #
    # 2011A 05Aug
    # 
    '/TauPlusX/Run2011A-05Aug2011-v1/AOD':aug05,
    '/Tau/Run2011A-05Aug2011-v1/AOD':aug05,
    '/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':aug05,
    '/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':aug05,
    '/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':aug05,
    #
    # 2011A 03Oct (PR V6)
    #
    '/TauPlusX/Run2011A-03Oct2011-v1/AOD':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    #
    # 2011B PR V1
    # 
    '/TauPlusX/Run2011B-PromptReco-v1/AOD':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER':pr2011,
    # 
    # 2012A & B
    #
    '/TauPlusX/Run2012A-PromptReco-v1/AOD':pr2012AB,
    '/TauPlusX/Run2012B-PromptReco-v1/AOD':pr2012AB,
    '/TauPlusX/Run2012B-13Jul2012-v1/AOD':jul13_2012,
    '/TauPlusX/Run2012A-recover-06Aug2012-v1/AOD':aug6_2012,
    '/TauPlusX/Run2012A-13Jul2012-v1/AOD':jul13_2012,
    '/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run196090to196531_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012AB,
    '/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':jul13_2012,
    '/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':aug6_2012,
    '/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':jul13_2012,
    #
    # 2012C
    #
    '/TauPlusX/Run2012C-PromptReco-v1/AOD':pr2012C,
    '/TauPlusX/Run2012C-24Aug2012-v1/AOD':aug24_2012,
    '/TauPlusX/Run2012C-PromptReco-v2/AOD':pr2012C,
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':aug24_2012,
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER':pr2012C,
    #
    # non Tau samples
    #
    '/DoubleMu/Run2011A-16Jan2012-v1/AOD':pr2011,
    '/DoubleMu/Run2011B-16Jan2012-v1/AOD':pr2011 
    }
