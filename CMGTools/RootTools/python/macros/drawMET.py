# can draw MET or MHT
# check the effect of jet ID and of the cleaning filters
#
# just copy/paste to your python prompt all lines below, or part of them.

from ROOT import gDirectory, TLegend,gPad, TCanvas
nEvents = 999999999999

met = 'met'

plotJetId = True
plotFilters = True

notId99 = 'jetsVLId99Failed.@obj.size()>0'
notId95 = 'jetsVLId99Failed.@obj.size()>0'

jId = notId99

filters = '!(hbheNoise && inconsMuons && greedyMuons)'

# MET plot, effect of jetID and cleaning filters

# addCut = ' && met.obj[0].et()<500'
addCut = ''

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

h1.SetLineWidth(2)

gPad.SetLogy()

legend = TLegend(0.5,0.5,0.85,0.8)

h1.Draw()

if plotJetId:
    events.Project('h2', met + '.obj.et()', jId + addCut, '', nEvents)
    if jId == notId99:
        h2.SetFillColor(2)
        legend.AddEntry(h2,'jetsVLId99Failed')
    elif jId == notId95:
        h2.SetFillColor(2)
        legend.AddEntry(h2,'jetsVLId95Failed')        
    else:
        print 'What are you doing?'
    h2.Draw('same')

if plotFilters:
    h3.SetFillStyle(3002)
    h3.SetFillColor(5)
    events.Project('h3', met + '.obj.et()', filters + addCut, '', nEvents)
    legend.AddEntry(h3, 'rejected by cleaning filters')
    h3.Draw('same')

legend.Draw()

gPad.SaveAs('met.png')
