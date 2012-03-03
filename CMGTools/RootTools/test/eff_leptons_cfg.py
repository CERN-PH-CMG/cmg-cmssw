import copy
import glob
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.Production.dataset import createDataset

def getFiles(dataset, user, pattern):
    from CMGTools.Production.datasetToSource import datasetToSource
    print 'getting files for', dataset,user,pattern
    ds = datasetToSource( user, dataset, pattern, True )
    files = ds.fileNames
    return ['root://eoscms//eos/cms%s' % f for f in files]


period = 'Period_2011A'

baseDir = '2011'
filePattern = 'tree*.root'

def idMuon(muon):
    return muon.getSelection('cuts_vbtfmuon')

def isoLepton(lepton):
    return lepton.relIso(0.5)<0.1
    
def passLepton(lepton):
    return True

effMuAna = cfg.Analyzer(
    'EfficiencyAnalyzer',
    # recsel = 'cuts_vbtfmuon',
    recselFun = idMuon,
    # refselFun = idMuon,
    instance = 'cmgMuonSelStdLep',
    type = 'std::vector<cmg::Muon>',
    genPdgId = 13
    )


def idElectron(electron):
    return electron.getSelection('cuts_vbtf80ID')


effEleAna = cfg.Analyzer(
    'EfficiencyAnalyzer',
    # recsel = 'cuts_vbtfmuon',
    recselFun = isoLepton,
    refselFun = idElectron,
    instance = 'cmgElectronSel',
    type = 'std::vector<cmg::Electron>',
    genPdgId = 11
    )





#########################################################################################


DYJetsFall11 = cfg.MCComponent(
    name = 'DYJetsFall11',
    files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/TestMVAs', 'cmgtools_group','tree.*root')[:10],
    # files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2/PAT_CMG_V2_5_0', 'cmgtools', 'tree.*root')[:10],
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = [],
    effCorrFactor = 1 )


DYJetsChamonix = cfg.MCComponent(
    name = 'DYJetsChamonix',
    files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/TestMVAs', 'cmgtools', 'tree.*root')[:10],
    # files = createDataset('LOCAL','/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0', '.*root', True).listOfGoodFiles(),
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = [],
    effCorrFactor = 1 )


QCDMu = cfg.MCComponent(
    name = 'QCDMu',
    files = getFiles('/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0', 'cmgtools', 'tree.*root'),
    xSection = 3048., # dummy 
    nGenEvents = 34915945, # dummy 
    triggers = [],
    effCorrFactor = 1 )



#########################################################################################



selectedComponents  = [DYJetsChamonix] 

DYJetsChamonix.splitFactor = 5
DYJetsFall11.splitFactor = 5
QCDMu.splitFactor = 5
QCDMu.files = QCDMu.files[:5]

test = False
if test:
    sam = DYJetsChamonix
    selectedComponents = [sam]
    sam.splitFactor = 1


sequence = cfg.Sequence( [
    effMuAna,
    # effEleAna
    ] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
