from CMGTools.H2TauTau.proto.plotter.PlotConfigs import VariableCfg

from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass

all_vars = [    
    VariableCfg(name='mvis', binning=binning_svfitMass, unit='GeV', xtitle='m_{vis}'),
    VariableCfg(name='mt', binning={'nbinsx':20, 'xmin':0., 'xmax':200.}, unit='GeV', xtitle='m_{T}'),
    VariableCfg(name='mt_leg2', binning={'nbinsx':20, 'xmin':0., 'xmax':300.}, unit='GeV', xtitle='m_{T} #tau'),
    VariableCfg(name='n_vertices', binning={'nbinsx':51, 'xmin':-0.5, 'xmax':50.5}, unit=None, xtitle='N_{vertices}'),
    VariableCfg(name='n_jets', binning={'nbinsx':12, 'xmin':-0.5, 'xmax':11.5}, unit=None, xtitle='N_{jets}'),
    VariableCfg(name='n_jets_20', binning={'nbinsx':12, 'xmin':-0.5, 'xmax':11.5}, unit=None, xtitle='N_{jets} (20 GeV)'),
    VariableCfg(name='n_bjets', binning={'nbinsx':12, 'xmin':-0.5, 'xmax':11.5}, unit=None, xtitle='N_{b jets}'),
    VariableCfg(name='met_phi', binning={'nbinsx':12, 'xmin':-3.141593, 'xmax':3.141593}, unit=None, xtitle='E_{T}^{miss} #Phi'),
    VariableCfg(name='met_pt', binning={'nbinsx':12, 'xmin':0., 'xmax':200.}, unit='GeV', xtitle='E_{T}^{miss}'),
    VariableCfg(name='delta_eta_l1_l2', binning={'nbinsx':12, 'xmin':0, 'xmax':4.5}, unit=None, xtitle='#Delta#eta(#tau, #mu)'),
    VariableCfg(name='delta_r_l1_l2', binning={'nbinsx':12, 'xmin':0, 'xmax':4.5}, unit=None, xtitle='#Delta R(#tau, #mu)'),
    VariableCfg(name='vbf_mjj', binning={'nbinsx':40, 'xmin':0, 'xmax':1000.}, unit='GeV', xtitle='m_{jj}'),
    VariableCfg(name='vbf_deta', binning={'nbinsx':40, 'xmin':-7., 'xmax':7.}, unit=None, xtitle='#Delta#eta (VBF)'),
    VariableCfg(name='jet1_pt', binning={'nbinsx':40, 'xmin':0., 'xmax':400.}, unit='GeV', xtitle='jet 1 p_{T}'),
    VariableCfg(name='jet2_pt', binning={'nbinsx':40, 'xmin':0., 'xmax':400.}, unit='GeV', xtitle='jet 2 p_{T}'),
    VariableCfg(name='jet1_eta', binning={'nbinsx':40, 'xmin':-5., 'xmax':5.}, unit=None, xtitle='jet 1 #eta'),
    VariableCfg(name='jet2_eta', binning={'nbinsx':40, 'xmin':-5., 'xmax':5.}, unit=None, xtitle='jet 2 #eta'),
    VariableCfg(name='l1_pt', binning={'nbinsx':40, 'xmin':0., 'xmax':400.}, unit='GeV', xtitle='muon p_{T}'),
    VariableCfg(name='l2_pt', binning={'nbinsx':40, 'xmin':0., 'xmax':400.}, unit='GeV', xtitle='tau p_{T}'),
    VariableCfg(name='l1_eta', binning={'nbinsx':20, 'xmin':-2.5, 'xmax':2.5}, unit=None, xtitle='muon #eta'),
    VariableCfg(name='l2_eta', binning={'nbinsx':20, 'xmin':-2.5, 'xmax':2.5}, unit=None, xtitle='tau #eta'),
    VariableCfg(name='l2_decayMode', binning={'nbinsx':12, 'xmin':-0.5, 'xmax':11.5}, unit=None, xtitle='tau decay mode'),
    VariableCfg(name='l2_mass', binning={'nbinsx':15, 'xmin':0., 'xmax':3.}, unit='GeV', xtitle='tau mass'),
]

dict_all_vars = {}
for v in all_vars:
    dict_all_vars[v.name] = v

def getVars(names):
    return [dict_all_vars[n] for n in names]
    
