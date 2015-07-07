import os
os.system("source /afs/cern.ch/cms/slc6_amd64_gcc491/external/py2-cx-oracle/5.1-cms/etc/profile.d/init.sh")

import cx_Oracle as db

import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps
from PhysicsTools.HeppyCore.framework.heppy import getHeppyOption

from PhysicsTools.Heppy.analyzers.objects.TauAnalyzer import TauAnalyzer

from CMGTools.H2TauTau.proto.analyzers.TauTreeProducer import TauTreeProducer
from CMGTools.H2TauTau.proto.analyzers.TauGenTreeProducer import TauGenTreeProducer

# from CMGTools.H2TauTau.proto.samples.phys14.connector import httConnector
from CMGTools.TTHAnalysis.samples.samples_13TeV_74X import DYJetsToLL_M50
from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import genAna, vertexAna


# Get all heppy options; set via "-o production" or "-o production=True"

# production = True run on batch, production = False (or unset) run locally
production = getHeppyOption('production')


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

# ###################################################
# ### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
# ###################################################
# my_connect = httConnector('htt_6mar15_manzoni_nom', 'CMS',
#                           '.*root', 'mt', production=production)
# my_connect.connect()
# MC_list = my_connect.MC_list


creator = ComponentCreator()
ggh160 = creator.makeMCComponent("GGH160", "/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM/TAUMU_743_TEST1", "htautau_group", ".*root", 1.0)


# from CMGTools.H2TauTau.proto.samples.spring15.connector import httConnector
# my_connect = httConnector('TAUMU_743_TEST1', 'htautau_group',
#                           '.*root', 'mt', production=production, 
#                           splitFactor=1e5)
# my_connect.connect()
# ggh160 = my_connect.MC_list[0]


###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = [ggh160] #DYJetsToLL_M50] # [ggh125]
sequence = cfg.Sequence([
    genAna,
    vertexAna,
    tauAna,
    treeProducer,
    genTreeProducer
])

if not production:
    cache = True
    comp = selectedComponents[0]
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    # comp.files = comp.files[:1]


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
