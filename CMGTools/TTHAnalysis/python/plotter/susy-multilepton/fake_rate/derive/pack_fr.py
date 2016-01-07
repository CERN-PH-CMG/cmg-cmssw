#!/usr/bin/env python

#this has to be in sync with what is defined in bin2Dto1Dlib.cc

bins_pt=(10,15,25,35,50,70)
bins_eta=(0,1,2,2.5)
binning_code=0

import os,sys
sys.argv.append('-b-')
import ROOT
ROOT.gROOT.SetBatch(True)
sys.argv.remove('-b-')
from array import *
ROOT.gStyle.SetPaintTextFormat(".2f")
ROOT.gStyle.SetOptStat(0)

NEWDIR="."

if "/bin2Dto1Dlib_cc.so" not in ROOT.gSystem.GetLibraries():
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/bin2Dto1Dlib.cc+" % os.environ['CMSSW_BASE']);

def makeH2D(name,xedges,yedges):
    return ROOT.TH2F(name,name,len(xedges)-1,array('f',xedges),len(yedges)-1,array('f',yedges))

def fillPlot2D(th2,plot1d):
    for xbin in xrange(1,th2.GetNbinsX()+1):
        for ybin in xrange(1,th2.GetNbinsY()+1):
            xval = th2.GetXaxis().GetBinCenter(xbin)
            yval = th2.GetYaxis().GetBinCenter(ybin)
            xbin1d = int(ROOT.gROOT.ProcessLine("bin2Dto1D(%f,%f,%d);" % (xval,yval,binning_code)))
            th2.SetBinContent(xbin,ybin,plot1d.GetBinContent(xbin1d))
            th2.SetBinError(xbin,ybin,plot1d.GetBinError(xbin1d))

def readPlot1D(th2,filename,plotname):
    slicefile = ROOT.TFile.Open(filename)
    if not slicefile: raise RuntimeError, "Cannot open "+filename
    plot = slicefile.Get(plotname)
    if not plot: 
        slicefile.ls()
        raise RuntimeError, "Cannot find "+plotname+" in "+filename
    fillPlot2D(th2,plot)
    slicefile.Close()

def assemble2D(name,filename,plotname):
    out = ROOT.TFile.Open(NEWDIR+"/"+name+".root","RECREATE")
    out.cd()
    th2 = makeH2D(name,bins_pt,bins_eta)
    try:
        readPlot1D(th2,filename,plotname)
    except RuntimeError:
        print "Impossible to open file "+filename+", skipping..."
        return
    c = ROOT.TCanvas("canv_"+th2.GetName(),"canv_"+th2.GetName())
    c.cd()
    th2.GetZaxis().SetRangeUser(0,0.7)
    th2.GetXaxis().SetTitle("Lepton p_{T} (GeV)")
    th2.GetYaxis().SetTitle("Lepton |#eta|")
    th2.Draw("TEXT COLZ E1")
    c.SaveAs(NEWDIR+"/"+name+".pdf")
    out.WriteTObject(th2)
    out.ls()
    out.Close()
    
if __name__ == "__main__":
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)
    PLOTSPATH="FR"
    PLOTSPREFIX="derive"
    NEWDIR=PLOTSPATH+"_packed"
    os.mkdir(NEWDIR)
    for ob in ["Mu","El"]:
        for ptchoice in ["pt","conept","jetpt"]:
            for insitu in [False,True]:
                if insitu:
                    dset="TT"
                else:
                    if ob=="Mu":
                        dset = "QCD_Mu"
                    else:
                        dset = "QCD_El"
                assemble2D("FR_FO1_%s_eta_%s%s" % (ob,ptchoice,"_insitu" if insitu else ""),"%s/%s_multiiso_ON_FO1%s%s_eta_%s/plots.root" % (PLOTSPATH,PLOTSPREFIX,ob,"InSitu" if insitu else "",ptchoice),"multiiso_eta_%s_%s" % (ptchoice,dset))
                if ob=="El":
                    assemble2D("FR_FO2_%s_eta_%s%s" % (ob,ptchoice,"_insitu" if insitu else ""),"%s/%s_multiiso_AND_elMVAtight_ON_FO2%s%s_eta_%s/plots.root" % (PLOTSPATH,PLOTSPREFIX,ob,"InSitu" if insitu else "",ptchoice),"multiiso_AND_elMVAtight_eta_%s_%s" % (ptchoice,dset))
                assemble2D("FR_FO3_%s_eta_%s%s" % (ob,ptchoice,"_insitu" if insitu else ""),"%s/%s_multiiso_ON_FO3%s%s_eta_%s/plots.root" % (PLOTSPATH,PLOTSPREFIX,ob,"InSitu" if insitu else "",ptchoice),"multiiso_eta_%s_%s" % (ptchoice,dset))
                assemble2D("FR_FO4_%s_eta_%s%s" % (ob,ptchoice,"_insitu" if insitu else ""),"%s/%s_multiiso_ON_FO4%s%s_eta_%s/plots.root" % (PLOTSPATH,PLOTSPREFIX,ob,"InSitu" if insitu else "",ptchoice),"multiiso_eta_%s_%s" % (ptchoice,dset))
