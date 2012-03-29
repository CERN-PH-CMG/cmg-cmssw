#!/bin/env python

## --------------------------------------------------------------------------------------------------------------------------------------------
from lip.Tools.plothelper import *
from lip.Tools.rootutils import *
from lip.Tools.roctools import *

from CMGTools.RootTools.utils.PileupJetHistograms import mkBinLabels

## --------------------------------------------------------------------------------------------------------------------------------------------
def normalize(h):
    h.Scale( 1./h.Integral() ) 

## --------------------------------------------------------------------------------------------------------------------------------------------
def appendtitle(h,tit):
    h.SetTitle( h.GetTitle()+tit )

## --------------------------------------------------------------------------------------------------------------------------------------------
def plot_kin(infile,hth):

    ## instantiate plot helper
    helper = PlotHelper( [ { "file":infile, "dir":"MatchedCleanJets",   "id":"MatchedCleanJets",   "label":"NoPU", "nostack" : 1 },
                           { "file":infile, "dir":"UnmatchedCleanJets", "id":"UnmatchedCleanJets", "label":"PU",   "nostack" : 1 },
                           ## { "file":infile, "dir":"QuarkMatchedCleanJets",   "id":"QuarkMatchedCleanJets",   "label":"u,d,s",  },
                           ## { "file":infile, "dir":"GluonMatchedCleanJets",   "id":"GluonMatchedCleanJets",   "label":"g", },
                           ]
                         ) 

    ## plots and canvas styles. functions in the list are successively applied
    helper.defaultStyles = [
        [ (setcolors,ROOT.kGreen+2), ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenDiamond)     ],
        [ (setcolors,ROOT.kRed+2),   ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenTriangleUp)  ],
        [ (setcolors,ROOT.kBlue+2),  ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenDiamond)     ],
        [ (setcolors,ROOT.kOrange+2), ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenDiamond)     ],
        ]
    canvstyle = []

    ## histograms and plots
    variables = ["pt","eta","deltaJetMatch"]

    histos_to_read = [ ( "%s_h_%s" % ("%(id)s",var), [ normalize, (xtitle,var) ], var ) for var in variables ]
    plots = [ ( [var], "histo", "pe", canvstyle,  (0.5,0.6,0.8,0.9),   0.5, "kin_%s" % var ) for var in variables ]

    ## read the histograms
    helper.read(histos_to_read)

    try:
        ## pupt = helper.histos["PU"]["eta"]
        ## nopupt = helper.histos["NoPU"]["eta"]
        pupt = helper.histos["PU"]["pt"]
        nopupt = helper.histos["NoPU"]["pt"]
        
        helper.reweight = pupt.Clone("reweight")
        helper.reweight.Divide(pupt,nopupt)
        helper.reweight_f = ROOT.TF1("f","pol2(0)+expo(3)",20,100.)
        helper.reweight.Fit(helper.reweight_f,"R+")
    except:
        pass

    ## make plots
    tab = hth.body().add( HtmlTable() )
    plot_page(plots,tab,helper,None)

    canv = ROOT.TCanvas("reweight","reweight",800,600)
    canv.cd()
    helper.reweight.Draw()    
    tab.row().cell( HtmlPlot(canv,False,"",True,True,True) )

    return helper

## --------------------------------------------------------------------------------------------------------------------------------------------
def plot_jet_id(infile,hth, 
                variables_to_plot=[("nvtx",),
                                   ("jetPt","jetEta","jetPhi"),
                                   ("nParticles","nCharged", "nNeutrals"),
                                   ("leadFrac","secondFrac","thirdFrac","fourthFrac"),
                                   ("leadChFrac","secondChFrac","thirdChFrac","fourthChFrac"),
                                   ("leadNeutFrac","secondNeutFrac","thirdNeutFrac","fourthNeutFrac"),
                                   ("leadEmFrac","secondEmFrac","thirdEmFrac","fourthEmFrac"),
                                   ("etaW","phiW",## "jetW",
                                    "majW","minW"),
                                   ("dRMean","dRLeadCent","dRLead2nd"),
                                   ("dRMeanNeut","dRMeanEm","dRMeanCh"),
                                   ("frac01","frac02","frac03","frac04","frac05"),
                                   ("chFrac01","chFrac02","chFrac03","chFrac04","chFrac05"),
                                   ("neutFrac01","neutFrac02","neutFrac03","neutFrac04","neutFrac05"),
                                   ("emFrac01","emFrac02","emFrac03","emFrac04","emFrac05"),
                                   ("ptD",),
                                   ("d0","dZ"),
                                   ],
                helper_inputs =    [],
                vtxlabels = [ "_vtx%s" % l for l in  mkBinLabels((15,20,30),addOFlow=True) ],
                ptbins = [],
                etalables = ["central","endNOtk","fwd"]
                ):
    from array import array
    from ROOT import setStyle, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TF1

    ## etalables = ["endNOtk"]
    if len(helper_inputs) == 0:
        helper_inputs = [ { 
            "file":infile, "dir":"UnmatchedCleanHistosId", "id":"UnmatchedCleanHistosId",
            "cat": vtx,  "label":"PU %s < N_{vtx} < %s" % tuple(vtx.replace("_vtx","").split("_")), "nostack" : 1 } for vtx in vtxlabels ] + [ { 
            "file":infile, "dir":"QuarkMatchedCleanHistosId",   "id":"QuarkMatchedCleanHistosId",
            "cat": vtx,  "label":"u,d,s %s < N_{vtx} < %s"   % tuple(vtx.replace("_vtx","").split("_")), "nostack" : 1 } for vtx in vtxlabels ] + [ { 
            "file":infile, "dir":"GluonMatchedCleanHistosId",   "id":"GluonMatchedCleanHistosId",
            "cat": vtx,  "label":"glu %s < N_{vtx} < %s"   % tuple(vtx.replace("_vtx","").split("_")), "nostack" : 1 } for vtx in vtxlabels ]    
    if len(ptbins) == 0:
        ptbins = (20,30,50)
    ptlabels  = [ "_pt%s" % l for l in mkBinLabels(ptbins,addOFlow=True) ]
        
    ## instantiate plot helper
    helper = PlotHelper( 
        helper_inputs
        ) 
    helper.defaultStyles = [
        [ (setcolors,ROOT.kRed-4+2*i),   ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenTriangleUp)  ]  for i in range(len(vtxlabels))
        ]  + [
        [ (setcolors,ROOT.kBlue-4+2*i),   ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenTriangleUp)  ]  for i in range(len(vtxlabels)) 
        ]  + [
        [ (setcolors,ROOT.kMagenta-4+2*i),   ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kOpenTriangleUp)  ]  for i in range(len(vtxlabels)) 
        ] 

    canvstyle = []
    
    ## list of variables to plot: one row per tuple
    #### variables_to_plot=[ 
    ####     ("mva",),
    ####     ("jetPt","jetEta","jetPhi"),
    ####     ("nParticles","nCharged", "nNeutrals"),
    ####     ("leadFrac","secondFrac","thirdFrac","fourthFrac"),
    ####     ("leadChFrac","secondChFrac","thirdChFrac","fourthChFrac"),
    ####     ("leadNeutFrac","secondNeutFrac","thirdNeutFrac","fourthNeutFrac"),
    ####     ("leadEmFrac","secondEmFrac","thirdEmFrac","fourthEmFrac"),
    ####     ("etaW","phiW",## "jetW",
    ####      "majW","minW"),
    ####     ("dRMean","dRLeadCent","dRLead2nd"),
    ####     ("dRMeanNeut","dRMeanEm","dRMeanCh"),
    ####     ("frac01","frac02","frac03","frac04","frac05"),
    ####     ("chFrac01","chFrac02","chFrac03","chFrac04","chFrac05"),
    ####     ("neutFrac01","neutFrac02","neutFrac03","neutFrac04","neutFrac05"),
    ####     ("emFrac01","emFrac02","emFrac03","emFrac04","emFrac05"),
    ####     ("ptD",),
    ####     ("d0","dZ"),
    ####                     ]

    ## compile the list of histograms to be read  and set the layout of the output web page 
    histos_to_read = []
    page_template  = []
    roc_page_template  = []
    for variabs in variables_to_plot:
        ## each tuple corresponds to one histogram
        ##   (histo name, list of modifiers, new name)
        histos_to_read += [ ("%s_%s%s%s_%s" % ("%(id)s",eta,"%(cat)s",pt,var),
                             [ normalize, (appendtitle," %s < p_{T} < %s" % tuple(pt.replace("_pt","").split("_")) ) ],
                             "%s%s_%s" % (eta,pt,var) )
                            for var in variabs for eta in etalables for pt in ptlabels
                            ]
        for pt in ptlabels:
            ## each tuple in the list corresponds to a plot
            ##  ( list_of_histograms, MC draw option, data draw option, canvas style, legend constructor arguments, y-scale adjustment, canvas name )
            page_template += [
                (["%s_%s" % (pt,var)], "histo", "pe", canvstyle,  (0.5,0.6,0.8,0.9),   0.5, "" ) 
                for var in variabs 
                ]
            ## an empty tuple breaks the line
            page_template.append( () )
        ## ROC integrals
        roc_page_template += [
            (["_roc_integrals_%s" % var], "nostackl", "pe", canvstyle+[("SetGridx",True),("SetGridy",True)],  (0.5,0.7,0.8,0.9),   None, "rocint_%s" % var ) 
            for var in variabs 
            ]
        roc_page_template.append( () )
        
    ## read the histograms
    helper.read(histos_to_read)

    ## compute the ROC curves
    rochelper = PlotHelper([])
    rochelper.defaultStyles = [
        [ (setcolors,ROOT.kBlue-8+2*i),   ("SetFillStyle",0), ("SetLineWidth",2), ("SetMarkerStyle",ROOT.kFullCircle+i)  ]  for i in range(len(vtxlabels))
        ]
    rochelper.optsMap["nostackl"] = "l"
    
    for vtx in vtxlabels:
        lab = "%s < N_{vtx} < %s" % tuple(vtx.replace("_vtx","").split("_"))
        for eta in etalables:
            for variabs in variables_to_plot:
                for var in variabs:
                    iname = "%s_roc_integrals%s_%s" % (eta,vtx,var)
                    integrals = TH1F(iname, "%s %s; p_{T} (GeV/c);" % (var,vtx), len(ptbins), array('d',ptbins+(100.,)) )
                    integrals.GetYaxis().SetRangeUser(0.,1.2)
                    rochelper.add(integrals,"%s_roc_integrals_%s" % (eta,var),lab)
                    for ipt in range(len(ptlabels)):
                        hname =  "%s%s_%s" % (eta,ptlabels[ipt],var)
                        sig = helper.histos["u,d,s %s" % lab][hname]
                        bkg = helper.histos["PU %s" % lab][hname]
                        roc = ROCBuilder("%s%s" % (hname,vtx),str(sig.GetTitle()).replace("NoPU",sig.GetXaxis().GetTitle()),sig,bkg).getRoc()
                        rochelper.add(roc,hname,lab)
                        integrals.SetBinContent(ipt+1,roc.Integral("width"))
            
    ## prepare one page per eta region
    rochelper.nodisc = TF1("nodisc","1. - x",0.,1.)
    rochelper.nodisc_int = TF1("nodisc_int","0.5",0.,1000.)
    rochelper.full_nodisc_int = TF1("full_nodisc_int","1.0",0.,1000.)
    for eta in etalables:
        page = hth.addPage(eta)
        page.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")
        page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
        page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
        tab = page.body().add( HtmlTable() )

        roc_page = hth.addPage("ROC_%s" % eta)
        roc_page.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")
        roc_page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
        roc_page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
        roc_tab = roc_page.body().add( HtmlTable() )

        rocint_page = hth.addPage("ROCIntegrals_%s" % eta)
        rocint_page.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")
        rocint_page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
        rocint_page.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
        rocint_tab = rocint_page.body().add( HtmlTable() )

        ## build the list of plots from the template
        page_plots = []
        roc_plots  = []
        rocint_plots  = []
        for p in page_template:
            if len(p) > 0:
                page_plots.append( ( map(lambda x: eta+x, p[0]), )+p[1:] )
                roc_plots.append( ( map(lambda x: eta+x, p[0]), )+("nostackl",p[2],p[3]+[("SetGridx",True),("SetGridy",True)])+p[4:5]+(1.5,"roc_%s" % eta+p[0][0]) )
            else:
                page_plots.append(p)
                roc_plots.append(p)
        for p in roc_page_template:
            if len(p) > 0:
                rocint_plots.append( ( map(lambda x: eta+x, p[0]), )+p[1:6]+(eta+p[6],) )
            else:
                rocint_plots.append(p)
                
        ## generate the plots
        plot_page(page_plots,tab,helper,None,plots_per_row=5)
        for k,v in rochelper.histos.iteritems():
            print k ## , v.keys()
            
        plot_page(roc_plots,roc_tab,rochelper,None,[rochelper.nodisc],plots_per_row=5)## ,html_plot_options=(False,"",True,True,False))
        plot_page(rocint_plots,rocint_tab,rochelper,None,[rochelper.nodisc_int],plots_per_row=5)
        
 
    return helper,rochelper

## --------------------------------------------------------------------------------------------------------------------------------------------
def main(infile,outdir):

    ## initialize ROOT style and set-up the html helper
    setStyle()
    hth = HtmlHelper(outdir)
    hth.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")  
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("..").set("href","../?C=M;O=D")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("kinematics").set("href","./")

    kh = plot_kin(infile,hth)

    ih = plot_jet_id(infile,hth)
    
    ## done: create images and html 
    hth.dump()
    
    ###  reweightX = [ kh.reweight.GetBinLowEdge(i) for  i in range(1,kh.reweight.GetNbinsX())]
    ###  reweightY = [ kh.reweight.GetBinContent(i) for  i in range(1,kh.reweight.GetNbinsX())]
    ###  print reweightX
    ###  print reweightY
    
    print kh.reweight_f.GetExpFormula("p")
    

## --------------------------------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    from sys import argv
    from ROOT import gROOT
    gROOT.SetBatch(True)

    loadToolsLib()
    from ROOT import setStyle, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TF1

    main(*argv[1:])
