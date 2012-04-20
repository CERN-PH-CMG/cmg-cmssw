#!/usr/bin/env python

from optparse import OptionParser, make_option
from CMG.JetIDAnalysis.ntupletools import *
from classify import getTMVASettings

def main(options, args):
    from ROOT import gROOT
    gROOT.SetBatch(options.batch)

    from lip.Tools.rootutils import loadToolsLib, getQuantilesGraphs, apply_modifs, xtitle, ytitle
    loadToolsLib()

    from ROOT import TCut, TCanvas, gDirectory, gPad, kRed, kBlue, kWhite, setStyle, TLegend, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TPaveText
    setStyle()


    print "Getting list of files"
    files = getListOfFiles(options.indir,options.filePattern)
    
    chain = mkChain(files,options.treename)
    ## chain = mkChain(files[0:100],options.treename)
    ## chain = mkChain(files[0:1],options.treename)
    
    hth = HtmlHelper(options.outdir)
    hth.header().addobj( HtmlTag("link") ).set("rel","stylesheet").set("type","text/css").set("href","../res/style.css")  
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../../res/jquery.js").txt(" ")
    hth.header().addobj( HtmlTag("script") ).set("language","javascript").set("type","text/javascript").set("src","../res/style.js").txt(" ")
    hth.navbar().cell( HtmlTag("a") ).firstChild().txt("..").set("href","../?C=M;O=D")
    tab = hth.body().add( HtmlTable() )
    
    quants = { }
    objs = []
    
    var,binning = options.variable.split(":")
    limits = [ float(i) for i in  binning.split(",")[1:] ]
    delta = 0.1*max( abs(limits[0]),  abs(limits[1]))
    limits[0] -= delta
    limits[1] += delta
    
    sig_style = [ ("SetLineColor",kBlue), ("SetMarkerColor",kBlue), (xtitle,"p_{T} (GeV)"), (ytitle,"%s cut" % var), ("SetFillStyle",0) ]
    bkg_style = [ ("SetLineColor",kRed), ("SetMarkerColor",kRed), (xtitle,"p_{T} (GeV)"), (ytitle,"%s cut" % var), ("SetFillStyle",0) ]
    lab_style = [ ("SetFillColor",kWhite), ("SetShadowColor",kWhite), ("SetLineColor",kWhite) ]

    for cut,name,vars in options.categories:
        print "Making TH2 ", name
        
        chain.Draw("%s:jetPt>>h_sig_%s(25,10,60,%s)" % (var, name, binning), TCut(cut)*TCut(options.sigcut),"colz goff")
        chain.Draw("%s:jetPt>>h_bkg_%s(25,10,60,%s)" % (var, name, binning), TCut(cut)*TCut(options.bkgcut), "colz goff")
        
        h_sig = gDirectory.Get("h_sig_%s" % name)
        h_bkg =  gDirectory.Get("h_bkg_%s" % name)

        print "Gatting quantiles ", name
        sig_qvals = [0.1,0.2,0.2,0.5,0.6 ,0.05]
        bkg_qvals = [0.4,0.7,0.6,0.8,0.9 ,0.95]
        quants[name] = getQuantilesGraphs(h_sig,sig_qvals), getQuantilesGraphs(h_bkg,bkg_qvals)

        for i in range(len(sig_qvals)):
            quants[name][0][i].SetTitle("%1.0f%% efficiency" % ((1.-sig_qvals[i])*100.) );
            apply_modifs(quants[name][0][i],sig_style)
            ## quants[name][0][i].Fit("pol3")
            ## apply_modifs(quants[name][0][i].GetListOfFunctions().At(0),sig_style)
            
        for i in range(len(bkg_qvals)):
            quants[name][1][i].SetTitle("%1.0f%% rejection" % (bkg_qvals[i]*100.) );
            apply_modifs(quants[name][1][i],bkg_style)
            ## quants[name][1][i].Fit("pol3")
            ## apply_modifs(quants[name][1][i].GetListOfFunctions().At(0),bkg_style)

        nvals = len(sig_qvals)
        plotsperline = 4 
        ### canv   = TCanvas(name,name,1800,1800)
        ###  nlines = 2
        ### nrows  = nvals / nlines
        ### if nvals % nlines != 0:
        ###     nrows += 1
        ### canv.Divide(nlines,nrows)
        ### objs.append(canv)
        for i in range(nvals):
            ## canv.cd(i+1)
            if i % plotsperline == 0:
                row = tab.row()
            canv   = TCanvas("%s_%d"%(name,i),("%s_%d"%(name,i)),800,1000)
            canv.SetGridx(), canv.SetGridy()

            quants[name][0][i].Draw("ap")
            quants[name][1][i].Draw("p")
            quants[name][0][i].GetYaxis().SetLimits(limits[0],limits[1])
            quants[name][0][i].GetYaxis().SetRangeUser(limits[0],limits[1])

            leg1 = TLegend(0.2,0.95,0.5,1.0)
            apply_modifs(leg1,lab_style)
            leg1.AddEntry(quants[name][0][i],"","lp")
            leg1.Draw("SAME")

            leg2 = TLegend(0.5,0.95,0.8,1.0)
            apply_modifs(leg2,lab_style)
            leg2.AddEntry(quants[name][1][i],"","lp")
            leg2.Draw("SAME")
            
            lab = TPaveText(0.1,0.95,0.2,1.0,"brNDC")
            apply_modifs(lab,lab_style)
            lab.AddText(name)
            lab.Draw("SAME")

            row.cell( HtmlPlot(canv,False,"",True,True,True) )
            objs.append(lab)
            objs.append(leg1)
            objs.append(leg2)
            objs.append(canv)

        ## if not os.path.isdir(options.outdir):
        ##     os.mkdir(options.outdir)
        ## for ext in "png","C":    
        ##     canv.SaveAs(os.path.join(options.outdir,"%s.%s" %( canv.GetName(), ext ) ))
    if not os.path.isdir(options.outdir):
            os.mkdir(options.outdir)
    hth.dump()
    
    return quants, objs 
        
        
    
## ------------------------------------------------------------------------------------------------------------------------------------------------------    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
            make_option("-i", "--indir",
                        action="store", type="string", dest="indir",
                        default="/store/cmst3/user/musella/JETID_NTUPLES",
                        help="input directory", metavar="DIR"
                        ),
            make_option("-f", "--files",
                        action="store", type="string", dest="filePattern",
                        default="*.root",
                        help="pattern of files to be read", metavar="PATTERN"
                        ), 
            make_option("-t", "--treeName",
                        action="store", type="string", dest="treename",
                        default="pfjetanalyzer/tree",
                        help="TTree name", metavar="TREENAME"
                        ),
            make_option("-o", "--outdir",
                        action="store", type="string", dest="outdir",
                        default="./",
                        help="outputfile", metavar="DIR"
                        ),
            make_option("-T", "--tmvaSettings",
                        action="store", dest="tmvaSettings", type="string",
                        default="common.json",
                        help="settings for the TMVA training"
                        ),
            make_option("-S", "--sigcut",
                        action="store", dest="sigcut", type="string",
                        default="",
                        help="signal definition"
                        ),
            make_option("-B", "--bkgcut",
                        action="store", dest="bkgcut", type="string",
                        default="",
                        help="background definition"
                        ),
            make_option("-v", "--variable",
                        action="store", dest="variable", type="string",
                        default="mva:100,-1.,1.", metavar="VAR:NBINS,MIN,MAX"
                        ),
            make_option("-b", "--batch",
                        action="store_true", dest="batch",
                        default=False,
                        ),
            make_option("-I", "--interactive",
                        action="store_false", dest="batch",
                        ),
            ])

    (options, args) = parser.parse_args()
    if options.tmvaSettings != "":
        getTMVASettings(options.tmvaSettings, options)

    quants = main(options, args)
