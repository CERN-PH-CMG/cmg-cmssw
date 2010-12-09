#!/usr/bin/env python

import os, sys,  imp, re, pprint

import FWCore.ParameterSet.Config as cms


cfgFileName = 'RA2_ana_HotSkimSueAnn_cfg.py'



# loading process from master python file

# sourceModule = 'CMGTools.SusyJetMET.Sources.HotSkims.'
sources = [
#    'SueAnnHot_0to5',
#    'SueAnnHot_5to15',
#    'SueAnnHot_15to30',
#    'SueAnnHot_30to50',
#    'SueAnnHot_50to80',
#    'SueAnnHot_80to120',
#    'SueAnnHot_120to170',
    'SueAnnHot_170to300',
    'SueAnnHot_300to470',
    'SueAnnHot_470to600',
    'SueAnnHot_600to800',
    'SueAnnHot_800to1000',
    'SueAnnHot_1000to1400',
    'SueAnnHot_1400to1800',
    'SueAnnHot_1800toInf'
    ]

for source in sources:

    tmpFileName = 'tmp_%s_cfg.py' % source
    
    os.system("cat %s | sed -e \"s/sourceExt = 'SueAnnHot'/sourceExt = '%s'/\" > %s" % (cfgFileName, source,tmpFileName))
    os.system('nohup cmsRun %s &' % tmpFileName )
