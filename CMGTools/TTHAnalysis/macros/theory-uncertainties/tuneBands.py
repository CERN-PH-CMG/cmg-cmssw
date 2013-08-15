
#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *
from scaleBands import scaleBand
    
if __name__ == "__main__":
    from sys import argv
    fin = ROOT.TFile(argv[1])
    fbase = argv[1].replace(".root","");
    fout = ROOT.TFile(fbase+".bands.root", "RECREATE")
    ROOT.gROOT.ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    c1 = ROOT.TCanvas("c1","c1")
    plots = [ "nJet25", ]
    if "3l" in argv[1]:
        plots += [ "qTimesNJ" ]
        plots += [ "q3l" ]
    else:
        plots += "htJet25,htJet25Lep,htJet25All,ptavgEtaJets,drll,lep2Eta,mll".split(",")
    for var in plots:
        for L,N in ('', False), ('_norm',True):
            for P in ["TTH",]:
                scales = [ "tune"+X for X in "P11 ProQ20 Z2".split() ]
                bandS = scaleBand(fin, var+"_"+P, True, norm=N, scales=scales, basePostfix="_tuneZ2Star")
                if not bandS: continue
                bandS.SetFillColor(33); bandS.SetFillStyle(1001);
                bands = ROOT.TMultiGraph("%s_%s_bands%s"%(var,P,L), "Tune Uncertainty bands")
                for b in bandS,:
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

