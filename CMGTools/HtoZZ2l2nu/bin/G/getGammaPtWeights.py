#!/usr/bin/env python

import ROOT
import os,sys
import getopt

def usage() :
    print ' '
    print 'getGammaPtWeights.py [options]'
    print '  -i : input file'
    print ' '
    
#parse the options
try:
    # retrive command line options
    shortopts  = "i:h?"
    opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
    # print help information and exit:
    print "ERROR: unknown options in argument %s" % sys.argv[1:]
    usage()
    sys.exit(1)

inputFile='plotter.root'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-i'): inputDir = ''
                        
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import showPlotsAndMCtoDataComparison, formatForCmsPublic, getNewCanvas, setStyle
                                                                     
#plots to retrieve
cats=['photon20','photon30','photon50','photon75','photon90','photon125']
gammaPt = None
zPt = None

#open file and get plots
f = ROOT.TFile.Open(inputFile)
for c in cats:
    iGammaPt = f.Get('proc_1/'+c+'_qt_1')
    iZPt = f.Get('proc_2/'+c+'_qt_2')
    if(gammaPt is None) :
        gammaPt = iGammaPt.Clone('gamma_pt')
        zPt = iZPt.Clone('z_pt')
    else :
        gammaPt.Add(iGammaPt)
        zPt.Add(iZPt)
gammaPt.SetDirectory(0)
zPt.SetDirectory(0)
ptWeights=zPt.Clone('gammaptweight')
ptWeights.Divide(gammaPt)
ptWeights.SetDirectory(0)
f.Close()

#show the output
setStyle()
c = getNewCanvas("gammac","gammac",False)
c.SetWindowSize(600,600)
c.SetCanvasSize(600,600)

#show
stack=ROOT.TList()
stack.Add(gammaPt)
data=ROOT.TList()
data.Add(zPt)
spimpose=ROOT.TList()
leg=showPlotsAndMCtoDataComparison(c,stack,spimpose,data)
formatForCmsPublic(c.cd(1),leg,'CMS preliminary',2)
c.cd()
c.Modified()
c.Update()
raw_input(' *** Any key to end')

#save
fout = ROOT.TFile.Open('gammaptweight.root','RECREATE')
fout.cd()
ptWeights.Write()
c.Write()
fout.Close()

print ' *** Gamma pT weights available @ gammaptweight.root'

