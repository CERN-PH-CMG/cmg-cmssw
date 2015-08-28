import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.TauEleAnalyzer import TauEleAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauEle import H2TauTauTreeProducerTauEle
from CMGTools.H2TauTau.proto.analyzers.DYLLReweighterTauEle import DYLLReweighterTauEle
from CMGTools.H2TauTau.proto.analyzers.TauDecayModeWeighter import TauDecayModeWeighter
from CMGTools.H2TauTau.proto.analyzers.TauFakeRateWeighter import TauFakeRateWeighter
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.SVfitProducer import SVfitProducer

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import commonSequence, genAna, dyJetsFakeAna, puFileData, puFileMC, eventSelector

# e-tau specific configuration settings

# 'Nom', 'Up', 'Down', or None
shift = None
syncntuple = False
computeSVfit = False
production = True  # production = True run on batch, production = False run locally

#production = True  # production = True run on batch, production = False run locally

# When ready, include weights from CMGTools.H2TauTau.proto.weights.weighttable

# hlt_tauEffWeight_mc = 'effTau_eTau_MC_2012ABCDSummer13'
# hlt_tauEffWeight = 'effTau_eTau_Data_2012ABCDSummer13'
# hlt_eleEffWeight_mc = 'effEle_eTau_MC_2012ABCD'
# hlt_eleEffWeight = 'effEle_eTau_Data_2012ABCDSummer13'

hlt_tauEffWeight_mc = None
hlt_tauEffWeight = None
hlt_eleEffWeight_mc = None
hlt_eleEffWeight = None

dyJetsFakeAna.channel = 'et'

# Define e-tau specific modules

tauEleAna = cfg.Analyzer(
    TauEleAnalyzer,
    name='TauEleAnalyzer',
    pt1=23,
    eta1=2.1,
    iso1=0.1,
    looseiso1=9999.,
    pt2=20,
    eta2=2.3,
    iso2=1.5,
    looseiso2=9999.,
    m_min=10,
    m_max=99999,
    dR_min=0.5,
    from_single_objects=True,
    verbose=False
)

dyLLReweighterTauEle = cfg.Analyzer(
    DYLLReweighterTauEle,
    name='DYLLReweighterTauEle',
    # 2012
    W1p0PB=1.,  # 1.37, # weight for 1 prong 0 Pi Barrel
    W1p0PE=1.,  # 1.11,
    W1p1PB=1.,  # 2.18,
    W1p1PE=1.,  # 0.47,
    verbose=False
)

tauDecayModeWeighter = cfg.Analyzer(
    TauDecayModeWeighter,
    name='TauDecayModeWeighter',
    legs=['leg2']
)

tauFakeRateWeighter = cfg.Analyzer(
    TauFakeRateWeighter,
    name='TauFakeRateWeighter'
)

tauWeighter = cfg.Analyzer(
    LeptonWeighter,
    name='LeptonWeighter_tau',
    effWeight=None,
    effWeightMC=None,
    lepton='leg2',
    verbose=False,
    disable=True,
)

eleWeighter = cfg.Analyzer(
    LeptonWeighter,
    name='LeptonWeighter_ele',
    effWeight=None,
    effWeightMC=None,
    lepton='leg1',
    verbose=False,
    disable=True,
    idWeight=None,
    isoWeight=None
)

treeProducer = cfg.Analyzer(
    H2TauTauTreeProducerTauEle,
    name='H2TauTauTreeProducerTauEle'
)

syncTreeProducer = cfg.Analyzer(
    H2TauTauTreeProducerTauEle,
    name='H2TauTauSyncTreeProducerTauEle',
    varStyle='sync',
    # skimFunction='event.isSignal'
)


svfitProducer = cfg.Analyzer(
    SVfitProducer,
    name='SVfitProducer',
    # integration='VEGAS',
    integration='MarkovChain',
    # verbose=True,
    # order='21', # muon first, tau second
    l1type='ele',
    l2type='tau'
)

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
# from CMGTools.H2TauTau.proto.samples.phys14.connector import httConnector
# my_connect = httConnector('htt_6mar15_manzoni_nom', 'htautau_group',
#                           '.*root', 'et', production=production)
# my_connect.connect()
# MC_list = my_connect.MC_list

from CMGTools.RootTools.samples.samples_13TeV_74X import TT_pow, DYJetsToLL_M50, WJetsToLNu, WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf, QCDPtEMEnriched, WWTo2L2Nu, ZZp8, WZp8, SingleTop, QCDPtbcToE
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import SingleElectron_Run2015B_17Jul, SingleElectron_Run2015B
from CMGTools.H2TauTau.proto.samples.spring15.triggers_tauEle import mc_triggers as mc_triggers_et
from CMGTools.H2TauTau.proto.samples.spring15.triggers_tauEle import data_triggers as data_triggers_et

# from CMGTools.H2TauTau.proto.samples.spring15.higgs import HiggsGGH125, HiggsVBF125, HiggsTTH125

from CMGTools.RootTools.utils.splitFactor import splitFactor

# Get all heppy options; set via "-o production" or "-o production=True"




samples = [TT_pow, DYJetsToLL_M50, WJetsToLNu, WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf]

samples = [TT_pow, DYJetsToLL_M50, WJetsToLNu, WWTo2L2Nu, ZZp8, WZp8]

# samples += [HiggsGGH125, HiggsVBF125, HiggsTTH125]
samples += SingleTop  + QCDPtEMEnriched + QCDPtbcToE

split_factor = 1e5

for sample in samples:
    sample.triggers = mc_triggers_et
    sample.splitFactor = splitFactor(sample, split_factor)

data_list = [SingleElectron_Run2015B_17Jul, SingleElectron_Run2015B]

for sample in data_list:
    sample.triggers = data_triggers_et
    sample.splitFactor = splitFactor(sample, split_factor)
    sample.json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt'
    sample.lumi = 40.03


for sample in samples:
    sample.triggers = mc_triggers_et
    sample.splitFactor = splitFactor(sample, split_factor)



###################################################
###              ASSIGN PU to MC                ###
###################################################
for mc in samples:
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = samples
selectedComponents = [DYJetsToLL_M50]
selectedComponents = data_list
# selectedComponents = mc_dict['HiggsGGH125']
# for c in selectedComponents : c.splitFactor *= 5

###################################################
###                  SEQUENCE                   ###
###################################################
sequence = commonSequence
sequence.insert(sequence.index(genAna), tauEleAna)
sequence.append(tauDecayModeWeighter)
sequence.append(tauFakeRateWeighter)
sequence.append(tauWeighter)
sequence.append(eleWeighter)
sequence.insert(sequence.index(dyJetsFakeAna) + 1, dyLLReweighterTauEle)
if computeSVfit:
    sequence.append(svfitProducer)
sequence.append(treeProducer)
if syncntuple:
    sequence.append(syncTreeProducer)

###################################################
###             CHERRY PICK EVENTS              ###
###################################################
# eventSelector.toSelect = [186583]
# sequence.insert(0, eventSelector)

###################################################
###            SET BATCH OR LOCAL               ###
###################################################
if not production:
    cache = True
    # comp = my_connect.mc_dict['HiggsGGH125']
    # comp = ggh160
    comp = selectedComponents[0]
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
#    comp.files = comp.files[:1]


# the following is declared in case this cfg is used in input to the
# heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config(components=selectedComponents,
                    sequence=sequence,
                    services=[],
                    events_class=Events
                    )

printComps(config.components, True)


def modCfgForPlot(config):
    config.components = []
