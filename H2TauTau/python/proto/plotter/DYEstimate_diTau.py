import imp
import inspect
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math

def embeddedScaleFactor(anaDir, selCompsNoSignal, weightsNoSignal, selCompsDataMass, weightsDataMass, weight, prong, trigMatch=True):
    if trigMatch :
      cutDY = 'abs(jet1Eta)<3.0 && jet1Pt>50. && l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && isFake==0 && isElectron==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 && (l1TrigMatched==1 && l2TrigMatched==1 && jetTrigMatched!=0)'
    else:
      cutDY = 'abs(jet1Eta)<3.0 && jet1Pt>50. && l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && isFake==0 && isElectron==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 '
    if prong :
      cutDY += ' && l1DecayMode<3  && l2DecayMode<3'
      cutDY.replace('abs(jet1Eta)<3.0 && jet1Pt>50.','')
    inclusiveForEmbeddedNormalizationDY    = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    #50,0,500,
     			    30,0,300,
     			    cut    = cutDY, 
     			    weight = 'weight/triggerWeight',
     			    embed  = False)
    
    cutEmb = 'abs(jet1Eta)<3.0 && jet1Pt>50. && l1Pt>45 && l2Pt>45 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && l2MVAEle>0.5 && l1MedMVAIso>0.5 && l2MedMVAIso>0.5 '
    if prong :
      cutEmb += ' && l1DecayMode<3  && l2DecayMode<3'
      cutEmb.replace('abs(jet1Eta)<3.0 && jet1Pt>50.','')
    inclusiveForEmbeddedNormalizationEmbed = H2TauTauDataMC('svfitMass', anaDir, selCompsNoSignal, weightsNoSignal,
     			    #50,0,500,
     			    30,0,300,
     			    cut    = cutEmb, 
     			    weight = 'weight',
     			    embed  = False)
    
    ### sum up all the embedded components
    embeddedHist = None
    for name,comp in selCompsNoSignal.items():
        if comp.isEmbed:
	     if embeddedHist == None:
	         embeddedHist = copy.deepcopy(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
             else:
	         embeddedHist.Add(inclusiveForEmbeddedNormalizationEmbed.Hist(name))
   
    print "lumi                        ", inclusiveForEmbeddedNormalizationDY.intLumi
    print "DY events in inclusive      ", inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()
    print "Embedded events in inclusive", embeddedHist.Integral()
    
    embeddedScaleFactor = inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()/embeddedHist.Integral()  ### 0.97 from Valentina
    #embeddedScaleFactor = 1/0.8*inclusiveForEmbeddedNormalizationDY.Hist("DYJets").Integral()/embeddedHist.Integral()  ### 0.97 from Valentina
    print "embeddedScaleFactor", embeddedScaleFactor

    # plotting
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
    print "Data events in boosted ee", inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").weighted.Integral()
    print "DYJets events in boosted ee", (inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").weighted.Integral()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").weighted.Integral())

    zeeScaleFactor = inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").weighted.Integral()/ \
        (inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").weighted.Integral()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").weighted.Integral())
    print "zeeScaleFactor", zeeScaleFactor, "+-", math.sqrt(pow(0.2,2)+pow(0.2,2))*zeeScaleFactor

    ymax = max(inclusiveForEmbeddedNormalizationZeeBB.Hist("Data").GetMaximum(),(inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets").GetMaximum()+inclusiveForEmbeddedNormalizationZeeBB.Hist("DYJets_Electron").GetMaximum()))*1.5
    inclusiveForEmbeddedNormalizationZeeBB.DrawStack("HIST",0,300,0,ymax)
    
    gPad.SaveAs("inclusiveForZeeNormalization.png")
    gPad.WaitPrimitive()

