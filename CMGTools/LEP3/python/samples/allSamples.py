import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.Production.getFiles import getFiles

file_owner = 'cbern'
filePattern = 'cmgTuple.*root'
cache = True

ELEL = cfg.MCComponent(
    name = 'ELEL',
    files = [],
    xSection = 4.2, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

GAGA = cfg.MCComponent(
    name = 'GAGA',
    files = [],
    xSection = 1., 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

HZVIS = cfg.MCComponent(
    name = 'HZVIS',
    files = [],
    xSection = 0.2, 
    nGenEvents = 97000,
    triggers = [],
    effCorrFactor = 1 )

INVIS = cfg.MCComponent(
    name = 'INVIS',
    files = [],
    xSection = 0.158, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

MUMU = cfg.MCComponent(
    name = 'MUMU',
    files = [],
    xSection = 4.2, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

QQBAR = cfg.MCComponent(
    name = 'QQBAR',
    files = [],
    xSection = 50., 
    nGenEvents = 5998000.,
    triggers = [],
    effCorrFactor = 1 )

TAUTAU = cfg.MCComponent(
    name = 'TAUTAU',
    files = [],
    xSection = 4.2, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

WENU = cfg.MCComponent(
    name = 'WENU',
    files = [],
    xSection = 1.37, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 16., 
    nGenEvents = 1946000.,
    triggers = [],
    effCorrFactor = 1 )

ZEE = cfg.MCComponent(
    name = 'ZEE',
    files = [],
    xSection = 3.8, 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

ZNNB = cfg.MCComponent(
    name = 'ZNNB',
    files = [],
    xSection = 1., 
    nGenEvents = 1.,
    triggers = [],
    effCorrFactor = 1 )

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 1.3, 
    nGenEvents = 982000.,
    triggers = [],
    effCorrFactor = 1 )


allSamples = [
    ELEL,
    GAGA,
    HZVIS,
    INVIS,
    MUMU,
    QQBAR,
    TAUTAU,
    WENU,
    WW,
    ZEE,
    ZNNB,
    ZZ,
    ]

for sam in allSamples:
    sam.files = getFiles('/'.join(['/LEP3',sam.name]),
                         file_owner, filePattern, cache)
    sam.splitFactor = int( sam.nGenEvents / 30000 ) 
