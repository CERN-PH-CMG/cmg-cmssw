#!/usr/bin/env python
import os, sys
import ROOT
from optparse import OptionParser

usage="usage: %prog [options] chmid_database.root \nrun with --help to get list of options"
parser = OptionParser(usage=usage)
(options, args) = parser.parse_args()

BASENAME1 = os.path.splitext(os.path.basename(args[0]))[0]
BASENAME2 = os.path.splitext(os.path.basename(args[1]))[0]

try:
    print "Processing %s, %s" % (args[0], args[1])
    assert os.path.isfile(args[0])
    assert os.path.isfile(args[1])
    assert os.path.splitext(args[0])[1] == '.root'
    assert os.path.splitext(args[1])[1] == '.root'
except (IndexError, AssertionError):
    print "ERROR: Please provide two root files as arguments"
    parser.print_help()
    sys.exit(1)


def make2DPlot(histo, outname='probmap'):
    ROOT.gStyle.SetPaintTextFormat("5.2g");
    ROOT.gStyle.SetOptStat(0000)
    ROOT.gStyle.SetOptTitle(0)
    c = ROOT.TCanvas("c", "C", 800, 600)
    c.SetRightMargin(0.15)
    c.SetLogx(1)
    histo.SetMinimum(0.)
    histo.SetMaximum(0.005)
    histo.Draw("colz text")

    for ext in ['.pdf', '.png']:
        c.SaveAs("%s%s" % (outname, ext))

def make1DPlots(hist_data, hist_mc, basename=''):
    xhists_da = [hist_data.ProjectionX(hist_data.GetName()+'_data_%d'%i, i, i+1)
                                    for i in range(hist_data.GetNbinsY())]
    xhists_mc = [hist_mc.ProjectionX(hist_mc.GetName()+'_mc_%d'%i, i, i+1)
                                    for i in range(hist_mc.GetNbinsY())]

    assert len(xhists_da) == len(xhists_mc)

    for n,(hda,hmc) in enumerate(zip(xhists_da, xhists_mc)):
        hda.SetLineWidth(2)
        hda.SetLineColor(ROOT.kBlack)
        hda.SetMarkerStyle(20)
        hda.SetMarkerSize(1.5)

        hda.GetYaxis().SetTitle("Charge MisId. Prob.")
        hda.GetYaxis().SetTitleOffset(1.6)
        hda.GetXaxis().SetTitleOffset(1.25)

        hmc.SetLineWidth(2)
        hmc.SetLineColor(ROOT.kAzure+2)
        hmc.SetFillColor(ROOT.kAzure+2)
        hmc.SetFillStyle(3004)
        hmc.SetMarkerStyle(4)
        hmc.SetMarkerSize(1.5)
        hmc.SetMarkerColor(ROOT.kAzure+2)

        hmc.SetMinimum(0.000)
        hda.SetMinimum(0.000)
        ymax = 0.02 if n==1 else 0.002
        hmc.SetMaximum(ymax)
        hda.SetMaximum(ymax)

        leg = ROOT.TLegend(0.70, 0.65, 0.90, 0.85)
        leg.SetBorderSize(0)
        leg.SetFillColor(0)
        leg.SetFillStyle(0)
        leg.SetShadowColor(0)
        leg.SetTextFont(43)
        leg.SetTextSize(20)
        leg.AddEntry(hda, "Data 0.83 fb^{-1}", "P")
        leg.AddEntry(hmc, "DY MC", "F")

        c = ROOT.TCanvas("c", "C", 800, 600)
        c.SetRightMargin(0.05)
        c.SetLeftMargin(0.15)
        c.SetLogx(1)
        hda.Draw("PE")
        hmc.Draw("E2 same")
        leg.Draw()

        tlat = ROOT.TLatex()
        tlat.SetNDC(1)
        tlat.SetTextFont(43)
        tlat.SetTextSize(20)
        label = 'Barrel' if n == 0 else 'Endcaps'
        tlat.DrawLatex(0.20, 0.83, label)

        hda.GetXaxis().SetMoreLogLabels()

        for ext in ['.pdf', '.png']:
            c.SaveAs("chmid_prob_proj_%d%s%s" % (n, basename, ext))

    return 0

def printTable(hist,title=''):
    print 70*'-'

    if title:
        print title

    xbins = list(hist.GetXaxis().GetXbins())
    ybins = list(hist.GetYaxis().GetXbins())

    print '{:^10}'.format('eta\pt'),
    for binx in xbins[:-1]: print '{:^17}'.format('{:3.0f} GeV'.format(binx)),
    print ''

    for ny,biny in enumerate(ybins[:-1]):
        print '{:10.3f}'.format(biny),
        for nx in range(hist.GetNbinsX()):
            print ' {:5.4f}'.format(100*hist.GetBinContent(nx+1,ny+1)),
            print '+- {:6.4f}'.format(100*hist.GetBinError(nx+1,ny+1)),
        print ''

def main():
    ROOT.gROOT.SetBatch(1)

    print 70*'-'
    print "Reading data histo from %s" % args[0]
    print "Reading MC histo from %s" % args[1]
    print 70*'-'

    ofile = ROOT.TFile.Open(args[0], "READ")
    histo_da = ofile.Get("chargeMisId")
    histo_da.SetDirectory(0)
    ofile.Close()

    try: histo_da.GetName()
    except AttributeError:
        print "ERROR: Could not read data histo from %s" % args[0]
        return -1

    ofile = ROOT.TFile.Open(args[1], "READ")
    histo_mc = ofile.Get("chargeMisId")
    histo_mc.SetDirectory(0)
    ofile.Close()

    try: histo_mc.GetName()
    except AttributeError:
        print "ERROR: Could not read MC histo from %s" % args[1]
        return -1


    make2DPlot(histo_da, outname='chmid_prob_map_'+BASENAME1)
    make2DPlot(histo_mc, outname='chmid_prob_map_'+BASENAME2)
    make1DPlots(histo_da, histo_mc, basename='_%s_%s'%(BASENAME1,BASENAME2))
    printTable(histo_da,title='Data')
    printTable(histo_mc,title='MC')

    print 70*'-'

    return 0



if __name__ == '__main__':
    sys.exit(main())
