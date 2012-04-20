#!/bin/env python

## --------------------------------------------------------------------------------------------------------------------------------------------
from lip.Tools.plothelper import *
from lip.Tools.rootutils import *
from lip.Tools.roctools import *

from plotPileupJetHistograms import plot_jet_id, mkBinLabels
## from CMGTools.RootTools.utils.PileupJetHistograms import mkBinLabels
import os

## --------------------------------------------------------------------------------------------------------------------------------------------
def main(indir,input,outdir):

    ## initialize ROOT style and set-up the html helper
    setStyle()
    if not os.path.isdir(outdir):
        os.mkdir(outdir)
    hth = HtmlHelper(outdir)
    hth.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")  
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("..").set("href","../?C=M;O=D")

    inputs = [ i.split(":") for i in input.split(",") if ":" in i  ]
    from binning import vtxlabels, ptbins, ptlabels, etalables

    helper = PlotHelper( [ { 
        "file":"%s/%s" % (indir,file), "dir":var, "id":var,
        "cat":vtx,   "label": "%s" % (lab), "nostack" : 1 } for vtx in vtxlabels[-1:] for file,var,lab in inputs ] )
    
    helper.defaultStyles = [
        [ (setcolors,ROOT.kRed),  ("SetMarkerStyle",ROOT.kFullCircle),       ], 
        [ (setcolors,ROOT.kBlack),  ("SetMarkerStyle",ROOT.kFullTriangleDown), ], 
        [ (setcolors,ROOT.kMagenta),  ("SetMarkerStyle",ROOT.kOpenDiamond),      ], 
        [ (setcolors,ROOT.kGreen),  ("SetMarkerStyle",ROOT.kFullStar),         ], 
        [ (setcolors,ROOT.kBlue), ("SetMarkerStyle",ROOT.kOpenTriangleUp),   ], 
        [ (setcolors,ROOT.kOrange),   ("SetMarkerStyle",ROOT.kOpenCircle),       ], 
        [ (setcolors,ROOT.kOrange-4),  ("SetMarkerStyle",ROOT.kOpenStar),         ], 
        [ (setcolors,ROOT.kOrange+2),  ("SetMarkerStyle",ROOT.kOpenStar),         ], 
        ]

    histos_to_read = [ ("%s%s%s" % (eta,"%(cat)s",pt), [], "%s%s" % (eta,pt))  for eta in etalables for pt in ptlabels ]
    helper.read(histos_to_read)

    canvstyle = []
    lab_style = [ ("SetFillColor",kWhite), ("SetShadowColor",kWhite), ("SetLineColor",kWhite) ]
    
    helper.nodisc = TF1("nodisc","1. - x",0.,1.)
    tab = hth.body().add( HtmlTable() )

    page_plots = []
    for eta in etalables:
        for pt in ptlabels:
            lab = TPaveText(0.1,0.95,0.4,1.0,"brNDC")
            apply_modifs(lab,lab_style)
            rng = pt.replace("_pt","").split("_")
            lab.AddText("%s %s<p_{T}<%s" % ( eta, rng[0], rng[1] )  ) 
            lab.Draw("SAME")
            page_plots += [ (["%s%s" % (eta,pt)], "histo", "pe", canvstyle,  (0.2,0.2,0.5,0.5),   0.5, "",  [lab]) ]
        page_plots += [ () ]
            
    plot_page(page_plots,tab,helper,None,[helper.nodisc],plots_per_row=3)
    
    ## done: create images and html 
    hth.dump()

    print outdir
    
## --------------------------------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    from sys import argv
    from ROOT import gROOT
    gROOT.SetBatch(True)

    loadToolsLib()
    from ROOT import setStyle, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TF1, TFile, TPaveText, kWhite

    main(*argv[1:])
