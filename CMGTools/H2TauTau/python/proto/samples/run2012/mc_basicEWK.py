import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles


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


mc_basicEWK = [
    DYJets,
    WJets,
    TTJets
    ]

