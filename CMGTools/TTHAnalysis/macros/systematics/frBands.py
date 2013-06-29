
#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *
from os.path import dirname
from CMGTools.TTHAnalysis.tools.plotDecorations import *

def makeBand(file, base, norm=False, ratio=False, sources=[], ymin=0.02, basePostfix="",outPostfix=""):
    href = file.Get(base+basePostfix)
    if not bool(href): return None
    print "doing ",base," with ",sources
    bins = href.GetNbinsX()
    fullscale = href.Integral()
    allsources = [ s+"Up"  for s in sources ] + [ s+"Dn" for s in sources ]
    hs = [ file.Get("%s_%s" % (base,s)) for s in allsources ]
    for (h,s) in zip(hs,allsources):
        if not h: raise RuntimeError, "Missing histogram %s_%s" % (base,s)
    if norm:
        for h in hs: h.Scale(fullscale/h.Integral())
    ret = ROOT.TGraphAsymmErrors(bins)
    for b in xrange(bins):
        y0 = href.GetBinContent(b+1)
        if y0/fullscale < ymin: continue
        ys = [ h.GetBinContent(b+1) for h in hs ]
        yhi = max(ys); ylo = min(ys)
        ymid = 0.5*(yhi+ylo)
        dx = 0.5*href.GetBinWidth(b+1)
        div = y0 if ratio else 1.0
        ret.SetPoint(b, href.GetBinCenter(b+1), ymid/div)
        ret.SetPointError(b, dx, dx, (ymid-ylo)/div, (yhi-ymid)/div)
        #print "bin %d, x = %5.2f: ref = %5.2f, min = %5.2f, max = %5.2f, ratio %s: point = %6.3f   -%6.3f / +%6.3f" % (b+1,href.GetBinCenter(b+1),y0,ylo,yhi,ratio,ymid/div,(ymid-ylo)/div, (yhi-ymid)/div)
    ret.SetName("%s%s%s_band%s" % (base,("_ratio" if ratio else ""),("_norm" if norm else ""),outPostfix))
    ret.GetXaxis().SetTitle(href.GetXaxis().GetTitle())
    return ret

 
if __name__ == "__main__":
    from sys import argv
    fin = ROOT.TFile(argv[1])
    fbase  = dirname(argv[1])
    fout = ROOT.TFile(fbase+"/systFR.bands.root", "RECREATE")
    ROOT.gROOT.ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    c1 = ROOT.TCanvas("c1","c1")
    plots = [ "nJet25" ]
    leptons = []
    if "3l" in argv[1] or "_em" in argv[1]:
        leptons = [ "mu", "el" ]
    elif "_mumu" in argv[1]:
        leptons = [ "mu" ]
    elif "_ee" in argv[1]:
        leptons = [ "el" ]
    else: raise RuntimeError, "No idea which leptons to test"
    if "3l" in argv[1]:
        plots += [ "finalMVA" ]
    else:
        plots += [ "MVA_2LSS_4j_6var" ]

    for L in leptons:
        FRnorm = [ L ];
        FRshape = [ L+"Pt", L+"Eta" ]
        if L == "el": FRshape += [ "elBarrel" ]
        for var in plots:
            P = "FR_data"
            for LR,R in ('', False), ('_ratio',True):
                bandN = makeBand(fin, var+"_"+P, norm=False, ratio=R, sources=FRnorm, outPostfix="_N")
                if not bandN: continue
                bandS = makeBand(fin, var+"_"+P, norm=True, ratio=R, sources=FRshape, outPostfix="_S")
                if not bandS: continue
                bandN.SetFillColor(33);  bandN.SetFillStyle(1001);
                bandS.SetFillColor(226); bandS.SetFillStyle(1001);
                bandN.SetName("%s_%s_bands_%s_norm%s"%(var,P,L,LR))
                bandS.SetName("%s_%s_bands_%s_norm%s"%(var,P,L,LR))
                bands = ROOT.TMultiGraph("%s_%s_bands_%s%s"%(var,P,L,LR), "FR Uncertainty bands")
                for b in bandN,bandS:
                    bands.Add(b, "E2")
                    fout.WriteTObject(b)
                fout.WriteTObject(bands)
                bands.Draw("AE2")
                bands.GetXaxis().SetTitle(bandS.GetXaxis().GetTitle())
                bands.GetYaxis().SetTitle("Event yield ratio" if R else "Events")
                if R: bands.GetYaxis().SetRangeUser(0.0,2.5)
                leg = doLegend(.67,.75,.92,.91)
                leg.AddEntry(bandN, "Norm.",  "F")
                leg.AddEntry(bandS, "Shape",  "F")
                leg.Draw()
                doCMSSpam("CMS Preliminary",textSize=0.035)
                c1.Print(fbase+"/systFR_"+L+"_"+var+LR+".png")
                c1.Print(fbase+"/systFR_"+L+"_"+var+LR+".pdf")
                del leg
    fout.Close()

