#!/usr/bin/env python
import ROOT, math, itertools
import re
from CMGTools.TTHAnalysis.tools.plotDecorations import *

## batch mode
ROOT.gROOT.SetBatch(True)

class RFactorMaker:        
    def getHistosFromFile(self, tfile, pname, sel, systematics):
        ret = {}
        directions = ['up','down']
        for v in systematics:
            hist1D = tfile.Get("metnomu_%s_%s" % (pname,v)).Clone()
            hist1D.SetDirectory(None)
            remove = '|'.join(directions)
            regex = re.compile(r'('+remove+')', flags=re.IGNORECASE)
            basesyst = regex.sub("", v)
            if('nominal' not in v and basesyst not in self.systs[(pname,sel)]): self.systs[(pname,sel)].append(basesyst)
            ret[(pname,sel,basesyst)] = hist1D
        return ret

    def __init__(self, srfile, crfile, num, den, systematics):
        self.selections = ['SR','CR']
        procs = [num,den]
        files = [srfile,crfile]
        histsdic = dict(zip(self.selections,files))
        seldic = dict(zip(self.selections,procs))

        self.histsUp = {} 
        self.histsDown = {}
        self.hists_nominal = {}
        self.numproc = num
        self.denproc = den
        self.systs = {}
        for sel,v in histsdic.iteritems():
            proc=seldic[sel]
            print "Look for proc ",proc," with sel ",sel," in file ",v
            self.systs[(proc,sel)] = []
            tfile = ROOT.TFile(v)
            self.histsUp.update(self.getHistosFromFile(tfile,proc,sel,systematics[(proc,sel,'up')]))
            self.histsDown.update(self.getHistosFromFile(tfile,proc,sel,systematics[(proc,sel,'down')]))
            self.hists_nominal.update(self.getHistosFromFile(tfile,proc,sel,["nominal"]))
            tfile.Close()

    def computeFullError(self,outfile,writeHistos=False):
        ret = {}
        procs = [self.numproc,self.denproc]
        seldic = dict(zip(self.selections,procs))
        for sel in self.selections: 
            p=seldic[sel]
            print "List of systematics for proc ",p," and sel ",sel," = ",self.systs[(p,sel)]
            histNom = self.hists_nominal[(p,sel,'nominal')]
            histFullErr = histNom.Clone("metnomu_%s_fullErr" % p)
            for b in range(1,histFullErr.GetNbinsX()+1):
                statErr = histNom.GetBinError(b)
                totSyst = 0
                for s in self.systs[(p,sel)]:
                    histUp = self.histsUp[(p,sel,s)]
                    histDown = self.histsDown[(p,sel,s)]
                    errUp = abs(histUp.GetBinContent(b) - histNom.GetBinContent(b))
                    errDn = abs(histDown.GetBinContent(b) - histNom.GetBinContent(b))
                    val = max(errUp,errDn)
                    totSyst += pow(val,2)
                totSyst = math.sqrt(totSyst)
                print "bin ",b, " value = ",histNom.GetBinContent(b)," +/- ",statErr, " (stat) +/- ",totSyst, " (syst) "
                histFullErr.SetBinError(b,math.sqrt(statErr*statErr + totSyst*totSyst))
            if(writeHistos):
                histFullErr.SetDirectory(outfile)
                outfile.WriteTObject(histFullErr)
            ret[(p,sel)] = histFullErr
        return ret

    def computeRFactors(self,hists,outfile,unc):
        rfac = hists[(self.numproc,'SR')].Clone("rfac_%s_%s" % (self.denproc,unc))
        num = hists[(self.numproc,'SR')]
        denom = hists[(self.denproc,'CR')]
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
        graph_full.SetFillColor(ROOT.kOrange-3);
        graph_full.SetMarkerStyle(0)        
        graph_full.GetYaxis().SetTitle(text)
        ROOT.gStyle.SetErrorX(0.5);
        graph_full.Draw("A E2")
        xmin = hist_full.GetXaxis().GetXmin()
        xmax = hist_full.GetXaxis().GetXmax()
        graph_full.GetXaxis().SetRangeUser(xmin,xmax)
        graph_full.GetXaxis().SetNdivisions(505)
        ymin = .9*graph_full.GetYaxis().GetXmin()
        ymax = 1.1*graph_full.GetYaxis().GetXmax()
        graph_full.GetYaxis().SetRangeUser(ymin,ymax)
        graph_full.Draw("A E2")
        graph_stat.Draw("PE SAME")

        legWidth=0.30
        (x1,y1,x2,y2) = (.93-legWidth, .75, .93, .93)
        leg = ROOT.TLegend(x1,y1,x2,y2)
        leg.SetFillColor(0)
        leg.SetShadowColor(0)
        leg.SetLineColor(0)
        leg.SetTextFont(42)
        leg.SetTextSize(0.03)
        leg.AddEntry(graph_stat,'Stat. Uncert.','PE')
        leg.AddEntry(graph_full,'Stat. + Syst. Uncert.','F')
        leg.Draw()

        self.printCanvas(c1, name, lumi, text, options, xoffs, spam)

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] numproc denproc numfile.root denfile.root [numsel] [densel]")
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'")
    parser.add_option("-l","--lumi",   dest="lumi", type="float", default=2.3, help="Text size")
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="./", help="print out plots in this directory");
    (options, args) = parser.parse_args()

    if len(args) < 4:
        print "Usage: program [options] numproc denproc numfile.root denfile.root [numsel] [densel]"
        exit()

    num_proc=args[0]; den_proc=args[1]
    num_file=args[2]; den_file=args[3]
    if len(args) == 6: num_sel=args[4]; den_sel=args[5]
    else: num_sel='CR'; den_sel='SR'

    systsUpL   = ['lepID_up']
    systsDownL = ['lepID_down']

    systsUpG   = ['QCD_renScaleUp', 'QCD_facScaleUp', 'QCD_pdfUp', 'EWK_up']
    systsDownG = ['QCD_renScaleDown', 'QCD_facScaleDown', 'QCD_pdfDown', 'EWK_down']

    titles = {'DYJetsHT':'R_{Z(#mu#mu)}',
              'WJetsHT':'R_{W(#mu#mu)}'}

    systs={}

    if den_proc=='DYJetsHT' or den_proc=='WJetsHT':
        systs[(den_proc,'CR','up')]=systsUpL
        systs[(den_proc,'CR','down')]=systsDownL
    elif den_proc=='GJetsHT':
        systs[(den_proc,'CR','up')]=systsUpG
        systs[(den_proc,'CR','down')]=systsDownG
    else:
        print "ERROR! Numerator processes can be only DYJetsHT or WJetsHT or GJetsHT"
        exit()

    if num_proc=='ZNuNuHT':
        systs[(num_proc,'SR','up')]=[]
        systs[(num_proc,'SR','down')]=[]
        if den_proc=='DYJetsHT': title = 'R_{Z}'
        elif den_proc=='WJetsHT': title = 'R_{Z/W}'
        elif den_proc=='GJetsHT': title = 'R_{#gamma}'
        else: exit()
    elif num_proc=='WJetsHT':
        systs[(num_proc,'SR','up')]=[]
        systs[(num_proc,'SR','down')]=[]
        if den_proc=='WJetsHT': title = 'R_{W}'
        else:
            print "Num is ",num_proc," so only WJetsHT is allowed as denominator"
            exit()
    else:
        print "ERROR! Numerator processes can be only ZNuNuHT or WJetsHT"
        exit()


    outname = options.out if options.out else options.printDir+"/rfactors_"+num_proc+num_sel+"_Over_"+den_proc+den_sel+".root"
    outfile = ROOT.TFile(outname,"RECREATE")

    rfm = RFactorMaker(num_file,den_file,num_proc,den_proc,systs)
    hists = rfm.computeFullError(outfile)
    rfac_full = rfm.computeRFactors(hists,outfile,"full")
    hists_statonly = {}
    hists_statonly[(num_proc,'SR')] = rfm.hists_nominal[(num_proc,'SR','nominal')]
    hists_statonly[(den_proc,'CR')] = rfm.hists_nominal[(den_proc,'CR','nominal')]
    rfac_statonly = rfm.computeRFactors(hists_statonly,outfile,"stat")
    name = outname.replace(".root","")
    lumi = options.lumi if options.lumi else "xxx"
    rfm.makePlot(rfac_statonly,rfac_full,name,lumi,title,[])

    outfile.Close()

