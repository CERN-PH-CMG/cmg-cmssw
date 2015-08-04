from CMGTools.H2TauTau.proto.plotter.PlotConfigs import SampleCfg, HistogramCfg

from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc

from CMGTools.H2TauTau.proto.plotter.HistCreator import createHistogram, setSumWeights

from CMGTools.H2TauTau.proto.plotter.HistDrawer import HistDrawer

from CMGTools.H2TauTau.proto.plotter.Variables import all_vars

example_cut = '&&'.join([cat_Inc])# , cat_VBF])

# -> Command line
analysis_dir = '/afs/cern.ch/user/s/steggema/work/746_pre6/CMSSW_7_4_6_patch6/src/CMGTools/H2TauTau/cfgPython/mt/Prod2015B'
tree_prod_name = 'H2TauTauTreeProducerTauMu'


from CMGTools.RootTools.samples.samples_13TeV_74X import TT_pow, DYJetsToLL_M50, WJetsToLNu, QCD_Mu15, WWTo2L2Nu, ZZp8, WZp8

# -> Possibly from cfg like in the past, but may also make sense to enter directly
samples = [
    SampleCfg(name='Ztt', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (geninfo_TT)'),
    SampleCfg(name='Ztt_ZL', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (geninfo_LL && geninfo_fakeid == 1)'),
    SampleCfg(name='Ztt_ZJ', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (!geninfo_TT && !(geninfo_LL && geninfo_fakeid == 1))'),
    SampleCfg(name='WJetsToLNu', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WJetsToLNu.xSection, sumweights=WJetsToLNu.nGenEvents),
    SampleCfg(name='TTJets', dir_name='TTLep_pow', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=TT_pow.xSection, sumweights=TT_pow.nGenEvents),
    SampleCfg(name='ZZ', dir_name='ZZp8', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=ZZp8.xSection, sumweights=ZZp8.nGenEvents),
    SampleCfg(name='WZ', dir_name='WZp8', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WZp8.xSection, sumweights=WZp8.nGenEvents),
    SampleCfg(name='WW', dir_name='WWTo2L2Nu', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WWTo2L2Nu.xSection, sumweights=WWTo2L2Nu.nGenEvents),
    SampleCfg(name='QCD', dir_name='QCD_Mu15', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=QCD_Mu15.xSection),
    SampleCfg(name='Data', dir_name='SingleMuon_Run2015B', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, is_data=True),
]
# -> Can add cross sections for samples either explicitly, or from file, or from cfg

for sample in samples:
    setSumWeights(sample)

# Taken from Variables.py, can get subset with e.g. getVars(['mt', 'mvis'])
variables = all_vars

cfg_example = HistogramCfg(name='example', var=None, cfgs=samples, cut=example_cut, lumi=40.0)

for variable in variables:
    cfg_example.var = variable
    plot = createHistogram(cfg_example)
    plot.Group('Diboson', ['ZZ', 'WZ', 'WW'])
    HistDrawer.draw(plot)
    # HistDrawer.drawRatio(plot)

