from CMGTools.H2TauTau.proto.plotter.PlotConfigs import VariableCfg, SampleCfg, HistogramCfg
<<<<<<< HEAD

from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.categories_common import cat_VBF

from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass

example_cut = '&&'.join(cat_Inc, cat_VBF)
=======
from CMGTools.H2TauTau.proto.plotter.DataMCPlot import DataMCPlot

from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.categories_common import cat_VBF
from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass

from ROOT import TH1F

example_cut = '&&'.join([cat_Inc, cat_VBF])

>>>>>>> dc73afd... Merge remote-tracking branch 'origin/CMGTools-from-CMSSW_7_4_3_AddH2Tau' into CMGTools-from-CMSSW_7_4_3__746Test


def createHistogram(hist_cfg):
    '''Method to create actual histogram from histogram config.
    '''
<<<<<<< HEAD
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

=======
    plot = DataMCPlot(hist_cfg.name)
    vcfg = hist_cfg.var
    for cfg in hist_cfg.cfgs:
        # First check whether it's a sub-histo or not
        if isinstance(cfg, HistogramCfg):
            hist = createHistogram(cfg)
        else:
            # It's a sample cfg
            hname = hist_cfg.name + ' ' + cfg.name
            if 'xmin' in vcfg.binning:
                hist = TH1F(hname, '', vcfg.binning['nbinsx'], 
                    vcfg.binning['xmin'], vcfg.binning['xmax'])
            else:
                hist = TH1F(hname, '', len(vcfg.binning)-1, vcfg.binning)

            file_name = '/'.join([cfg.ana_dir, cfg.dir_name, cfg.tree_prod_name, 'tree.root'])

            ttree = plot.readTree(file_name, cfg.tree_name)
            print 'read tree', ttree, 'from file', file_name

            cut = hist_cfg.cut

            ttree.Project(hname, vcfg.name, cut)

            import pdb; pdb.set_trace()

            if cfg.norm_cut:
                pass
            if cfg.shape_cut:
                pass

            plot.histos.append(hist)



# -> Command line
analysis_dir = '/afs/cern.ch/user/s/steggema/work/heppy/CMSSW_7_2_3/src/CMGTools/H2TauTau/cfgPython/mt/BTEST/'
tree_prod_name = 'H2TauTauTreeProducerTauMu'

# -> Possibly from cfg like in the past, but may also make sense to enter directly
samples = [
    SampleCfg(name='HiggsTTHInclusive125', ana_dir=analysis_dir, tree_prod_name=tree_prod_name),
    # SampleCfg(name='HiggsGGH125', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, is_signal=True),
    # SampleCfg(name='TTJets', ana_dir=analysis_dir, tree_prod_name=tree_prod_name),
]

# -> Can add cross sections for samples either explicitly, or from file, or from cfg

variables = [
    VariableCfg(name='m_svfit', binning=binning_svfitMass, unit='GeV', xtitle='m_{SVFit}')
]

cfg_vbf_tight = HistogramCfg(name='vbf', var=None, cfgs=samples, cut=example_cut)

for variable in variables:
    cfg_vbf_tight.var = variable
    createHistogram(cfg_vbf_tight)
>>>>>>> dc73afd... Merge remote-tracking branch 'origin/CMGTools-from-CMSSW_7_4_3_AddH2Tau' into CMGTools-from-CMSSW_7_4_3__746Test


# default_pars = {'cut':cut, 'anaDir':anaDir, 'weight':weight}

# samples = {
#     'HiggsGGH125':{'cut':cut}
# }

