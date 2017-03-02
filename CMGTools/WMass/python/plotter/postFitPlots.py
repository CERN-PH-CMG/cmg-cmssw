#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *
from os.path import dirname,basename
from CMGTools.TTHAnalysis.tools.plotDecorations import *
from CMGTools.MonoXAnalysis.plotter.mcPlots import *

mergeMap = { 
#    "ttH_hww" : "ttH",
#    "ttH_hzz" : "ttH",
#    "ttH_htt" : "ttH",
#    "TTWW" : "RARE",
#    "TBZ" : "RARE",
#    "WWqq" : "RARE",
#    "WWDPI" : "RARE",
#    "VVV" : "RARE",
#    "TTGStar" : "TTZ",
}

swapMap = {
    "ZM" : { "ZLL" : "ZNuNu" },
    "ZE" : { "ZLL" : "ZNuNu" },
    "WM" : { },
    "WE" : { },
}

options = None
if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mca.txt plotfile varname mlfile region")
    addPlotMakerOptions(parser)
    (options, args) = parser.parse_args()
    region = args[4];
    options.path = "/data1/emanuele/monox/TREES_MET_80X_V4/" if region in ['SR','ZM','WM'] else "/data1/emanuele/monox/TREES_1LEP_80X_V4/"
    if "HOSTNAME" in os.environ:  
        if os.environ["HOSTNAME"] == "pccmsrm29.cern.ch":
            options.path = "/u2/emanuele/TREES_MET_80X_V4/" if region in ['SR','ZM','WM'] else "/u2/emanuele/TREES_1LEP_80X_V4/"
    options.lumi = 24.47
    mcap = MCAnalysis(args[0],options)
    basedir = dirname(args[1]);
    infile = ROOT.TFile(args[1]);
    var    = args[2];
    mlfile = ROOT.TFile(args[3]);
    ROOT.gROOT.ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc(0)")
    ROOT.gROOT.ForceStyle(False)
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetPaperSize(20.,25.)
    for O,MLD in ("prefit","prefit"), ("postfit_b","fit_b"), ("postfit_s","fit_s"):
      mldir  = mlfile.GetDirectory("shapes_"+MLD);
      if not mldir: raise RuntimeError, mlfile
      outfile = ROOT.TFile(basedir + "/"+O+"_" + basename(args[2]), "RECREATE")
      processes = [p for p in reversed(mcap.listBackgrounds())] + mcap.listSignals()

      hdata = infile.Get(var+"_data")
      stack = ROOT.THStack(var+"_stack","")
      plots = {'data':hdata}
      if options.poisson:
            pdata = getDataPoissonErrors(hdata, False, True)
            hdata.poissonGraph = pdata ## attach it so it doesn't get deleted
      argset =  mlfile.Get("norm_"+MLD)
      for p in processes:
        pout = (swapMap[region])[p] if p in swapMap[region] else p
        rvar = argset.find("%s/%s" % (region,pout))
        if not rvar: continue
        h = infile.Get(var+"_"+p)
        if not h: 
            print "Missing %s_%s for %s" % (var,p, p)
            continue
        h = h.Clone(var+"_"+p)
        h.SetDirectory(0)
        hpf = mldir.Get("%s/%s" % (region,pout))
        hpf_scale = rvar.getVal()/hpf.Integral()
        if not hpf: 
            if h.Integral() > 0 and p not in swapMap[region]: raise RuntimeError, "Could not find post-fit shape for %s" % p
            continue
        
        for b in xrange(1, h.GetNbinsX()+1):
            h.SetBinContent(b, hpf.GetBinContent(b)*hpf.GetBinWidth(b))
            h.SetBinError(b, hpf.GetBinError(b)*hpf.GetBinWidth(b))
        plots[p] = h
        h.SetName(var+"_"+p)
        stack.Add(h)

      dump = open("%s/%s_%s.txt" % (basedir,O,var), "w")
      pyields = { "background":[0,0], "data":[plots["data"].Integral(), plots["data"].Integral()]}
      #argset.Print("V")
      for p in processes:
        pout = (swapMap[region])[p] if p in swapMap[region] else p
        #h = infile.Get(var+"_"+pout)
        #if not h: continue
        if pout not in pyields: pyields[pout] = [0,0]
        rvar = argset.find("%s/%s" % (region,pout))
        if not rvar: continue
        pyields[pout][0] += rvar.getVal()       
        pyields[pout][1] += rvar.getError() if pout == "ttH" else rvar.getError()**2
        if not mcap.isSignal(p): 
            pyields["background"][0] += rvar.getVal()       
            pyields["background"][1] += rvar.getError()**2
      for p in pyields.iterkeys():
        if p != "ttH": pyields[p][1] = sqrt(pyields[p][1])
      maxlen = max([len(mcap.getProcessOption(p,'Label',p)) for p in mcap.listSignals(allProcs=True) + mcap.listBackgrounds(allProcs=True)]+[7])
      fmt    = "%%-%ds %%9.2f +/- %%9.2f\n" % (maxlen+1)
      for p in mcap.listSignals(allProcs=True) + mcap.listBackgrounds(allProcs=True) + ["background","data"]:
        if p not in pyields: continue
        if p in ["background","data"]: dump.write(("-"*(maxlen+45))+"\n");
        dump.write(fmt % (mcap.getProcessOption(p,'Label',p) if p not in ["background","data"] else p.upper(), pyields[p][0], pyields[p][1]))
      dump.close() 

      htot = hdata.Clone(var+"_total")
      htotpf = mldir.Get(region+"/total")
      scale_htotpf = (pyields["background"][0] + sum([pyields[p][0] for p in mcap.listSignals(allProcs=True)]))/htotpf.Integral()
      hbkg = hdata.Clone(var+"_total_background")
      hbkgpf = mldir.Get(region+"/total_background")
      scale_hbkgpf = pyields["background"][0]/hbkgpf.Integral()
      for b in xrange(1, h.GetNbinsX()+1):
          htot.SetBinContent(b, htotpf.GetBinContent(b)*htotpf.GetBinWidth(b))
          htot.SetBinError(b, htotpf.GetBinError(b)*htotpf.GetBinWidth(b))
          hbkg.SetBinContent(b, hbkgpf.GetBinContent(b)*hbkgpf.GetBinWidth(b))
          hbkg.SetBinError(b, hbkgpf.GetBinError(b)*hbkgpf.GetBinWidth(b))
      for h in plots.values() + [htot]:
         outfile.WriteTObject(h)
      doRatio = True
      htot.GetYaxis().SetRangeUser(0, 1.8*max(htot.GetMaximum(), hdata.GetMaximum()))
      ## Prepare split screen
      c1 = ROOT.TCanvas("c1", "c1", 600, 750); c1.Draw()
      c1.SetWindowSize(600 + (600 - c1.GetWw()), (750 + (750 - c1.GetWh())));
      p1 = ROOT.TPad("pad1","pad1",0,0.29,1,0.99);
      p1.SetBottomMargin(0.03);
      p1.Draw();
      p2 = ROOT.TPad("pad2","pad2",0,0,1,0.31);
      p2.SetTopMargin(0);
      p2.SetBottomMargin(0.3);
      p2.SetFillStyle(0);
      p2.Draw();
      p1.cd();
      ## Draw absolute prediction in top frame
      htot.Draw("HIST")
      #htot.SetLabelOffset(9999.0);
      #htot.SetTitleOffset(9999.0);
      stack.Draw("HIST F SAME")
      if options.showMCError:
          totalError = doShadedUncertainty(htot)
      if options.poisson:
        hdata.poissonGraph.Draw("PZ SAME")
      else:
        hdata.Draw("E SAME")
      htot.Draw("AXIS SAME")
#      hSigOutline = plots["ttH"].Clone()
#      hSigOutline.SetLineWidth(5)
#      hSigOutline.SetLineStyle(7)
#      hSigOutline.SetLineColor(205)
#      hSigOutline.SetFillColor(0)
#      hSigOutline.SetFillStyle(1)
#      hSigOutline.Scale(5)
#      hSigOutline.Draw("HIST SAME")
      leg = doLegend(plots,mcap,corner='TR',textSize=0.045,cutoff=0.0001)
      leg.SetHeader({'prefit': "Pre-fit", "postfit_b": "Post-fit, #mu = 1", "postfit_s": "Post-fit, #hat{#mu}"}[O]+"\n")
      leg.SetLineColor(0)
#      leg.AddEntry(hSigOutline, "ttH x 5", "L")
#      lspam = "CMS Preliminary" #, options.lspam
#      if "2lss" in args[2] and "/em/" in args[2]:
#            lspam += r", e^{#pm}#mu^{#pm} region"
#      if "2lss" in args[2] and "/ee/" in args[2]:
#            lspam += r", e^{#pm}e^{#pm} region"
#      if "2lss" in args[2] and "/mumu/" in args[2]:
#            lspam += r", #mu^{#pm}#mu^{#pm} region"
#      if "/3l" in args[2]:
#            lspam += ", 3l region"
#      if "/4l" in args[2]:
#            lspam += ", 4l region"
      doTinyCmsPrelim(hasExpo = False,textSize=(0.045 if doRatio else 0.033), xoffs=-0.03,
                      textLeft = options.lspam, textRight = options.rspam, lumi = options.lumi)
      ## Draw relaive prediction in the bottom frame
      p2.cd() 
      rdata,rnorm,rnorm2,rline = doRatioHists(PlotSpec(var,var,"",{}),plots,htot, htot, maxRange=options.maxRatioRange, fitRatio=options.fitRatio)
      c1.cd()
      c1.Print("%s/%s_%s.png" % (basedir,O,var))
      c1.Print("%s/%s_%s.pdf" % (basedir,O,var))
      del c1
      outfile.Close()
