import PhysicsTools.HeppyCore.framework.config as cfg
from DMPD.Heppy.samples.Data.fileLists import datasamples

sampleSingleMuon_Run2015B_17Jul2015_v1 = cfg.Component(
        files      = datasamples["SingleMuon_Run2015B_17Jul2015_v1"]["files"],
        name       = "SingleMuon_Run2015B_17Jul2015_v1",
        splitFactor= 20,
)

sampleSingleMuon_Run2015B_PromptReco_v1 = cfg.Component(
        files      = datasamples["SingleMuon_Run2015B_PromptReco_v1"]["files"],
        name       = "SingleMuon_Run2015B_PromptReco_v1",
        splitFactor= 20,
)

sampleSingleElectron_Run2015B_17Jul2015_v1 = cfg.Component(
        files      = datasamples["SingleElectron_Run2015B_17Jul2015_v1"]["files"],
        name       = "SingleElectron_Run2015B_17Jul2015_v1",
        splitFactor= 20,
)

sampleSingleElectron_Run2015B_PromptReco_v1 = cfg.Component(
        files      = datasamples["SingleElectron_Run2015B_PromptReco_v1"]["files"],
        name       = "SingleElectron_Run2015B_PromptReco_v1",
        splitFactor= 20,
)

sampleDoubleMuon_Run2015B_17Jul2015_v1 = cfg.Component(
        files      = datasamples["DoubleMuon_Run2015B_17Jul2015_v1"]["files"],
        name       = "DoubleMuon_Run2015B_17Jul2015_v1",
        splitFactor= 20,
)

sampleDoubleMuon_Run2015B_PromptReco_v1 = cfg.Component(
        files      = datasamples["DoubleMuon_Run2015B_PromptReco_v1"]["files"],
        name       = "DoubleMuon_Run2015B_PromptReco_v1",
        splitFactor= 20,
)

sampleDoubleEG_Run2015B_17Jul2015_v1 = cfg.Component(
        files      = datasamples["DoubleEG_Run2015B_17Jul2015_v1"]["files"],
        name       = "DoubleEG_Run2015B_17Jul2015_v1",
        splitFactor= 20,
)

sampleDoubleEG_Run2015B_PromptReco_v1 = cfg.Component(
        files      = datasamples["DoubleEG_Run2015B_PromptReco_v1"]["files"],
        name       = "DoubleEG_Run2015B_PromptReco_v1",
        splitFactor= 20,
)

sampleMET_Run2015B_17Jul2015_v1 = cfg.Component(
        files      = datasamples["MET_Run2015B_17Jul2015_v1"]["files"],
        name       = "MET_Run2015B_17Jul2015_v1",
        splitFactor= 20,
)

sampleMET_Run2015B_PromptReco_v1 = cfg.Component(
        files      = datasamples["MET_Run2015B_PromptReco_v1"]["files"],
        name       = "MET_Run2015B_PromptReco_v1",
        splitFactor= 20,
)
