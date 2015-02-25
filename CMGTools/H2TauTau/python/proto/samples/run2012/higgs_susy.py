import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.yellowreport.YRParser import yrparser8TeV

HiggsSUSYBB80 = cfg.MCComponent(
    name = 'HiggsSUSYBB80',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB90 = cfg.MCComponent(
    name = 'HiggsSUSYBB90',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB100 = cfg.MCComponent(
    name = 'HiggsSUSYBB100',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB110 = cfg.MCComponent(
    name = 'HiggsSUSYBB110',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB120 = cfg.MCComponent(
    name = 'HiggsSUSYBB120',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB130 = cfg.MCComponent(
    name = 'HiggsSUSYBB130',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB140 = cfg.MCComponent(
    name = 'HiggsSUSYBB140',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB160 = cfg.MCComponent(
    name = 'HiggsSUSYBB160',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB180 = cfg.MCComponent(
    name = 'HiggsSUSYBB180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 967197*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB200 = cfg.MCComponent(
    name = 'HiggsSUSYBB200',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB250 = cfg.MCComponent(
    name = 'HiggsSUSYBB250',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB300 = cfg.MCComponent(
    name = 'HiggsSUSYBB300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB350 = cfg.MCComponent(
    name = 'HiggsSUSYBB350',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB400 = cfg.MCComponent(
    name = 'HiggsSUSYBB400',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB450 = cfg.MCComponent(
    name = 'HiggsSUSYBB450',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB500 = cfg.MCComponent(
    name = 'HiggsSUSYBB500',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB600 = cfg.MCComponent(
    name = 'HiggsSUSYBB600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB700 = cfg.MCComponent(
    name = 'HiggsSUSYBB700',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB800 = cfg.MCComponent(
    name = 'HiggsSUSYBB800',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB900 = cfg.MCComponent(
    name = 'HiggsSUSYBB900',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB1000 = cfg.MCComponent(
    name = 'HiggsSUSYBB1000',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )


HiggsSUSYGluGlu80 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu80',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu90 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu90',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu100 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu100',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu110 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu110',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu120 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu120',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu130 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu130',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu140 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu140',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu160 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu160',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu180 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu200 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu200',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu250 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu250',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu300 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu350 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu350',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu400 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu400',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu450 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu450',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu500 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu500',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu600 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu700 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu700',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu800 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu800',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu900 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu900',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu1000 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu1000',
    files = [],
    xSection = 1.0, 
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )



#############

mc_higgs_SUSYBB = [
    HiggsSUSYBB80,
    HiggsSUSYBB90,
    HiggsSUSYBB100,
    HiggsSUSYBB110,
    HiggsSUSYBB120,
    HiggsSUSYBB130,
    HiggsSUSYBB140,
    HiggsSUSYBB160,
    HiggsSUSYBB180,
    HiggsSUSYBB200,
    HiggsSUSYBB250,
    HiggsSUSYBB300,
    HiggsSUSYBB350,
    HiggsSUSYBB400,
    HiggsSUSYBB450,
    HiggsSUSYBB500,
    HiggsSUSYBB600,
    HiggsSUSYBB700,
    HiggsSUSYBB800,
    HiggsSUSYBB900,
    HiggsSUSYBB1000,
    ]

mc_higgs_SUSYGluGlu = [
    HiggsSUSYGluGlu80,
    HiggsSUSYGluGlu90,
    HiggsSUSYGluGlu100,
    HiggsSUSYGluGlu110,
    HiggsSUSYGluGlu120,
    HiggsSUSYGluGlu130,
    HiggsSUSYGluGlu140,
    HiggsSUSYGluGlu160,
    HiggsSUSYGluGlu180,
    HiggsSUSYGluGlu200,
    HiggsSUSYGluGlu250,
    HiggsSUSYGluGlu300,
    HiggsSUSYGluGlu350,
    HiggsSUSYGluGlu400,
    HiggsSUSYGluGlu450,
    HiggsSUSYGluGlu500,
    HiggsSUSYGluGlu600,
    HiggsSUSYGluGlu700,
    HiggsSUSYGluGlu800,
    HiggsSUSYGluGlu900,
    HiggsSUSYGluGlu1000,
    ]


mc_higgs_susy = copy.copy( mc_higgs_SUSYBB )
#mc_higgs_susy = copy.copy( mc_higgs_SUSYGluGlu )
mc_higgs_susy.extend( mc_higgs_SUSYGluGlu )

