#!/usr/bin/env python
import ROOT, math, itertools
import re
from CMGTools.TTHAnalysis.tools.plotDecorations import *

## batch mode
ROOT.gROOT.SetBatch(True)

class RFactorMaker:        
    def getHistosFromFile(self, tfile, pname, systematics):
        ret = {}
        directions = ['up','down']
        for v in systematics:
            hist1D = tfile.Get("metnomu_%s_%s" % (pname,v)).Clone()
            hist1D.SetDirectory(None)
            remove = '|'.join(directions)
            regex = re.compile(r'('+remove+')', flags=re.IGNORECASE)
            basesyst = regex.sub("", v)
            if('nominal' not in v and basesyst not in self.systs): self.systs.append(basesyst)
            ret[(pname,basesyst)] = hist1D
        return ret

    def __init__(self, srfile, crfile, num, den, systematicsUp, systematicsDown):
        files = [srfile,crfile]
        procs = [num,den]
        histsdic = dict(zip(procs,files))

        self.histsUp = {} 
        self.histsDown = {}
        self.hists_nominal = {}
        self.numproc = num
        self.denproc = den
        self.systs = []
        for k,v in histsdic.iteritems():
            print "Look for proc ",k, "in file ",v
            tfile = ROOT.TFile(v)
            self.histsUp.update(self.getHistosFromFile(tfile,k,systematicsUp))
            self.histsDown.update(self.getHistosFromFile(tfile,k,systematicsDown))
            self.hists_nominal.update(self.getHistosFromFile(tfile,k,["nominal"]))
            tfile.Close()

    def computeFullError(self,outfile,writeHistos=False):
        ret = {}
        procs = [self.numproc,self.denproc]
        for p in procs: 
            histNom = self.hists_nominal[(p,'nominal')]
            histFullErr = histNom.Clone("metnomu_%s_fullErr" % p)
            for b in range(1,histFullErr.GetNbinsX()+1):
                statErr = histNom.GetBinError(b)
                totSyst = 0
                for s in self.systs:
                    histUp = self.histsUp[(p,s)]
                    histDown = self.histsDown[(p,s)]
                    errUp = abs(histUp.GetBinContent(b) - histNom.GetBinContent(b))
                    errDn = abs(histDown.GetBinContent(b) - histNom.GetBinContent(b))
                    val = max(errUp,errDn)
                    totSyst += val*val
                totSyst = math.sqrt(totSyst)
                print "bin ",b, " value = ",histNom.GetBinContent(b)," +/- ",statErr, " (stat) +/- ",totSyst, " (syst) "
                histFullErr.SetBinError(b,math.sqrt(statErr*statErr + totSyst*totSyst))
            if(writeHistos):
                histFullErr.SetDirectory(outfile)
                outfile.WriteTObject(histFullErr)
            ret[p] = histFullErr
        return ret

    def computeRFactors(self,hists,outfile,unc):
        rfac = hists[self.numproc].Clone("rfac_%s_%s" % (self.denproc,unc))
        num = hists[self.numproc]
        denom = hists[self.denproc]
        for b in range(1,rfac.GetNbinsX()+1): 
            r = num.GetBinContent(b)/denom.GetBinContent(b)
            sigmar = r * math.sqrt(math.pow(num.GetBinError(b)/num.GetBinContent(b),2) + math.pow(denom.GetBinError(b)/denom.GetBinContent(b),2))
            rfac.SetBinContent(b,r)
            rfac.SetBinError(b,sigmar)
        rfac.SetDirectory(outfile)
        outfile.WriteTObject(rfac)
        return rfac


    def printCanvas(self, c1, name, lumi, text, options, xoffs=0, spam=[]):
        if   lumi > 3.54e+1: lumitext = "%.0f fb^{-1}" % lumi
        elif lumi > 3.54e+0: lumitext = "%.1f fb^{-1}" % lumi
        elif lumi > 3.54e-1: lumitext = "%.2f fb^{-1}" % lumi
        elif lumi > 3.54e-2: lumitext = "%.0f pb^{-1}" % (lumi*1000)
        elif lumi > 3.54e-3: lumitext = "%.1f pb^{-1}" % (lumi*1000)
        else                       : lumitext = "%.2f pb^{-1}" % (lumi*1000)
        
        doSpam("#bf{CMS} #it{%s}" % ("Simulation" if "_ref" in name else "Preliminary"), 0.15+xoffs, 0.955, 0.55+xoffs, 0.998, align=12, textSize=0.04)
        doSpam(lumitext+" (13 TeV)", .55+xoffs, .955, .97, .998, align=32, textSize=0.04)

        if spam:
            y0 = 0.91 - 0.04*1.2
            for line in spam:
                niceline = re.sub(r"(\s+)-(\d+)",r"\1#minus\2", line)
                doSpam(niceline, 0.50, y0, 0.90, y0 + 0.04*1.2, textSize=0.04, align=22)
                y0 -= 0.04 * 1.2
        for ext in "png", "pdf":
            c1.Print("%s.%s" % (name, ext))
        log = open("%s.%s"  % (name, "txt"), "w")
        for line in text: log.write(line +"\n")
        log.close()

    def makePlot(self,hist_stat,hist_full, name, lumi, text, options, spam=[]):
        ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
        c1 = ROOT.TCanvas("c1","c1")
        xoffs = 0;
        graph_stat = histToGraph(hist_stat)
        graph_full = histToGraph(hist_full)

        graph_stat.SetMarkerStyle(ROOT.kFullCircle)
        graph_stat.SetMarkerSize(1.5)
        graph_full.SetFillColor(ROOT.kOrange);
        graph_full.SetLineColor(ROOT.kOrange+3);
        graph_full.SetMarkerStyle(0)        
        graph_full.GetYaxis().SetTitle(text)
        ROOT.gStyle.SetErrorX(0.5);
        graph_full.Draw("A E2")
        graph_stat.Draw("PE SAME")
        self.printCanvas(c1, name, lumi, text, options, xoffs, spam)

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] srfile.root crfile.root")
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'")
    parser.add_option("-l","--lumi",   dest="lumi", type="float", default=2.3, help="Text size")
    (options, args) = parser.parse_args()

    systsUp   = ['QCD_renScaleUp', 'QCD_facScaleUp', 'QCD_pdfUp', 'EWK_up']
    systsDown = ['QCD_renScaleDown', 'QCD_facScaleDown', 'QCD_pdfDown', 'EWK_down']

    num_procs = ["ZNuNuHT"]
    den_procs = ["DYJetsHT"]
    titles = {'DYJetsHT':'R_{Z(#mu#mu)}'}

    outname = options.out if options.out else "rfactors.root"
    outfile = ROOT.TFile(outname,"RECREATE")
    for num in num_procs:
        for den in den_procs:
            rfm = RFactorMaker(args[0],args[1],num,den,systsUp,systsDown)
            hists = rfm.computeFullError(outfile)
            rfac_full = rfm.computeRFactors(hists,outfile,"full")
            hists_statonly = {}
            hists_statonly[num] = rfm.hists_nominal[num,'nominal']
            hists_statonly[den] = rfm.hists_nominal[den,'nominal']
            rfac_statonly = rfm.computeRFactors(hists_statonly,outfile,"stat")
            name = "plot_rfac_"+den
            lumi = options.lumi if options.lumi else "xxx"
            rfm.makePlot(rfac_statonly,rfac_full,name,lumi,titles[den],[])

    outfile.Close()

