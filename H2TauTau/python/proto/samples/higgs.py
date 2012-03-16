import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

#COLIN : need to add the correct cross-sections

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



higgs_vbf = [
    HiggsVBF110,
    HiggsVBF115,
    HiggsVBF120,
    HiggsVBF125,
    HiggsVBF130,
    HiggsVBF135,
    ]


higgs = higgs_vbf

