import os, sys
from ROOT import gROOT, TFile, TCanvas, TH1F, TGraphAsymmErrors, TChain

from CMGTools.RootTools.RootTools import *
from CMGTools.RootTools.Style import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

style1 = Style()
style2 = Style( markerColor = 2, lineColor = 2)
style3 = Style( markerColor = 3, lineColor = 3)
style4 = Style( markerColor = 4, lineColor = 4)
style5 = Style( markerColor = 5, lineColor = 5)

basename = 'rfio:/castor/cern.ch/cms/store/cmst3/user/lenzip/CMG/SusyLP/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/CMGtuple/'
vfile = [ ] 
for i in range(6):
  vfile.append(basename+'susy_tree_CMG_'+str(i)+'.root')

events = TChain('Events', 'Events')
for file in vfile:
  events.AddFile(file)       
#file = TFile( sys.argv[1] )
#events = file.Get('Events')

from CMGTools.RootTools.cmgTuple import *

cmg = cmgTuple( events )

LPcanvas = TCanvas('LPcanvas','LP', 1000,800)
LPcanvas.Divide(2,3)

num1 = TH1F('num1', 'num1', 100, 0, 100)
num2 = TH1F('num2', 'num2', 100, 0, 100)

den1 = TH1F('den1', 'den1', 100, 0, 100)
den2 = TH1F('den2', 'den2', 100, 0, 100)

selnum1 = 'cmgMuonSelVec.size()>0 && cmgMuonSel[0].getSelection(\"cuts_vbtfmuon\") && cmgMuonSel[0].getSelection(\"cuts_isomuon\")'
selnum2 = 'cmgMuonSelVec.size()>1 && cmgMuonSel[1].getSelection(\"cuts_vbtfmuon\") && cmgMuonSel[1].getSelection(\"cuts_isomuon\")'

selden1 = 'cmgMuonSelVec.size()>0 && cmgMuonSel[0].getSelection(\"cuts_vbtfmuon\")'
selden2 = 'cmgMuonSelVec.size()>1 && cmgMuonSel[1].getSelection(\"cuts_vbtfmuon\")'

LPcanvas.cd(1)
cmg.Draw('cmgMuonSel[0].pt()>>den1', selden1)
LPcanvas.cd(2)
cmg.Draw('cmgMuonSel[1].pt()>>den2', selden2)
LPcanvas.cd(3)
cmg.Draw('cmgMuonSel[0].pt()>>num1', selnum1)
LPcanvas.cd(4)
cmg.Draw('cmgMuonSel[1].pt()>>num2', selnum2)

eff1 = TGraphAsymmErrors(num1, den1)
eff2 = TGraphAsymmErrors(num2, den2)

LPcanvas.cd(5)
eff1.Draw('AP')
sRedPoints.formatHisto(eff2)
eff2.Draw('P')


#LPnocut = TH1F('LPnocut', 'LPnocut', 20, -0.5, 1.5)
LPplus = TH1F('LPplus', 'LPplus', 20, -0.5, 1.5)
LPplus.Sumw2()
LPminus = TH1F('LPminus', 'LPminus', 20, -0.5, 1.5)
LPminus.Sumw2()
sBlack.formatHisto(LPplus)
sRedPoints.formatHisto(LPminus)

LPcanvas.cd(6)
#cmg.Draw('LPWMuNu[0].lp()>>LPnocut', 'LPWMuNuVec.size()>0')
selection = 'LPWMuNuVec.size()>0 && LPWMuNu.getSelection(\"cuts_ptLoose\") && LPPFJetSelVec.size()>1 && LPMHTPFJet30LooseSelVec.size()>0'
cmg.Draw('LPWMuNu[0].lp()>>LPplus', selection+' && LPWMuNu.charge()==1',)
cmg.Draw('LPWMuNu[0].lp()>>LPminus', selection + ' && LPWMuNu.charge()==-1', 'sames')

outputFile = TFile('plots.root', 'RECREATE')
outputFile.cd()

LPcanvas.Write()

otherPlots = TCanvas('otherPlots','otherPlots', 1000,800)
otherPlots.Divide(2,3)
otherPlots.cd(1)
njets = TH1F('njets', 'njets', 10, -0.5, 9.5)
cmg.Draw('LPPFJetSelVec.size()>>njets', 'LPWMuNuVec.size()>0')

jet1 = TH1F('jet1', 'jet1', 200, 0, 200)
style1.formatHisto(jet1)
jet2 = TH1F('jet2', 'jet2', 200, 0, 200)
style2.formatHisto(jet2)
jet3 = TH1F('jet3', 'jet3', 200, 0, 200)
style3.formatHisto(jet3)
jet4 = TH1F('jet4', 'jet4', 200, 0, 200)
style4.formatHisto(jet4)
otherPlots.cd(2)
cmg.Draw('LPPFJetSel[0].pt()>>jet1', 'LPPFJetSelVec.size()>0 && LPWMuNuVec.size()>0')
cmg.Draw('LPPFJetSel[1].pt()>>jet2', 'LPPFJetSelVec.size()>1 && LPWMuNuVec.size()>0', 'sames')
cmg.Draw('LPPFJetSel[2].pt()>>jet3', 'LPPFJetSelVec.size()>2 && LPWMuNuVec.size()>0', 'sames')
cmg.Draw('LPPFJetSel[3].pt()>>jet4', 'LPPFJetSelVec.size()>3 && LPWMuNuVec.size()>0', 'sames')


otherPlots.Write()
