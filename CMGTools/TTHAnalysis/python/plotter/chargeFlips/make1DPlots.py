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
    histo.SetMaximum(0.005)
    histo.Draw("colz text")

    for ext in ['.pdf', '.png']:
        c.SaveAs("%s%s" % (outname, ext))

def make1DPlots(hist_data, hist_mc):
    xhists_da = [hist_data.ProjectionX(hist_data.GetName()+'_data_%d'%i, i, i+1)
                                    for i in range(hist_data.GetNbinsY())]
    xhists_mc = [hist_mc.ProjectionX(hist_mc.GetName()+'_mc_%d'%i, i, i+1)
                                    for i in range(hist_mc.GetNbinsY())]

    assert len(xhists_da) == len(xhists_mc)

    for n,(hda,hmc) in enumerate(zip(xhists_da, xhists_mc)):
        hda.SetLineWidth(2)
        hda.SetLineColor(ROOT.kBlack)
        hda.SetMarkerStyle(22)

        hmc.SetLineWidth(1)
        hmc.SetLineColor(ROOT.kAzure+2)
        hmc.SetFillColor(ROOT.kAzure+2)
        hmc.SetFillStyle(3004)

        hmc.SetMinimum(0.000)
        hda.SetMinimum(0.000)
        ymax = 0.01 if n==1 else 0.001
        hmc.SetMaximum(ymax)
        hda.SetMaximum(ymax)

        c = ROOT.TCanvas("c", "C", 800, 600)
        c.SetLogx(1)
        hda.Draw("PE")
        hmc.Draw("E2 same")

        hda.GetXaxis().SetMoreLogLabels()

        for ext in ['.pdf', '.png']:
            c.SaveAs("proj_%d%s" % (n, ext))

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
        print "ERROR: Could not read data histo from %s" % args[1]
        return -1


    make2DPlot(histo_da, outname='probmap_'+BASENAME1)
    make2DPlot(histo_mc, outname='probmap_'+BASENAME2)
    # make1DPlots(histo_da, histo_mc)
    printTable(histo_da,title='Data')
    printTable(histo_mc,title='MC')

    print 70*'-'

    return 0



if __name__ == '__main__':
    sys.exit(main())
