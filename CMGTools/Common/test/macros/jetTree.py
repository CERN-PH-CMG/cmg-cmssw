import os, sys
from ROOT import * 

class jetHistos:
    def __init__(self, name):
        self.respEta_ = TProfile('respEta'+name,'',20, -5, 5)
        self.respPt_ = TH2F('respPt'+name,'',100, 0, 1000)


def plotJets( tree, canvas, jeth):
    # canvas.cd(1)
    # tree.Draw('(jet.pt()-genJet.pt())/genJet.pt():genJet().eta()>>'+ jeth.respEta_.GetName(),'genJet.pt()>30')
    
    canvas.cd(2)
    tree.Draw('(jet.pt()-genJet.pt())/genJet.pt():genJet().pt()>>'+ jeth.respPt_.GetName(),'genJet.pt()>0 && abs(genJet.eta())<1.5')

def showBin( hist, bin):
    hist.ProjectionY('',bin,bin,'').Fit('gaus')

from CMGTools.RootTools.RootTools import *

gROOT.Macro( os.path.expanduser( '~/rootlogon.C' ) )

# file = TFile( sys.argv[1] )

print sys.argv[1]
events = Chain('Events', sys.argv[1])

#COLIN CMG tuple does not work in that case.
# alias setting is too naive
# from CMGTools.RootTools.cmgTuple import *

# cmg = cmgTuple( events )
# cmg.printAliases()
# PFcanvas = TCanvas('PFcanvas','PF Jets', 1000,800)
# PFcanvas.Divide(3,2)

# PFcanvas.cd(1)

# cmg.Draw('selectedPatJetsPFlow.pt()')
# cmg.Draw('selectedPatJetsPFlow.genJet().pt()')

patJets = 'patJets_selectedPatJetsPFlow__PAT'
events.SetAlias('jet', patJets)


# PF = jetHistos('PF')
# plotJets( events, canvas, PF )

from response import response

def plotPtResponse( response ): 
    
    canvas = response.canvas
    canvas.cd(1)
    response.h2d = TH2F('responsePt','Leading jet;p_{T}(gen) (GeV);p_{T}(rec)/p_{T}(gen)',50, 0, 1000, 50, 0,2)
    events.Draw('jet.obj[0].pt()/jet.obj[0].genJet().pt():jet.obj[0].genJet().pt()>>'+ response.h2d.GetName(),'jet.obj[0].genJet().pt()>0 && abs(jet.obj[0].genJet().eta())<1.5',"col")
    
    response.FitSlicesY()
    response.Draw()

    canvas.SaveAs(response.name + '.png')


def plotEtaResponse( response ): 
    
    canvas = response.canvas
    canvas.cd(1)
    response.h2d = TH2F('responseEta','Leading jet;#eta(gen);p_{T}(rec)/p_{T}(gen)',50, -5, 5, 50, 0,2)
    events.Draw('jet.obj[0].pt()/jet.obj[0].genJet().pt():jet.obj[0].genJet().eta()>>'+ response.h2d.GetName(),'jet.obj[0].genJet().pt()>0 && jet.obj[0].pt()>30',"col")
    
    response.FitSlicesY()
    response.Draw()

    canvas.SaveAs(response.name + '.png')
    

if __name__ == '__main__':

    pt = response('pt')
    plotPtResponse( pt )
    eta = response('eta')
    plotEtaResponse( eta )
