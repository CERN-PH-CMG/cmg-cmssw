#!/bin/env python

## --------------------------------------------------------------------------------------------------------------------------------------------
from lip.Tools.plothelper import *
from lip.Tools.rootutils import *
from lip.Tools.roctools import *

from plotPileupJetHistograms import plot_jet_id
from CMGTools.RootTools.utils.PileupJetHistograms import mkBinLabels

## --------------------------------------------------------------------------------------------------------------------------------------------
def main(infile,outdir,label,variables=""):

    ## initialize ROOT style and set-up the html helper
    setStyle()
    hth = HtmlHelper(outdir)
    hth.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")  
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("..").set("href","../?C=M;O=D")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("kinematics").set("href","./")

    vtxlabels = [ "_vtx%s" % l for l in  mkBinLabels((1,10,20),addOFlow=True) ]
    ptbins = (10,20,30)
    ptlabels  = [ "_pt%s" % l for l in mkBinLabels((10,20,30),addOFlow=True) ]
    helper_inputs = [ { 
        "file":infile, "dir":"background", "id":"background",
        "cat":vtx,   "label":"PU %s < N_{vtx} < %s" % tuple(vtx.replace("_vtx","").split("_")), "nostack" : 1 } for vtx in vtxlabels ] + [ { 
        "file":infile, "dir":"signal",   "id":"signal",
        "cat": vtx,  "label":"u,d,s %s < N_{vtx} < %s"   % tuple(vtx.replace("_vtx","").split("_")), "nostack" : 1 } for vtx in vtxlabels ]
    
    ih = plot_jet_id(infile,hth,
                     variables_to_plot = [ ("dRMean","dR2Mean","jetW","nParticles"),
                                           (v for v in variables.split(':') if v != "" and v != "dRMean" and v != "dR2Mean" ),
                                           ("Fisher_%s" % label,"BDT_%s" % label)
                                           ],
                     helper_inputs = helper_inputs,
                     vtxlabels = vtxlabels,
                     ptbins = ptbins,
                     etalables = ["central","endNOtk","fwd"]
                     )
    
    ## done: create images and html 
    hth.dump()
    

## --------------------------------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    from sys import argv
    from ROOT import gROOT
    gROOT.SetBatch(True)

    loadToolsLib()
    from ROOT import setStyle, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TF1

    main(*argv[1:])
