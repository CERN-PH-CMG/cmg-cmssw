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
    elif o in('-i'): inputFile = a
                        
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import showPlots, formatForCmsPublic, getNewCanvas, setStyle
                                                                     
#plots to retrieve
#cats=['photon20','photon30','photon50','photon75','photon90','photon125']
cats=['photon20','photon30','photon50','photon75','photon125']
gammaPt = None
zPt = None
zTauTauPt = None

#open file and get plots
f = ROOT.TFile.Open(inputFile)
for c in cats:
    iGammaPt = f.Get('proc_3/'+c+'_qtvsnjets_3')
    iZPt = f.Get('proc_1/'+c+'_qtvsnjets_1')
    iZTauTauPt = f.Get('proc_2/'+c+'_qtvsnjets_2')
    if(gammaPt is None) :
        gammaPt = iGammaPt.Clone('gamma_pt')
        zPt = iZPt.Clone('z_pt')
        zTauTauPt = iZTauTauPt.Clone('ztautau_pt')
    else :
        gammaPt.Add(iGammaPt)
        zPt.Add(iZPt)
        zTauTauPt.Add(iZTauTauPt)
gammaPt.SetDirectory(0)
zPt.SetDirectory(0)
zTauTauPt.SetDirectory(0)
f.Close()

#re-scale Z->ll and gamma to 1

gammaPt.Scale(1./gammaPt.Integral())
totalzll=zPt.Integral()
#totalztautau=zTauTauPt.Integral()
zPt.Scale(1./totalzll)
zTauTauPt.Scale(1./totalzll)

ptWeights=zPt.Clone('gammaptweight')
ptWeights.Divide(gammaPt)
ptWeights.SetDirectory(0)
ptWeights.GetYaxis().SetTitle("p_{T}^{Z#rightarrow ll}/p_{T}^{#gamma}")


#show the output
setStyle()
c = getNewCanvas("gammac","gammac",False)
c.SetWindowSize(600,600)
c.SetCanvasSize(600,600)

#show
stack=ROOT.TList()
stack.Add(zPt)
stack.Add(zTauTauPt)
data=ROOT.TList()
data.Add(gammaPt)
spimpose=ROOT.TList()
leg=showPlots(c,stack,spimpose,data)
formatForCmsPublic(c.cd(1),leg,'CMS preliminary',2)
c.cd(2)
ptWeights.Draw("colztext")
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

