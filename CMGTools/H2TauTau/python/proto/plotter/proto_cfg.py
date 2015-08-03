from CMGTools.H2TauTau.proto.plotter.PlotConfigs import VariableCfg, SampleCfg, HistogramCfg

from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass

from CMGTools.H2TauTau.proto.plotter.HistCreator import createHistogram, setSumWeights

from CMGTools.H2TauTau.proto.plotter.HistDrawer import HistDrawer

example_cut = '&&'.join([cat_Inc])# , cat_VBF])


# -> Command line
analysis_dir = '/home/user/ytakahas/ForMarcel'
tree_prod_name = 'H2TauTauTreeProducerTauMu'


from CMGTools.RootTools.samples.samples_13TeV_74X import WJetsToLNu # TT_pow, DYJetsToLL_M50, 


# -> Possibly from cfg like in the past, but may also make sense to enter directly
samples = [
    SampleCfg(name='WJetsToLNu', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WJetsToLNu.xSection, sumweights=WJetsToLNu.nGenEvents),
    SampleCfg(name='QCD', dir_name='QCDflat', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=2022100000.),
    SampleCfg(name='Data', dir_name='first2pb', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, is_data=True),
]
# -> Can add cross sections for samples either explicitly, or from file, or from cfg

for sample in samples:
    setSumWeights(sample)

# -> Should put list of standard variables into extra file
variables = [
    # VariableCfg(name='mvis', binning=binning_svfitMass, unit='GeV', xtitle='m_{vis}'),
    VariableCfg(name='mt', binning={'nbinsx':10, 'xmin':0., 'xmax':200.}, unit='GeV', xtitle='m_{T}'),
]

cfg_example = HistogramCfg(name='example', var=None, cfgs=samples, cut=example_cut, lumi=2.0)

for variable in variables:
    cfg_example.var = variable
    plot = createHistogram(cfg_example)
    HistDrawer.draw(plot)
    # HistDrawer.drawRatio(plot)

