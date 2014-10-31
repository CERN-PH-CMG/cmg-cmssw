#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreDATAsequence)

from CMGTools.H2TauTau.proto.samples.getFiles import getFiles


data_Run2011A_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011A_12Oct2013_v1',
    files = getFiles('/SingleMu/Run2011A-12Oct2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    # files = ['file:/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/CMSSW_5_3_14/src/CMGTools/WMass/cfg/test/cmgTuple.root'],
    intLumi =  4749.90,
    triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )
    
data_Run2011B_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011B_12Oct2013_v1',
    files = getFiles('/SingleMu/Run2011B-12Oct2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )

data_Run2011AB_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011AB_12Oct2013_v1',
    files =  [],
    intLumi =  4749.90,
triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )

data_Run2011AB_12Oct2013_v1.files = data_Run2011A_12Oct2013_v1.files + data_Run2011B_12Oct2013_v1.files

# selectedComponents = [data_Run2011A_12Oct2013_v1,data_Run2011B_12Oct2013_v1]
data_Run2011A_12Oct2013_v1.splitFactor = 1000
data_Run2011B_12Oct2013_v1.splitFactor = 1000
data_Run2011AB_12Oct2013_v1.splitFactor = 2000
selectedComponents = [data_Run2011A_12Oct2013_v1,data_Run2011B_12Oct2013_v1]

# # TEST
# selectedComponents = [data_Run2011A_12Oct2013_v1]
# data_Run2011A_12Oct2013_v1.files = data_Run2011A_12Oct2013_v1.files[:10]
# data_Run2011A_12Oct2013_v1.splitFactor = 1

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


