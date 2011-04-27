import os, sys
from ROOT import gROOT, TFile, TCanvas, gPad

from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

#file = TFile( sys.argv[1] )
#events = file.Get('Events')

events = Chain('Events', sys.argv[1])

# from CMGTools.RootTools.cmgTuple import *

# cmg = cmgTuple( events )

events.SetAlias('pfjets','cmgPFJets_cmgPFJetSel__PAT')
events.SetAlias('jets','cmgBaseJets_cmgPFBaseJetSel__PAT')
events.SetAlias('mht','cmgBaseMETs_RA2MHTPFJet30__ANA')
events.SetAlias('ht','cmgBaseMETs_RA2MHTPFJet50Central__ANA')
events.SetAlias('met','cmgBaseMETs_cmgPFMET__PAT')
events.SetAlias('ele','cmgElectrons_cmgElectronSel__PAT')
events.SetAlias('mu','cmgMuons_cmgMuonSel__PAT')


####################   Jets  #######################


jetCan = TCanvas('jetCan','jets', 1000,800)
jetCan.Divide(2,2)

# tails: need to put in the event cleaners :
# HBHENoiseFilter, scrapping, PF filters.
# need to keep the events filterd by these guys. 

doPlots = False

jetCan.cd(1)
events.Draw('pfjets.obj.pt()-jets.obj.pt()')
jetCan.cd(2)
events.SetLineColor(1)
events.Draw('jets.obj[2].pt()','','')
events.SetLineColor(2)
events.Draw('jets.obj[1].pt()','','same')
events.SetLineColor(3)
events.Draw('jets.obj[0].pt()','','same')
events.SetLineColor(1)

jetCan.cd(3)
events.Draw('pfjets.obj.component(4).fraction()')
gPad.SetLogy()
jetCan.cd(4)
events.Draw('pfjets.obj.component(5).fraction()')
gPad.SetLogy()


####################   MET / MHT  #######################

metCan = TCanvas('metCan','MHT & HT', 1000,800)
metCan.Divide(2,2)

metCan.cd(1)
events.Draw('mht.obj.et()')
gPad.SetLogy()
metCan.cd(2)
events.Draw('ht.obj.sumEt()')
metCan.cd(3)
events.Draw('met.obj.et()')
gPad.SetLogy()
metCan.cd(4)
events.Draw('met.obj.et():mht.obj.et()','','col')

####################  Electrons  #######################

eleCan = TCanvas('eleCan','Electrons', 1000,800)
events.Draw('ele.obj.pt()')

####################  Muons  #######################

eleCan = TCanvas('muCan','Muons', 1000,800)
events.Draw('mu.obj.pt()')

