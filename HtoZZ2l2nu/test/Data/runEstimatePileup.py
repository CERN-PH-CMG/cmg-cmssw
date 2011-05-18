import os,sys

output=sys.argv[1]
if(len(sys.argv)>2):
  jsonfile = sys.argv[2]
  print "Running lumiCalc"
  os.system('lumiCalc.py -c frontier://LumiCalc/CMS_LUMI_PROD -i ' + jsonfile + ' --nowarning overview > ' + output + '_lumi.txt')
  print "Running estimatePileup"
  os.system('estimatePileup.py --maxPileupBin=20 --saveRuns -i '+ jsonfile + ' ' + output + ".root")

import ROOT
from ROOT import TFile,TGraphAsymmErrors
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import setStyle, getNewCanvas, formatPlot, formatForCmsPublic, showPlots

print "Drawing graphs"
evol=TGraphAsymmErrors()
file = TFile(output+".root")
runPileup=None
ipoint=0
for key in file.GetListOfKeys() :
  kname=key.GetName()
  if(kname.find('pileup_')<0): continue
  runnb=int(kname.split('_')[1])
  h=file.Get(kname)
  avgpileup=h.GetMean()
  rmspileup=h.GetRMS()
  evol.SetPoint(ipoint,runnb,avgpileup)
  evol.SetPointError(ipoint,0,0,rmspileup,rmspileup)
  ipoint=ipoint+1
  if(runPileup is None) :
    runPileup = h.Clone('runPileup')
    formatPlot( runPileup, 1,1,1,20,0,True,True,1,1,1)
file.Close()

setStyle()
c = getNewCanvas("evolc","evolc",False)
c.SetWindowSize(1200,600)
c.Divide(2,1)
c.cd(1)
evol.Draw('ap')
c.cd(2)
runPileup.DrawNormalized('hist')
c.SaveAs(output + "_summary.C")
c.SaveAs(output + "_summary.root")
print "Average pileup: " + str(runPileup.GetMean()) + ' +/- ' + str(runPileup.GetMeanError())



