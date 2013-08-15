#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *

def replicaPdfBand(file, base, pdf, reference, eigenvectors, norm=False):
    href = file.Get(base+reference)
    bins = href.GetNbinsX()
    if norm: href.Scale(1.0/href.Integral(0,bins+1))
    values = [ [0 for b in xrange(bins) ] for r in xrange(eigenvectors+1) ]
    for e in xrange(eigenvectors+1):
        hist = file.Get("%s_%s_%d" % (base,pdf,e))
        if norm: hist.Scale(1.0/hist.Integral(0,bins+1))
        for b in xrange(bins):
            val  = hist.GetBinContent(b+1)
            vref = href.GetBinContent(b+1)
            values[e][b] = val/vref if vref else 1
    ret = ROOT.TGraphAsymmErrors(bins)
    for b in xrange(bins):
        avg = sum([values[i][b] for i in xrange(1,eigenvectors+1)])/eigenvectors
        rms = sqrt(sum([(values[i][b]-avg)**2  for i in xrange(1,eigenvectors+1)])/eigenvectors)
        ret.SetPoint(b, href.GetBinCenter(b+1), avg)
        dx = 0.5*href.GetBinWidth(b+1)
        ret.SetPointError(b, dx, dx, rms, rms)
    ret.SetName("%s_band_%s" % (base,pdf))
    return ret

def eigenPdfBand(file, base, pdf, reference, eigenvectors, norm=False):
    if (eigenvectors % 2 != 0): raise RuntimeError
    href = file.Get(base+reference)
    bins = href.GetNbinsX()
    if norm: href.Scale(1.0/href.Integral(0,bins+1))
    values = [ [0,0,0]  for b in xrange(bins) ]
    central = file.Get("%s_%s_%d" % (base,pdf,0))
    if norm: central.Scale(1.0/central.Integral(0,bins+1))
    for b in xrange(bins):
        val  = central.GetBinContent(b+1)
        vref = href.GetBinContent(b+1)
        values[b][0] = val/vref if vref else 1
    for e in xrange(eigenvectors/2):
        h1 = file.Get("%s_%s_%d" % (base,pdf,2*e+1))
        h2 = file.Get("%s_%s_%d" % (base,pdf,2*e+2))
        if norm: h1.Scale(1.0/h1.Integral(0,bins+1))
        if norm: h2.Scale(1.0/h2.Integral(0,bins+1))
        for b in xrange(bins):
            vref = href.GetBinContent(b+1)
            if vref == 0: continue
            d1 = (h1.GetBinContent(b+1) - central.GetBinContent(b+1))/vref
            d2 = (h2.GetBinContent(b+1) - central.GetBinContent(b+1))/vref
            dlo = min([0,d1,d2])
            dhi = max([0,d1,d2])
            values[b][1] += dlo**2
            values[b][2] += dhi**2
    ret = ROOT.TGraphAsymmErrors(bins)
    for b in xrange(bins):
        ret.SetPoint(b, href.GetBinCenter(b+1), values[b][0])
        dx = 0.5*href.GetBinWidth(b+1)
        ret.SetPointError(b, dx, dx, sqrt(values[b][1]), sqrt(values[b][2]))
    ret.SetName("%s_band_%s" % (base,pdf))
    return ret
    
if __name__ == "__main__":
    from sys import argv
    fin = ROOT.TFile(argv[1])
    fout = ROOT.TFile(argv[2], "RECREATE")
    fbase = argv[2].replace(".root","");
    ROOT.gROOT.ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    c1 = ROOT.TCanvas("c1","c1")
    plots = [ ("nJet25",  "N(jet, p_{T} > 25 GeV)") ]
    if "3l" in argv[1]:
        plots += [ ("qTimesNJ",  "Q_{3l} * N(jet, p_{T} > 25 GeV)") ]
        plots += [ ("q3l",  "Q_{3l}") ]
        plots += [ ("lepEta3",  "max(|#eta_{l1}|,  |#eta_{l2}|,  |#eta_{l3}|)") ]
    else:
        plots += [ ("qTimesNJ",  "Q_{l1} * N(jet, p_{T} > 25 GeV)") ]
        plots += [ ("q2l",  "Q_{l1} + Q_{l2}") ]
        plots += [ ("lepEta2max",  "max(|#eta_{l1}|,  |#eta_{l2}|)") ]
        plots += [ ("lepEta2min",  "min(|#eta_{l1}|,  |#eta_{l2}|)") ]
    for (var,xtitle) in plots:
      for L,N in ('', False), ('_norm',True):
        for P in "ttH TTW TTZ".split():
            bandN = replicaPdfBand(fin, var+"_"+P, "NNPDF21_100", "_CT10_0", 100, norm=N)
            bandC = eigenPdfBand(fin, var+"_"+P, "CT10", "_CT10_0", 52, norm=N)
            bandM = eigenPdfBand(fin, var+"_"+P, "MSTW2008lo68cl", "_CT10_0", 38, norm=N)
            bandC.SetFillColor(33); bandN.SetFillStyle(1001);
            bandN.SetFillColor(206); bandN.SetFillStyle(3006);
            bandM.SetFillColor(214); bandM.SetFillStyle(3007);
            bands = ROOT.TMultiGraph("%s_%s_bands%s"%(var,P,L), "PDF Uncertainty bands")
            for b in bandC, bandN, bandM:
                b.SetName(b.GetName()+L)
                b.GetXaxis().SetTitle(xtitle)
                fout.WriteTObject(b)
                bands.Add(b, "E2")
            fout.WriteTObject(bands)
            bands.Draw("AE2")
            bands.GetXaxis().SetTitle(xtitle)
            bands.GetYaxis().SetTitle("Yield ratio" if not N else "Normalized yield ratio")
            c1.Print(fbase+"_"+var+"_"+P+L+".png")
            c1.Print(fbase+"_"+var+"_"+P+L+".pdf")
    fout.Close()

