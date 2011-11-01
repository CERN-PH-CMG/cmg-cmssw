import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F, TH1F, TH1D , TProfile, TLegend
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

# adding current directory in PYTHONPATH
sys.path.append('.')

from CMGTools.RootTools.TaggedFile import *
from CMGTools.RootTools.Style import *
from CMGTools.RootTools.TreeFunctions import *
from CMGTools.RootTools.AliasSetter import * 
from CMGTools.RootTools.CutCorrelations import * 
from CMGTools.RootTools.GraphErrors import * 
from CMGTools.RootTools.Legend import * 
from CMGTools.RootTools.runLumiReport import *

from CMGTools.Common.macros.aliases import * 

TREES = []


def Init( pattern, process='PAT'):
    events = Chain('Events', pattern)
    lumi = Chain('LuminosityBlocks', pattern)

    TREES.append(events)
    TREES.append(lumi)
    
    aliases = AliasSetter(events, eventsAliases, process)
    aliases = AliasSetter(events, pat, 'PAT')
    aliases = AliasSetter(events, reco, 'RECO')
    aliases = AliasSetter(lumi, luminosityBlocksAliases, process)

    return events,lumi


if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = '''
    RootInit.py <pattern>
    Initialize the events and lumi trees for a set of root files described by pattern.
    Example:
    \t RootInit.py "*.root"
    '''
    parser.add_option("-p", "--process", 
                      dest="process", 
                      help="process name for the CMG objects, usually PAT or ANA",
                      default='PAT')
    
    (options,args) = parser.parse_args()

    if len(args)!=1:
        parser.print_help()
        print
        print 'Please specify a pattern for your files'
        sys.exit(1)

    pattern = args[0]

    (events, lumi) = Init( pattern, options.process)
