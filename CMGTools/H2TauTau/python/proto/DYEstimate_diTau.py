import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math

def embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight):
    inclusiveForEmbeddedNormalizationDY = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    30,0,300,
     			    cut = 'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && isFake==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5', weight=weight,
     			    embed=False)
    inclusiveForEmbeddedNormalizationEmbed = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    30,0,300,
     			    cut = 'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5', weight=weight,
     			    embed=False)
    embeddedHist=None
    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     if embeddedHist==None:
	         embeddedHist=copy.deepcopy(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
             else:
	         embeddedHist.Add(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
    ymax = max(inclusiveForEmbeddedNormalizationDY.Hist("DYJets").GetMaximum(),embeddedHist.GetMaximum())
   
    inclusiveForEmbeddedNormalizationDY.Hist("DYJets").weighted.Draw("HISTe")
    inclusiveForEmbeddedNormalizationDY.Hist("DYJets").weighted.GetYaxis().SetRangeUser(0,ymax*1.5)
    embeddedHist.weighted.Draw("HISTeSAME")

    print "DY events in inclusive", inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()
    print "Embedded events in inclusive", embeddedHist.Integral()
    
    gPad.SaveAs("inclusiveForEmbeddedNormalization.png")
    gPad.WaitPrimitive()
   
    embeddedScaleFactor = inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()/embeddedHist.Integral()
    print "embeddedScaleFactor", embeddedScaleFactor

    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     comp.embedFactor = embeddedScaleFactor
    for mass,comps in selCompsDataMass.items():
      for name,comp in comps.items():
        if comp.isEmbed:
	     comp.embedFactor = embeddedScaleFactor

def zeeScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, embed):
    inclusiveForEmbeddedNormalizationEE = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    30,0,300,
     			    cut = 'l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 && l1MVAEle<0.5 && l2MVAEle<0.5 && jet1Pt>30', weight=weight,
     			    embed=embed)
    inclusiveForEmbeddedNormalizationEE.Hist("DYJets").Scale(0.87*0.87)
    inclusiveForEmbeddedNormalizationEE.Hist("DYJets_Electron").Scale(0.87*0.87)

    ymax = max(inclusiveForEmbeddedNormalizationEE.Hist("Data").GetMaximum(),(inclusiveForEmbeddedNormalizationEE.Hist("DYJets").GetMaximum()+inclusiveForEmbeddedNormalizationEE.Hist("DYJets_Electron").GetMaximum()))*1.5
    inclusiveForEmbeddedNormalizationEE.DrawStack("HIST",0,300,0,ymax)
    
    print "Data events in boosted ee", inclusiveForEmbeddedNormalizationEE.Hist("Data").Integral()
    print "DYJets events in boosted ee", (inclusiveForEmbeddedNormalizationEE.Hist("DYJets").Integral()+inclusiveForEmbeddedNormalizationEE.Hist("DYJets_Electron").Integral())

    gPad.SaveAs("inclusiveForZeeNormalization.png")
    gPad.WaitPrimitive()

    zeeScaleFactor = inclusiveForEmbeddedNormalizationEE.Hist("Data").Integral()/ \
        (inclusiveForEmbeddedNormalizationEE.Hist("DYJets").Integral()+inclusiveForEmbeddedNormalizationEE.Hist("DYJets_Electron").Integral())
    print "zeeScaleFactor", zeeScaleFactor

    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     comp.embedFactor *= zeeScaleFactor
    for mass,comps in selCompsDataMass.items():
      for name,comp in comps.items():
        if comp.isEmbed:
	     comp.embedFactor *= zeeScaleFactor

