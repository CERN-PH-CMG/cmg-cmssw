import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from   CMGTools.H2TauTau.proto.samples.higgs import *


DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = [],
    xSection = 3048.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


WJets = cfg.MCComponent(
    name = 'WJets',
    files = [],
    xSection = 31314.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = [],
    xSection = 165.8,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


QCD15 = cfg.MCComponent(
    name = 'QCD15',
    files = [],
    xSection = 8.159e08,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD30 = cfg.MCComponent(
    name = 'QCD30',
    files = [],
    xSection = 5.312e07,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD50 = cfg.MCComponent(
    name = 'QCD50',
    files = [],
    xSection = 6.359e06,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD80 = cfg.MCComponent(
    name = 'QCD80',
    files = [],
    xSection = 7.843e05,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

ggHTT125 = cfg.MCComponent(
    name = 'ggHTT125',
    files = [],
    xSection = 15.3*6.37e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

VBFHTT125 = cfg.MCComponent(
    name = 'VBFHTT125',
    files = [],
    xSection = 1.211*6.37e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )


mc_fall11 = [
    DYJets,
    WJets,
    TTJets,
    #QCD15,
    #QCD30,
    #QCD50,
    #QCD80,
    ggHTT125,
    VBFHTT125,
    ]

mc_fall11.extend( higgs )

