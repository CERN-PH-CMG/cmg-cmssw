
import os, sys
from ROOT import gROOT, TFile

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )
file = TFile( sys.argv[1] )

from CMGTools.RootTools.RootDir import *

rootDir = RootDir( file, sRedPoints )

# rootDir.subDirs_['cmgMuonHistograms'].Draw()
# rootDir.subDirs_['cmgElectronHistograms'].Draw()
# rootDir.subDirs_['pfJetHistograms'].Draw()
