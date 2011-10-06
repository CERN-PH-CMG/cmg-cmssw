# this macro reads pat muons.

# mus = 'patMuons_selectedPatMuonsAK5LC__PAT'
# mus = 'patMuons_selectedPatMuonsAK5__PAT'

def setAliases( tree, mus, dBetaFactor = 0.5): 
    tree.SetAlias('ch',mus + '.obj[0].chargedHadronIso()')
    tree.SetAlias('nh',mus + '.obj[0].neutralHadronIso()')
    tree.SetAlias('ph',mus + '.obj[0].photonIso()')
    
    tree.SetAlias('chpu',mus + '.obj[0].puChargedHadronIso()')
    
    tree.SetAlias('comb', 'ch+nh+ph')
    
    tree.SetAlias('pt', mus + '.obj[0].pt()')
    tree.SetAlias('nvert','vertices.@obj.size()')
    
    tree.SetAlias('combrel', 'comb/pt')
    
    tree.SetAlias('combrelDBeta', '(ch + max(0, nh+ph-%f*chpu))/pt' % dBetaFactor)
    


from CMGTools.RootTools.Style import *

# plot effect of the dbeta correction

class MyHistograms:
    def __init__(self, name):
        self.name = name
        pass
    def formatHistos(self, style):
        style.formatHisto( self.h1_NVert )
        style.formatHisto( self.h1_NVert_Iso )
        style.formatHisto( self.eff_NVert )
    def effVsNVert(self, cut, addCut, nEvents):
        self.h1_NVert = TH1F('h1_NVert'+self.name,'',20,0,20)
        self.h1_NVert_Iso = self.h1_NVert.Clone('h1_NVert_Iso'+self.name)
        print 'n_tot...'
        events.Draw('nvert>>h1_NVert'+self.name, addCut,'goff', nEvents)
        print 'n_sel...'
        events.Draw('nvert>>h1_NVert_Iso'+self.name, addCut + '&&' + cut,'goff', nEvents)
        self.eff_NVert = self.h1_NVert_Iso.Clone('eff_NVert'+self.name)
        self.eff_NVert.SetTitle(';#Vertices;Isolation Efficiency')
        self.eff_NVert.SetStats(0)
        self.eff_NVert.Sumw2()
        self.h1_NVert.Sumw2()   
        self.eff_NVert.Divide( self.h1_NVert_Iso, self.h1_NVert, 1, 1, 'B')
        print self.h1_NVert.GetEntries()
        

dBetaFactor = 0.5
setAliases( events, 'mus', dBetaFactor)

cVsVert = TCanvas('cVsVert','Isolation vs Number of vertices')

nEv = 999999

zCut =  '   diMus.@obj.size()==1'
zCut += '&& diMus.obj[0].mass()>70 && diMus.obj[0].mass()<115'
zCut += '&& diMus.obj[0].leg1().charge()*diMus.obj[0].leg2().charge()<0'

tagCut1 = 'diMus.obj[0].leg1().pt()>20'
tagCut1 += '&& diMus.obj[0].leg1().relIso()<0.2'
# tagCut1 += '&& diMus.obj[0].leg1().getSelection("cuts_vbtfmuon_dxy")<0.2'
tagCut1 += '&& diMus.obj[0].leg1().getSelection("cuts_vbtfmuon")'

tagCut = zCut + ' && ' + tagCut1

isoCut = '0.1'
probeCut = 'diMus.obj[0].leg2().relIso()<'+isoCut 

histos = MyHistograms('combrel')
histos.effVsNVert( probeCut, tagCut, nEv)
histos.formatHistos( sRedPoints )

probeCut = 'diMus.obj[0].leg2().relIso(0.5)<'+isoCut 

histosDBeta = MyHistograms('combrelDBeta')
histosDBeta.effVsNVert( probeCut, tagCut, nEv)
histosDBeta.formatHistos( sBlueSquares )

histos.eff_NVert.GetYaxis().SetRangeUser(0.3,1)
histos.eff_NVert.Draw()
histosDBeta.eff_NVert.Draw('same')

legend = TLegend(0.12,0.12,0.63,0.33, 'iso < %s; #Delta#beta fact=%2.2f' % (isoCut,dBetaFactor) )
legend.AddEntry(histos.eff_NVert, 'No #Delta#beta correction')
legend.AddEntry(histosDBeta.eff_NVert, '#Delta#beta correction')
legend.Draw()

gPad.SaveAs('dbetaCorEffect_%s_%2.2f.png' % (isoCut, dBetaFactor))




from ROOT import TProfile

pr1 = TProfile('pr1',';N_{vertices};Isolation',10,1,10)
pr1.SetStats(0)
pr1.SetLineColor(1)
events.Draw('combrel:nvert>>pr1','combrel<0.15','', nEv)

pr2 = TProfile('pr2','',10,1,10)
pr2.SetLineColor(4)
events.Draw('combrelDBeta:nvert>>pr2','combrel<0.15','same',nEv)

pr3 = TProfile('pr3','',10,1,10)
events.Draw('combrelDBeta1:nvert>>pr3','combrel<0.15','same',nEv)

pr4 = TProfile('pr4','',10,1,10)
events.Draw('combrelDBeta2:nvert>>pr4','combrel<0.15','same',nEv)


legend = TLegend(0.12,0.12,0.4,0.3)
legend.AddEntry( 'pr1', 'no dbeta')
legend.AddEntry( 'pr2', 'dbeta')
legend.Draw()

# plot isolation values for the various components:

cComp = TCanvas('cComp','Isolation Components')
cComp.SetLogy()

hIsoCh = TH1F('hIsoCh',';Isolation',100,0,1)
hIsoCh.SetStats(0)
hIsoNh = hIsoCh.Clone('hIsoNh')
hIsoNh.SetLineColor(2)
hIsoPh = hIsoCh.Clone('hIsoPh')
hIsoPh.SetLineColor(4)

events.Draw('ch/pt>>hIsoCh','','goff',nEv)
events.Draw('nh/pt>>hIsoNh','','goff',nEv)
events.Draw('ph/pt>>hIsoPh','','goff',nEv)

hIsoNh.Draw()
hIsoCh.Draw('same')
hIsoPh.Draw('same')

legend = TLegend(0.52,0.62,0.88,0.8)
legend.AddEntry( 'hIsoCh', 'charged hadrons')
legend.AddEntry( 'hIsoNh', 'neutral hadrons')
legend.AddEntry( 'hIsoPh', 'photons')

legend.Draw()

# plot combined and dbeta cor combined:

cCombine = TCanvas('cCombine','Combined Isolation')
cCombine.SetLogy()

hIsoComb = hIsoCh.Clone('hIsoComb')
hIsoComb.SetLineColor(1)
hIsoCombDBeta = hIsoCh.Clone('hIsoCombDBeta')
hIsoCombDBeta.SetLineColor(4)

events.Draw('combrel>>hIsoComb','','goff',nEv)
events.Draw('combrelDBeta>>hIsoCombDBeta','','goff',nEv)

hIsoComb.Draw()
hIsoCombDBeta.Draw('same')

legend = TLegend(0.52,0.62,0.88,0.8)
legend.AddEntry( 'hIsoComb', 'no delta beta')
legend.AddEntry( 'hIsoCombDBeta', 'delta beta')
legend.Draw()
