import os, sys
from ROOT import * 

class jetHistos:
    def __init__(self, name):
        self.eta_ = TH1F('eta'+name,'',100, -5, 5)
        self.pt_ = TH1F('pt'+name,'',100, 0, 1000)
        self.canvas_ = TCanvas(name,name,1000,1000 )
        self.canvas_.Divide(2,2)
        self.canvas_.cd(1).SetLogy()

def showBin( hist, bin):
    hist.ProjectionY('',bin,bin,'').Fit('gaus')

from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )


def setAliases():
    print 'setting aliases'
    jets = 'cmgBaseJets_cmgPFJetSel__ANA'
    events.SetAlias('jet', jets)

rootfile  = sys.argv[1]

events = Chain('Events', rootfile)
setAliases()


def plotJets(jetHs):
    jetHs.canvas_.cd(1)
    events.Draw('jet.obj.pt()>>' + jetHs.pt_.GetName() )
    jetHs.canvas_.cd(2)
    events.Draw('jet.obj.eta()>>' + jetHs.eta_.GetName() )
    

if __name__ == '__main__':

    pass
    
    # plotCor()

