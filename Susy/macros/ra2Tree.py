import os, sys
from ROOT import gROOT, TFile

from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

file = TFile( sys.argv[1] )
events = file.Get('Events')

from CMGTools.RootTools.cmgTuple import *

cmg = cmgTuple( events )
