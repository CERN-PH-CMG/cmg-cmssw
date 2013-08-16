#!/bin/env python

import sys, os, re
from ROOT import *

# parse a list of files to look for the limit information
class parsefilesforstats:
    def __init__(self, files):

        self.stats = []

        # build the regular expression
        self.re = re.compile('m0 = (.*) ; statlevel = (.*) ; lowerlimit = (.*) ; upperlimit = (.*)$')
        self.re2 = re.compile('^   9  xs.+\s+(.+)$')
        
        # loop over the files
        for file in files:
            try:
                ifstream = open(file, 'r')
            except IOError, err:
                print "%s: %s" %(file, str(err))
                break

            # now that we have a good file, parse it
            while 1:
                line=ifstream.readline()
                if not line:
                    break
                result = self.re.search(line)
                result2 = self.re2.search(line)
                if result2:
                    keepresult = result2
                if result:
                    print result.group(0)
                    if result.group(4)==' ':
                        upperlimit=-1
                    else:
                        upperlimit = float(result.group(4))

                    self.stats.append( (float(result.group(1)),
                                        int(result.group(2)),
                                        float(result.group(3)),
                                        upperlimit) )

class createHistogram:
    def __init__(self, whichstats, stats):
        self.hist = TH1F("hist"+str(whichstats),"",21,450.0,450.0+21*100.0)
        self.hist.SetMarkerStyle(20+whichstats)
        color = kBlack
        if whichstats>0:
            color = kBlue+whichstats-2
        if whichstats==4:
            color = kRed+2
        self.hist.SetLineColor(color)
        self.hist.SetMarkerColor(color)
        for stat in stats:
            if stat[1]!=whichstats:
                continue           
            self.hist.Fill(stat[0], stat[3])
            
if __name__ == "__main__":
    parse = parsefilesforstats(sys.argv[1:])
    hist0 = createHistogram(0,parse.stats)
    hist1 = createHistogram(1,parse.stats)
    hist2 = createHistogram(2,parse.stats)
    hist3 = createHistogram(3,parse.stats)
    hist4 = createHistogram(4,parse.stats)

    # create my own histogram
    hist5 = createHistogram(5,parse.stats)
    hist5.hist.SetBinContent(1, 99.0)
    hist5.hist.SetBinContent(2, 161.)
    hist5.hist.SetBinContent(3, 93.2)
    hist5.hist.SetBinContent(4, 64.7)
    hist5.hist.SetBinContent(5, 49.4)
    hist5.hist.SetBinContent(6, 20.1)
    hist5.hist.SetBinContent(7, 14.0)
    hist5.hist.SetBinContent(8, 14.2)
    hist5.hist.SetBinContent(9, 9.04)
    hist5.hist.SetBinContent(10, 6.14)
    hist5.hist.SetBinContent(11, 3.56)
    hist5.hist.SetBinContent(12, 2.65)
    hist5.hist.SetBinContent(13, 2.61)
    hist5.hist.SetBinContent(14, 2.47)
    hist5.hist.SetBinContent(15, 2.32)
    hist5.hist.SetBinContent(16, 2.25)

    c1 = TCanvas( 'limits', 'Example with Formula', 200, 10, 700, 500 )
    hist0.hist.Draw("pL")
    hist1.hist.Draw("pLsame")
    hist2.hist.Draw("pLsame")
    hist3.hist.Draw("pLsame")
    hist4.hist.Draw("pLsame")
    hist5.hist.Draw("pLsame")
    hist0.hist.GetXaxis().SetTitle("Invariant Mass [GeV]")
    hist0.hist.GetYaxis().SetTitle("95% C.L. Upper Limit [pb]")
    hist0.hist.SetStats(0)

    hist0.hist.SetMaximum(2*max([hist0.hist.GetMaximum(),
                                 hist1.hist.GetMaximum(),
                                 hist2.hist.GetMaximum(),
                                 hist3.hist.GetMaximum(),
                                 hist4.hist.GetMaximum(),
                                 hist5.hist.GetMaximum()]))
    hist0.hist.SetMinimum(1)
    gPad.SetLogy(1)

    leg = TLegend(0.5,0.5,0.8,0.8)
    leg.AddEntry(hist5.hist, "stat only from PAS", "p")
    leg.AddEntry(hist4.hist, "jet resolution", "p")
    leg.AddEntry(hist3.hist, "jet energy", "p")
    leg.AddEntry(hist2.hist, "luminosity", "p")
    leg.AddEntry(hist1.hist, "background", "p")
    leg.AddEntry(hist0.hist, "statistical only", "p")
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    leg.Draw()

    c1.SetFillColor(0)
    c1.Update()
    
    c2 = TCanvas( 'difflimits', 'Example with Formula', 200, 10, 700, 500 )
    hDiff1 = hist1.hist.Clone("hDiff1")
    hDiff2 = hist2.hist.Clone("hDiff2")
    hDiff3 = hist3.hist.Clone("hDiff3")
    hDiff4 = hist4.hist.Clone("hDiff4")
    for bin in range(hist0.hist.GetNbinsX()+1):
        baseline = hist0.hist.GetBinContent(bin)
        if baseline==0:
            baseline=1
        hDiff1.SetBinContent(bin, hist1.hist.GetBinContent(bin)/baseline-1)
        hDiff2.SetBinContent(bin, hist2.hist.GetBinContent(bin)/baseline-1)
        hDiff3.SetBinContent(bin, hist3.hist.GetBinContent(bin)/baseline-1)
        hDiff4.SetBinContent(bin, hist4.hist.GetBinContent(bin)/baseline-1)

    hDiff1.Draw("pL")
    hDiff2.Draw("pLsame")
    hDiff3.Draw("pLsame")
    hDiff4.Draw("pLsame")
    hDiff2.GetXaxis().SetTitle("Invariant Mass [GeV]")
    hDiff2.GetYaxis().SetTitle("Relative Difference v. stat-only")
    hDiff2.SetStats(0)

    leg2 = TLegend(0.5,0.5,0.8,0.8)
    leg2.AddEntry(hDiff4, "jet resolution", "p")
    leg2.AddEntry(hDiff3, "jet energy", "p")
    leg2.AddEntry(hDiff2, "luminosity", "p")
    leg2.AddEntry(hDiff1, "background", "p")
    leg2.SetFillColor(0)
    leg2.SetBorderSize(0)
    leg2.Draw()

    c2.SetFillColor(0)
    c2.Update()
    
