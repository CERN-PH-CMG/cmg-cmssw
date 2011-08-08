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
from ROOT import showPlotsAndMCtoDataComparison, formatForCmsPublic, getNewCanvas, setStyle
                                                                     
#plots to retrieve
#cats=['photon20','photon30','photon50','photon75','photon90','photon125']
cats=['photon20','photon30','photon50','photon75','photon125']
gammaPt = []
zPt = []
#zTauTauPt = []
#open file and get plots
f = ROOT.TFile.Open(inputFile)
for c in cats:
    iGammaPt   = [ f.Get('proc_3/'+c+'_qt_3') , f.Get('proc_3/'+c+'_qtvsnjets_3') ]
    totalGamma=iGammaPt[0].Integral()
    iZPt       = [ f.Get('proc_1/'+c+'_qt_1') , f.Get('proc_1/'+c+'_qtvsnjets_1') ]
    totalZ2ll=iZPt[0].Integral()
#    iZTauTauPt = [ f.Get('proc_2/'+c+'_qt_2') , f.Get('proc_2/'+c+'_qtvsnjets_2') ]
    for i in xrange(0,2): 
        if(len(gammaPt)<=i):
            gammaPt.append( iGammaPt[i].Clone('gamma_pt_'+str(i)) )
            gammaPt[i].Reset("ICE")
            gammaPt[i].SetDirectory(0)
            zPt.append( iZPt[i].Clone('z_pt'+str(i)) )
            zPt[i].Reset("ICE")
            zPt[i].SetDirectory(0)
#            zTauTauPt.append( iZTauTauPt[i].Clone('ztautau_pt'+str(i)) )
#            zTauTauPt[i].Reset("ICE")
#            zTauTauPt[i].SetFillStyle(0)
#            zTauTauPt[i].SetDirectory(0)
        gammaPt[i].Add(iGammaPt[i],1./totalGamma)
        zPt[i].Add(iZPt[i],1./totalZ2ll)
#        zTauTauPt[i].Add(iZTauTauPt[i],1./totalZ2ll)
f.Close()

#compute the weights
ptWeights=[]
for i in xrange(0,2):
    ptWeights.append( zPt[i].Clone('gammaptweight_'+str(i)) )
    ptWeights[i].Divide(gammaPt[i])
    ptWeights[i].SetDirectory(0)
    ptWeights[i].GetYaxis().SetTitle("p_{T}^{Z#rightarrow ll}/p_{T}^{#gamma}")

    for ibin in xrange(2,ptWeights[i].GetXaxis().GetNbins()+1) :
        if(i==0) :
            binc=ptWeights[i].GetBinContent(ibin)
            if(binc<=0):
                newbinc=ptWeights[i].GetBinContent(ibin-1)
                newbinerr=ptWeights[i].GetBinError(ibin-1)
                ptWeights[i].SetBinContent(ibin,newbinc)
                ptWeights[i].SetBinError(ibin,newbinerr)
        else :
            for jbin in xrange(1,ptWeights[i].GetYaxis().GetNbins()+1) :
                binc=ptWeights[i].GetBinContent(ibin,jbin)
                if(binc<=0):
                    newbinc=ptWeights[i].GetBinContent(ibin-1,jbin)
                    newbinerr=ptWeights[i].GetBinError(ibin-1,jbin)
                    ptWeights[i].SetBinContent(ibin,jbin,newbinc)
                    ptWeights[i].SetBinError(ibin,jbin,newbinerr)

#display results
setStyle()

#show the inclusive output
c = getNewCanvas("gammac","gammac",False)
c.SetWindowSize(600,600)
c.SetCanvasSize(600,600)
stack=ROOT.TList()
stack.Add(gammaPt[0])
data=ROOT.TList()
data.Add(zPt[0])
spimpose=ROOT.TList()
leg=showPlotsAndMCtoDataComparison(c,stack,spimpose,data)
formatForCmsPublic(c.cd(1),leg,'CMS preliminary',2)
#c.cd(2)
#ptWeights[0].Draw("e1")
#yscale=0.7/0.28
#ptWeights[0].GetYaxis().SetRangeUser(0,5.3);
#ptWeights[0].GetXaxis().SetTitleOffset(0.85);
#ptWeights[0].GetXaxis().SetLabelSize(0.04 * yscale);
#ptWeights[0].GetXaxis().SetTitleSize(0.05 * yscale);
#ptWeights[0].GetXaxis().SetTickLength( 0.03 * yscale );
#ptWeights[0].GetYaxis().SetTitleOffset(0.5);
#ptWeights[0].GetYaxis().SetLabelSize(0.04 * yscale);
#ptWeights[0].GetYaxis().SetTitleSize(0.04 * yscale);
c.cd()
c.Modified()
c.Update()

#sgow the differential weights
cdiff = getNewCanvas("diffgammac","diffgammac",False)
cdiff.SetWindowSize(500,500)
cdiff.SetCanvasSize(500,500)
ptWeights[1].Draw("colztext")
cdiff.cd()
cdiff.Modified()
cdiff.Update()

#save
raw_input(' *** Any key to end')
fout = ROOT.TFile.Open('gammaptweight.root','RECREATE')
fout.cd()
ptWeights[0].Write()
ptWeights[1].Write()
c.Write()
cdiff.Write()
fout.Close()

print ' *** Gamma pT weights available @ gammaptweight.root'

