import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.yellowreport.YRParser import yrparser7TeV

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF115 = cfg.MCComponent(
    name = 'HiggsVBF115',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF120 = cfg.MCComponent(
    name = 'HiggsVBF120',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF125 = cfg.MCComponent(
    name = 'HiggsVBF125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF130 = cfg.MCComponent(
    name = 'HiggsVBF130',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF135 = cfg.MCComponent(
    name = 'HiggsVBF135',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF140 = cfg.MCComponent(
    name = 'HiggsVBF140',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF145 = cfg.MCComponent(
    name = 'HiggsVBF145',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF150 = cfg.MCComponent(
    name = 'HiggsVBF150',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )


############# Gluon fusion ###############

HiggsGGH110 = cfg.MCComponent(
    name = 'HiggsGGH110',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH115 = cfg.MCComponent(
    name = 'HiggsGGH115',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH120 = cfg.MCComponent(
    name = 'HiggsGGH120',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH125 = cfg.MCComponent(
    name = 'HiggsGGH125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH130 = cfg.MCComponent(
    name = 'HiggsGGH130',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH135 = cfg.MCComponent(
    name = 'HiggsGGH135',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH140 = cfg.MCComponent(
    name = 'HiggsGGH140',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH145 = cfg.MCComponent(
    name = 'HiggsGGH145',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH150 = cfg.MCComponent(
    name = 'HiggsGGH150',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )


############# VH  ###############

HiggsVH110 = cfg.MCComponent(
    name = 'HiggsVH110',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH115 = cfg.MCComponent(
    name = 'HiggsVH115',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH120 = cfg.MCComponent(
    name = 'HiggsVH120',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH125 = cfg.MCComponent(
    name = 'HiggsVH125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH130 = cfg.MCComponent(
    name = 'HiggsVH130',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH135 = cfg.MCComponent(
    name = 'HiggsVH135',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH140 = cfg.MCComponent(
    name = 'HiggsVH140',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH145 = cfg.MCComponent(
    name = 'HiggsVH145',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH150 = cfg.MCComponent(
    name = 'HiggsVH150',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )



#############

mc_higgs_vbf = [
    HiggsVBF110,
    HiggsVBF115,
    HiggsVBF120,
    HiggsVBF125,
    HiggsVBF130,
    HiggsVBF135,
    HiggsVBF140,
    HiggsVBF145,
    # HiggsVBF150,
    ]

mc_higgs_ggh = [
    HiggsGGH110,
    HiggsGGH115,
    HiggsGGH120,
    HiggsGGH125,
    HiggsGGH130,
    HiggsGGH135,
    HiggsGGH140,
    HiggsGGH145,
    # HiggsGGH150,
    ]

mc_higgs_vh = [
    HiggsVH110,
    HiggsVH115,
    HiggsVH120,
    HiggsVH125,
    HiggsVH130,
    HiggsVH135,
    HiggsVH140,
    HiggsVH145,
    # HiggsVH150,
    ]


mc_higgs = copy.copy( mc_higgs_vbf )
mc_higgs.extend( mc_higgs_ggh )
mc_higgs.extend( mc_higgs_vh )


pattern = re.compile('Higgs(\D+)(\d+)')
for h in mc_higgs:
    # import pdb; pdb.set_trace()
    m = pattern.match( h.name )
    process = m.group(1)
    mass = float(m.group(2))
    xSection = 0.
    if process=='VH':
        xSection += yrparser7TeV.get(mass)['WH']['sigma']
        xSection += yrparser7TeV.get(mass)['ZH']['sigma']
    else:
        xSection += yrparser7TeV.get(mass)[process]['sigma']
    br = yrparser7TeV.get(mass)['H2F']['tautau']
    h.xSection = xSection*br
    h.branchingRatio = br
    # print h.name, 'sigma*br =', h.xSection, 'sigma =', xSection, 'br =', h.branchingRatio


## print 'generating shifted samples'


## mc_higgs_up = sampleShift( locals(), mc_higgs, 'Up')
## mc_higgs_down = sampleShift( locals(), mc_higgs, 'Down')




