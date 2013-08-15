from CMGTools.RootTools.HistogramComparison import * 

nEvents = 500000

jet1 = 'jets'
jet2 = 'jetsPBNR_G95'

id = idG95Failed

etahist = HistogramComparison('eta',';#eta (GeV)',50,-5,5)
events.Draw(jet1+'.obj.eta()>>'+etahist.h1.GetName(), jet1+'.obj.pt()>30', 'goff', nEvents)
events.Draw(jet2+'.obj.eta()>>'+etahist.h2.GetName(), jet2+'.obj.pt()>30', 'goff', nEvents)
etahist.setUpLegend( 'all jets', 'failing jets', 0.53,0.80,0.77,0.99)
etahist.draw()

pthist = HistogramComparison('pt',';p_{T} (GeV)',100,0,500)
events.Draw(jet1+'.obj.pt()>>'+pthist.h1.GetName(), jet1+'.obj.pt()>30', 'goff', nEvents)
events.Draw(jet2+'.obj.pt()>>'+pthist.h2.GetName(), jet2+'.obj.pt()>30', 'goff', nEvents)
pthist.setUpLegend( 'all jets', 'failing jets', 0.63,0.63,0.89,0.82)
pthist.draw()

hthist = HistogramComparison('ht',';HT (GeV)',100,0,1000)
events.Draw('ht.obj.sumEt()>>'+hthist.h1.GetName(),'', 'goff', nEvents)
events.Draw('ht.obj.sumEt()>>'+hthist.h2.GetName(), id, 'goff', nEvents)
hthist.setUpLegend( 'all events', 'failing' , 0.63,0.63,0.89,0.82)
hthist.draw()

# njetshist = HistogramComparison('njets',';N jets',20,0,20)
# events.Draw('jets.@obj.size()>>'+njetshist.h1.GetName(),'', 'goff', nEvents)
# events.Draw('jets.@obj.size()>>'+njetshist.h2.GetName(), id, 'goff', nEvents)
# njetshist.setUpLegend( 'all events', 'failing' )
# njetshist.draw()

