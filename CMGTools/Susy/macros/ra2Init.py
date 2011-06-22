import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

#file = TFile( sys.argv[1] )
#events = file.Get('Events')

def init(pattern):
    events = Chain('Events', pattern)

    events.SetAlias('pfjets','cmgPFJets_cmgPFJetSel__PAT')
    events.SetAlias('jets','cmgBaseJets_cmgPFBaseJetSel__PAT')
    events.SetAlias('mht','cmgBaseMETs_RA2MHTPFJet30__SUSY')
    events.SetAlias('ht','cmgBaseMETs_RA2MHTPFJet50Central__SUSY')
    events.SetAlias('met','cmgBaseMETs_cmgPFMET__PAT')
    events.SetAlias('ele','cmgElectrons_cmgElectronSel__PAT')
    events.SetAlias('mu','cmgMuons_cmgMuonSel__PAT')
    
    events.SetAlias('hbheNoiseFilter','bool_HBHENoiseFilterResultProducer_HBHENoiseFilterResult_PAT.obj')
    events.SetAlias('inconsMuons', 'bool_inconsistentMuonsTagging_Result_PAT.obj')
    events.SetAlias('greedyMuons', 'bool_greedyMuonsTagging_Result_PAT.obj')
    
    
    events.SetAlias('run','EventAuxiliary.id().run()')
    events.SetAlias('lumi','EventAuxiliary.id().luminosityBlock()')

    return events

fullCleaning = 'hbheNoiseFilter && greedyMuons && inconsMuons && pfjets.obj.component(5).fraction()<0.99 && pfjets.obj.component(4).fraction()<0.99'

leptonVeto = 'mu.@obj.size()==0 && ele.@obj.size()==0'
leptonVeto2 = '(mu.@obj.size()==0 || mu.obj.relIso()<0.1) && (ele.@obj.size()==0 || ele.obj.relIso()<0.1)'

highMHT = 'mht.obj[0].pt()>600 && ht.obj[0].sumEt()>350'

allCuts = fullCleaning + ' && ' + leptonVeto + ' && ' + highMHT
allCutsNoVeto = fullCleaning + ' && ' + highMHT


def setEventList( cut=None ):
    print 'now browsing the full tree... might take a while, but drawing will then be faster!'
    events.Draw('>>pyplus', cut)
    from ROOT import gDirectory
    pyplus = gDirectory.Get('pyplus')
    pyplus.SetReapplyCut(True)
    events.SetEventList(pyplus)
    
def scan( cut=None ):
    out = events.Scan('EventAuxiliary.id().run():EventAuxiliary.id().luminosityBlock():EventAuxiliary.id().event():mht.obj.pt():met.obj.pt():ht.obj.sumEt()', cut, 'colsize=14')


def lumiReport( cuts=None, hist=None, opt=''):
    
    if hist==None:
        events.Draw('lumi:run','',opt)
    else:
        events.Draw('lumi:run>>'+hist.GetName(), '',opt)
    gPad.SaveAs('lumiReport.png')


# can create other trees with other set of files
tree1 = init(sys.argv[1])
events = tree1
