#!/bin/env python


from CMGTools.H2TauTau.macros.h2TauTauInit import *

from optparse import OptionParser
import sys

parser = OptionParser()

parser.usage = '''
h2TauTauPlot.py <root rile pattern>.
'''
parser.add_option("-l", "--legendFile", 
                  dest="legendFile", 
                  help="file containing the legend description",
                  default=None)

(options,args) = parser.parse_args()

if len(args) != 1:
    print 'ERROR: please provide a pattern for your root files'
    sys.exit(1)

pattern = args[0]

(events,lumi) = h2TauTauInit( pattern )
