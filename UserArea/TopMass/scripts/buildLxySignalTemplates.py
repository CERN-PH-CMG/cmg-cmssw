#!/usr/bin/env python

import os
import optparse
import sys
from array import array
from math import sqrt,pow
import ROOT
from ROOT import *

usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-c', '--cdir' ,    dest='cdir'     , help='Name of the input directory with the control trees.'         , default='/store/cmst3/user/jueugste/directHists_merged')
parser.add_option('-t', '--tdir' ,    dest='tdir'     , help='Name of the input directory with the target trees.'          , default='/store/cmst3/user/psilva/Top')
parser.add_option('-u', '--use' ,     dest='fopt'     , help='Use a previous optimization result.'                         , default='')
(opt, args) = parser.parse_args()

ctrlDir   = opt.cdir
targetDir = opt.tdir
optFile   = opt.fopt

targetList={ '1635':    ['TTJets_mass163_5Histos',       'MC8TeV_TTJets_163v5_filt1_summary' ],
             '1665':    ['TTJets_mass166_5Histos',       'MC8TeV_TTJets_166v5_filt1_summary' ],
             '1695':    ['TTJets_mass169_5Histos',       'MC8TeV_TTJets_169v5_filt1_summary' ],
             '1725':    ['TTJets_MassiveBinDECAYHistos', 'MC8TeV_TTJets_filt1_summary'       ],
             '1755':    ['TTJets_mass175_5Histos',       'MC8TeV_TTJets_175v5_filt1_summary' ],
             '1785':    ['TTJets_mass178_5Histos',       'MC8TeV_TTJets_178v5_filt1_summary' ],
             '1815':    ['TTJets_mass181_5Histos',       'MC8TeV_TTJets_181v5_filt1_summary' ],
             'mepsdown':['TTJets_matchingdownHistos',    'MC8TeV_TTJets_matchingdown_summary'],
             'mepsup':  ['TTJets_matchingupHistos',      'MC8TeV_TTJets_matchingup_summary'  ],
             'q2down':  ['TTJets_scaledownHistos',       'MC8TeV_TTJets_scaledown_summary'   ],
             'q2up':    ['TTJets_scaleupHistos',         'MC8TeV_TTJets_scaleup_summary'     ]
             }

#compile the script
gROOT.ProcessLine('.L ClassifyJetKinematics.C+')
gROOT.SetBatch()

#
#first optimize based on the central sample, if needed
#
if(optFile==''):
    ctrlFile   = targetList['1725'][0]
    targetFile = targetList['1725'][1]
    print 'Optimization file was not found running first optimization'
    ClassifyJetKinematics(ctrlDir+'/'+ctrlFile+'.root',targetDir+'/'+targetFile+'.root','gamma',0.3)
    optFile="JetKinematicsOptimization.root"


#
# Now build the templates
#
print 'Using results from optimization stored @ %s'%(optFile)
os.system('rm LxyTemplates.root')
for key in targetList:
    ctrlFile   = targetList[key][0]
    targetFile = targetList[key][1]
    ClassifyJetKinematics(ctrlDir+'/'+ctrlFile+'.root',targetDir+'/'+targetFile+'.root','gamma',0.3,optFile,key)

