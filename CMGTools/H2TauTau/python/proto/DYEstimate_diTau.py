import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math

def embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight):
    inclusiveForEmbeddedNormalizationDY = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    50,0,500,
     			    cut = 'l1Pt>20 && l2Pt>20 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && isFake==0 && isElectron==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5', weight='weight*((triggerWeight<=0)+(triggerWeight>0)/triggerWeight)',
     			    embed=False)
    inclusiveForEmbeddedNormalizationEmbed = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    50,0,500,
     			    cut = 'l1Pt>20 && l2Pt>20 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5', weight='embedWeight*vertexWeight',
     			    embed=False)
    embeddedHist=None
    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     if embeddedHist==None:
	         embeddedHist=copy.deepcopy(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
             else:
	         embeddedHist.Add(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
   
    print "lumi", inclusiveForEmbeddedNormalizationDY.intLumi
    print "DY events in inclusive", inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()
    print "Embedded events in inclusive", embeddedHist.Integral()
    
    embeddedScaleFactor = inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()/embeddedHist.Integral()
    print "embeddedScaleFactor", embeddedScaleFactor


    # applying scale factor from Jose MuTau (...and don't work for us)
    #embeddedScaleFactor = 0.356715 / 0.95 * 5.1 / 3.95
    embeddedHist.Scale(embeddedScaleFactor)

    ymax = max(inclusiveForEmbeddedNormalizationDY.Hist("DYJets").GetMaximum(),embeddedHist.GetMaximum())
    inclusiveForEmbeddedNormalizationDY.Hist("DYJets").weighted.Draw("HISTe")
    inclusiveForEmbeddedNormalizationDY.Hist("DYJets").weighted.GetYaxis().SetRangeUser(0,ymax*1.5)
    embeddedHist.weighted.Draw("HISTeSAME")

    gPad.SaveAs("inclusiveForEmbeddedNormalization.png")
    gPad.WaitPrimitive()
   
    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     comp.embedFactor = embeddedScaleFactor
    for mass,comps in selCompsDataMass.items():
      for name,comp in comps.items():
        if comp.isEmbed:
	     comp.embedFactor = embeddedScaleFactor

def zeeScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, embed):
    # Data/MC scale factors for e->tau fake rate from 2012 ICHEP Object approval presentation: 0.85 for Barrel, 0.65 for Endcap
    inclusiveForEmbeddedNormalizationZeeBB = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    30,70,160,
     			    cut = 'abs(l1Eta)<1.5 && abs(l2Eta)<1.5 && l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 && l1MVAEle<0.5 && l2MVAEle<0.5 && jet1Pt>50', weight=weight,
     			    embed=embed)
    #inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").Scale(1./(0.85*0.85))
    #inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").Scale(1./(0.85*0.85))
    #inclusiveForEmbeddedNormalizationZeeBE = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
    # 			    30,0,300,
    # 			    cut = '((abs(l1Eta)>1.5 && abs(l2Eta)<1.5) || (abs(l1Eta)<1.5 && abs(l2Eta)>1.5)) && l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 && l1MVAEle<0.5 && l2MVAEle<0.5 && jet1Pt>30', weight=weight,
    # 			    embed=embed)
    #inclusiveForEmbeddedNormalizationZeeBE.Hist("DYJets").Scale(0.85*0.65)
    #inclusiveForEmbeddedNormalizationZeeBE.Hist("DYJets_Electron").Scale(0.85*0.65)
    #inclusiveForEmbeddedNormalizationZeeEE = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
    # 			    30,0,300,
    # 			    cut = 'abs(l1Eta)>1.5 && abs(l2Eta)>1.5 && l1Pt>35 && l2Pt>35 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 && l1MVAEle<0.5 && l2MVAEle<0.5 && jet1Pt>30', weight=weight,
    # 			    embed=embed)
    #inclusiveForEmbeddedNormalizationZeeEE.Hist("DYJets").Scale(0.65*0.65)
    #inclusiveForEmbeddedNormalizationZeeEE.Hist("DYJets_Electron").Scale(0.65*0.65)

    print "Data events in boosted ee", inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").weighted.Integral()
    print "DYJets events in boosted ee", (inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").weighted.Integral()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").weighted.Integral())

    zeeScaleFactor = inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").weighted.Integral()/ \
        (inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").weighted.Integral()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").weighted.Integral())
    print "zeeScaleFactor", zeeScaleFactor, "+-", math.sqrt(pow(0.2,2)+pow(0.2,2))*zeeScaleFactor

    #inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").weighted.Scale(zeeScaleFactor)
    #inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").weighted.Scale(zeeScaleFactor)
    #inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Fakes").weighted.Scale(zeeScaleFactor)

    ymax = max(inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").GetMaximum(),(inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").GetMaximum()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").GetMaximum()))*1.5
    inclusiveForEmbeddedNormalizationZeeBB.DrawStack("HIST",0,300,0,ymax)
    
    gPad.SaveAs("inclusiveForZeeNormalization.png")
    gPad.WaitPrimitive()

    #for name,comp in selCompsNoSignal.items():
    #    if comp.isEmbed:
	#     comp.embedFactor *= zeeScaleFactor
    #for mass,comps in selCompsDataMass.items():
    #  for name,comp in comps.items():
    #    if comp.isEmbed:
	#     comp.embedFactor *= zeeScaleFactor

