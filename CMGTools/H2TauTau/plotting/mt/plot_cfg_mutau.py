import copy

from CMGTools.H2TauTau.proto.plotter.PlotConfigs import SampleCfg, HistogramCfg
from CMGTools.H2TauTau.proto.plotter.categories_TauMu import cat_Inc
from CMGTools.H2TauTau.proto.plotter.HistCreator import createHistogram, setSumWeights
from CMGTools.H2TauTau.proto.plotter.HistDrawer import HistDrawer
from CMGTools.H2TauTau.proto.plotter.Variables import all_vars


cuts = {}

inc_cut = '&&'.join([cat_Inc])
inc_cut += '&& l2_decayModeFinding'

cuts['OSlowMT'] = inc_cut + '&& l1_charge != l2_charge && mt<40'
cuts['SSlowMT'] = inc_cut + '&& l1_charge == l2_charge && mt<40'

cuts['OShighMT'] = inc_cut + '&& l1_charge != l2_charge && mt>40'
cuts['SShighMT'] = inc_cut + '&& l1_charge == l2_charge && mt>40'


qcd_from_same_sign = False

# -> Command line
analysis_dir = '/afs/cern.ch/user/s/steggema/work/746_pre6/CMSSW_7_4_6_patch6/src/CMGTools/H2TauTau/cfgPython/mt/Prod2015B'
singlet_dir = '/afs/cern.ch/user/s/steggema/work/746_pre6/CMSSW_7_4_6_patch6/src/CMGTools/H2TauTau/cfgPython/mt/ExtraProd2015'
tree_prod_name = 'H2TauTauTreeProducerTauMu'
data_dir = '/afs/cern.ch/user/s/steggema/work/746_pre6/CMSSW_7_4_6_patch6/src/CMGTools/H2TauTau/cfgPython/mt/ExtraDataProd2015'


from CMGTools.RootTools.samples.samples_13TeV_74X import TT_pow, DYJetsToLL_M50, WJetsToLNu, QCD_Mu15, WWTo2L2Nu, ZZp8, WZp8, T_tWch, TBar_tWch, TToLeptons_tch, TToLeptons_sch

# -> Possibly from cfg like in the past, but may also make sense to enter directly
samples = [
    SampleCfg(name='Ztt', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (geninfo_TT)'),
    SampleCfg(name='Ztt_ZL', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (geninfo_LL && geninfo_fakeid == 1)'),
    SampleCfg(name='Ztt_ZJ', dir_name='DYJetsToLL_M50', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=DYJetsToLL_M50.xSection, sumweights=DYJetsToLL_M50.nGenEvents, weight_expr='weight * (!geninfo_TT && !(geninfo_LL && geninfo_fakeid == 1))'),
    SampleCfg(name='WJetsToLNu', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WJetsToLNu.xSection, sumweights=WJetsToLNu.nGenEvents),
    SampleCfg(name='TTJets', dir_name='TTLep_pow', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=TT_pow.xSection, sumweights=TT_pow.nGenEvents),
    SampleCfg(name='T_tWch', dir_name='T_tWch', ana_dir=singlet_dir, tree_prod_name=tree_prod_name, xsec=T_tWch.xSection, sumweights=T_tWch.nGenEvents),
    SampleCfg(name='TBar_tWch', dir_name='TBar_tWch', ana_dir=singlet_dir, tree_prod_name=tree_prod_name, xsec=TBar_tWch.xSection, sumweights=TBar_tWch.nGenEvents),
    SampleCfg(name='TToLeptons_tch', dir_name='TToLeptons_tch', ana_dir=singlet_dir, tree_prod_name=tree_prod_name, xsec=TToLeptons_tch.xSection, sumweights=TBar_tWch.nGenEvents),
    SampleCfg(name='TToLeptons_sch', dir_name='TToLeptons_sch', ana_dir=singlet_dir, tree_prod_name=tree_prod_name, xsec=TToLeptons_sch.xSection, sumweights=TToLeptons_sch.nGenEvents),
    SampleCfg(name='ZZ', dir_name='ZZp8', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=ZZp8.xSection, sumweights=ZZp8.nGenEvents),
    SampleCfg(name='WZ', dir_name='WZp8', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WZp8.xSection, sumweights=WZp8.nGenEvents),
    SampleCfg(name='WW', dir_name='WWTo2L2Nu', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=WWTo2L2Nu.xSection, sumweights=WWTo2L2Nu.nGenEvents),
    SampleCfg(name='QCD', dir_name='QCD_Mu15', ana_dir=analysis_dir, tree_prod_name=tree_prod_name, xsec=QCD_Mu15.xSection),
    SampleCfg(name='Data', dir_name='SingleMuon_Run2015B', ana_dir=data_dir, tree_prod_name=tree_prod_name, is_data=True),
]

# -> Can add cross sections for samples either explicitly, or from file, or from cfg
for sample in samples:
    setSumWeights(sample)

if qcd_from_same_sign:
    samples_ss = copy.deepcopy(samples)

    samples = [s for s in samples if sample.name != 'QCD']

    for sample in samples_ss:
        if sample.name != 'Data':
            # Subtract background from data
            sample.scale = -1.

    qcd = HistogramCfg(name='QCD', var=None, cfgs=samples_ss, cut=inc_cut, lumi=40.0)

    samples.append(qcd)

# Taken from Variables.py, can get subset with e.g. getVars(['mt', 'mvis'])
variables = all_vars

cfg_example = HistogramCfg(name='example', var=None, cfgs=samples, cut=inc_cut, lumi=40.0)


for cut_name in cuts:
    cfg_example.cut = cuts[cut_name]
    if qcd_from_same_sign and 'OS' in cut_name:
        qcd.cut = cuts[cut_name].replace('l1_charge != l2_charge', 'l1_charge == l2_charge')

    for variable in variables:
        cfg_example.var = variable
        if qcd_from_same_sign:
            qcd.var = variable # Can put into function but we will not want it by default if we take normalisations from e.g. high MT
        
        plot = createHistogram(cfg_example, verbose=True)
        plot.Group('Diboson', ['ZZ', 'WZ', 'WW'])
        plot.Group('Single t', ['T_tWch', 'TBar_tWch', 'TToLeptons_sch', 'TToLeptons_tch'])
        plot.Group('ZLL', ['Ztt_ZL', 'Ztt_ZJ'], style=plot.Hist('Ztt_ZL').style)
        HistDrawer.draw(plot, plot_dir='plots/'+cut_name)

