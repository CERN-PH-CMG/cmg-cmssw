from CMGTools.H2TauTau.proto.plotter.PlotConfigs import VariableCfg, SampleCfg, HistogramCfg

from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.categories_common import cat_VBF

from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass

example_cut = '&&'.join(cat_Inc, cat_VBF)


def createHistogram(hist_cfg):
    '''Method to create actual histogram from histogram config.
    '''
    for cfg in hist_cfg.cfgs:
        pass



cfg_vbf_tight = HistogramCfg(name='vbf', var='m_svfit', cut=example_cut)

# -> Command line
analysis_dir = '/afs/cern.ch/user/s/steggema/work/heppy/CMSSW_7_2_3/src/CMGTools/H2TauTau/cfgPython/mt/TEST/'

# -> Possibly from cfg like in the past, but may also make sense to enter directly
samples = [
    SampleCfg(name='ZTT', ana_dir=analysis_dir),
    SampleCfg(name='HiggsGGH125', ana_dir=analysis_dir),
    SampleCfg(name='TTJets', ana_dir=analysis_dir),
]

variables = [
    VariableCfg(name='m_svfit', binning=binning_svfitMass
]



# default_pars = {'cut':cut, 'anaDir':anaDir, 'weight':weight}

# samples = {
#     'HiggsGGH125':{'cut':cut}
# }

