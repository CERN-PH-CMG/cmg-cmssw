import os, sys
from ROOT import gROOT, TFile, TProfile

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

if len(sys.argv)!=2:
    print 'usage: python jetHistograms.py <root file>'
    sys.exit(1)
    
file = TFile( sys.argv[1] )

from CMGTools.RootTools.RootDir import *

rootDir = RootDir( file, sBlueSquares )
pfJetH = rootDir.subDirs_['pfJetHistograms']

print pfJetH.subDirs_


pfJetH.subDirs_['ChargedHadrons'].DrawAll()
