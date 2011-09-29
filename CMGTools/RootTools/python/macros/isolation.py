events.SetAlias('ch','patMuonsPFlow.obj.chargedHadronIso()')
events.SetAlias('nh','patMuonsPFlow.obj.neutralHadronIso()')
events.SetAlias('ph','patMuonsPFlow.obj.photonIso()')

events.SetAlias('chpu','patMuonsPFlow.obj.puChargedHadronIso()')

events.SetAlias('comb', 'ch+nh+ph')

events.SetAlias('pt', 'patMuonsPFlow.obj.pt()')
events.SetAlias('nvert','vertices.@obj.size()')

events.SetAlias('combrel', 'comb/pt')

events.SetAlias('combrelDBeta', '(ch + max(0, nh+ph-0.5*chpu))/pt')


nEv = 9999999

# plot effect of the dbeta correction

cVsVert = TCanvas('cVsVert','Isolation vs Number of vertices')

from ROOT import TProfile

pr1 = TProfile('pr1',';N_{vertices};Isolation',10,1,10)
pr1.SetStats(0)
pr1.SetLineColor(1)
events.Draw('combrel:nvert>>pr1','','', nEv)

pr2 = TProfile('pr2','',10,1,10)
pr2.SetLineColor(4)
events.Draw('combrelDBeta:nvert>>pr2','','same',nEv)

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
