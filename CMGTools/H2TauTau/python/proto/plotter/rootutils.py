import re
import copy
import time
from ROOT import gPad, TCanvas, TPad, TPaveText, TBox 

can = None,
pad = None
padr = None
ratio = None

xtitles = {
    'l1_pt':'p_{T,#tau} (GeV)',
    'l1Jet_pt':'p_{T,#tau jet} (GeV)',
    'l2_pt':'p_{T,#mu} (GeV)',
    'l2Jet_pt':'p_{T,#mu jet} (GeV)',
    'l1_eta':'#eta_{#tau}',
    'l2_eta':'#eta_{#mu}',
    'l1_rawMvaIso':'MVA #tau iso',
    'l2_relIso05':'#mu iso',
    'mt':'m_{T} (GeV/c^{2})',
    'visMass':'m_{vis} (GeV)',
    'svfitMass':'m_{sv} (GeV)',
    'nJets':'N_{jets}',
    'jet1_pt':'p_{T,jet1} (GeV)',
    'jet2_pt':'p_{T,jet1} (GeV)',
    'jet1_eta':'#eta_{#tau}',
    'jet2_eta':'#eta_{#tau}',
    }

xtitles_TauEle = {
    'l1_pt':'p_{T,#tau} (GeV)',
    'l1Jet_pt':'p_{T,#tau jet} (GeV)',
    'l2_pt':'p_{T,e} (GeV)',
    'l2Jet_pt':'p_{T,e jet} (GeV)',
    'l1_eta':'#eta_{#tau}',
    'l2_eta':'#eta_{e}',
    'l1_rawMvaIso':'MVA #tau iso',
    'l1_relIso05':'#tau iso',
    'l2_relIso05':'e iso',
    'mt':'m_{T} (GeV/c^{2})',
    'visMass':'m_{vis} (GeV)',
    'svfitMass':'m_{sv} (GeV)',
    'nJets':'N_{jets}',
    'jet1_pt':'p_{T,jet1} (GeV)',
    'jet2_pt':'p_{T,jet1} (GeV)',
    'jet1_eta':'#eta_{#tau}',
    'jet2_eta':'#eta_{#tau}',
    }

def savePlot(name):
    if gPad is None:
        print 'no active canvas'
        return
    fileName = '%s/%s' % (anaDir, name)
    print 'pad', gPad.GetName(), 'saved to', fileName    
    gPad.SaveAs( fileName )   


def buildCanvas():
    global can, pad, padr
    can = TCanvas('can','',800,800)
    can.cd()
    can.Draw()
    sep = 0.35
    pad = TPad('pad','',0.01,sep,0.99, 0.99)
    pad.SetBottomMargin(0.04)
    padr = TPad('padr','',0.01, 0.01, 0.99, sep)
    padr.SetTopMargin(0.04)
    padr.SetBottomMargin(0.3)
    pad.Draw()
    padr.Draw()
    return can, pad, padr


def CMSPrelim(self, pad, channel ):
    pad.cd()
    year = ''
    if self.dataComponents[0].find('2012'):
        year = '2012'
        energy = 8
    elif self.dataComponents[0].find('2011'):
        year = '2011'
        energy = 7       
    lumi = self.weights['TTJets'].intLumi/1e3
    theStr = 'CMS Preliminary {year}, {lumi:3.3} fb^{{-1}}, #sqrt{{s}} = {energy:d} TeV'.format( year=year, lumi=lumi, energy=energy)
    lowX = 0.06
    lowY = 0.85
    self.cmsprel = TPaveText(lowX, lowY, lowX+0.8, lowY+0.16, "NDC")
    self.cmsprel.SetBorderSize(   0 )
    self.cmsprel.SetFillStyle(    0 )
    self.cmsprel.SetTextAlign(   12 )
    self.cmsprel.SetTextSize ( 0.05 )
    self.cmsprel.SetTextFont (   62 )
    self.cmsprel.AddText(theStr)
    self.cmsprel.Draw('same')
    
    self.chan = TPaveText(0.8, lowY, 0.90, lowY+0.18, "NDC")
    self.chan.SetBorderSize(   0 )
    self.chan.SetFillStyle(    0 )
    self.chan.SetTextAlign(   12 )
    self.chan.SetTextSize ( 0.1 )
    self.chan.SetTextFont (   62 )
    self.chan.AddText(channel)
    self.chan.Draw('same')


unitpat = re.compile('.*\((.*)\)\s*$')

keeper = []


def draw(plot, doBlind=True, channel='TauMu', plotprefix = None):
    print plot
    blindxmin = None
    blindxmax = None
    doBlind = (plot.varName == 'svfitMass') and doBlind
    if doBlind:
        blindxmin = 100
        blindxmax = 160
        plot.Blind(blindxmin, blindxmax, False)
        
    if channel=='TauEle': xtitles = xtitles_TauEle
    xtitle = xtitles.get( plot.varName, None )
    if xtitle is None:
        xtitle = ''
    global can, pad, padr, ratio
    if pad is None:
        can, pad, padr = buildCanvas()
    pad.cd()
    plot.DrawStack('HIST')
    h = plot.stack.hists[0]
    h.GetXaxis().SetLabelColor(0)
    h.GetXaxis().SetLabelSize(0)
    gevperbin = h.GetXaxis().GetBinWidth(1)
    # mat = unitpat.match( xtitle )
    # unit = ''
    # if mat:
    #     unit = mat.group(1)
    #h.GetYaxis().SetTitle('Events/{gevperbin} {unit}'.format(gevperbin=gevperbin,
    #                                                         unit=unit))
    h.GetYaxis().SetTitle('Events')
    h.GetYaxis().SetTitleOffset(1.1)
    # plot.CMSPrelim('#tau_{#mu}#tau_{h}')
    padr.cd()
    ratio = copy.deepcopy(plot)
    ratio.legendOn = False
    if doBlind:
        ratio.Blind(blindxmin, blindxmax, True)
    ratio.DrawRatioStack('HIST', ymin=0.01, ymax=2)
    hr = ratio.stack.hists[0]
    hr.GetYaxis().SetNdivisions(4)
    hr.GetYaxis().SetTitle('Exp./Obs.')
    hr.GetYaxis().SetTitleSize(0.1)
    hr.GetYaxis().SetTitleOffset(0.5)
    hr.GetXaxis().SetTitle('{xtitle}'.format(xtitle=xtitle))
    hr.GetXaxis().SetTitleSize(0.13)
    rls = 0.075
    hr.GetYaxis().SetLabelSize(rls)
    hr.GetXaxis().SetLabelSize(rls)
    # hr.GetYaxis().SetTitleSize(0.08)
    # hr.GetYaxis().SetTitleOffset(0.6)
    # hr.GetXaxis().SetTitleSize(1)

    padr.Update()
    
    # blinding
    if plot.blindminx:
        pad.cd()
        max = plot.stack.totalHist.GetMaximum()
        box = TBox( plot.blindminx, 0,  plot.blindmaxx, max )
        box.SetFillColor(1)
        box.SetFillStyle(3004)
        box.Draw()
        # import pdb; pdb.set_trace()
        keeper.append(box)
    
    if channel == 'TauMu' : CMSPrelim( plot, pad, '#tau_{#mu}#tau_{h}')
    elif channel == 'TauEle' : CMSPrelim ( plot, pad, '#tau_{e}#tau_{h}')
    can.cd()
    if plotprefix == None : plotname = plot.varName
    else : plotname = plotprefix + '_' + plot.varName
    can.SaveAs( plotname + '.png')

cantemp = None

def drawTemplates(plot):
    global cantemp
    cantemp = TCanvas('cantemp', 'templates', 800,800)
    cantemp.cd()
    for hist in plot.histosDict.values():
        if hist.name not in ['TTJets','Ztt','WJets', 'QCD']:
            continue
        hist.Draw()
        gPad.Update()
        cantemp.SaveAs(hist.name + '.png')
        time.sleep(1)
