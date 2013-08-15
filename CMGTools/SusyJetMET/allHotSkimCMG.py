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
    'hotskim_QCD170to300',
    'hotskim_QCD300to470',
    'hotskim_QCD470to600',
    'hotskim_QCD600to800',
    'hotskim_QCD800to1000',
    'hotskim_QCD1000to1400',
    'hotskim_QCD1400to1800',
    'hotskim_QCD1800toInf'
    ]

for source in sources:

    tmpFileName = 'tmp_%s_cfg.py' % source
    
    os.system("cat %s | sed -e \"s/fileStr = ''/fileStr = '%s'/\" > %s" % (cfgFileName, source,tmpFileName))
    os.system('nohup cmsRun %s &' % tmpFileName )
