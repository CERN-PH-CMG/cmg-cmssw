#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *
from CMGTools.TTHAnalysis.tools.plotDecorations import *

def replicaPdfBand(file, base, pdf, reference, eigenvectors, norm=False, relative=True):
    href = file.Get(base+reference)
    if not bool(href): return None
    bins = href.GetNbinsX()
    if norm: href.Scale(1.0/href.Integral(0,bins+1))
    values = [ [0 for b in xrange(bins) ] for r in xrange(eigenvectors+1) ]
    for e in xrange(eigenvectors+1):
        hist = file.Get("%s_%s_%d" % (base,pdf,e))
        if norm: hist.Scale(1.0/hist.Integral(0,bins+1))
        for b in xrange(bins):
            val  = hist.GetBinContent(b+1)
            vref = href.GetBinContent(b+1) if relative else 1
            values[e][b] = val/vref if vref else 1
    ret = ROOT.TGraphAsymmErrors(bins)
    for b in xrange(bins):
        avg = sum([values[i][b] for i in xrange(1,eigenvectors+1)])/eigenvectors
        rms = sqrt(sum([(values[i][b]-avg)**2  for i in xrange(1,eigenvectors+1)])/eigenvectors)
        ret.SetPoint(b, href.GetBinCenter(b+1), avg)
        dx = 0.5*href.GetBinWidth(b+1)
        ret.SetPointError(b, dx, dx, rms, rms)
    ret.SetName("%s_band_%s" % (base,pdf))
    ret.GetXaxis().SetTitle(href.GetXaxis().GetTitle())
    return ret

def eigenPdfBand(file, base, pdf, reference, eigenvectors, norm=False, relative=True):
    if (eigenvectors % 2 != 0): raise RuntimeError
    href = file.Get(base+reference)
    if not bool(href): return None
    bins = href.GetNbinsX()
    if norm: href.Scale(1.0/href.Integral(0,bins+1))
    values = [ [0,0,0]  for b in xrange(bins) ]
    central = file.Get("%s_%s_%d" % (base,pdf,0))
    if norm: central.Scale(1.0/central.Integral(0,bins+1))
    for b in xrange(bins):
        val  = central.GetBinContent(b+1)
        vref = href.GetBinContent(b+1) if relative else 1
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
        plots += ["jet1pT","htJet25","htJet25ratio1224Lep","lepEta3","minDrllAFOS","bestMTopHad","finalMVA"]
    else:
        plots += ["lep2Pt" ,"lep2Eta" ,"drl2j" ,"mtW1" ,"htJet25" ,"mhtJet25","MVA_2LSS_4j_6var" ]
    for var in plots:
      for L,N in ('', False), ('_norm',True):
        for P in "ttH TTW TTZ WZ ZZ".split():
            bandN = replicaPdfBand(fin, var+"_"+P, "NNPDF21_100", "_CT10_0", 100, norm=N)
            bandC = eigenPdfBand(fin, var+"_"+P, "CT10", "_CT10_0", 52, norm=N)
            bandM = eigenPdfBand(fin, var+"_"+P, "MSTW2008lo68cl", "_CT10_0", 38, norm=N)
            if not bandN: continue
            bandC.SetFillColor(33); bandN.SetFillStyle(1001);
            bandN.SetFillColor(206); bandN.SetFillStyle(3006);
            bandM.SetFillColor(214); bandM.SetFillStyle(3007);
            bands = ROOT.TMultiGraph("%s_%s_bands%s"%(var,P,L), "PDF Uncertainty bands")
            for b in bandC, bandN, bandM:
                b.SetName(b.GetName()+L)
                fout.WriteTObject(b)
                bands.Add(b, "E2")
            fout.WriteTObject(bands)
            bands.Draw("AE2")
            bands.GetXaxis().SetTitle(bandC.GetXaxis().GetTitle())
            bands.GetYaxis().SetTitle("Yield ratio" if not N else "Normalized yield ratio")
            bands.GetYaxis().SetRangeUser(0.88 if N else 0.70, 1.17 if N and (P != "TTW") else 1.35)
            bands.GetYaxis().SetDecimals(1)
            leg = doLegend(.67,.75,.92,.91)
            leg.AddEntry(bandC, "CT10",     "F")
            leg.AddEntry(bandN, "NNPDF21",  "F")
            leg.AddEntry(bandM, "MSTW2008", "F")
            leg.Draw()
            doCMSSpam("CMS Simulation",textSize=0.035)
            c1.Print(fbase+"_"+var+"_"+P+L+".png")
            c1.Print(fbase+"_"+var+"_"+P+L+".pdf")
            del leg
    fout.Close()

