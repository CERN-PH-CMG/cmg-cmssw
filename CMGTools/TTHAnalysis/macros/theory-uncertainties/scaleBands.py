
#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *

def scaleBand(file, base, center=False, norm=False, scales=["scaleUp","scaleDown"], ymin=0.05, basePostfix=""):
    href = file.Get(base+basePostfix)
    if not bool(href): return None
    print "doing ",base," with ",scales
    bins = href.GetNbinsX()
    if norm: href.Scale(1.0/href.Integral(0,bins+1))
    fullscale = href.Integral(0,bins+1)
    hs = [ file.Get("%s_%s" % (base,s)) for s in scales ]
    for (h,s) in zip(hs,scales):
        if not h: raise RuntimeError, "Missing histogram %s_%s" % (base,s)
    if norm:
        for h in hs: h.Scale(1.0/h.Integral(0,bins+1))
    ret = ROOT.TGraphAsymmErrors(bins)
    for b in xrange(bins):
        y0 = href.GetBinContent(b+1)
        if y0/fullscale < ymin: continue
        ys = [ h.GetBinContent(b+1) for h in hs ]
        yhi = max(ys); ylo = min(ys)
        ymid = 0.5*(yhi+ylo)
        dx = 0.5*href.GetBinWidth(b+1)
        if center: y0 = ymid
        ret.SetPoint(b, href.GetBinCenter(b+1), ymid/y0)
        ret.SetPointError(b, dx, dx, (ymid-ylo)/y0, (yhi-ymid)/y0)
    ret.SetName("%s_band" % (base,))
    ret.GetXaxis().SetTitle(href.GetXaxis().GetTitle())
    return ret

    
if __name__ == "__main__":
    from sys import argv
    fin = ROOT.TFile(argv[1])
    fbase = argv[1].replace(".root","");
    fout = ROOT.TFile(fbase+".bands.root", "RECREATE")
    ROOT.gROOT.ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    c1 = ROOT.TCanvas("c1","c1")
    plots = [ "nJet25" ]
    if "3l" in argv[1]:
        #plots += [ "qTimesNJ","q3l" ]
        plots += "htJet25,htJet25Lep,htJet25All,ptavgEtaJets".split(",")
    else:
        #plots += [ ("qTimesNJ",  "Q_{l1} * N(jet, p_{T} > 25 GeV)") ]
        #plots += [ ("q2l",  "Q_{l1} + Q_{l2}") ]
        plots += "htJet25,htJet25Lep,htJet25All,ptavgEtaJets".split(",")
    for var in plots:
        for L,N in ('', False), ('_norm',True):
            for P in "TTH TTW TTZ".split():
                scales  = [ "scaleUp", "scaleDown", "nominal" ]
                matches = [ "xqtUp", "xqtDown", "nominal" ]
                bandS = scaleBand(fin, var+"_"+P, True, norm=N, scales=scales)
                bandM = scaleBand(fin, var+"_"+P, True, norm=N, scales=matches)
                if not bandS: continue
                bandS.SetFillColor(33); bandS.SetFillStyle(1001);
                bandM.SetFillColor(1);  bandM.SetFillStyle(3004);
                bands = ROOT.TMultiGraph("%s_%s_bands%s"%(var,P,L), "Theory Uncertainty bands")
                for b in bandS,bandM,:
                    b.SetName(b.GetName()+L)
                    fout.WriteTObject(b)
                    bands.Add(b, "E2")
                fout.WriteTObject(bands)
                bands.Draw("AE2")
                bands.GetYaxis().SetTitle("Yield ratio" if not N else "Normalized yield ratio")
                bands.GetYaxis().SetRangeUser(0.5,1.5)
                c1.Print(fbase+"_"+var+"_"+P+L+".png")
                c1.Print(fbase+"_"+var+"_"+P+L+".pdf")
    fout.Close()

