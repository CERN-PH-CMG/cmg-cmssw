from operator import attrgetter
import copy
import fnmatch

from ROOT import TLegend, TLine, TPad, TFile, gROOT

from CMGTools.RootTools.DataMC.Histogram import Histogram
from CMGTools.RootTools.DataMC.Stack import Stack

from CMGTools.H2TauTau.proto.plotter.H2TauStyle import histPref, Style

def ymax(hists):
    def getmax(h):
        hw = h.weighted
        return hw.GetBinContent(hw.GetMaximumBin())
    maxs = map(getmax, hists)
    ymax = max(maxs)*1.1
    if ymax == 0:
        ymax = 1
    return ymax


class DataMCPlot(object):

    '''Handles a Data vs MC plot.

    Features a list of histograms (some of them being stacked),
    and several Drawing functions.
    '''
    _f_keeper = {}
    _t_keeper = {}

    def __init__(self, name):
        self.histosDict = {}
        self.histos = []
        self.supportHist = None
        self.name = name
        self.stack = None
        self.legendOn = True
        self.legend = None
        self.legendBorders = 0.20, 0.46, 0.44, 0.89
        self.legendPos = 'left'
        # self.lastDraw = None
        # self.lastDrawArgs = None
        self.nostack = None
        self.blindminx = None
        self.blindmaxx = None
        self.groups = {}
        self.axisWasSet = False
        self.histPref = histPref

    def readTree(self, file_name, tree_name='tree'):
        if file_name in self.__class__._t_keeper:
            ttree = self.__class__._t_keeper[file_name]
        else:
            tfile = self.__class__._f_keeper[file_name] = TFile.Open(file_name)
            ttree = self.__class__._t_keeper[file_name] = tfile.Get(tree_name)

        gROOT.cd()

        return ttree

    def Blind(self, minx, maxx, blindStack):
        self.blindminx = minx
        self.blindmaxx = maxx
        if self.stack and blindStack:
            self.stack.Blind(minx, maxx)
        if self.nostack:
            for hist in self.nostack:
                hist.Blind(minx, maxx)

    def AddHistogram(self, name, histo, layer=0, legendLine=None, stack=True):
        '''Add a ROOT histogram, with a given name.

        Histograms will be drawn by increasing layer.'''
        tmp = Histogram(name, histo, layer, legendLine, stack=stack)
        self.histos.append(tmp)
        self.histosDict[name] = tmp
        return tmp

    def Group(self, groupName, namesToGroup, layer=None, style=None):
        '''Group all histos with names in namesToGroup into a single
        histo with name groupName. All histogram properties are taken
        from the first histogram in namesToGroup.
        See UnGroup as well
        '''
        groupHist = None
        realNames = []
        actualNamesInGroup = []
        for name in namesToGroup:
            hist = self.histosDict.get(name, None)
            if hist is None:
                print 'warning, no histo with name', name
                continue
            if groupHist is None:
                groupHist = hist.Clone(groupName)
                self.histos.append(groupHist)
                self.histosDict[groupName] = groupHist
            else:
                groupHist.Add(hist)
            actualNamesInGroup.append(name)
            realNames.append(hist.realName)
            hist.on = False
        if groupHist:
            self.groups[groupName] = actualNamesInGroup
            groupHist.realName = ','.join(realNames)
            if style is not None:
                groupHist.SetStyle(style)
            self._ApplyPrefs()

    def UnGroup(self, groupName):
        '''Ungroup groupName, recover the histograms in the group'''
        group = self.groups.get(groupName, None)
        if group is None:
            print groupName, 'is not a group in this plot.'
            return
        for name in group:
            self.histosDict[name].on = True
        self.histosDict[groupName].on = False

    def Replace(self, name, pyhist):
        '''Not very elegant... should have a clone function in Histogram...'''
        oldh = self.histosDict.get(name, None)
        if oldh is None:
            print 'histogram', name, 'does not exist, cannot replace it.'
            return

        pythist = copy.deepcopy(pyhist)
        pythist.layer = oldh.layer
        pythist.stack = oldh.stack
        pythist.name = oldh.name
        pythist.legendLine = oldh.legendLine
        pythist.SetStyle(oldh.style)
        pythist.weighted.SetFillStyle(oldh.weighted.GetFillStyle())

        index = self.histos.index(oldh)
        self.histosDict[name] = pythist
        self.histos[index] = pythist

    def _SortedHistograms(self, reverse=False):
        '''Returns the histogram dictionary, sorted by increasing layer,
        excluding histograms which are not "on".

        This function is used in all the Draw functions.'''
        byLayer = sorted(self.histos, key=attrgetter('layer'))
        byLayerOn = [hist for hist in byLayer if (hist.on is True)]
        if reverse:
            byLayerOn.reverse()
        return byLayerOn

    def Hist(self, histName):
        '''Returns an histogram.

        Print the DataMCPlot object to see which histograms are available.'''
        return self.histosDict[histName]

    def DrawNormalized(self, opt=''):
        '''All histograms are drawn as PDFs, even the stacked ones'''
        same = ''
        for hist in self._SortedHistograms():
            hist.obj.DrawNormalized(same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()
        if TPad.Pad():
            TPad.Pad().Update()
        # self.lastDraw = 'DrawNormalized'
        # self.lastDrawArgs = [ opt ]

    def Draw(self, opt=''):
        '''All histograms are drawn.'''
        same = ''
        self.supportHist = None
        for hist in self._SortedHistograms():
            if self.supportHist is None:
                self.supportHist = hist
            hist.Draw(same + opt)
            if same == '':
                same = 'same'
        yaxis = self.supportHist.GetYaxis()
        yaxis.SetRangeUser(0.01, ymax(self._SortedHistograms()))
        self.DrawLegend()
        if TPad.Pad():
            TPad.Pad().Update()
        # self.lastDraw = 'Draw'
        # self.lastDrawArgs = [ opt ]

    def CreateLegend(self, ratio=False):
        if self.legend is None:
            self.legend = TLegend(*self.legendBorders)
            self.legend.SetFillColor(0)
            self.legend.SetFillStyle(0)
            self.legend.SetLineColor(0)
        else:
            self.legend.Clear()
        hists = self._SortedHistograms(reverse=True)
        if ratio:
            hists = hists[:-1]  # removing the last histo.
        for index, hist in enumerate(hists):
            hist.AddEntry(self.legend)

    def DrawLegend(self, ratio=False):
        '''Draw the legend.'''
        if self.legendOn:
            self.CreateLegend(ratio)
            self.legend.Draw('same')

    def DrawRatio(self, opt=''):
        '''Draw ratios : h_i / h_0.

        h_0 is the histogram with the smallest layer, and h_i, i>0 are the other histograms.
        if the DataMCPlot object contains N histograms, N-1 ratio plots will be drawn.
        To take another histogram as the denominator, change the layer of this histogram by doing:
        dataMCPlot.Hist("histName").layer = -99 '''
        same = ''
        denom = None
        self.ratios = []
        for hist in self._SortedHistograms():
            if denom == None:
                denom = hist
                continue
            ratio = copy.deepcopy(hist)
            ratio.obj.Divide(denom.obj)
            ratio.obj.Draw(same)
            self.ratios.append(ratio)
            if same == '':
                same = 'same'
        self.DrawLegend(ratio=True)
        if TPad.Pad():
            TPad.Pad().Update()
        # self.lastDraw = 'DrawRatio'
        # self.lastDrawArgs = [ opt ]

    def DrawDataOverMCMinus1(self, ymin=-0.5, ymax=0.5):
        stackedHists = []
        dataHist = None
        for hist in self._SortedHistograms():
            if hist.stack is False:
                dataHist = hist
                continue
            stackedHists.append(hist)
        self._BuildStack(stackedHists, ytitle='Data/MC')
        mcHist = self.stack.totalHist
        self.dataOverMCHist = copy.deepcopy(dataHist)
        # self.dataOverMCHist.Add(mcHist, -1)
        self.dataOverMCHist.Divide(mcHist)
        self.dataOverMCHist.Draw()
        yaxis = self.dataOverMCHist.GetYaxis()
        yaxis.SetRangeUser(ymin + 1., ymax + 1.)
        yaxis.SetTitle('Data/MC')
        yaxis.SetNdivisions(5)
        fraclines = 0.2
        if ymax <= 0.2 or ymin >= -0.2:
            fraclines = 0.1
        self.DrawRatioLines(self.dataOverMCHist, fraclines, 1.)
        if TPad.Pad():
            TPad.Pad().Update()

    def DrawRatioStack(self, opt='',
                       xmin=None, xmax=None, ymin=None, ymax=None):
        '''Draw ratios.

        The stack is considered as a single histogram.'''
        denom = None
        # import pdb; pdb.set_trace()
        histForRatios = []
        denom = None
        for hist in self._SortedHistograms():
            if hist.stack is False:
                # if several unstacked histograms, the highest layer is used
                denom = hist
                continue
            histForRatios.append(hist)
        self._BuildStack(histForRatios, ytitle='MC/Data')
        self.stack.Divide(denom.obj)
        if self.blindminx and self.blindmaxx:
            self.stack.Blind(self.blindminx, self.blindmaxx)
        self.stack.Draw(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax)
        self.ratios = []
        for hist in self.nostack:
            if hist is denom:
                continue
            ratio = copy.deepcopy(hist)
            ratio.obj.Divide(denom.obj)
            ratio.obj.Draw('same')
            self.ratios.append(ratio)
        self.DrawLegend(ratio=True)
        self.DrawRatioLines(denom, 0.2, 1)
        if TPad.Pad():
            TPad.Pad().Update()

    def DrawNormalizedRatioStack(self, opt='',
                                 xmin=None, xmax=None,
                                 ymin=None, ymax=None):
        '''Draw ratios.

        The stack is considered as a single histogram.
        All histograms are normalized before computing the ratio'''
        denom = None
        histForRatios = []
        for hist in self._SortedHistograms():
            # taking the first histogram (lowest layer)
            # as the denominator histogram.
            if denom == None:
                denom = copy.deepcopy(hist)
                continue
            # other histograms will be divided by the denominator
            histForRatios.append(hist)
        self._BuildStack(histForRatios, ytitle='MC p.d.f. / Data p.d.f.')
        self.stack.Normalize()
        denom.Normalize()
        self.stack.Divide(denom.weighted)
        self.stack.Draw(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax)
        self.ratios = []
        for hist in self.nostack:
            # print 'nostack ', hist
            ratio = copy.deepcopy(hist)
            ratio.Normalize()
            ratio.obj.Divide(denom.weighted)
            ratio.obj.Draw('same')
            self.ratios.append(ratio)
        self.DrawLegend(ratio=True)
        self.DrawRatioLines(denom, 0.2, 1)
        if TPad.Pad():
            TPad.Pad().Update()
        # self.lastDraw = 'DrawNormalizedRatioStack'
        # self.lastDrawArgs = [ opt ]

    def DrawRatioLines(self, hist, frac=0.2, y0=1.):
        '''Draw a line at y = 1, at 1+frac, and at 1-frac.

        hist is used to get the x axis range.'''
        xmin = hist.obj.GetXaxis().GetXmin()
        xmax = hist.obj.GetXaxis().GetXmax()
        line = TLine()
        line.DrawLine(xmin, y0, xmax, y0)
        line.SetLineStyle(2)
        line.DrawLine(xmin, y0+frac, xmax, y0+frac)
        line.DrawLine(xmin, y0-frac, xmax, y0-frac)

    def DrawStack(self, opt='',
                  xmin=None, xmax=None, ymin=None, ymax=None):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.'''
        self._BuildStack(self._SortedHistograms(), ytitle='Events')
        same = 'same'
        if len(self.nostack) == 0:
            same = ''
        self.supportHist = None
        for hist in self.nostack:
            hist.Draw()
            if not self.supportHist:
                self.supportHist = hist
        self.stack.Draw(opt+same,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax)
        if self.supportHist is None:
            self.supportHist = self.stack.totalHist
        if not self.axisWasSet:
            mxsup = self.supportHist.weighted.GetBinContent(
                self.supportHist.weighted.GetMaximumBin()
            )
            mxstack = self.stack.totalHist.weighted.GetBinContent(
                self.stack.totalHist.weighted.GetMaximumBin()
            )
            mx = max(mxsup, mxstack)
            if ymin is None:
                ymin = 0.01
            if ymax is None:
                ymax = mx*1.3
            self.supportHist.GetYaxis().SetRangeUser(ymin, ymax)
            self.axisWasSet = True
        for hist in self.nostack:
            if self.blindminx:
                hist.Blind(self.blindminx, self.blindmaxx)
            hist.Draw('same')

        if self.supportHist.weighted.GetMaximumBin() < self.supportHist.weighted.GetNbinsX()/2:
            self.legendBorders = 0.62, 0.46, 0.88, 0.89
            self.legendPos = 'right'

        self.DrawLegend()
        if TPad.Pad():
            TPad.Pad().Update()

    def DrawNormalizedStack(self, opt='',
                            xmin=None, xmax=None, ymin=0.001, ymax=None):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.
        all histograms out of the stack, and the stack itself, are shown as PDFs.'''
        self._BuildStack(self._SortedHistograms(), ytitle='p.d.f.')
        self.stack.DrawNormalized(opt,
                                  xmin=xmin, xmax=xmax,
                                  ymin=ymin, ymax=ymax)
        for hist in self.nostack:
            hist.obj.DrawNormalized('same')
        self.DrawLegend()
        if TPad.Pad():
            TPad.Pad().Update()
        # self.lastDraw = 'DrawNormalizedStack'
        # self.lastDrawArgs = [ opt ]

    def Rebin(self, factor):
        '''Rebin, and redraw.'''
        # the dispatching technique is not too pretty,
        # but keeping a self.lastDraw function initialized to one of the Draw functions
        # when calling it creates a problem in deepcopy.
        for hist in self.histos:
            hist.Rebin(factor)
        self.axisWasSet = False

    def NormalizeToBinWidth(self):
        '''Normalize each Histograms bin to the bin width.'''
        for hist in self.histos:
            hist.NormalizeToBinWidth()

    def _BuildStack(self, hists, ytitle=None):
        '''build a stack from a list of Histograms.

        The histograms for which Histogram.stack is False are put in self.nostack'''
        self.stack = None
        self.stack = Stack(self.name+'_stack', ytitle=ytitle)
        self.nostack = []
        for hist in hists:
            if hist.stack:
                self.stack.Add(hist)
            else:
                self.nostack.append(hist)

    def _GetHistPref(self, name):
        '''Return the preference dictionary for a given component'''
        thePref = None
        for prefpat, pref in self.histPref.iteritems():
            if fnmatch.fnmatch( name, prefpat):
                if thePref is not None:
                    print 'several matching preferences for', name 
                thePref = pref
        if thePref is None:
            print 'cannot find preference for hist',name
            thePref = {'style':Style(), 'layer':999}
        return thePref

    def _ApplyPrefs(self):
        for hist in self.histos:
            pref = self._GetHistPref(hist.name)
            hist.layer = pref['layer']
            hist.SetStyle(pref['style'])
            hist.legendLine = pref['legend']

    def __str__(self):
        if self.stack is None:
            self._BuildStack(self._SortedHistograms(), ytitle='Events')
        tmp = [' '.join(['DataMCPlot: ', self.name])]
        tmp.append('Histograms:')
        for hist in self._SortedHistograms(reverse=True):
            tmp.append(' '.join(['\t', str(hist)]))
        tmp.append('Stack yield = {integ:7.1f}'.format(integ=self.stack.integral))
        return '\n'.join(tmp)


if __name__ == '__main__':

    plot = DataMCPlot('plot')
