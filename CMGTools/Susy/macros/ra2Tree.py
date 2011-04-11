import os, sys
from ROOT import gROOT, TFile, TCanvas

from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

file = TFile( sys.argv[1] )
events = file.Get('Events')

from CMGTools.RootTools.cmgTuple import *

cmg = cmgTuple( events )

RA2canvas = TCanvas('RA2canvas','RA2', 1000,800)
RA2canvas.Divide(3,2)

RA2canvas.cd(1)
cmg.Draw('cmgPFBaseJetSel.pt()')
RA2canvas.cd(2)
cmg.Draw('cmgMHTPFJets30.pt()')
RA2canvas.cd(3)
cmg.Draw('cmgMHTPFJets30.pt():cmgMHTCaloJets30.pt()','','col')
RA2canvas.cd(4)
cmg.Draw('RA2cmgMHTPFJets50.sumEt()')
RA2canvas.cd(5)
cmg.Draw('cmgMuonSel.pt()')
RA2canvas.cd(6)
cmg.Draw('cmgElectronSel.pt()')


