import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math


def QCDEstimate(prefix,prefix1,xmin,xmax,\
                plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,\
                plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS,\
                plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,\
                plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS,\
                plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS,\
                plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,\
                log):
      ymax=plotVarDataLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoSS.varName+"_qcdLooseSS.png")
      gPad.WaitPrimitive()
      
      ymax=plotVarDataSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_qcdTightSS.png")
      gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlOS.varName+"_qcdAverageControlOS.png")
      gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlLooseIsoOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlLooseIsoOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlLooseIsoOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlLooseIsoOS.varName+"_qcdAverageControlLooseIsoOS.png")
      gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlLooseIsoSS.varName+"_qcdAverageControlLooseIsoSS.png")
      gPad.WaitPrimitive()
      
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
      
      #print "QCDlooseLowControlLooseIsoOS:", QCDlooseLowControlLooseIsoOS.Integral()
      #print "QCDlooseLowControlLooseIsoSS:", QCDlooseLowControlLooseIsoSS.Integral()
      osSSl=QCDlooseLowControlLooseIsoOS.Integral()/QCDlooseLowControlLooseIsoSS.Integral()
      osSSlErr=osSSl*math.sqrt(1./QCDlooseLowControlLooseIsoOS.Integral() + 1./QCDlooseLowControlLooseIsoSS.Integral())
      print "QCDlooseLowControlLooseIsoOS / QCDlooseLowControlLooseIsoSS",osSSl, "+-", osSSlErr
      
      #print "QCDlooseAverageControlLooseIsoOS:", QCDlooseAverageControlLooseIsoOS.Integral()
      #print "QCDlooseAverageControlLooseIsoSS:", QCDlooseAverageControlLooseIsoSS.Integral()
      osSSm=QCDlooseAverageControlLooseIsoOS.Integral()/QCDlooseAverageControlLooseIsoSS.Integral()
      osSSmErr=osSSm*math.sqrt(1./QCDlooseAverageControlLooseIsoOS.Integral() + 1./QCDlooseAverageControlLooseIsoSS.Integral())
      print "QCDlooseAverageControlLooseIsoOS / QCDlooseAverageControlLooseIsoSS",osSSm, "+-", osSSmErr
      
      #print "QCDlooseHighControlLooseIsoOS:", QCDlooseHighControlLooseIsoOS.Integral()
      #print "QCDlooseHighControlLooseIsoSS:", QCDlooseHighControlLooseIsoSS.Integral()
      osSSh=QCDlooseHighControlLooseIsoOS.Integral()/QCDlooseHighControlLooseIsoSS.Integral()
      osSShErr=osSSh*math.sqrt(1./QCDlooseHighControlLooseIsoOS.Integral() + 1./QCDlooseHighControlLooseIsoSS.Integral())
      print "QCDlooseHighControlLooseIsoOS / QCDlooseHighControlLooseIsoSS",osSSh, "+-", osSShErr
      
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

def QCDEstimate1(prefix,prefix1,xmin,xmax,\
                 plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,\
                 plotVarDataLowControlOS, plotVarDataLowControlSS, plotVarDataAverageControlOS,\
                 plotVarDataAverageControlSS, plotVarDataHighControlOS, plotVarDataHighControlSS,\
                 plotVarDataLowControlLooseIsoOS, plotVarDataLowControlLooseIsoSS,\
                 plotVarDataAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoSS,\
                 plotVarDataHighControlLooseIsoOS, plotVarDataHighControlLooseIsoSS,\
                 log):
      ymax=plotVarDataLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoSS.varName+"_qcdLooseSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_qcdTightSS.png")
      #gPad.WaitPrimitive()
      
      #ymax=plotVarDataAverageControlOS.Hist("Data").GetMaximum()*1.5
      #if log:
        #  plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	#  gPad.SetLogy()
      #else:
        #  plotVarDataAverageControlOS.DrawStack("HIST",xmin,xmax,0,ymax)
	#  gPad.SetLogy(False)
      #gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlOS.varName+"_qcdAverageControlOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlLooseIsoOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlLooseIsoOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlLooseIsoOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlLooseIsoOS.varName+"_qcdAverageControlLooseIsoOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlLooseIsoSS.varName+"_qcdAverageControlLooseIsoSS.png")
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

      #QCDlooseLowControlLooseIsoOS=copy.deepcopy(plotVarDataLowControlLooseIsoOS.Hist("Data"))
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Fakes"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Photon"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("DYJets_Electron"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("WJets"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("WJets_Fakes"),-1)
      #QCDlooseLowControlLooseIsoOS.Add(plotVarDataLowControlLooseIsoOS.Hist("TTJets"),-1)

      #QCDlooseLowControlLooseIsoSS=copy.deepcopy(plotVarDataLowControlLooseIsoSS.Hist("Data"))
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Fakes"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Photon"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("DYJets_Electron"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("WJets"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("WJets_Fakes"),-1)
      #QCDlooseLowControlLooseIsoSS.Add(plotVarDataLowControlLooseIsoSS.Hist("TTJets"),-1)

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

      #QCDlooseHighControlLooseIsoOS=copy.deepcopy(plotVarDataHighControlLooseIsoOS.Hist("Data"))
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Fakes"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Photon"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("DYJets_Electron"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("WJets"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("WJets_Fakes"),-1)
      #QCDlooseHighControlLooseIsoOS.Add(plotVarDataHighControlLooseIsoOS.Hist("TTJets"),-1)

      #QCDlooseHighControlLooseIsoSS=copy.deepcopy(plotVarDataHighControlLooseIsoSS.Hist("Data"))
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Fakes"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Photon"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("DYJets_Electron"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("WJets"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("WJets_Fakes"),-1)
      #QCDlooseHighControlLooseIsoSS.Add(plotVarDataHighControlLooseIsoSS.Hist("TTJets"),-1)

      #print "QCDlooseSS:", QCDlooseSS.Integral()
      #print "QCDtightSS:", QCDtightSS.Integral()
      tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./QCDtightSS.Integral() + 1./QCDlooseSS.Integral())
      print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      #print "QCDlooseLowControlLooseIsoOS:", QCDlooseLowControlLooseIsoOS.Integral()
      #print "QCDlooseLowControlLooseIsoSS:", QCDlooseLowControlLooseIsoSS.Integral()
      #osSSl=QCDlooseLowControlLooseIsoOS.Integral()/QCDlooseLowControlLooseIsoSS.Integral()
      #osSSlErr=osSSl*math.sqrt(1./QCDlooseLowControlLooseIsoOS.Integral() + 1./QCDlooseLowControlLooseIsoSS.Integral())
      #print "QCDlooseLowControlLooseIsoOS / QCDlooseLowControlLooseIsoSS",osSSl, "+-", osSSlErr
      
      #print "QCDlooseAverageControlLooseIsoOS:", QCDlooseAverageControlLooseIsoOS.Integral()
      #print "QCDlooseAverageControlLooseIsoSS:", QCDlooseAverageControlLooseIsoSS.Integral()
      osSSm=QCDlooseAverageControlLooseIsoOS.Integral()/QCDlooseAverageControlLooseIsoSS.Integral()
      osSSmErr=osSSm*math.sqrt(1./QCDlooseAverageControlLooseIsoOS.Integral() + 1./QCDlooseAverageControlLooseIsoSS.Integral())
      print "QCDlooseAverageControlLooseIsoOS / QCDlooseAverageControlLooseIsoSS",osSSm, "+-", osSSmErr
      
      #print "QCDlooseHighControlLooseIsoOS:", QCDlooseHighControlLooseIsoOS.Integral()
      #print "QCDlooseHighControlLooseIsoSS:", QCDlooseHighControlLooseIsoSS.Integral()
      #osSSh=QCDlooseHighControlLooseIsoOS.Integral()/QCDlooseHighControlLooseIsoSS.Integral()
      #osSShErr=osSSh*math.sqrt(1./QCDlooseHighControlLooseIsoOS.Integral() + 1./QCDlooseHighControlLooseIsoSS.Integral())
      #print "QCDlooseHighControlLooseIsoOS / QCDlooseHighControlLooseIsoSS",osSSh, "+-", osSShErr
      
      QCDScale=osSSm*tightLoose
      
      for i in range(QCDlooseSS.obj.GetXaxis().GetNbins()):
          if QCDlooseSS.obj.GetBinContent(i+1)<=0:
	      QCDlooseSS.obj.SetBinContent(i+1,0.001)
          if QCDlooseSS.weighted.GetBinContent(i+1)<=0:
	      QCDlooseSS.weighted.SetBinContent(i+1,0.001)

      return QCDlooseSS,QCDScale

def QCDEstimate2(prefix,prefix1,xmin,xmax,\
                 plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,\
                 log):
      ymax=plotVarDataLooseIsoOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoOS.varName+"_qcdLooseOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoSS.varName+"_qcdLooseSS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSS.varName+"_qcdTightSS.png")
      #gPad.WaitPrimitive()
      
      QCDlooseOS=copy.deepcopy(plotVarDataLooseIsoOS.Hist("Data"))
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Fakes"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Photon"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("DYJets_Electron"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("WJets"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("WJets_Fakes"),-1)
      QCDlooseOS.Add(plotVarDataLooseIsoOS.Hist("TTJets"),-1)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Fakes"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Photon"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("DYJets_Electron"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("WJets"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("WJets_Fakes"),-1)
      QCDlooseSS.Add(plotVarDataLooseIsoSS.Hist("TTJets"),-1)

      QCDtightSS=copy.deepcopy(plotVarDataSS.Hist("Data"))
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Fakes"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Photon"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Electron"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets_Fakes"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("TTJets"),-1)

      #print "QCDlooseSS:", QCDlooseSS.Integral()
      #print "QCDtightSS:", QCDtightSS.Integral()
      tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./QCDtightSS.Integral() + 1./QCDlooseSS.Integral())
      print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      QCDScale=tightLoose
      
      for i in range(QCDlooseOS.obj.GetXaxis().GetNbins()):
          if QCDlooseOS.obj.GetBinContent(i+1)<=0:
	      QCDlooseOS.obj.SetBinContent(i+1,0.001)
          if QCDlooseOS.weighted.GetBinContent(i+1)<=0:
	      QCDlooseOS.weighted.SetBinContent(i+1,0.001)

      return QCDlooseOS,QCDScale
