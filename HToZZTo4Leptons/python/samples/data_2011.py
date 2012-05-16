import CMGTools.RootTools.fwlite.Config as cfg
import os

json ='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'
#json='/afs/cern.ch/user/n/ndefilip//public/CertTot2011.txt'
data_DoubleElectronA = cfg.DataComponent(
    name = 'data_DoubleElectronA',
    files = [],
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMuA = cfg.DataComponent(
    name = 'data_DoubleMuA',
    files = [],
    intLumi = 1,
    triggers = [],
    json = json
    )


data_DoubleElectronB = cfg.DataComponent(
    name = 'data_DoubleElectronB',
    files = [],
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMuB = cfg.DataComponent(
    name = 'data_DoubleMuB',
    files = [],
    intLumi = 1,
    triggers = [],
    json = json
    )

data_DoubleMu_2011 = [data_DoubleMuA,
                      data_DoubleMuB]
data_DoubleElectron_2011 = [data_DoubleElectronA,
                            data_DoubleElectronB]
