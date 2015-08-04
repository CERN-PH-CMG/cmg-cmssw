import pickle

from CMGTools.H2TauTau.proto.plotter.PlotConfigs import HistogramCfg
from CMGTools.H2TauTau.proto.plotter.DataMCPlot import DataMCPlot

from ROOT import TH1F

def initHist(hist, vcfg):
    hist.Sumw2()
    xtitle = vcfg.xtitle
    if vcfg.unit:
        xtitle += ' ({})'.format(vcfg.unit)
    hist.GetXaxis().SetTitle(xtitle)
    hist.SetStats(False)

def createHistogram(hist_cfg):
    '''Method to create actual histogram (DataMCPlot) instance from histogram 
    config.
    '''
    plot = DataMCPlot(hist_cfg.var.name)
    plot.lumi = hist_cfg.lumi
    vcfg = hist_cfg.var
    for cfg in hist_cfg.cfgs:
        # First check whether it's a sub-histo or not
        if isinstance(cfg, HistogramCfg):
            hist = createHistogram(cfg)
        else:
            # It's a sample cfg
            hname = hist_cfg.name + ' ' + cfg.name + ' ' + vcfg.name
            if 'xmin' in vcfg.binning:
                hist = TH1F(hname, '', vcfg.binning['nbinsx'], 
                    vcfg.binning['xmin'], vcfg.binning['xmax'])
            else:
                hist = TH1F(hname, '', len(vcfg.binning)-1, vcfg.binning)

            initHist(hist, vcfg)

            file_name = '/'.join([cfg.ana_dir, cfg.dir_name, cfg.tree_prod_name, 'tree.root'])

            ttree = plot.readTree(file_name, cfg.tree_name)
            print 'read tree', ttree, 'from file', file_name

            norm_cut = hist_cfg.cut
            shape_cut = hist_cfg.cut

            if cfg.norm_cut:
                norm_cut = cfg.norm_cut

            if cfg.shape_cut:
                shape_cut = cfg.shape_cut

            weight = cfg.weight_expr if cfg.weight_expr else hist_cfg.weight

            if hist_cfg.weight:
                norm_cut = '({c}) * {we}'.format(c=norm_cut, we=weight)
                shape_cut = '({c}) * {we}'.format(c=shape_cut, we=weight)


            ttree.Project(hname, vcfg.name, norm_cut)

            if shape_cut != norm_cut:
                scale = hist.Integral()
                ttree.Project(hname, vcfg.name, shape_cut)
                hist.Scale(scale/hist.Integral())

            stack = not cfg.is_data and not cfg.is_signal

            plot_hist = plot.AddHistogram(cfg.name, hist, stack=stack)

            if not cfg.is_data:
                plot_hist.SetWeight(hist_cfg.lumi*cfg.xsec/cfg.sumweights)

    plot._ApplyPrefs()
    return plot

def setSumWeights(sample):
    if sample.is_data:
        return

    pckfile = '/'.join([sample.ana_dir, sample.dir_name, 'SkimAnalyzerCount', 'SkimReport.pck'])
    pckobj  = pickle.load(open(pckfile,'r'))
    counters = dict(pckobj)
    if 'Sum Weights' in counters:
        sample.sumweights = counters['Sum Weights']
    