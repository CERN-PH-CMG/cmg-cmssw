
#!/usr/bin/env python
import ROOT
ROOT.gROOT.SetBatch(True)

from math import *
from scaleBands import *
    
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
        plots += ["jet1pT","htJet25","htJet25ratio1224Lep","lepEta3","minDrllAFOS","bestMTopHad","finalMVA"]
    else:
        plots += ["lep2Pt" ,"lep2Eta" ,"drl2j" ,"mtW1" ,"htJet25" ,"mhtJet25","MVA_2LSS_4j_6var" ]
    for var in plots:
        for L,N in ('', False), ('_norm',True):
            for P in ["TTH",]:
                scales = [ "tune"+X for X in "P11 ProQ20 Z2".split() ]
                bandS = scaleBand(fin, var+"_"+P, True, norm=N, scales=scales, basePostfix="_tuneZ2Star", ymin=1e-3)
                bandX = scaleBand(fin, var+"_"+P, True, norm=N, scales=scales+["tuneD6T"], basePostfix="_tuneZ2Star", ymin=1e-3, outPostfix="_wD6T")
                if not bandS: continue
                bandX.SetFillColor(208); bandS.SetFillStyle(1001);
                bandS.SetFillColor(33); bandS.SetFillStyle(1001);
                bands = ROOT.TMultiGraph("%s_%s_bands%s"%(var,P,L), "Tune Uncertainty bands")
                for b in bandX,bandS,:
                    b.SetName(b.GetName()+L)
                    fout.WriteTObject(b)
                    bands.Add(b, "E2")
                fout.WriteTObject(bands)
                bands.Draw("AE2")
                bands.GetXaxis().SetTitle(bandX.GetXaxis().GetTitle())
                bands.GetYaxis().SetTitle("Yield ratio" if not N else "Normalized yield ratio")
                bands.GetYaxis().SetRangeUser(0.5,1.6)
                bands.GetYaxis().SetDecimals(1)
                leg = doLegend(.20,.77,.92,.91)
                leg.AddEntry(bandS, "Envelop of Z2*, Z2, ProQ20, P11", "F")
                leg.AddEntry(bandX, "Envelop including also D6T tune", "F")
                leg.Draw()
                doCMSSpam("CMS Simulation",textSize=0.035)
                c1.Print(fbase+"_"+var+"_"+P+L+".png")
                c1.Print(fbase+"_"+var+"_"+P+L+".pdf")
                del leg
    fout.Close()

