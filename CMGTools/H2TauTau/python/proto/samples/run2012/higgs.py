import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.yellowreport.YRParser import yrparser8TeV
# from CMGTools.H2TauTau.proto.samples.sampleShift import sampleShift

HiggsVBF90 = cfg.MCComponent(
    name = 'HiggsVBF90',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF95 = cfg.MCComponent(
    name = 'HiggsVBF95',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF100 = cfg.MCComponent(
    name = 'HiggsVBF100',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF105 = cfg.MCComponent(
    name = 'HiggsVBF105',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

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

HiggsVBF155 = cfg.MCComponent(
    name = 'HiggsVBF155',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF160 = cfg.MCComponent(
    name = 'HiggsVBF160',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

############# Gluon fusion ###############

HiggsGGH90 = cfg.MCComponent(
    name = 'HiggsGGH90',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH95 = cfg.MCComponent(
    name = 'HiggsGGH95',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH100 = cfg.MCComponent(
    name = 'HiggsGGH100',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH105 = cfg.MCComponent(
    name = 'HiggsGGH105',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

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

HiggsGGH155 = cfg.MCComponent(
    name = 'HiggsGGH155',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH160 = cfg.MCComponent(
    name = 'HiggsGGH160',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )


############# VH  ###############

HiggsVH90 = cfg.MCComponent(
    name = 'HiggsVH90',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH95 = cfg.MCComponent(
    name = 'HiggsVH95',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH100 = cfg.MCComponent(
    name = 'HiggsVH100',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH105 = cfg.MCComponent(
    name = 'HiggsVH105',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

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

HiggsVH155 = cfg.MCComponent(
    name = 'HiggsVH155',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH160 = cfg.MCComponent(
    name = 'HiggsVH160',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )





HiggsVBFtoWW125 = cfg.MCComponent(
    name = 'HiggsVBFtoWW125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGHtoWW125 = cfg.MCComponent(
    name = 'HiggsGGHtoWW125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVHtoWW125 = cfg.MCComponent(
    name = 'HiggsVHtoWW125',
    files = [],
    xSection = None, 
    nGenEvents = 0,
    triggers = [],
    effCorrFactor = 1 )



#############

mc_higgs_vbf = [
    HiggsVBF90,
    HiggsVBF95,
    HiggsVBF100,
    HiggsVBF105,
    HiggsVBF110,
    HiggsVBF115,
    HiggsVBF120,
    HiggsVBF125,
    HiggsVBF130,
    HiggsVBF135,
    HiggsVBF140,
    HiggsVBF145,
    HiggsVBF150,
    HiggsVBF155,
    HiggsVBF160,
    HiggsVBFtoWW125,
    ]

mc_higgs_ggh = [
    HiggsGGH90,
    HiggsGGH95,
    HiggsGGH100,
    HiggsGGH105,
    HiggsGGH110,
    HiggsGGH115,
    HiggsGGH120,
    HiggsGGH125,
    HiggsGGH130,
    HiggsGGH135,
    HiggsGGH140,
    HiggsGGH145,
    HiggsGGH150,
    HiggsGGH155,
    HiggsGGH160,
    HiggsGGHtoWW125,
    ]

mc_higgs_vh = [
    HiggsVH90,
    HiggsVH95,
    HiggsVH100,
    HiggsVH105,
    HiggsVH110,
    HiggsVH115,
    HiggsVH120,
    HiggsVH125,
    HiggsVH130,
    HiggsVH135,
    HiggsVH140,
    HiggsVH145,
    HiggsVH150,
    HiggsVH155,
    HiggsVH160,
    HiggsVHtoWW125
    ]


mc_higgs = copy.copy( mc_higgs_vbf )
mc_higgs.extend( mc_higgs_ggh )
mc_higgs.extend( mc_higgs_vh )


pattern = re.compile('Higgs(\D+)(\d+)')
for h in mc_higgs:
    m = pattern.match( h.name )
    process = m.group(1)
    if 'toWW' in process :
      process = process.replace('toWW','')
      isToWW = True
    else :
      isToWW = False     
    mass = float(m.group(2))
    xSection = 0.
    try:
        if process=='VH':
            xSection += yrparser8TeV.get(mass)['WH']['sigma']
            xSection += yrparser8TeV.get(mass)['ZH']['sigma']
        else:
            xSection += yrparser8TeV.get(mass)[process]['sigma']
    except KeyError:
        print 'Higgs mass', mass, 'not found in cross section tables. Interpolating linearly at +- 1 GeV...'
        if process=='VH':
            xSection += 0.5 * (yrparser8TeV.get(mass-1.)['WH']['sigma'] + xSection + yrparser8TeV.get(mass+1.)['WH']['sigma'])
            xSection += 0.5 * (yrparser8TeV.get(mass-1.)['ZH']['sigma'] + yrparser8TeV.get(mass+1.)['ZH']['sigma'])
        else:
            xSection += 0.5 * (yrparser8TeV.get(mass-1.)[process]['sigma'] + yrparser8TeV.get(mass+1.)[process]['sigma'])

    if not isToWW :
      br = yrparser8TeV.get(mass)['H2F']['tautau']
    else :
      br = yrparser8TeV.get(mass)['H2B']['WW']
    h.xSection = xSection*br
    h.branchingRatio = br
    print h.name, 'sigma*br =', h.xSection, 'sigma =', xSection, 'br =', h.branchingRatio


# print 'generating shifted samples'
# mc_higgs_up = sampleShift( locals(), mc_higgs, 'Up')
# mc_higgs_down = sampleShift( locals(), mc_higgs, 'Down')
