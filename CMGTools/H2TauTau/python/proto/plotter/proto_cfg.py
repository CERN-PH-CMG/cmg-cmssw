from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.categories_common import cat_VBF

example_cut = '&&'.join(cat_Inc, cat_VBF)

class VariableCfg(object):
    '''Configuration object for a variable.

    "binning" is a dict with either nbinsx, xmin, xmax for equidistant binning
    or nbinsx, bins=array([...]).
    '''
    def __init__(self, name='m_svfit', binning=None, xtitle=None, unit=None):
        self.name = name
        self.binning = {'nbinsx':10, 'xmin':0., 'xmax':200.} if binning is None else binning
        self.unit = unit
        self.xtitle = name if xtitle is None else xtitle


class SampleCfg(object):
    '''Configuration object for a sample contribution within a histogram.
    If specific properties are empty, overall defaults will be assumed.
    '''
    def __init__(self, name='Default', ana_dir='', scale=1., 
        weight_expr=None, norm_cut=None, shape_cut=None):
        self.name = name
        self.ana_dir = ana_dir
        self.scale = scale # generic scale, e.g. scale signal by factor 5
        # a sample-specific extra weight as saved in a tree
        self.weight_expr = weight_expr 
        self.norm_cut = norm_cut
        self.shape_cut = shape_cut

        self.is_signal = False # Will only be used to build stacks
        self.is_data = False # Will not be used to build stacks


class HistogramCfg(object):
    '''Configuration object for a histogram made up of several sub-contributions.

    A histogram can in turn have a sub-contribution from another histogram cfg. 
    '''
    def __init__(self, name='Default', var=None, cfgs=None, cut='', 
                 weight='weight', shape_cfgs=None, use_signal_for_stack=False):
        self.name = name # e.g. 'vbf tight'
        self.var = VariableCfg() if var is None else var # e.g. 'm_svfit'
        self.cfgs = [] if cfgs is None else cfgs # List of sample and/or histogram cfgs
        self.shape_cfgs = shape_cfgs # If the shape is constructed in a different way
        self.cut = cut
        self.weight = weight
        self.use_signal_for_stack = use_signal_for_stack


def createHistogram(hist_cfg):
    '''Method to create actual histogram from histogram config.
    '''
    for cfg in hist_cfg.cfgs:
        pass





cfg_vbf_tight = HistogramCfg(name='vbf', var='m_svfit', cut=example_cut)






# default_pars = {'cut':cut, 'anaDir':anaDir, 'weight':weight}

# samples = {
#     'HiggsGGH125':{'cut':cut}
# }

