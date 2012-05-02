#!/usr/bin/env python

from optparse import OptionParser, make_option
from CMG.JetIDAnalysis.ntupletools import *
from classify import getTMVASettings

def main(options, args):
    from ROOT import gROOT
    gROOT.SetBatch(options.batch)

    from lip.Tools.rootutils import loadToolsLib, getQuantilesGraphs, apply_modifs, xtitle, ytitle
    loadToolsLib()

    from ROOT import TCut, TCanvas, gDirectory, gPad, kRed, kBlue, kWhite, setStyle, gStyle, TLegend, HtmlHelper, HtmlTag, HtmlTable, HtmlPlot, TPaveText, kOpenCircle, TF1, TLine
    setStyle()
    gStyle.SetOptStat(0)
    gStyle.SetOptFit(0)
    

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
    wptag = hth.body().addobj( HtmlTag("pre") )
    tab = hth.body().add( HtmlTable() )
    
    quants = { }
    objs = []
    
    var,binning = options.variable.split(":")
    limits = [ float(i) for i in  binning.split(",")[1:] ]
    delta = 0.1*max( abs(limits[0]),  abs(limits[1]))
    limits[0] -= delta
    limits[1] += delta

    xlab = "p_{T} (GeV)"
    if options.vsnvtx:
            xlab = "N_{PV}"
    sig_style = [ ("SetLineColor",kBlue), ("SetMarkerColor",kBlue), (xtitle,xlab), (ytitle,"%s cut" % var), ("SetFillStyle",0),  ("SetMarkerColor",kOpenCircle) ]
    bkg_style = [ ("SetLineColor",kRed), ("SetMarkerColor",kRed), (xtitle,xlab), (ytitle,"%s cut" % var), ("SetFillStyle",0) ]
    lab_style = [ ("SetFillColor",kWhite), ("SetShadowColor",kWhite), ("SetLineColor",kWhite), ("SetFillStyle",0) ]

    fitl =  TF1("fitl","pol1",12,19)
    fitr =  TF1("fitl","pol1",20,35)
    fita =  TF1("fita","pol1",10,40)

    wps = { "loose":[0,0.05,0.1,0.1,0.1], "medium":[0,0.10,0.2,0.2,0.2], "tight":[1,0.9,0.9,0.9,0.9] }
    wpf = { }
    for a in wps.keys():
        wpf[a] = {}
    wpref = 22.

    icat = 0
    for cut,name,vars in options.categories:
        print "Making TH2 ", name

        vtxbins = [  (10,20), (20,999), (1,999) ]
        if options.integrateall:
            vtxbins = [  (1,999) ]
        ivtx = 0
        for vtx in vtxbins:
            print ( name, )+vtx
            hname = "%s_%d_%d" % ( ( name, )+vtx )

            vtxcut = TCut("nvtx > %d && nvtx < %d" % vtx)

            if options.vsnvtx:
                chain.Draw("%s:nvtx>>h_sig_%s(50,0.5,50.5,%s)" % (var, hname, binning), TCut("jetPt>20")*vtxcut*TCut(cut)*TCut(options.sigcut), "colz goff")
                chain.Draw("%s:nvtx>>h_bkg_%s(50,0.5,50.5,%s)" % (var, hname, binning), TCut("jetPt>20")*vtxcut*TCut(cut)*TCut(options.bkgcut), "colz goff")
            else:
                chain.Draw("%s:jetPt>>h_sig_%s(25,10,60,%s)" % (var, hname, binning), vtxcut*TCut(cut)*TCut(options.sigcut), "colz goff")
                chain.Draw("%s:jetPt>>h_bkg_%s(25,10,60,%s)" % (var, hname, binning), vtxcut*TCut(cut)*TCut(options.bkgcut), "colz goff")

            gDirectory.ls()
            h_sig = gDirectory.Get("h_sig_%s" % hname)
            h_bkg =  gDirectory.Get("h_bkg_%s" % hname)
            
            print "Getting quantiles ", hname 
            sig_qvals = [0.1,0.2,0.2,0.5,0.6 ,0.05, 0.1]
            bkg_qvals = [0.4,0.7,0.6,0.8,0.9 ,0.95, 0.9]
            quants[hname] = getQuantilesGraphs(h_sig,sig_qvals), getQuantilesGraphs(h_bkg,bkg_qvals)
            
            for i in range(len(sig_qvals)):
                if options.integrateall:
                    quants[hname][0][i].SetTitle("%1.0f%% efficiency" % (((1.-sig_qvals[i])*100.) ))
                    if options.vsnvtx:
                        quants[hname][0][i].Fit(fita,"R+")
                    else:
                        quants[hname][0][i].Fit(fitl,"R+")
                        quants[hname][0][i].Fit(fitr,"R+")
                    for wpn,wp in wps.iteritems():
                        if wp[0] == 0 and wp[icat+1] == sig_qvals[i]:
                            wpf[wpn][name] = quants[hname][0][i].GetListOfFunctions()
                else:
                    quants[hname][0][i].SetTitle("%1.0f%% efficiency %d<N_{PV}<%d" % (((1.-sig_qvals[i])*100.,)+vtx) )
                style = sig_style+[("SetLineColor",kBlue-2*ivtx), ("SetMarkerColor",kBlue-2*ivtx)]
                apply_modifs(quants[hname][0][i], style)
                for f in quants[hname][0][i].GetListOfFunctions():
                    apply_modifs(f,style)

            
            for i in range(len(bkg_qvals)):
                if options.integrateall:
                    quants[hname][1][i].SetTitle("%1.0f%% rejection" % ((bkg_qvals[i]*100.) ))
                    if options.vsnvtx:
                        quants[hname][1][i].Fit(fita,"R+")
                    else:
                        quants[hname][1][i].Fit(fitl,"R+")
                        quants[hname][1][i].Fit(fitr,"R+")
                    for wpn,wp in wps.iteritems():
                        if wp[0] == 1 and wp[icat+1] == bkg_qvals[i]:
                            wpf[wpn][name] = quants[hname][1][i].GetListOfFunctions()
                else:
                    quants[hname][1][i].SetTitle("%1.0f%% rejection  %d<N_{PV}<%d" % ((bkg_qvals[i]*100.,)+vtx) );
                style = bkg_style+[("SetLineColor",kRed-2*ivtx), ("SetMarkerColor",kRed-2*ivtx)] 
                apply_modifs(quants[hname][1][i],style)
                for f in quants[hname][1][i].GetListOfFunctions():
                    apply_modifs(f,style)
            ivtx += 1

        chain.Draw("nvtx>>h_nvtx_%s(50,0.5,50.5)"% (hname), TCut(cut)*TCut(options.sigcut), "goff")
        canv   = TCanvas("%s_nvtx"%(name),("%s"%(name)),800,1000)
        canv.cd()
        h_nvtx = gDirectory.Get("h_nvtx_%s"%hname)
        line = TLine(h_nvtx.GetMean(),0.,h_nvtx.GetMean(),h_nvtx.GetMaximum())
        h_nvtx.Draw("hist")
        line.Draw("same")
        objs.append(line)
        objs.append(canv)
        tab.row().cell( HtmlPlot(canv,False,"",True,True,True) )

        nvals = len(sig_qvals)
        plotsperline = 4
        for i in range(nvals):
            if i % plotsperline == 0:
                row = tab.row()
            canv   = TCanvas("%s_%d"%(name,i),("%s_%d"%(name,i)),800,1000)
            canv.SetGridx(), canv.SetGridy()

            first = True
            for vtx in vtxbins:
                kname = "%s_%d_%d" % ( ( name, )+vtx )
                if first:
                    quants[kname][0][i].Draw("ap")
                else:
                    quants[kname][0][i].Draw("p")
                quants[kname][1][i].Draw("p")
                quants[kname][0][i].GetYaxis().SetLimits(limits[0],limits[1])
                quants[kname][0][i].GetYaxis().SetRangeUser(limits[0],limits[1])

                if first:
                    leg1 = TLegend(0.2,0.95,0.5,1.0)
                    apply_modifs(leg1,lab_style)
                    leg2 = TLegend(0.5,0.95,0.8,1.0)
                    apply_modifs(leg2,lab_style)
                    first = False
                    
                leg1.AddEntry(quants[kname][0][i],"","lp")
                leg2.AddEntry(quants[kname][1][i],"","lp")
                
            leg1.Draw("SAME")
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
            
    wpstr = "Working points\n"
    for wpn,wpfs in wpf.iteritems():
        wpstr += "%s\n" % wpn
        for cat,funcs in wpfs.iteritems():
            wpstr += "%s " % cat
            for f in funcs:
                if options.vsnvtx:
                    wpstr += "%1.0f-%1.0f: %1.3f " % (f.GetXmin(), f.GetXmax(), f.GetParameter(1))
                else:
                    wpstr += "%1.0f-%1.0f: %1.2f " % (f.GetXmin(), f.GetXmax(), f.Eval(wpref))
            wpstr += "\n"
    wptag.txt(str(wpstr))

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
            make_option("-a", "--integrateall",
                        action="store_true", dest="integrateall",
                        default=False
                        ),
            make_option("-V", "--vsnvtx",
                        action="store_true", dest="vsnvtx",
                        default=False
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
