import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )


process = 'ANA'


from CMGTools.RootTools.TreeFunctions import *
from CMGTools.RootTools.AliasSetter import * 
from CMGTools.RootTools.CutCorrelations import * 
from CMGTools.Common.macros.aliases import * 

if len(sys.argv) != 2:
    print 'please provide the pattern to your root files in argument'
    sys.exit(1)

events = Chain('Events',sys.argv[1])
lumi = Chain('LuminosityBlocks',sys.argv[1])

aliases = AliasSetter(events, eventsAliases, 'ANA')
aliases = AliasSetter(lumi, luminosityBlocksAliases, 'ANA')

from CMGTools.RootTools.runLumiReport import *

# now you can use your aliases, by doing e.g.:
# events.Draw('met.obj.et()')

# or by pasting some commands from files in the CMGTools/RootTools/python/macros directory

# or by executing one of the python functions we have loaded:
# scan( events, 'met.obj.et()>350' )
# lumisProcessedForEachRun(histos,lumi)
