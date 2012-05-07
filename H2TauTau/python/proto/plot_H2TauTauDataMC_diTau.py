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
                                       plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                       plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,
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
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoSS.varName+"_qcdLooseSS.png")
      gPad.WaitPrimitive()
      
      ymax=plotVarDataLowControlOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLowControlOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLowControlOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLowControlOS.varName+"_qcdLooseLowControlOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLowControlSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLowControlSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLowControlSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLowControlSS.varName+"_qcdLooseLowControlSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlOS.varName+"_qcdLooseAverageControlOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlSS.varName+"_qcdLooseAverageControlSS.png")
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

      QCDlooseLowControlOS=copy.deepcopy(plotVarDataLowControlOS.Hist("Data"))
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("DYJets"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("DYJets_Photon"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("DYJets_Electron"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("WJets"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("WJets_Fakes"),-1)
      QCDlooseLowControlOS.Add(plotVarDataLowControlOS.Hist("TTJets"),-1)

      QCDlooseLowControlSS=copy.deepcopy(plotVarDataLowControlSS.Hist("Data"))
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("DYJets"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("DYJets_Photon"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("DYJets_Electron"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("WJets"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("WJets_Fakes"),-1)
      QCDlooseLowControlSS.Add(plotVarDataLowControlSS.Hist("TTJets"),-1)

      QCDlooseAverageControlOS=copy.deepcopy(plotVarDataAverageControlOS.Hist("Data"))
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("DYJets"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("DYJets_Fakes"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("DYJets_Photon"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("DYJets_Electron"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("WJets"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("WJets_Fakes"),-1)
      QCDlooseAverageControlOS.Add(plotVarDataAverageControlOS.Hist("TTJets"),-1)

      QCDlooseAverageControlSS=copy.deepcopy(plotVarDataAverageControlSS.Hist("Data"))
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("DYJets"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("DYJets_Fakes"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("DYJets_Photon"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("DYJets_Electron"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("WJets"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("WJets_Fakes"),-1)
      QCDlooseAverageControlSS.Add(plotVarDataAverageControlSS.Hist("TTJets"),-1)

      QCDlooseHighControlOS=copy.deepcopy(plotVarDataHighControlOS.Hist("Data"))
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("DYJets"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("DYJets_Photon"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("DYJets_Electron"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("WJets"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("WJets_Fakes"),-1)
      QCDlooseHighControlOS.Add(plotVarDataHighControlOS.Hist("TTJets"),-1)

      QCDlooseHighControlSS=copy.deepcopy(plotVarDataHighControlSS.Hist("Data"))
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("DYJets"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("DYJets_Photon"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("DYJets_Electron"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("WJets"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("WJets_Fakes"),-1)
      QCDlooseHighControlSS.Add(plotVarDataHighControlSS.Hist("TTJets"),-1)

      QCDlooseLowControlLooseIsoOS=copy.deepcopy(plotVarDataLowControlLooseIsoOS.Hist("Data"))
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("WJets"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseLowControlLooseIsoSS=copy.deepcopy(plotVarDataLowControlLooseIsoSS.Hist("Data"))
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("WJets"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("TTJets"),-1)

      QCDlooseAverageControlLooseIsoOS=copy.deepcopy(plotVarDataAverageControlLooseIsoOS.Hist("Data"))
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("WJets"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseAverageControlLooseIsoOS.Add(plotVarDataAverageControlLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseAverageControlLooseIsoSS=copy.deepcopy(plotVarDataAverageControlLooseIsoSS.Hist("Data"))
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("WJets"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseAverageControlLooseIsoSS.Add(plotVarDataAverageControlLooseIsoSS.Hist("TTJets"),-1)

      QCDlooseHighControlLooseIsoOS=copy.deepcopy(plotVarDataHighControlLooseIsoOS.Hist("Data"))
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("WJets"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseHighControlLooseIsoSS=copy.deepcopy(plotVarDataHighControlLooseIsoSS.Hist("Data"))
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("WJets"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("TTJets"),-1)

      #print "QCDlooseSS:", QCDlooseSS.Integral()
      #print "QCDtightSS:", QCDtightSS.Integral()
      tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./QCDtightSS.Integral() + 1./QCDlooseSS.Integral())
      print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      #print "QCDlooseLowControlOS:", QCDlooseLowControlOS.Integral()
      #print "QCDlooseLowControlSS:", QCDlooseLowControlSS.Integral()
      #osSSl=QCDlooseLowControlOS.Integral()/QCDlooseLowControlSS.Integral()
      #osSSlErr=osSSl*math.sqrt(1./QCDlooseLowControlOS.Integral() + 1./QCDlooseLowControlSS.Integral())
      #print "QCDlooseLowControlOS / QCDlooseLowControlSS",osSSl, "+-", osSSlErr
      
      #print "QCDlooseAverageControlOS:", QCDlooseAverageControlOS.Integral()
      #print "QCDlooseAverageControlSS:", QCDlooseAverageControlSS.Integral()
      #osSSm=QCDlooseAverageControlOS.Integral()/QCDlooseAverageControlSS.Integral()
      #osSSmErr=osSSm*math.sqrt(1./QCDlooseAverageControlOS.Integral() + 1./QCDlooseAverageControlSS.Integral())
      #print "QCDlooseAverageControlOS / QCDlooseAverageControlSS",osSSm, "+-", osSSmErr
      
      #print "QCDlooseHighControlOS:", QCDlooseHighControlOS.Integral()
      #print "QCDlooseHighControlSS:", QCDlooseHighControlSS.Integral()
      #osSSh=QCDlooseHighControlOS.Integral()/QCDlooseHighControlSS.Integral()
      #osSShErr=osSSh*math.sqrt(1./QCDlooseHighControlOS.Integral() + 1./QCDlooseHighControlSS.Integral())
      #print "QCDlooseHighControlOS / QCDlooseHighControlSS",osSSh, "+-", osSShErr
      
      #print "QCDlooseLowControlLooseIsoSS:", QCDlooseLowControlLooseIsoSS.Integral()
      tightLoosel=QCDlooseLowControlSS.Integral()/QCDlooseLowControlLooseIsoSS.Integral()
      tightLooselErr=tightLoosel*math.sqrt(1./QCDlooseLowControlSS.Integral() + 1./QCDlooseLowControlLooseIsoSS.Integral())
      print "QCDlooseLowControlSS / QCDlooseLowControlLooseIsoSS",tightLoosel, "+-", tightLooselErr
      
      #print "QCDlooseAverageControlLooseIsoSS:", QCDlooseAverageControlLooseIsoSS.Integral()
      tightLoosem=QCDlooseAverageControlSS.Integral()/QCDlooseAverageControlLooseIsoSS.Integral()
      tightLoosemErr=tightLoosem*math.sqrt(1./QCDlooseAverageControlSS.Integral() + 1./QCDlooseAverageControlLooseIsoSS.Integral())
      print "QCDlooseAverageControlSS / QCDlooseAverageControlLooseIsoSS",tightLoosem, "+-", tightLoosemErr
      
      #print "QCDlooseHighControlLooseIsoSS:", QCDlooseHighControlLooseIsoSS.Integral()
      tightLooseh=QCDlooseHighControlSS.Integral()/QCDlooseHighControlLooseIsoSS.Integral()
      tightLoosehErr=tightLooseh*math.sqrt(1./QCDlooseHighControlSS.Integral() + 1./QCDlooseHighControlLooseIsoSS.Integral())
      print "QCDlooseHighControlSS / QCDlooseHighControlLooseIsoSS",tightLooseh, "+-", tightLoosehErr

      #QCDScale=tightLoosem*osSSm
      
      tightOSLooseSSl=QCDlooseLowControlOS.Integral()/QCDlooseLowControlLooseIsoSS.Integral()
      tightOSLooseSSlErr=tightOSLooseSSl*math.sqrt(1./QCDlooseLowControlOS.Integral() + 1./QCDlooseLowControlLooseIsoSS.Integral())
      print "QCDlooseLowControlOS / QCDlooseLowControlLooseIsoSS",tightOSLooseSSl, "+-", tightOSLooseSSlErr
      
      tightOSLooseSSm=QCDlooseAverageControlOS.Integral()/QCDlooseAverageControlLooseIsoSS.Integral()
      tightOSLooseSSmErr=tightOSLooseSSm*math.sqrt(1./QCDlooseAverageControlOS.Integral() + 1./QCDlooseAverageControlLooseIsoSS.Integral())
      print "QCDlooseAverageControlOS / QCDlooseAverageControlLooseIsoSS",tightOSLooseSSm, "+-", tightOSLooseSSmErr
      
      tightOSLooseSSh=QCDlooseHighControlOS.Integral()/QCDlooseHighControlLooseIsoSS.Integral()
      tightOSLooseSShErr=tightOSLooseSSh*math.sqrt(1./QCDlooseHighControlOS.Integral() + 1./QCDlooseHighControlLooseIsoSS.Integral())
      print "QCDlooseHighControlOS / QCDlooseHighControlLooseIsoSS",tightOSLooseSSh, "+-", tightOSLooseSShErr
      
      QCDScale=tightOSLooseSSm
      
      for i in range(QCDlooseSS.obj.GetXaxis().GetNbins()):
          if QCDlooseSS.obj.GetBinContent(i+1)<=0:
	      QCDlooseSS.obj.SetBinContent(i+1,0.001)
          if QCDlooseSS.weighted.GetBinContent(i+1)<=0:
	      QCDlooseSS.weighted.SetBinContent(i+1,0.001)

      return QCDlooseSS,QCDScale

def saveForLimit(plotVarDataOS, category="SM1"):
    rootfile=TFile("tauTau_"+category+"_mH125.root","RECREATE")
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

    cuts=[#("ATLAS",'l1Pt>35 && l2Pt>25 && diTauCharge==0 && jet1Pt>40 && dRtt<2.2 && mttj>225',' && met>20',' && l1TigIso>0.5 && l2TigIso>0.5',True),
	  #("CMSl40met25j40dR2tm",'l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>40',' && met>20',' && ((l1TigIso>0.5 && l2MedIso>0.5) || (l1MedIso>0.5 && l2TigIso>0.5)) && dRtt<2.0',True),
	  #("CMSl40met25j40dR2tm",'l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>40',' && dRtt<2.0',' && ((l1TigIso>0.5 && l2MedIso>0.5) || (l1MedIso>0.5 && l2TigIso>0.5)) && met>20',False),
	  #("CMSl40met25j40dR2tt",'l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>40 && dRtt<2.0',' && met>20',' && (l1TigIso>0.5 && l2TigIso>0.5)',True),
	  ("CMSl40j40dR2tt",'l1Pt>40 && l2Pt>40 && abs(l1Eta)<2.1 && abs(l2Eta)<2.1 && diTauCharge==0 && jet1Pt>40',' && dRtt<2.0',' && (l1TigIso>0.5 && l2TigIso>0.5)',False),
	  ]

    prefix1="~/workspace/ditau/plots/IsoTauLoosee1fb_"

    for prefix,cut,antiqcdcut,isocut,qcdEstimate in cuts:
     for var, nx, xmin, xmax in [
         ('svfitMass',15, 0, 300),
         ('visMass',15, 0, 300),
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

      if qcdEstimate:
        # MET based QCD estimation
        looseisocut=" && l1LooIso>0.5 && l2LooIso>0.5"
        lowcontrolcut=" && met<10"
        averagecontrolcut=" && met<20"
        highcontrolcut=" && met>10 && met<20"
      else:
        # dR based QCD estimation
        looseisocut=" && l1LooIso>0.5 && l2LooIso>0.5"
        lowcontrolcut=" && dRtt>3.0"
        averagecontrolcut=" && dRtt>2.0"
        highcontrolcut=" && dRtt>2.0 && dRtt<3.0"

      cutSS=cut.replace("diTauCharge==0","diTauCharge!=0")
      
      plotVarDataSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+antiqcdcut, weight=weight,
     			    embed=options.embed)
			    
      plotVarDataLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+antiqcdcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+antiqcdcut, weight=weight,
     			    embed=options.embed)
			    
      plotVarDataLowControlOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+lowcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLowControlSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+lowcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataAverageControlOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataAverageControlSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataHighControlOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+highcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataHighControlSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+isocut+highcontrolcut, weight=weight,
     			    embed=options.embed)

      plotVarDataLowControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+lowcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataLowControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+lowcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataAverageControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataAverageControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+averagecontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataHighControlLooseIsoOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+looseisocut+highcontrolcut, weight=weight,
     			    embed=options.embed)
      plotVarDataHighControlLooseIsoSS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cutSS+looseisocut+highcontrolcut, weight=weight,
     			    embed=options.embed)

      QCDShape, QCDScale = QCDEstimate(plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,
                                       plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS, plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,
                                       plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS, plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS, plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,
				       log)

      plotVarDataOS = H2TauTauDataMC(var, anaDir, selComps, weights,
     			    nx, xmin, xmax,
     			    cut = cut+isocut+antiqcdcut, weight=weight,
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
        if prefix=="CMSl40j40dR2tt":
          saveForLimit(copy.deepcopy(plotVarDataOS),"SM1")
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
     			    cut = cut+isocut+antiqcdcut, weight=weight,
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
      gPad.WaitPrimitive()
