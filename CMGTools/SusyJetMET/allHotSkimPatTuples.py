#!/usr/bin/env python

import os, sys,  imp, re, pprint

import FWCore.ParameterSet.Config as cms


cfgFileName = 'RA2_patTuple_cfg.py'



# loading process from master python file

sourceModule = 'CMGTools.SusyJetMET.Sources.HotSkims.'
sources = [
    'hotskim_QCD170to300',
    'hotskim_QCD300to470',
    'hotskim_QCD470to600',
    'hotskim_QCD600to800',
    'hotskim_QCD800to1000',
    'hotskim_QCD1000to1400',
    'hotskim_QCD1400to1800',
    'hotskim_QCD1800toInf'
    ]


# sources = [ sources[0] ]

for source in sources:

    tmpFileName = 'tmp_%s_cfg.py' % source
    
    os.system("cat RA2_patTuple_cfg.py | sed -e \"s/sourceExt = 'hotskim_QCD'/sourceExt = '%s'/\" > %s" % (source,tmpFileName))
    os.system('nohup cmsRun %s &' % tmpFileName)
