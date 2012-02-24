import copy
import CMGTools.RootTools.fwlite.Config as cfg


effMuAna = cfg.Analyzer(
    'EfficiencyAnalyzer',
    # recsel = 'cuts_vbtfmuon'
    instance = 'cmgMuonSel',
    type = 'std::vector<cmg::Muon>',
    genPdgId = 13
    )

## files = [
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_967.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_968.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_969.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_97.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_971.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_972.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_973.root',
##     '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_974.root',
##     ]

## files = ['root://eoscms//eos/cms%s' % f for f in files]


def getFiles(datasets, user, pattern):
    from CMGTools.Production.datasetToSource import datasetToSource
    files = []
    for d in datasets:
        ds = datasetToSource( user,
                              d,
                              pattern
                              )
        files.extend(ds.fileNames)
    return ['root://eoscms//eos/cms%s' % f for f in files]

files = getFiles(['/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0'], 'cmgtools','tree.*root')

print files

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = files,
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = [],
    effCorrFactor = 1 )

selectedComponents =  [DYJets]

sequence = cfg.Sequence( [
    effMuAna
    ] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

DYJets.splitFactor = 100
