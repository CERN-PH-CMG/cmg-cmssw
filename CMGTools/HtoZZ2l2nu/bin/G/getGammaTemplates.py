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

setStyle()
paves=[] #this is a hack: make paves persistent (otherwise they run out of scope)
for v in variables :
    cnv = getNewCanvas(v+"c",v+"c",False)
    cnv.Clear()
    cnv.SetWindowSize(900,1200)
#    cnv.SetCanvasSize(900,1200)
    cnv.Divide(4,len(cats),0,0)

    gammaVariable=[]
    zVariable=[]
    zTauTauVariable=[]
    icat=0
    for c in cats:
        
        iscat=0
        for sc in subcats:
            p=cnv.cd(1+iscat+4*icat)
            if(iscat==0):              p.SetLeftMargin(0.12)
            if(iscat==len(subcats)-1): p.SetRightMargin(0.05)
            if(icat==len(cats)-1):     p.SetBottomMargin(0.15)

            iGammaVariable = f.Get('proc_3/'+c+sc+'_'+v+'_3')
            iGammaVariable.SetDirectory(0)
            iZVariable = f.Get('proc_2/'+c+sc+'_'+v+'_2')
            iZVariable.SetDirectory(0)
            iZTauTauVariable = f.Get('proc_1/'+c+sc+'_'+v+'_1')
            iZTauTauVariable.SetDirectory(0)

            iGammaVariable.Scale(iZVariable.Integral()/iGammaVariable.Integral())
            iZVariable.Draw("hist")
            iGammaVariable.Draw("e2same")

            if(len(gammaVariable)<=iscat) :
                gammaVariable.append( iGammaVariable.Clone(sc+'_gamma_'+v) )
                gammaVariable[iscat].Reset("ICE")
                gammaVariable[iscat].SetDirectory(0)
                zVariable.append( iZVariable.Clone(sc+'_z_'+v) )
                zVariable[iscat].Reset("ICE")
                zVariable[iscat].SetDirectory(0)
                zTauTauVariable.append( iZTauTauVariable.Clone(sc+'_ztautau_'+v) )
                zTauTauVariable[iscat].Reset("ICE")
                zTauTauVariable[iscat].SetDirectory(0)
            gammaVariable[iscat].Add(iGammaVariable)
            zVariable[iscat].Add(iZVariable)
            zTauTauVariable[iscat].Add(iZTauTauVariable)
            
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
            #pave.AddText('Kolmogorov %3.3f' % iZVariable.KolmogorovTest(iGammaVariable) )
            pave.AddText('#chi^{2}/ndof %3.3f' % iZVariable.Chi2Test(iGammaVariable,'WWCHI2/NDF') )
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
#    cnv2.SetCanvasSize(1200,300)
    cnv2.SetWindowSize(1200,300)
    cnv2.Divide(4,1,0,0)
    for i in xrange(1,5): 
        stack=ROOT.TList()
        stack.Add(zVariable[i-1])    
        data=ROOT.TList()
        data.Add(gammaVariable[i-1])
        spimpose=ROOT.TList()
        #zTauTauVariable[i-1].Rebin(2)
        zTauTauVariable[i-1].SetFillStyle(3004)
        #zTauTauVariable[i-1].SetLineWidth(2)
        spimpose.Add(zTauTauVariable[i-1])
        pad=cnv2.cd(i)
        leg=showPlotsAndMCtoDataComparison(pad,stack,spimpose,data)
        if(i==1) :
            formatForCmsPublic(pad.cd(1),leg,'CMS preliminary',2)
        else : leg.Delete()

        pad.cd(1)
        pave = ROOT.TPaveText(0.65,0.75,0.95,0.95,'NDC')
        pave.SetBorderSize(0)
        pave.SetFillStyle(0)
        pave.SetTextAlign(32)
        pave.SetTextFont(42)
        pave.AddText(subcatLabels[i-1]).SetTextFont(62)
        pave.AddText('')
        pave.AddLine()
        #pave.AddText('[p-values]')
        #pave.AddText('Kolmogorov %3.3f' % iZVariable.KolmogorovTest(iGammaVariable) )
        pave.AddText('#chi^{2}/ndof %3.3f' % zVariable[i-1].Chi2Test(gammaVariable[i-1],'WWCHI2/NDF') )
        pave.Draw()
        paves.append(pave)


    cnv2.cd()
    cnv2.Modified()
    cnv2.Update()
    cnv2.SaveAs(v+'inc.C')

f.Close()
    
raw_input(' *** Any key to end')


