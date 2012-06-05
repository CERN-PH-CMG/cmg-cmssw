import imp
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC_diTau import *
from CMGTools.RootTools.RootInit import *
import math

def substractMCbackground(QCDtightSS, plotVarDataSS):
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Fakes"),-1)
      #QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Photon"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("DYJets_Electron"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WJets_Fakes"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WW"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("WZ"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("ZZ"),-1)
      QCDtightSS.Add(plotVarDataSS.Hist("TTJets"),-1)

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
      substractMCbackground(QCDtightSS, plotVarDataSS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      QCDlooseOS=copy.deepcopy(plotVarDataLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      QCDlooseLowControlOS=copy.deepcopy(plotVarDataLowControlOS.Hist("Data"))
      substractMCbackground(QCDlooseLowControlOS, plotVarDataLowControlOS)

      QCDlooseLowControlSS=copy.deepcopy(plotVarDataLowControlSS.Hist("Data"))
      substractMCbackground(QCDlooseLowControlSS, plotVarDataLowControlSS)

      QCDlooseAverageControlOS=copy.deepcopy(plotVarDataAverageControlOS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlOS, plotVarDataAverageControlOS)

      QCDlooseAverageControlSS=copy.deepcopy(plotVarDataAverageControlSS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlSS, plotVarDataAverageControlSS)

      QCDlooseHighControlOS=copy.deepcopy(plotVarDataHighControlOS.Hist("Data"))
      substractMCbackground(QCDlooseHighControlOS, plotVarDataHighControlOS)

      QCDlooseHighControlSS=copy.deepcopy(plotVarDataHighControlSS.Hist("Data"))
      substractMCbackground(QCDlooseHighControlSS, plotVarDataHighControlSS)

      QCDlooseLowControlLooseIsoOS=copy.deepcopy(plotVarDataLowControlLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseLowControlLooseIsoOS, plotVarDataLowControlLooseIsoOS)

      QCDlooseLowControlLooseIsoSS=copy.deepcopy(plotVarDataLowControlLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseLowControlLooseIsoSS, plotVarDataLowControlLooseIsoSS)

      QCDlooseAverageControlLooseIsoOS=copy.deepcopy(plotVarDataAverageControlLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoOS)

      QCDlooseAverageControlLooseIsoSS=copy.deepcopy(plotVarDataAverageControlLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlLooseIsoSS, plotVarDataAverageControlLooseIsoSS)

      QCDlooseHighControlLooseIsoOS=copy.deepcopy(plotVarDataHighControlLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseHighControlLooseIsoOS, plotVarDataHighControlLooseIsoOS)

      QCDlooseHighControlLooseIsoSS=copy.deepcopy(plotVarDataHighControlLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseHighControlLooseIsoSS, plotVarDataHighControlLooseIsoSS)

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
      
      if tightOSLooseSSm<tightOSLooseSSmErr/2.:
          tightOSLooseSSm=tightOSLooseSSmErr/2.
      
      QCDScale=tightOSLooseSSm
      
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
      substractMCbackground(QCDtightSS, plotVarDataSS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      #QCDlooseOS=copy.deepcopy(plotVarDataLooseIsoOS.Hist("Data"))
      #substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      #QCDlooseLowControlOS=copy.deepcopy(plotVarDataLowControlOS.Hist("Data"))
      #substractMCbackground(QCDlooseLowControlOS, plotVarDataLowControlOS)

      #QCDlooseLowControlSS=copy.deepcopy(plotVarDataLowControlSS.Hist("Data"))
      #substractMCbackground(QCDlooseLowControlSS, plotVarDataLowControlSS)

      #QCDlooseAverageControlOS=copy.deepcopy(plotVarDataAverageControlOS.Hist("Data"))
      #substractMCbackground(QCDlooseAverageControlOS, plotVarDataAverageControlOS)

      #QCDlooseAverageControlSS=copy.deepcopy(plotVarDataAverageControlSS.Hist("Data"))
      #substractMCbackground(QCDlooseAverageControlSS, plotVarDataAverageControlSS)

      #QCDlooseHighControlOS=copy.deepcopy(plotVarDataHighControlOS.Hist("Data"))
      #substractMCbackground(QCDlooseHighControlOS, plotVarDataHighControlOS)

      #QCDlooseHighControlSS=copy.deepcopy(plotVarDataHighControlSS.Hist("Data"))
      #substractMCbackground(QCDlooseHighControlSS, plotVarDataHighControlSS)

      #QCDlooseLowControlLooseIsoOS=copy.deepcopy(plotVarDataLowControlLooseIsoOS.Hist("Data"))
      #substractMCbackground(QCDlooseLowControlLooseIsoOS, plotVarDataLowControlLooseIsoOS)

      #QCDlooseLowControlLooseIsoSS=copy.deepcopy(plotVarDataLowControlLooseIsoSS.Hist("Data"))
      #substractMCbackground(QCDlooseLowControlLooseIsoSS, plotVarDataLowControlLooseIsoSS)

      QCDlooseAverageControlLooseIsoOS=copy.deepcopy(plotVarDataAverageControlLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlLooseIsoOS, plotVarDataAverageControlLooseIsoOS)

      QCDlooseAverageControlLooseIsoSS=copy.deepcopy(plotVarDataAverageControlLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlLooseIsoSS, plotVarDataAverageControlLooseIsoSS)

      #QCDlooseHighControlLooseIsoOS=copy.deepcopy(plotVarDataHighControlLooseIsoOS.Hist("Data"))
      #substractMCbackground(QCDlooseHighControlLooseIsoOS, plotVarDataHighControlLooseIsoOS)

      #QCDlooseHighControlLooseIsoSS=copy.deepcopy(plotVarDataHighControlLooseIsoSS.Hist("Data"))
      #substractMCbackground(QCDlooseHighControlLooseIsoSS, plotVarDataHighControlLooseIsoSS)

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
      
      if osSSm<osSSmErr/2.:
          osSSm=osSSmErr/2.
      
      if tightLoose<tightLooseErr/2.:
          tightLoose=tightLooseErr/2.
      
      QCDScale=osSSm*tightLoose
      
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
      substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      QCDtightSS=copy.deepcopy(plotVarDataSS.Hist("Data"))
      substractMCbackground(QCDtightSS, plotVarDataSS)

      print "QCDlooseOS:", QCDlooseOS.Integral()
      print "QCDlooseSS:", QCDlooseSS.Integral()
      print "QCDtightSS:", QCDtightSS.Integral()
      tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./abs(QCDtightSS.Integral()) + 1./abs(QCDlooseSS.Integral()))
      print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      if tightLoose<tightLooseErr/2.:
          tightLoose=tightLooseErr/2.
      
      QCDScale=tightLoose
      
      return QCDlooseOS,QCDScale

def QCDEstimate3(prefix,prefix1,xmin,xmax,\
                 plotVarDataLooseIsoOS, plotVarDataLooseIsoSS,\
                 plotVarDataAverageControlSS,\
                 plotVarDataAverageControlLooseIsoSS,\
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
      
      ymax=plotVarDataLooseIsoOS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataLooseIsoOS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataLooseIsoOS.varName+"_qcdLooseOS.png")
      #gPad.WaitPrimitive()
      
      ymax=plotVarDataAverageControlSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataAverageControlSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataAverageControlSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataAverageControlSS.varName+"_qcdAverageControlSS.png")
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


      QCDlooseOS=copy.deepcopy(plotVarDataLooseIsoOS.Hist("Data"))
      substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      QCDlooseAverageControlSS=copy.deepcopy(plotVarDataAverageControlSS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlSS, plotVarDataAverageControlSS)

      QCDlooseAverageControlLooseIsoSS=copy.deepcopy(plotVarDataAverageControlLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseAverageControlLooseIsoSS, plotVarDataAverageControlLooseIsoSS)

      tightLoose=QCDlooseAverageControlSS.Integral()/QCDlooseAverageControlLooseIsoSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./QCDlooseAverageControlSS.Integral() + 1./QCDlooseAverageControlLooseIsoSS.Integral())
      print "QCDlooseAverageControlSS / QCDlooseAverageControlLooseIsoSS", tightLoose, "+-", tightLooseErr
      
      osSSm=QCDlooseOS.Integral()/QCDlooseSS.Integral()
      osSSmErr=osSSm*math.sqrt(1./QCDlooseOS.Integral() + 1./QCDlooseSS.Integral())
      print "QCDlooseOS / QCDlooseSS",osSSm, "+-", osSSmErr
      
      #print "QCDlooseHighControlLooseIsoOS:", QCDlooseHighControlLooseIsoOS.Integral()
      #print "QCDlooseHighControlLooseIsoSS:", QCDlooseHighControlLooseIsoSS.Integral()
      #osSSh=QCDlooseHighControlLooseIsoOS.Integral()/QCDlooseHighControlLooseIsoSS.Integral()
      #osSShErr=osSSh*math.sqrt(1./QCDlooseHighControlLooseIsoOS.Integral() + 1./QCDlooseHighControlLooseIsoSS.Integral())
      #print "QCDlooseHighControlLooseIsoOS / QCDlooseHighControlLooseIsoSS",osSSh, "+-", osSShErr
      
      if osSSm<osSSmErr/2.:
          osSSm=osSSmErr/2.
      
      if tightLoose<tightLooseErr/2.:
          tightLoose=tightLooseErr/2.
      
      QCDScale=osSSm*tightLoose
      
      return QCDlooseSS,QCDScale

def QCDEstimate4(prefix,prefix1,xmin,xmax,\
                 plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataSemiLooseIsoSS,\
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
      
      ymax=plotVarDataSemiLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSemiLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSemiLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSemiLooseIsoSS.varName+"_qcdSemiLooseSS.png")
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
      substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      QCDtightSS=copy.deepcopy(plotVarDataSS.Hist("Data"))
      substractMCbackground(QCDtightSS, plotVarDataSS)

      print "QCDlooseSS:", QCDlooseSS.Integral()
      print "QCDtightSS:", QCDtightSS.Integral()
      tightLoose=QCDtightSS.Integral()/QCDlooseSS.Integral()
      tightLooseErr=tightLoose*math.sqrt(1./abs(QCDtightSS.Integral()) + 1./abs(QCDlooseSS.Integral()))
      print "QCDtightSS / QCDlooseSS", tightLoose, "+-", tightLooseErr
      
      if tightLoose<tightLooseErr/2.:
          tightLoose=tightLooseErr/2.
      
      QCDScale=tightLoose
      
      return QCDlooseOS,QCDScale

def QCDEstimate5(prefix,prefix1,xmin,xmax,\
                 plotVarDataSS, plotVarDataLooseIsoOS, plotVarDataLooseIsoSS, plotVarDataSemiLooseIsoSS,\
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
      
      ymax=plotVarDataSemiLooseIsoSS.Hist("Data").GetMaximum()*1.5
      if log:
          plotVarDataSemiLooseIsoSS.DrawStack("HIST",xmin,xmax,0.1,ymax)
	  gPad.SetLogy()
      else:
          plotVarDataSemiLooseIsoSS.DrawStack("HIST",xmin,xmax,0,ymax)
	  gPad.SetLogy(False)
      gPad.SaveAs(prefix1+prefix+'_'+plotVarDataSemiLooseIsoSS.varName+"_qcdSemiLooseSS.png")
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
      substractMCbackground(QCDlooseOS, plotVarDataLooseIsoOS)

      QCDlooseSS=copy.deepcopy(plotVarDataLooseIsoSS.Hist("Data"))
      substractMCbackground(QCDlooseSS, plotVarDataLooseIsoSS)

      QCDtightSS=copy.deepcopy(plotVarDataSS.Hist("Data"))
      substractMCbackground(QCDtightSS, plotVarDataSS)

      print "QCDlooseSS:", QCDlooseSS.Integral()
      print "QCDlooseOS:", QCDlooseOS.Integral()
      OSSS=QCDlooseOS.Integral()/QCDlooseSS.Integral()
      OSSSErr=OSSS*math.sqrt(1./abs(QCDlooseOS.Integral()) + 1./abs(QCDlooseSS.Integral()))
      print "QCDlooseOS / QCDlooseSS", OSSS, "+-", OSSSErr
      
      if OSSS<OSSSErr/2.:
          OSSS=OSSSErr/2.
      
      QCDScale=OSSS
      
      return QCDtightSS,QCDScale
