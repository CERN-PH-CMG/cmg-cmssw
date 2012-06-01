import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.yellowreport.YRParser import yrparser

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF115 = cfg.MCComponent(
    name = 'HiggsVBF115',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF120 = cfg.MCComponent(
    name = 'HiggsVBF120',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF125 = cfg.MCComponent(
    name = 'HiggsVBF125',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF130 = cfg.MCComponent(
    name = 'HiggsVBF130',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF135 = cfg.MCComponent(
    name = 'HiggsVBF135',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )




HiggsGGH110 = cfg.MCComponent(
    name = 'HiggsGGH110',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH115 = cfg.MCComponent(
    name = 'HiggsGGH115',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH120 = cfg.MCComponent(
    name = 'HiggsGGH120',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH125 = cfg.MCComponent(
    name = 'HiggsGGH125',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH130 = cfg.MCComponent(
    name = 'HiggsGGH130',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )

HiggsGGH135 = cfg.MCComponent(
    name = 'HiggsGGH135',
    files = [],
    xSection = 100000, 
    nGenEvents = 100,
    triggers = [],
    effCorrFactor = 1 )


mc_higgs_vbf = [
    HiggsVBF110,
    HiggsVBF115,
    HiggsVBF120,
    HiggsVBF125,
    HiggsVBF130,
    HiggsVBF135,
    ]

mc_higgs_ggh = [
    HiggsGGH110,
    HiggsGGH115,
    HiggsGGH120,
    HiggsGGH125,
    HiggsGGH130,
    HiggsGGH135,
    ]

mc_higgs = copy.copy( mc_higgs_vbf )
mc_higgs.extend( mc_higgs_ggh )

pattern = re.compile('Higgs(\D+)(\d+)')
for h in mc_higgs:
    m = pattern.match( h.name )
    process = m.group(1)
    mass = float(m.group(2))
    h.xSection = yrparser.get(mass)[process]['sigma']
    print h.name, 'sigma = ', h.xSection


