import re
import copy

from math import log10, floor

from ROOT import TCanvas, TPaveText, TBox, gStyle
from CMGTools.RootTools.DataMC.Stack import Stack

from CMGTools.H2TauTau.proto.plotter.CMS_lumi import CMS_lumi

from CMGTools.H2TauTau.proto.plotter.officialStyle import officialStyle
officialStyle(gStyle)

class HistDrawer:
    ocan = None
    can = None
    pad = None
    padr = None

    @classmethod 
    def buildCanvas(cls):
        can = cls.can
        pad = cls.pad
        padr = cls.padr
        if not can:
            can = cls.can = TCanvas('can', '', 800, 800)

            can.Divide(1, 2, 0.0, 0.0)

            pad = cls.pad = can.GetPad(1)
            padr = cls.padr = can.GetPad(2)

            # Set Pad sizes
            pad.SetPad(0.0, 0.32, 1., 1.0)
            padr.SetPad(0.0, 0.00, 1., 0.34)

            pad.SetTopMargin(0.08)
            pad.SetLeftMargin(0.16)
            pad.SetBottomMargin(0.03)
            pad.SetRightMargin(0.05)

            padr.SetBottomMargin(0.35)
            padr.SetLeftMargin(0.16)
            padr.SetRightMargin(0.05)

        can.cd()
        can.Draw()
        pad.Draw()
        padr.Draw()

        return can, pad, padr

    @classmethod
    def buildCanvasSingle(cls):
        ocan = TCanvas('ocan', '', 600, 600)
        ocan.cd()
        ocan.Draw()
        return ocan

    @staticmethod
    def datasetInfo(plot):
        year = ''
        # if plot.dataComponents[0].find('2012') != -1:
        #     year = '2012'
        #     energy = 8
        # elif plot.dataComponents[0].find('2011') != -1:
        #     year = '2011'
        #     energy = 7
        # try:
        #     lumi = plot.weights['TTJets'].intLumi/1e3
        # except KeyError:
        #     lumi = plot.weights['TTJetsSemiLept'].intLumi/1e3
        year = '2015'
        lumi = 2.
        unit = 'pb'
        energy = 13
        return year, lumi, energy, unit

    @staticmethod
    def CMSPrelim(plot, pad, channel):
        pad.cd()
        year, lumi, energy, unit = HistDrawer.datasetInfo(plot)
        theStr = '{lumi:3.3} {unit}^{{-1}} ({energy:d} TeV)'.format(year=year, unit=unit, lumi=lumi, energy=energy)
        CMS_lumi(pad, theStr, iPosX=0)

        lowY = 0.77

        r = pad.GetRightMargin()
        l = pad.GetLeftMargin()
        posX = l + 0.045*(1-l-r)

        plot.chan = TPaveText(posX, lowY, 0.25, lowY+0.18, "NDC")
        plot.chan.SetBorderSize(0)
        plot.chan.SetFillStyle(0)
        plot.chan.SetTextAlign(12)
        plot.chan.SetTextSize(0.6*pad.GetTopMargin()) # To have it the same size as CMS_lumi
        plot.chan.SetTextFont(42)
        plot.chan.AddText(channel)
        plot.chan.Draw('same')


    unitpat = re.compile('.*\((.*)\)\s*$')

    keeper = []

    @staticmethod
    def draw(plot, do_ratio=True, channel='TauMu', plotprefix=None, SetLogy=0, mssm=False,
             blindxmin=None, blindxmax=None, unit=None):
        print plot
        Stack.STAT_ERRORS = True

        can = pad = padr = None

        if do_ratio:
            can, pad, padr = HistDrawer.buildCanvas()
        else:
            can = HistDrawer.buildCanvasSingle()

        pad.cd()
        pad.SetLogy(SetLogy)

        plot.DrawStack('HIST')

        h = plot.supportHist
        h.GetXaxis().SetLabelColor(1)
        # h.GetXaxis().SetLabelSize(1)

        unitsperbin = h.GetXaxis().GetBinWidth(1)
        ytitle = 'Events'
        if unit:
            round_to_n = lambda x, n: round(x, -int(floor(log10(abs(x)))) + (n - 1))
            ytitle += round_to_n(unitsperbin, 3)

        h.GetYaxis().SetTitle('Events')
        h.GetYaxis().SetTitleOffset(1.0)
        h.GetXaxis().SetTitleOffset(2.0)

        if do_ratio:
            padr.cd()
            ratio = copy.deepcopy(plot)
            ratio.legendOn = False

        if blindxmin or blindxmax:
            if not blindxmin:
                blindxmin = 0
            if not blindxmax:
                blindxmax = plot.GetXaxis().GetXmax()
            if do_ratio:
                ratio.Blind(blindxmin, blindxmax, True)
            plot.Blind(blindxmin, blindxmax, False)

        if do_ratio:
            ratio.DrawDataOverMCMinus1(-0.5, 0.5)
            hr = ratio.dataOverMCHist

            # ytp_ratio = float(pad.YtoPixel(0.)/padr.YtoPixel(0))
            # print 'YTP ratio', ytp_ratio
            # xtp_ratio = float(pad.XtoPixel(0.)/padr.XtoPixel(0))
            # print 'XTP ratio', xtp_ratio

            ytp_ratio = 2.
            xtp_ratio = 2.

            # hr.GetYaxis().SetNdivisions(4)

            hr.GetYaxis().SetTitleSize(h.GetYaxis().GetTitleSize() * xtp_ratio)
            hr.GetXaxis().SetTitleSize(h.GetXaxis().GetTitleSize() * ytp_ratio)
            
            hr.GetYaxis().SetTitleOffset(h.GetYaxis().GetTitleOffset() / xtp_ratio)
            hr.GetXaxis().SetTitleOffset(h.GetXaxis().GetTitleOffset() / ytp_ratio)

            hr.GetYaxis().SetLabelSize(h.GetYaxis().GetLabelSize() * xtp_ratio)
            hr.GetXaxis().SetLabelSize(h.GetXaxis().GetLabelSize() * ytp_ratio)

            h.GetXaxis().SetLabelColor(0)
            h.GetXaxis().SetLabelSize(0)
            padr.Update()

        # blinding
        if blindxmin or blindxmax:
            pad.cd()
            max = plot.stack.totalHist.GetMaximum()
            box = TBox(blindxmin, 0,  blindxmax, max)
            box.SetFillColor(1)
            box.SetFillStyle(3004)
            box.Draw()
            HistDrawer.keeper.append(box)
        print channel
        if channel == 'TauMu':
            HistDrawer.CMSPrelim(plot, pad, '#tau_{#mu}#tau_{h}')
        elif channel == 'TauEle':
            HistDrawer.CMSPrelim(plot, pad, '#tau_{e}#tau_{h}')
        can.cd()
        plotname = 'test'
        can.SaveAs(plotname + '.png')
        pad.SetLogy(0)
        return ratio

    drawRatio = draw


