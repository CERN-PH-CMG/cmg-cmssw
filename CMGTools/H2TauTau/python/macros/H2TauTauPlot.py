#!/bin/env python


from CMGTools.H2TauTau.macros.H2TauTauInit import *
from CMGTools.H2TauTau.macros.H2TauTauHistogramList import *

from optparse import OptionParser
import sys

parser = OptionParser()

parser.usage = '''
h2TauTauPlot.py <root rile pattern> <name>
'''
parser.add_option("-l", "--legendFile", 
                  dest="legendFile", 
                  help="file containing the legend description",
                  default=None)
parser.add_option("-c", "--cut", 
                  dest="cut", 
                  help="cut to be applied on the TTree",
                  default='tauMu.obj[0].getSelection("cuts_baseline")')
parser.add_option("-n", "--nEvents", 
                  dest="nEvents", 
                  help="number of events to process",
                  default=1e10)

(options,args) = parser.parse_args()

if len(args) != 2:
    print 'ERROR: please provide a pattern for your root files'
    print args
    sys.exit(1)

print 'WARNING: the cut is not finalized yet.... '

pattern = args[0]
name = args[1]

(events,lumi) = H2TauTauInit( pattern )

histos = H2TauTauHistogramList( name )
histos.FillHistos( events, options.cut )
histos.Write()
