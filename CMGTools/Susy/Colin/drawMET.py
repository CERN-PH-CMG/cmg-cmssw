# can draw MET or MHT
# check the effect of jet ID and of the cleaning filters
#
# just copy/paste to your python prompt all lines below, or part of them.

# from ROOT import gDirectory, TLegend,gPad, TCanvas

from CMGTools.RootTools.HistogramComparison import * 

nEvents = 1000000

met = 'mht'

title = ';MET (GeV)'
if met == 'mht':
    title = ';MHT (GeV)'


plotJetId = True

# sel = idG95Failed
# sel = idNH90G95Failed
sel = idNH95Failed

filters = '!(hbheNoise && inconsMuons && greedyMuons)'

# MET plot, effect of jetID and cleaning filters

# addCut = ' && met.obj[0].et()<500'
# addCut = ' && ht.obj.sumEt()>350 && beamHaloCSCTight==0'
# addCut = ' && ht.obj[0].sumEt()>350'
addCut = ''

histos = HistogramComparison('histos', title = title, bins = 100, max = 2000)

events.Draw(met + '.obj.et()>>'+histos.h1.GetName(), '1' + addCut, 'goff', nEvents)

events.Draw(met + '.obj.et()>>'+histos.h2.GetName(), sel + addCut, 'goff', nEvents)
if sel == id99Failed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','loose jet ID failed')
elif sel == id95Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','tight jet ID failed')
elif sel == id95_30_Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','tight jet ID failed (pT>30)')
elif sel == id95_50_Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','tight jet ID failed (pT>50)')
elif sel == idNH80Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','PBNR Failed (pT>30, fnh<0.80)')
elif sel == idNH90Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','PBNR Failed (pT>30, fnh<0.90)')
elif sel == idNH95Failed:
    # histos.h2.SetFillColor(2)
    histos.setUpLegend('All events','PBNR Failed (pT>30, fnh<0.95)')
elif sel == idG95Failed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','PBNR Failed (pT>30, fgam<0.95)')
elif sel == idNH90G95Failed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','PBNR Failed')
elif sel == beamHaloCSCTightFailed:
    # histos.h2.SetFillColor(5)
    histos.setUpLegend('All events','CSCTightHaloId failed')    
elif sel == beamHaloCSCLooseFailed:
    # histos.h2.SetFillColor(8)
    histos.setUpLegend('All events','CSCLooseHaloId failed')
elif sel == hbheNoise2010Failed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','HBHENoise 2010 failed')
elif sel == hbheNoise2011IsoFailed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','HBHENoise 2011 iso failed')
elif sel == hbheNoise2011NonIsoFailed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','HBHENoise 2011 non iso failed')
elif sel == ecalDeadTPFailed:
    # histos.h2.SetFillColor(4)
    histos.setUpLegend('All events','TP filter failed')
else:
    print 'What are you doing?'

# histos.computeEff()
histos.draw()
