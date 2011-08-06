#!/usr/bin/env python

import ROOT
import os,sys
import getopt

def usage() :
    print ' '
    print 'getGammaTemplates.py [options]'
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
cats=['photon20','photon30','photon50','photon75','photon125']
catLabels=['p_{T}^{#gamma}>20 GeV/c',
           'p_{T}^{#gamma}>30 GeV/c',
           'p_{T}^{#gamma}>50 GeV/c',
           'p_{T}^{#gamma}>75 GeV/c',
           'p_{T}^{#gamma}>125 GeV/c']
subcats=['eq0jets','eq1jets','geq2jets','vbf',]
subcatLabels=['=0 jets',
              '=1 jets',
              '#geq 2 jets',
              'VBF']
variables=['met','redmet']

#open file and get plots
f = ROOT.TFile.Open(inputFile)

zpt=f.Get('proc_2/qt_2')
catNorms=[ zpt.Integral( zpt.FindBin(20), zpt.FindBin(30)-1),
           zpt.Integral( zpt.FindBin(30), zpt.FindBin(50)-1),
           zpt.Integral( zpt.FindBin(50), zpt.FindBin(75)-1),
           zpt.Integral( zpt.FindBin(75), zpt.FindBin(125)-1),
           zpt.Integral( zpt.FindBin(125), zpt.GetXaxis().GetNbins() ) ]

setStyle()
paves=[] #this is a hack: make paves persistent (otherwise they run out of scope)
for v in variables :
    cnv = getNewCanvas(v+"c",v+"c",False)
    cnv.Clear()
    cnv.SetWindowSize(900,1200)
    cnv.SetCanvasSize(900,1200)
    cnv.Divide(4,len(cats),0,0)

    gammaTemplate=f.Get('proc_1/'+v+'_1')
    gammaTemplate.SetDirectory(0)
    gammaTemplate.Reset('ICE')
    zVariable=f.Get('proc_2/'+v+'_2') 
    zVariable.SetDirectory(0)
    
    icat=0
    for c in cats:

        iGammaMetInc = f.Get('proc_1/'+c+'_'+v+'_1')
        iGammaMetInc.SetDirectory(0)
        gammaTemplate.Add(iGammaMetInc,catNorms[icat]/iGammaMetInc.Integral()) 
        
        iscat=0
        for sc in subcats:
            p=cnv.cd(1+iscat+4*icat)
            if(iscat==0): p.SetLeftMargin(0.12)
            if(icat==len(cats)-1):p.SetBottomMargin(0.15)

            iGammaMet = f.Get('proc_1/'+c+sc+'_'+v+'_1')
            iGammaMet.SetDirectory(0)
            iZMet = f.Get('proc_2/'+c+sc+'_'+v+'_2')
            iZMet.SetDirectory(0)
            iGammaMet.Draw("hist")
            iZMet.Draw("e2same")
                  
            pave = ROOT.TPaveText(0.5,0.65,1.0,0.95,'NDC')
            pave.SetBorderSize(0)
            pave.SetFillStyle(0)
            pave.SetTextAlign(32)
            pave.SetTextFont(42)
            pave.AddText(catLabels[icat]).SetTextFont(62)
            pave.AddText(subcatLabels[iscat]).SetTextFont(62)
            pave.AddText('')
            #pave.AddLine()
            #pave.AddText('[p-values]')
            #pave.AddText('Kolmogorov %3.3f' % iZMet.KolmogorovTest(iGammaMet) )
            pave.AddText('#chi^{2}/ndof %3.3f' % iZMet.Chi2Test(iGammaMet,'WWCHI2/NDF') )
            pave.Draw()
            paves.append(pave)
            
            p.SetLogy()
            p.Modified()
            p.Update()
            
            iscat=iscat+1
        icat=icat+1
   

    cnv.cd()
    cnv.Modified()
    cnv.Update()
    cnv.SaveAs(v+'.C')
    cnv.SaveAs(v+'.pdf')
    cnv.SaveAs(v+'.png')

    #inclusive template canvas
    cnv2 = getNewCanvas(v+"templc",v+"templc",False)
    cnv2.Clear()
    cnv2.SetWindowSize(600,600)
    cnv2.SetCanvasSize(600,600)
    stack=ROOT.TList()
    stack.Add(gammaTemplate)
    data=ROOT.TList()
    data.Add(zVariable)
    spimpose=ROOT.TList()
    leg=showPlotsAndMCtoDataComparison(cnv2,stack,spimpose,data)
    formatForCmsPublic(cnv2.cd(1),leg,'CMS preliminary',2)
    cnv2.cd()
    cnv2.Modified()
    cnv2.Update()
    cnv2.SaveAs(v+'inc.C')

f.Close()
    
raw_input(' *** Any key to end')


