import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F, TH1F, TH1D , TLegend
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

# adding current directory in PYTHONPATH
sys.path.append('.')

process = 'ANA'

from CMGTools.RootTools.TaggedFile import *
from CMGTools.RootTools.Style import *
from CMGTools.RootTools.TreeFunctions import *
from CMGTools.RootTools.AliasSetter import * 
from CMGTools.RootTools.CutCorrelations import * 
from CMGTools.RootTools.GraphErrors import * 
from CMGTools.RootTools.Legend import * 
from CMGTools.Common.macros.aliases import * 

