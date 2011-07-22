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

# jetId aliases should now be included in RA2 aliases
from CMGTools.Susy.macros.ra2 import * 

if len(sys.argv) != 2:
    print 'please provide the pattern to your root files in argument'
    sys.exit(1)

events = Chain('Events',sys.argv[1])
lumi = Chain('LuminosityBlocks',sys.argv[1])

aliases = AliasSetter(events, eventsAliases, 'PAT')
aliases = AliasSetter(events, ra2, 'SUSY')
# aliases = AliasSetter(events, jetId, 'SUSY')
# aliases = AliasSetter(events, jetId, 'COPY')
aliases = AliasSetter(lumi, luminosityBlocksAliases, 'PAT')

from CMGTools.RootTools.runLumiReport import *

from CMGTools.Susy.macros.cutStrings import *
