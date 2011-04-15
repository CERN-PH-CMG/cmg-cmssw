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


rootfile  = sys.argv[1]

events = Chain('Events', rootfile)

#COLIN CMG tuple does not work in that case.
# alias setting is too naive
# from CMGTools.RootTools.cmgTuple import *

# cmg = cmgTuple( events )
# cmg.printAliases()
# PFcanvas = TCanvas('PFcanvas','PF Jets', 1000,800)
# PFcanvas.Divide(3,2)

# PFcanvas.cd(1)

raw = False 
genJetPtCut = 20
dRCut = True

basename = os.path.basename( rootfile )

def buildPrefix():
    pref = os.path.splitext( basename)[0]
    pref += '_' + str(genJetPtCut)
    if raw:
        pref += '_raw'
    if dRCut:
        pref += '_dR'
    return pref

patJets = 'patJets_patJetLead__ANA'
events.SetAlias('jet', patJets)

deltaPhi = '%s.obj.phi()-%s.obj.genJet().phi()' % (patJets, patJets)
events.SetAlias('dPhi',deltaPhi )
deltaEta = '%s.obj.eta()-%s.obj.genJet().eta()' % (patJets, patJets)
events.SetAlias('dEta',deltaEta )
deltaR = 'sqrt( dPhi*dPhi + dEta*dEta)'
events.SetAlias('dR',deltaR )

genJet = '%s.obj.genJet()' % patJets
events.SetAlias('genJet',genJet )


# PF = jetHistos('PF')
# plotJets( events, canvas, PF )

from response import response

def plotPtResponse( response ): 
    
    prefix = buildPrefix()

    print 'plotPtResponse : ', prefix, '...'

    canvas = response.canvas
    canvas.cd(1)
    response.h2d = TH2F('responsePt_' + response.name,';p_{T}(gen) (GeV);p_{T}(rec)/p_{T}(gen)',50, 0, 1000, 50, 0,2)
    # events.Draw('jet.obj[0].pt()/jet.obj[0].genJet().pt():jet.obj[0].genJet().pt()>>'+ response.h2d.GetName(),'jet.obj[0].genJet().pt()>0 && abs(jet.obj[0].genJet().eta())<1.5',"col")

    var = 'jet.obj.pt()/jet.obj.genJet().pt():jet.obj.genJet().pt()>>' + response.h2d.GetName()
    if raw:
        var = 'jet.obj.pt()*jet.obj.jecFactor(0)/jet.obj.genJet().pt():jet.obj.genJet().pt()>>' + response.h2d.GetName()
    cut = 'jet.obj.genJet().pt()>%s && abs(jet.obj.genJet().eta())<1.5' % genJetPtCut
    if dRCut:
        cut +=  '&& dR<0.2'
     
    events.Draw(var,cut,"col")
    
    response.FitSlicesY()
    response.Draw()
    
    canvas.SaveAs(prefix + '_' + response.name + '.png')


def plotEtaResponse( response ): 

    prefix = buildPrefix()

    print 'plotEtaResponse : ', prefix, '...'
    
    canvas = response.canvas
    canvas.cd(1)
    response.h2d = TH2F('responseEta_'+response.name,';#eta(gen);p_{T}(rec)/p_{T}(gen)',50, -5, 5, 50, 0,2)

    var = 'jet.obj.pt()/jet.obj.genJet().pt():jet.obj.genJet().eta()>>'+ response.h2d.GetName()
    cut = 'jet.obj.genJet().pt()>%s && dR<0.2' % genJetPtCut
    
    # events.Draw('jet.obj[0].pt()/jet.obj[0].genJet().pt():jet.obj[0].genJet().eta()>>'+ response.h2d.GetName(),'jet.obj[0].genJet().pt()>0 && jet.obj[0].pt()>30',"col")
    events.Draw(var, cut ,"col")
    
    response.FitSlicesY()
    response.Draw()
    
    canvas.SaveAs( prefix + '_' + response.name + '.png')
    

if __name__ == '__main__':

    dRCut = True
    drpt = response('drpt')
    plotPtResponse( drpt )

    dRCut = False
    nodrpt = response('nodrpt')
    plotPtResponse( nodrpt )
    
    # eta = response('eta')
    # plotEtaResponse( eta )
