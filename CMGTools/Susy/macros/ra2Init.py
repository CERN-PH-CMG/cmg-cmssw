import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

#file = TFile( sys.argv[1] )
#events = file.Get('Events')

filters = 'hbheNoiseFilter && greedyMuons && inconsMuons'
NOTfilters = '!(%s)' % filters


jetId = 'pfJetsVeryLoose99Failed.@obj.size()==0'
NOTjetId = '!(%s)' % jetId

jetIdTight = 'pfJetsVeryLoose95Failed.@obj.size()==0'
NOTjetIdTight = '!(%s)' % jetIdTight

fullCleaning = filters + ' && ' + jetId

leptonVeto = 'mu.@obj.size()==0 && ele.@obj.size()==0'
NOTleptonVeto = leptonVeto + ' && ' + jetId

# the following cut probably does not work correctly in root...
leptonVeto2 = '(mu.@obj.size()==0 || mu.obj.relIso()<0.1) && (ele.@obj.size()==0 || ele.obj.relIso()<0.1)'

highMHT = 'mht.obj[0].pt()>600 && ht.obj[0].sumEt()>350'

allCuts = fullCleaning + ' && ' + leptonVeto + ' && ' + highMHT
allCutsNoVeto = fullCleaning + ' && ' + highMHT

evSel = 'run==166782 && lumi==525 && event == 565817524'


def setEventList( tree, cut=None ):
    print 'now browsing the full tree... might take a while, but drawing will then be faster!'
    tree.Draw('>>pyplus', cut)
    from ROOT import gDirectory
    pyplus = gDirectory.Get('pyplus')
    pyplus.SetReapplyCut(True)
    tree.SetEventList(pyplus)
    
def scan( tree, cut=None ):
    out = tree.Scan('EventAuxiliary.id().run():EventAuxiliary.id().luminosityBlock():EventAuxiliary.id().event():mht.obj.pt():met.obj.pt():ht.obj.sumEt()', cut, 'colsize=14')


def lumiReport( cuts=None, hist=None, opt=''):
    
    if hist==None:
        events.Draw('lumi:run','',opt)
    else:
        events.Draw('lumi:run>>'+hist.GetName(), '',opt)
    gPad.SaveAs('lumiReport.png')


# can create other trees with other set of files
sys.path.insert(0,'.')
from AliasSetter import * 
from aliases import *

pattern = sys.argv[1]

events = Chain('Events', pattern)
eventsAliases = AliasSetter(events, cmgBasic, 'ANA')
eventsAliases.setAliases(ra2, 'SUSY')

lumi = Chain('LuminosityBlocks', pattern)
lumiAliases = AliasSetter(lumi, lumiBlocks, 'ANA')

