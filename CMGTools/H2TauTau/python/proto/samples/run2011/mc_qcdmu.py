import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg


QCDMuH20Pt15 = cfg.MCComponent(
    name = 'QCDMuH20Pt15',
    files = [],
    xSection = 1.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

QCDMuH15to20Pt5 = cfg.MCComponent(
    name = 'QCDMuH15to20Pt5',
    files = [],
    xSection = 1.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


mc_qcd = [
    QCDMuH20Pt15,
    QCDMuH15to20Pt5,    
    ]

