# just copy/paste the lines you want to your python prompt
# after having executed cmgBasicInit.py

from ROOT import gDirectory, TLegend,gPad, TCanvas
nEvents = 999999999999

plotFilters = False
met = 'mht'

# MET plot, effect of jetID and cleaning filters

addCut = ' && ht.obj.sumEt()>350'
# addCut = ' && met.obj.et()<500'
# addCut = ''

events.Draw(met + '.obj.et()', '1' + addCut, '', nEvents)

h1 = events.GetHistogram()

if met == 'met':
    h1.SetTitle(';MET (GeV)')
elif met == 'mht':
    h1.SetTitle(';MHT (GeV)')
else:
    pass

h2 = h1.Clone('h2')
h2.Reset()
h3 = h1.Clone('h3')
h3.Reset()
    
events.Project('h2', met + '.obj.et()',NOTjetIdTight + addCut, '', nEvents)

if plotFilters:
    events.Project('h3', met + '.obj.et()',NOTfilters + addCut, '', nEvents)

h1.SetLineWidth(2)
h2.SetFillStyle(3002)
h2.SetFillColor(5)
h3.SetFillStyle(3002)
h3.SetFillColor(4)

gPad.SetLogy()

legend = TLegend(0.5,0.5,0.85,0.8)
legend.AddEntry(h1,'PFMET>150')
legend.AddEntry(h2,'rejected by Jet ID')

h1.Draw()
h2.Draw('same')

if plotFilters:
    legend.AddEntry(h3,'rejected by cleaning filters')
    h3.Draw('same')

legend.Draw()


# jet plots

events.Draw('jets.obj.pt()','jets.obj.pt()<200')


# jet component plots

jetCompCan = TCanvas('jetCompCan','jet components', 800, 1000)
jetCompCan.Divide(1,2)
jetCompCan.cd(1)
events.Draw('pfjets.obj.component(4).fraction()')
events.GetHistogram().SetTitle(';photon fraction')
jetCompCan.cd(2)
events.Draw('pfjets.obj.component(5).fraction()')
events.GetHistogram().SetTitle(';neutral hadron fraction')
