import os, sys

filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    exec(open(filename).read())


from ROOT import gROOT, TFile, TCanvas, gPad, TBrowser, TH2F
from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

#file = TFile( sys.argv[1] )
#events = file.Get('Events')

process = 'ANA'

def init(pattern, process='ANA'):
    
    events = Chain('Events', pattern)

    events.SetAlias('pfJets','cmgPFJets_cmgPFJetSel__'+process)
    events.SetAlias('pfJetsLooseFailed','cmgPFJets_cmgPFJetLooseJetIdFailed__'+process)    
    events.SetAlias('pfJetsVeryLoose99Failed','cmgPFJets_cmgPFJetVeryLooseJetId99Failed__'+process)    
    events.SetAlias('pfJetsVeryLoose95Failed','cmgPFJets_cmgPFJetVeryLooseJetId95Failed__'+process)    
    events.SetAlias('jets','cmgBaseJets_cmgPFBaseJetSel__'+process)
    events.SetAlias('met','cmgBaseMETs_cmgPFMET__'+process)
    events.SetAlias('ele','cmgElectrons_cmgElectronSel__'+process)
    events.SetAlias('mu','cmgMuons_cmgMuonSel__'+process)
    
    events.SetAlias('hbheNoiseFilter','bool_HBHENoiseFilterResultProducer_HBHENoiseFilterResult_PAT.obj')
    events.SetAlias('inconsMuons', 'bool_inconsistentMuonsTagging_Result_PAT.obj')
    events.SetAlias('greedyMuons', 'bool_greedyMuonsTagging_Result_PAT.obj')
    
    events.SetAlias('run','EventAuxiliary.id().run()')
    events.SetAlias('lumi','EventAuxiliary.id().luminosityBlock()')

    return events

filters = 'hbheNoiseFilter && greedyMuons && inconsMuons'
NOTfilters = '!(%s)' % filters

jetId = 'pfJets.obj.component(5).fraction()<0.99 && pfJets.obj.component(4).fraction()<0.99'
NOTjetId = '!(%s)' % jetId

jetIdTight = 'pfJets.obj.component(5).fraction()<0.95 && pfJets.obj.component(4).fraction()<0.95'
NOTjetIdTight = '!(%s)' % jetIdTight




fullCleaning = filters + ' && ' + jetId

leptonVeto = 'mu.@obj.size()==0 && ele.@obj.size()==0'
leptonVeto2 = '(mu.@obj.size()==0 || mu.obj.relIso()<0.1) && (ele.@obj.size()==0 || ele.obj.relIso()<0.1)'


def setEventList( cut=None ):
    print 'now browsing the full tree... might take a while, but drawing will then be faster!'
    events.Draw('>>pyplus', cut)
    from ROOT import gDirectory
    pyplus = gDirectory.Get('pyplus')
    pyplus.SetReapplyCut(True)
    events.SetEventList(pyplus)

    
def scan( cut=None ):
    out = events.Scan('EventAuxiliary.id().run():EventAuxiliary.id().luminosityBlock():EventAuxiliary.id().event():met.obj.pt()', cut, 'colsize=14')


def lumiReport( cuts=None, hist=None, opt=''):
    
    if hist==None:
        events.Draw('lumi:run','',opt)
    else:
        events.Draw('lumi:run>>'+hist.GetName(), '',opt)
    gPad.SaveAs('lumiReport.png')


# can create other trees with other set of files
tree1 = init(sys.argv[1])
events = tree1
