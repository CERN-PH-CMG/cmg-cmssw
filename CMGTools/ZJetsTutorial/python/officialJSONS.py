
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
    # 2011. not sure the prompt reco json should be used in this case
    #
    '/DoubleMu/Run2011A-16Jan2012-v1/AOD':pr2011,
    '/DoubleMu/Run2011B-16Jan2012-v1/AOD':pr2011,
    #
    # 2012
    #
    '/DoubleMu/Run2012A-13Jul2012-v1/AOD':jul13_2012,
    '/DoubleMu/Run2012A-recover-06Aug2012-v1':aug6_2012,
    '/DoubleMu/Run2012B-13Jul2012-v4/AOD':jul13_2012,
    '/DoubleMu/Run2012C-24Aug2012-v1':aug24_2012,
    '/DoubleMu/Run2012C-PromptReco-v2/AOD':pr2012C,
    }
