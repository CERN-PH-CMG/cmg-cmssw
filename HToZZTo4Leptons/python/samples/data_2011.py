import CMGTools.RootTools.fwlite.Config as cfg
import os

json = '/'.join( [ os.environ['CMSSW_BASE'],
                   'src/CMGTools/HToZZTo4Leptons/json/Cert_PRL_2011.txt' ] ) 


#FIXME: json!
#FIXME: add cross triggers
#FIXME: read torino files directly ? 

data_DoubleElectron_Run2011A_16Jan2012 = cfg.DataComponent(
    name = 'data_DoubleElectron_Run2011A_16Jan2012',
    files = [],
    intLumi = 4700-2511,
    triggers = [],
    json = json
    )

data_DoubleMu_Run2011A_16Jan2012 = cfg.DataComponent(
    name = 'data_DoubleMu_Run2011A_16Jan2012',
    files = [],
    intLumi = 4700-2511,
    triggers = [],
    json = json
    )

# see http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Torino/ZZAnalysis/AnalysisStep/test/Plotter/Luminosity.txt?revision=1.36&view=markup

data_DoubleElectron_Run2011B_16Jan2012 = cfg.DataComponent(
    name = 'data_DoubleElectron_Run2011B_16Jan2012',
    files = [],
    intLumi = 2511,
    triggers = [],
    json = json
    )

data_DoubleMu_Run2011B_16Jan2012 = cfg.DataComponent(
    name = 'data_DoubleMu_Run2011B_16Jan2012',
    files = [],
    intLumi = 2511,
    triggers = [],
    json = json
    )

data_DoubleMu_2011 = [data_DoubleMu_Run2011A_16Jan2012,
                      data_DoubleMu_Run2011B_16Jan2012]
data_DoubleElectron_2011 = [data_DoubleElectron_Run2011A_16Jan2012,
                            data_DoubleElectron_Run2011B_16Jan2012]
