import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

from PhysicsTools.Heppy.analyzers.objects.TauAnalyzer import TauAnalyzer

from CMGTools.H2TauTau.proto.analyzers.TauTreeProducer import TauTreeProducer
from CMGTools.H2TauTau.proto.analyzers.TauGenTreeProducer import TauGenTreeProducer
from CMGTools.H2TauTau.proto.analyzers.TauIsolationCalculator import TauIsolationCalculator
from CMGTools.H2TauTau.proto.analyzers.FileCleaner import FileCleaner
from CMGTools.H2TauTau.proto.analyzers.JetAnalyzer import JetAnalyzer

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
from CMGTools.RootTools.utils.splitFactor import splitFactor

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import genAna, vertexAna


# Get all heppy options; set via "-o production" or "-o production=True"

# production = True run on batch, production = False (or unset) run locally
# production = getHeppyOption('production')
production = True


treeProducer = cfg.Analyzer(
    TauTreeProducer,
    name='TauTreeProducer'
)

genTreeProducer = cfg.Analyzer(
    TauGenTreeProducer,
    name='TauGenTreeProducer'
)

tauAna = cfg.Analyzer(
    TauAnalyzer,
    name='TauAnalyzer',
    loose_ptMin=20,
    loose_etaMax=9999,
    loose_dxyMax=1000.,
    loose_dzMax=0.2,
    loose_vetoLeptons=False,
    loose_leptonVetoDR=0.4,
    loose_decayModeID="decayModeFindingNewDMs",  # ignored if not set or ""
    loose_tauID="decayModeFindingNewDMs",
    loose_vetoLeptonsPOG=False, # If True, the following two IDs are required
    loose_tauAntiMuonID="againstMuonLoose3",
    loose_tauAntiElectronID="againstElectronLooseMVA5",
    loose_tauLooseID="decayModeFinding",
    inclusive_ptMin = 18,
    inclusive_etaMax = 9999,
    inclusive_dxyMax = 1000.,
    inclusive_dzMax = 0.4,
    inclusive_vetoLeptons = False,
    inclusive_leptonVetoDR = 0.4,
    inclusive_decayModeID = "decayModeFindingNewDMs", # ignored if not set or ""
    inclusive_tauID = "decayModeFindingNewDMs",
    inclusive_vetoLeptonsPOG = False, # If True, the following two IDs are required
    inclusive_tauAntiMuonID = "",
    inclusive_tauAntiElectronID = "",
)

tauIsoCalc = cfg.Analyzer(
    TauIsolationCalculator,
    name='TauIsolationCalculator'
)

fileCleaner = cfg.Analyzer(
    FileCleaner,
    name='FileCleaner'
)

jetAna = cfg.Analyzer(
    JetAnalyzer,
    name='JetAnalyzer',
    jetCol='slimmedJets',  # <- These are CHS jets
    # jetCol = 'patJetsAK4PF', # <- These are plain PF jets
    jetPt=20.,
    jetEta=4.7,
    relaxJetId=False,
    relaxPuJetId=True,
    jerCorr=False,
    #jesCorr = 1.,
    puJetIDDisc='pileupJetId:fullDiscriminant',
)

# ###################################################
# ### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
# ###################################################
# my_connect = httConnector('htt_6mar15_manzoni_nom', 'CMS',
#                           '.*root', 'mt', production=production)
# my_connect.connect()
# MC_list = my_connect.MC_list

from CMGTools.RootTools.samples.samples_13TeV_74X import TT_pow, DYJetsToLL_M50, WJetsToLNu, WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf

creator = ComponentCreator()
ggh160 = creator.makeMCComponent("GGH160", "/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0)


qcd_flat = creator.makeMCComponent("QCDflat", "/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/RunIISpring15DR74-Asympt25nsRaw_MCRUN2_74_V9-v3/MINIAODSIM", "CMS", ".*root", 1.0)


samples = [qcd_flat, TT_pow, DYJetsToLL_M50, WJetsToLNu, WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf]

# from CMGTools.H2TauTau.proto.samples.spring15.connector import httConnector
# my_connect = httConnector('TAUMU_743_TEST1', 'htautau_group',
#                           '.*root', 'mt', production=production, 
#                           splitFactor=1e5)
# my_connect.connect()
# ggh160 = my_connect.MC_list[0]

split_factor = 2e4
for sample in samples:
    sample.splitFactor = splitFactor(sample, split_factor)

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = [DYJetsToLL_M50] #DYJetsToLL_M50] # [ggh125]
sequence = cfg.Sequence([
    genAna,
    vertexAna,
    tauAna,
    tauIsoCalc,
    jetAna,
    treeProducer,
    genTreeProducer,
    fileCleaner
])

selectedComponents = samples

if not production:
    cache = True
    comp = selectedComponents[0]
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    # comp.files = comp.files[:1]


from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor("$CMSSW_BASE/src/CMGTools/H2TauTau/prod/puppiOnMiniAOD_cfg.py", addOrigAsSecondary=False)


# the following is declared in case this cfg is used in input to the
# heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config(components=selectedComponents,
                    sequence=sequence,
                    services=[],
                    preprocessor=preprocessor,
                    events_class=Events,
                    )

printComps(config.components, True)


def modCfgForPlot(config):
    config.components = []
