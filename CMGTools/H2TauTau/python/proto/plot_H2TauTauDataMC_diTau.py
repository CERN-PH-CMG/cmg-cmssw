import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import H2TauTauDataMC
from CMGTools.RootTools.Style import formatPad,Style
from ROOT import kPink, kOrange, kViolet
import math

sOrange = Style(lineColor=1, markerColor=kOrange, fillColor=kOrange)
sViolet = Style(lineColor=1, markerColor=kViolet, fillColor=kViolet)

sRedLine = Style(lineColor=2, markerColor=2, fillStyle=0)
sBlueLine = Style(lineColor=4, markerColor=4, fillStyle=0)

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    return selComps, weights
    
    
def QCDEstimate(plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                       plotVarDataLowMETOS, plotVarDataLowMETSS, plotVarDataMiddleMETOS, plotVarDataMiddleMETSS, plotVarDataHighMETOS, plotVarDataHighMETSS,
                                       plotVarDataLowMETLooseIsoOS, plotVarDataLowMETLooseIsoSS, plotVarDataMiddleMETLooseIsoOS, plotVarDataMiddleMETLooseIsoSS, plotVarDataHighMETLooseIsoOS, plotVarDataHighMETLooseIsoSS,
				       log):
      ymax=plotVarDataSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_qcdTightSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLooseIsoOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoOS.varName+"_qcdLooseOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoSS.varName+"_qcdLooseSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLowMETOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLowMETOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLowMETOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLowMETOS.varName+"_qcdLooseLowMETOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLowMETSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLowMETSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLowMETSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLowMETSS.varName+"_qcdLooseLowMETSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataMiddleMETOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataMiddleMETOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataMiddleMETOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataMiddleMETOS.varName+"_qcdLooseMiddleMETOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataMiddleMETSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataMiddleMETSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataMiddleMETSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataMiddleMETSS.varName+"_qcdLooseMiddleMETSS.png")
      #gPad.WaitPrimitive()
      
      QCDtightSS=copy.deepcopy(plotVarDataSS.Hist("Data"))
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Fakes"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Photon"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Electron"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets_Fakes"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("TTJets"),-1)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("WJets"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("TTJets"),-1)

      QCDlooseOS=copy.deepcopy(plotVarDataLooseIsoOS.Hist("Data"))
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("WJets"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseLowMETOS=copy.deepcopy(plotVarDataLowMETOS.Hist("Data"))
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("DYJets"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("DYJets_Photon"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("DYJets_Electron"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("WJets"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("WJets_Fakes"),-1)
      QCDlooseLowMETOS.Add(plotVarDataLowMETOS.Hist("TTJets"),-1)

      QCDlooseLowMETSS=copy.deepcopy(plotVarDataLowMETSS.Hist("Data"))
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("DYJets"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("DYJets_Photon"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("DYJets_Electron"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("WJets"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("WJets_Fakes"),-1)
      QCDlooseLowMETSS.Add(plotVarDataLowMETSS.Hist("TTJets"),-1)

      QCDlooseMiddleMETOS=copy.deepcopy(plotVarDataMiddleMETOS.Hist("Data"))
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("DYJets"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("DYJets_Fakes"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("DYJets_Photon"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("DYJets_Electron"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("WJets"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("WJets_Fakes"),-1)
      QCDlooseMiddleMETOS.Add(plotVarDataMiddleMETOS.Hist("TTJets"),-1)

      QCDlooseMiddleMETSS=copy.deepcopy(plotVarDataMiddleMETSS.Hist("Data"))
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("DYJets"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("DYJets_Fakes"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("DYJets_Photon"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("DYJets_Electron"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("WJets"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("WJets_Fakes"),-1)
      QCDlooseMiddleMETSS.Add(plotVarDataMiddleMETSS.Hist("TTJets"),-1)

      QCDlooseHighMETOS=copy.deepcopy(plotVarDataHighMETOS.Hist("Data"))
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("DYJets"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("DYJets_Photon"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("DYJets_Electron"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("WJets"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("WJets_Fakes"),-1)
      QCDlooseHighMETOS.Add(plotVarDataHighMETOS.Hist("TTJets"),-1)

      QCDlooseHighMETSS=copy.deepcopy(plotVarDataHighMETSS.Hist("Data"))
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("DYJets"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("DYJets_Photon"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("DYJets_Electron"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("WJets"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("WJets_Fakes"),-1)
      QCDlooseHighMETSS.Add(plotVarDataHighMETSS.Hist("TTJets"),-1)

      QCDlooseLowMETLooseIsoOS=copy.deepcopy(plotVarDataLowMETLooseIsoOS.Hist("Data"))
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("WJets"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseLowMETLooseIsoOS.Add(plotVarDataLowMETLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseLowMETLooseIsoSS=copy.deepcopy(plotVarDataLowMETLooseIsoSS.Hist("Data"))
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("WJets"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseLowMETLooseIsoSS.Add(plotVarDataLowMETLooseIsoSS.Hist("TTJets"),-1)

      QCDlooseMiddleMETLooseIsoOS=copy.deepcopy(plotVarDataMiddleMETLooseIsoOS.Hist("Data"))
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("WJets"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseMiddleMETLooseIsoOS.Add(plotVarDataMiddleMETLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseMiddleMETLooseIsoSS=copy.deepcopy(plotVarDataMiddleMETLooseIsoSS.Hist("Data"))
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("WJets"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseMiddleMETLooseIsoSS.Add(plotVarDataMiddleMETLooseIsoSS.Hist("TTJets"),-1)

      QCDlooseHighMETLooseIsoOS=copy.deepcopy(plotVarDataHighMETLooseIsoOS.Hist("Data"))
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("WJets"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseHighMETLooseIsoOS.Add(plotVarDataHighMETLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseHighMETLooseIsoSS=copy.deepcopy(plotVarDataHighMETLooseIsoSS.Hist("Data"))
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("WJets"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseHighMETLooseIsoSS.Add(plotVarDataHighMETLooseIsoSS.Hist("TTJets"),-1)

      #print "QCDlooseSS:", QCDlooseSS.Integral()
      #print "QCDtightSS:", QCDtightSS.Integral()
      #tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      #tightLooseErr=tightLoose*math.sqrt(1./QCDtightSS.Integral() + 1./QCDlooseSS.Integral())
      #print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      #print "QCDlooseLowMETOS:", QCDlooseLowMETOS.Integral()
      #print "QCDlooseLowMETSS:", QCDlooseLowMETSS.Integral()
      #osSSl=QCDlooseLowMETOS.Integral()/QCDlooseLowMETSS.Integral()
      #osSSlErr=osSSl*math.sqrt(1./QCDlooseLowMETOS.Integral() + 1./QCDlooseLowMETSS.Integral())
      #print "QCDlooseLowMETOS / QCDlooseLowMETSS",osSSl, "+-", osSSlErr
      
      #print "QCDlooseMiddleMETOS:", QCDlooseMiddleMETOS.Integral()
      #print "QCDlooseMiddleMETSS:", QCDlooseMiddleMETSS.Integral()
      #osSSm=QCDlooseMiddleMETOS.Integral()/QCDlooseMiddleMETSS.Integral()
      #osSSmErr=osSSm*math.sqrt(1./QCDlooseMiddleMETOS.Integral() + 1./QCDlooseMiddleMETSS.Integral())
      #print "QCDlooseMiddleMETOS / QCDlooseMiddleMETSS",osSSm, "+-", osSSmErr
      
      #print "QCDlooseHighMETOS:", QCDlooseHighMETOS.Integral()
      #print "QCDlooseHighMETSS:", QCDlooseHighMETSS.Integral()
      #osSSh=QCDlooseHighMETOS.Integral()/QCDlooseHighMETSS.Integral()
      #osSShErr=osSSh*math.sqrt(1./QCDlooseHighMETOS.Integral() + 1./QCDlooseHighMETSS.Integral())
      #print "QCDlooseHighMETOS / QCDlooseHighMETSS",osSSh, "+-", osSShErr
      
      #print "QCDlooseLowMETLooseIsoSS:", QCDlooseLowMETLooseIsoSS.Integral()
      #tightLoosel=QCDlooseLowMETSS.Integral()/QCDlooseLowMETLooseIsoSS.Integral()
      #tightLooselErr=tightLoosel*math.sqrt(1./QCDlooseLowMETSS.Integral() + 1./QCDlooseLowMETLooseIsoSS.Integral())
      #print "QCDlooseLowMETSS / QCDlooseLowMETLooseIsoSS",tightLoosel, "+-", tightLooselErr
      
      #print "QCDlooseMiddleMETLooseIsoSS:", QCDlooseMiddleMETLooseIsoSS.Integral()
      #tightLoosem=QCDlooseMiddleMETSS.Integral()/QCDlooseMiddleMETLooseIsoSS.Integral()
      #tightLoosemErr=tightLoosem*math.sqrt(1./QCDlooseMiddleMETSS.Integral() + 1./QCDlooseMiddleMETLooseIsoSS.Integral())
      #print "QCDlooseMiddleMETSS / QCDlooseMiddleMETLooseIsoSS",tightLoosem, "+-", tightLoosemErr
      
      #print "QCDlooseHighMETLooseIsoSS:", QCDlooseHighMETLooseIsoSS.Integral()
      #tightLooseh=QCDlooseHighMETSS.Integral()/QCDlooseHighMETLooseIsoSS.Integral()
      #tightLoosehErr=tightLooseh*math.sqrt(1./QCDlooseHighMETSS.Integral() + 1./QCDlooseHighMETLooseIsoSS.Integral())
      #print "QCDlooseHighMETSS / QCDlooseHighMETLooseIsoSS",tightLooseh, "+-", tightLoosehErr

      #QCDScale=tightLoosem*osSSm
      
      tightOSLooseSSl=QCDlooseLowMETOS.Integral()/QCDlooseLowMETLooseIsoSS.Integral()
      tightOSLooseSSlErr=tightOSLooseSSl*math.sqrt(1./QCDlooseLowMETOS.Integral() + 1./QCDlooseLowMETLooseIsoSS.Integral())
      print "QCDlooseLowMETOS / QCDlooseLowMETLooseIsoSS",tightOSLooseSSl, "+-", tightOSLooseSSlErr
      
      tightOSLooseSSm=QCDlooseMiddleMETOS.Integral()/QCDlooseMiddleMETLooseIsoSS.Integral()
      tightOSLooseSSmErr=tightOSLooseSSm*math.sqrt(1./QCDlooseMiddleMETOS.Integral() + 1./QCDlooseMiddleMETLooseIsoSS.Integral())
      print "QCDlooseMiddleMETOS / QCDlooseMiddleMETLooseIsoSS",tightOSLooseSSm, "+-", tightOSLooseSSmErr
      
      tightOSLooseSSh=QCDlooseHighMETOS.Integral()/QCDlooseHighMETLooseIsoSS.Integral()
      tightOSLooseSShErr=tightOSLooseSSh*math.sqrt(1./QCDlooseHighMETOS.Integral() + 1./QCDlooseHighMETLooseIsoSS.Integral())
      print "QCDlooseHighMETOS / QCDlooseHighMETLooseIsoSS",tightOSLooseSSh, "+-", tightOSLooseSShErr
      
      QCDScale=tightOSLooseSSm
      
      for i in range(QCDlooseSS.obj.GetXaxis().GetNbins()):
          if QCDlooseSS.obj.GetBinContent(i+1)<=0:
	      QCDlooseSS.obj.SetBinContent(i+1,0.0001)
          if QCDlooseSS.weighted.GetBinContent(i+1)<=0:
	      QCDlooseSS.weighted.SetBinContent(i+1,0.0001)

      return QCDlooseSS,QCDScale

def saveForLimit(plotVarDataOS):
    rootfile=TFile("tauTau_SM1_mH125.root","RECREATE")
    channel=rootfile.mkdir("tauTau_SM1")
    plotVarDataOS.Hist("ggHTT125").weighted.SetName("SM125")
    plotVarDataOS.Hist("VBFHTT125").weighted.SetName("VBF125")
    plotVarDataOS.Hist("DYJets").Add(plotVarDataOS.Hist("DYJets_Photon"))
    plotVarDataOS.Hist("DYJets").weighted.SetName("ZTT")
    plotVarDataOS.Hist("DYJets_Electron").weighted.SetName("ZL")
    plotVarDataOS.Hist("DYJets_Fakes").weighted.SetName("ZJ")
    plotVarDataOS.Hist("WJets").weighted.SetName("W")
    plotVarDataOS.Hist("TTJets").weighted.SetName("TT")
    plotVarDataOS.Hist("QCDdata").weighted.SetName("QCD")
    plotVarDataOS.Hist("Data").weighted.SetName("data_obs")
    channel.Add(plotVarDataOS.Hist("ggHTT125").weighted)
    channel.Add(plotVarDataOS.Hist("VBFHTT125").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Electron").weighted)
    channel.Add(plotVarDataOS.Hist("DYJets_Fakes").weighted)
    channel.Add(plotVarDataOS.Hist("WJets").weighted)
    channel.Add(plotVarDataOS.Hist("TTJets").weighted)
    channel.Add(plotVarDataOS.Hist("QCDdata").weighted)
    channel.Add(plotVarDataOS.Hist("Data").weighted)
    channel.Write()
    rootfile.Close()


NBINS = 100
XMIN  = 0
XMAX  = 200

if __name__ == '__main__':

    import copy
    from optparse import OptionParser
    from CMGTools.RootTools.RootInit import *

    parser = OptionParser()
    parser.usage = '''
    %prog <anaDir> <cfgFile>

    cfgFile: analysis configuration file, see CMGTools.H2TauTau.macros.MultiLoop
    anaDir: analysis directory containing all components, see CMGTools.H2TauTau.macros.MultiLoop.

    hist: histogram you want to plot
    '''
    parser.add_option("-B", "--box", 
                      dest="box", 
                      help="box. Default is Inclusive",
                      default='Inclusive')
    parser.add_option("-M", "--mtregion", 
                      dest="mtregion", 
                      help="mT region. Default is LowMT",
                      default='LowMT')
    parser.add_option("-H", "--histlist", 
                      dest="histlist", 
                      help="histogram list",
                      default=None)
    parser.add_option("-C", "--cut", 
                      dest="cut", 
                      help="cut to apply in TTree::Draw",
                      default='1')
    parser.add_option("-G", "--histgroup", 
                      dest="histgroup", 
                      help="histogram group",
                      default=None)
    parser.add_option("-R", "--rebin", 
                      dest="rebin", 
                      help="rebinning factor",
                      default=None)
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)

    dataName = 'Data'
    weight='weight'

    anaDir = args[0]
    hists = histogramSet( options )
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
    selCompsMC = {}
    selCompsMC['DYJets']=copy.deepcopy(selComps['DYJets'])
    selCompsMC['WJets']=copy.deepcopy(selComps['WJets'])
    selCompsMC['TTJets']=copy.deepcopy(selComps['TTJets'])
    selCompsMC['ggHTT125']=copy.deepcopy(selComps['ggHTT125'])
    selCompsMC['VBFHTT125']=copy.deepcopy(selComps['VBFHTT125'])
    weightsMC = {}
    weightsMC['DYJets']=copy.deepcopy(weights['DYJets'])
    weightsMC['WJets']=copy.deepcopy(weights['WJets'])
    weightsMC['TTJets']=copy.deepcopy(weights['TTJets'])
    weightsMC['ggHTT125']=copy.deepcopy(weights['ggHTT125'])
    weightsMC['VBFHTT125']=copy.deepcopy(weights['VBFHTT125'])
    
    # 1fb
    selComps['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    weights['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    weights['data_Run2011A_PromptReco_v6'].intLumi = 0.
    
    # IsoTau20
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 31.5
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 31.5

    # IsoTau25
    #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.+30.8
    #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.+30.8
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 139.+31.5
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 139.+31.5
    #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 0.
    #weights['data_Run2011A_05Aug2011_v1'].intLumi = 0.
    #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.

    # IsoTau35
    #selComps['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #weights['data_Run2011A_May10ReReco_v1'].intLumi = 0.
    #selComps['data_Run2011A_PromptReco_v4'].intLumi = 759.2
    #weights['data_Run2011A_PromptReco_v4'].intLumi = 759.2
    #selComps['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    #weights['data_Run2011A_05Aug2011_v1'].intLumi = 30.8
    #selComps['data_Run2011A_PromptReco_v6'].intLumi = 0.
    #weights['data_Run2011A_PromptReco_v6'].intLumi = 0.

    cuts=[#("ATLAS",'l1Pt>35 && l2Pt>25 && diTauCharge==0 && jet1Pt>40 && dRtt<2.2 && mttj>225',' && met>20',' && l1TigIso>0.5 && l2TigIso>0.5'),
	  ("CMSl40met25j70dR2tm",'l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>70 && dRtt<2.0',' && met>25',' && ((l1TigIso>0.5 && l2MedIso>0.5) || (l1MedIso>0.5 && l2TigIso>0.5))'),
	  ]

    prefix1="~/workspace/ditau/plots/IsoTauLoosee1fb_"

    for prefix,cut,metcut,isocut in cuts:
     for var, nx, xmin, xmax in [
         ('svfitMass',30, 0, 300),
         ('visMass',30, 0, 300),
         ('mt',20, 0, 200),
         ('pThiggs',20, 0, 300),
         ('diTauPt',20, 0, 300),
         ('l1Pt',25, 0, 250),
         ('l2Pt',25, 0, 250),
         ('l1Eta',20, -3, 3),
         ('l2Eta',20, -3, 3),
         ('jet1Eta',20, -5, 5),
         ('jet1Pt',20, 0, 500),
         ('jet2Eta',20, -5, 5),
         ('jet2Pt',20, 0, 500),
         ('dRtt',20, 0, 5),
         ('dPhitt',20, 0, 3.15),
         ('mttj',20, 0, 1000),
         ('met',20, 0, 200),
         ('diTauCharge',7, -3, 3),
         ('l1LooIso',3, -1, 2),
         ('l2LooIso',3, -1, 2),
         ('l1MedIso',3, -1, 2),
         ('l2MedIso',3, -1, 2),
         ('l1TigIso',3, -1, 2),
         ('l2TigIso',3, -1, 2),
         ('l1Prongs',5, -1, 4),
         ('l2Prongs',5, -1, 4),
      ]:
      #if "45" in cut and "Mass" in var:
      #    nx/=2
      if var in ["met","jet1Pt","jet2Pt","l1Pt","l2Pt"]:
          log=True
      else:
          log=False
      plotVarDataSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+isocut+metcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+metcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+metcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLowMETOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+" && met<10", weight=weight,
     			    embed=options.embed)
      plotVarDataLowMETSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+isocut+" && met<10", weight=weight,
     			    embed=options.embed)
      plotVarDataMiddleMETOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+" && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataMiddleMETSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+isocut+" && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataHighMETOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+" && met>10 && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataHighMETSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+isocut+" && met>10 && met<20", weight=weight,
     			    embed=options.embed)

      plotVarDataLowMETLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met<10", weight=weight,
     			    embed=options.embed)
      plotVarDataLowMETLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met<10", weight=weight,
     			    embed=options.embed)
      plotVarDataMiddleMETLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataMiddleMETLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataHighMETLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met>10 && met<20", weight=weight,
     			    embed=options.embed)
      plotVarDataHighMETLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut.replace("diTauCharge==0","diTauCharge!=0")+" && l1LooIso>0.5 && l2LooIso>0.5"+" && met>10 && met<20", weight=weight,
     			    embed=options.embed)

      QCDShape, QCDScale = QCDEstimate(plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                       plotVarDataLowMETOS, plotVarDataLowMETSS, plotVarDataMiddleMETOS, plotVarDataMiddleMETSS, plotVarDataHighMETOS, plotVarDataHighMETSS,
                                       plotVarDataLowMETLooseIsoOS, plotVarDataLowMETLooseIsoSS, plotVarDataMiddleMETLooseIsoOS, plotVarDataMiddleMETLooseIsoSS, plotVarDataHighMETLooseIsoOS, plotVarDataHighMETLooseIsoSS,
				       log)

      plotVarDataOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+metcut, weight=weight,
     			    embed=options.embed)
      plotVarDataOS.AddHistogram("QCDdata",QCDShape.weighted)
      plotVarDataOS.Hist('QCDdata').stack = True
      plotVarDataOS.Hist('QCDdata').SetStyle( sBlack )
      plotVarDataOS.Hist('QCDdata').layer = 5
      plotVarDataOS.Hist('QCDdata').Scale(QCDScale)
      #plotVarDataOS.Hist('ggHTT125').Scale(5)
      #plotVarDataOS.Hist('VBFHTT125').Scale(5)
      ymax=plotVarDataOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
      else:
          plotVarDataOS.DrawStack("HIST",xmin,xmax,0,ymax)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataOS.varName+"_data.png")
      if var=="svfitMass":
          saveForLimit(copy.deepcopy(plotVarDataOS))
      gPad.WaitPrimitive()
      print "Data:", plotVarDataOS.Hist("Data").Integral()
      print "TTJets:", plotVarDataOS.Hist("TTJets").Integral()
      print "DYJets:", plotVarDataOS.Hist("DYJets").Integral()+plotVarDataOS.Hist("DYJets_Photon").Integral()
      print "DYJets_Electron:", plotVarDataOS.Hist("DYJets_Electron").Integral()
      print "DYJets_Fakes:", plotVarDataOS.Hist("DYJets_Fakes").Integral()
      print "WJets:", plotVarDataOS.Hist("WJets").Integral()+plotVarDataOS.Hist("WJets_Fakes").Integral()
      print "QCDdata:", plotVarDataOS.Hist("QCDdata").Integral()
      print "ggHTT125:", plotVarDataOS.Hist("ggHTT125").Integral()
      print "VBFHTT125:", plotVarDataOS.Hist("VBFHTT125").Integral()

      plotVar = H2TauTauDataMC(var, anaDir, selCompsMC, weightsMC,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+metcut, weight=weight,
     			    embed=options.embed)
      print "MC lumi:", plotVar.intLumi
      plotVar.AddHistogram("QCDdata",QCDShape.weighted)
      plotVar.Hist('QCDdata').stack = True
      plotVar.Hist('QCDdata').SetStyle( sBlack )
      plotVar.Hist('QCDdata').layer = 5
      plotVar.Hist('QCDdata').Scale(plotVar.intLumi/plotVarDataOS.intLumi)
      plotVar.Hist('QCDdata').Scale(QCDScale)
      plotVar.Hist('ggHTT125').Scale(5)
      plotVar.Hist('VBFHTT125').Scale(5)
      ymax=(plotVar.Hist("QCDdata").GetMaximum()+
	  plotVar.Hist("TTJets").GetMaximum()+
	  plotVar.Hist("DYJets").GetMaximum()+
	  plotVar.Hist("DYJets_Fakes").GetMaximum()+
	  plotVar.Hist("DYJets_Photon").GetMaximum()+
	  plotVar.Hist("DYJets_Electron").GetMaximum()+
	  plotVar.Hist("WJets").GetMaximum()+
	  plotVar.Hist("WJets_Fakes").GetMaximum())*1.5
      if log:
          plotVar.DrawStack("HIST",xmin,xmax,0.1,ymax)
      else:
          plotVar.DrawStack("HIST",xmin,xmax,0,ymax)
      gPad.SaveAs(prefix1+prefix+'_'+plotVar.varName+"_mc.png")
      #gPad.WaitPrimitive()
