import os,sys
import getopt

def usage() :
      print ' '
      print 'runEstimatePileup.py [options]'
      print '   -j json file with lumi info'
      print '   -o output file'

#parse the options
try:
  # retrive command line options
  shortopts  = "j:o:h?"
  opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
  # print help information and exit:
  print "ERROR: unknown options in argument %s" % sys.argv[1:]
  usage()
  sys.exit(1)

#get the options
for o,a in opts:
  if o in("-?", "-h"):
    usage()
    sys.exit(1)
  elif o in('-j'):
    jsonfile = a
    print "Running lumiCalc"
    os.system('lumiCalc.py -c frontier://LumiCalc/CMS_LUMI_PROD -i ' + jsonfile + ' --nowarning overview > ' + output + '_lumi.txt')
    print "Running estimatePileup"
    os.system('estimatePileup.py --maxPileupBin=25 --saveRuns -i '+ jsonfile + ' ' + output + ".root")    
  elif o in('-o'): output=a
  

#run lumicalc

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



