from operator import itemgetter, attrgetter
import copy

from ROOT import TH1, THStack, TLegend, TLine, gPad

from CMGTools.RootTools.DataMC.Histogram import Histogram
from CMGTools.RootTools.DataMC.Stack import Stack



class DataMCPlot(object):
    '''Handles a Data vs MC plot.

    Features a list of histograms (some of them being stacked),
    and several Drawing functions.
    '''

    def __init__(self, name):
        self.histosDict = {}
        self.histos = []
        self.name = name
        self.stack = None
        self.legendOn = True
        self.legend = None
        self.legendBorders = 0.13,0.66,0.44,0.89
        self.lastDraw = None
        self.lastDrawArgs = None
        self.stack = None
        
    def AddHistogram(self, name, histo, layer=0, legendLine = None):
        '''Add a ROOT histogram, with a given name.

        Histograms will be drawn by increasing layer.'''
        tmp = Histogram(name, histo, layer, legendLine)
        self.histos.append( tmp )
        self.histosDict[name] = tmp
        # tmp.AddEntry( self.legend, legendLine)

    def Replace(self, name, pyhist):
        oldh = self.histosDict.get(name, None)
        if oldh is None:
            print 'histogram', name, 'does not exist, cannot replace it.'
            return
        else:
            index = self.histos.index( oldh )
            import pdb; pdb.set_trace
            self.histosDict[name] = pyhist
            self.histos[index] = pyhist
            
        
    def _SortedHistograms(self, reverse=False):
        '''Returns the histogram dictionary, sorted by increasing layer,
        excluding histograms which are not "on".

        This function is used in all the Draw functions.'''
        byLayer = sorted( self.histos, key=attrgetter('layer') )
        byLayerOn = [ hist for hist in byLayer if (hist.on is True) ]
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
            hist.obj.DrawNormalized( same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()
        gPad.Update()
        self.lastDraw = 'DrawNormalized'
        self.lastDrawArgs = [ opt ]

    def Draw(self, opt = ''):
        '''All histograms are drawn.'''
        same = ''
        for hist in self._SortedHistograms():
            hist.Draw( same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()
        gPad.Update()
        self.lastDraw = 'Draw'
        self.lastDrawArgs = [ opt ]


    def CreateLegend(self, ratio=False):
        if self.legend is None:
            self.legend = TLegend( *self.legendBorders )
        else:
            self.legend.Clear()
        hists = self._SortedHistograms(reverse=True)
        if ratio:
            hists = hists[:-1] # removing the last histo.
        for index, hist in enumerate(hists):
            hist.AddEntry( self.legend )
            

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
            ratio = copy.deepcopy( hist )
            ratio.obj.Divide( denom.obj )
            ratio.obj.Draw( same )
            self.ratios.append( ratio )
            if same == '':
                same = 'same'
        self.DrawLegend( ratio=True )
        gPad.Update()
        self.lastDraw = 'DrawRatio'
        self.lastDrawArgs = [ opt ]


    def DrawRatioStack(self,opt='',
                       xmin=None, xmax=None, ymin=None, ymax=None):
        '''Draw ratios.

        The stack is considered as a single histogram.'''
        denom = None
        histForRatios = []
        for hist in self._SortedHistograms():
            if denom == None:
                denom = hist
                continue
            histForRatios.append( hist )
        self._BuildStack( histForRatios, ytitle='MC/Data')
        self.stack.Divide( denom.obj )
        self.stack.Draw(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax )
        self.ratios = []
        for hist in self.nostack:
            # print 'nostack ', hist
            ratio = copy.deepcopy( hist )
            ratio.obj.Divide( denom.obj )
            ratio.obj.Draw('same')
            self.ratios.append( ratio )
        self.DrawLegend( ratio=True )
        self.DrawRatioLines(denom, xmin, xmax)
        gPad.Update()
        self.lastDraw = 'DrawRatioStack'
        self.lastDrawArgs = [ opt, ymin, ymax]
                
    def DrawNormalizedRatioStack(self,opt='',
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
            histForRatios.append( hist )
        self._BuildStack( histForRatios, ytitle='MC p.d.f. / Data p.d.f.')
        self.stack.Normalize()
        denom.Normalize()
        self.stack.Divide( denom.weighted )
        self.stack.Draw(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax )
        self.ratios = []
        for hist in self.nostack:
            # print 'nostack ', hist
            ratio = copy.deepcopy( hist )
            ratio.Normalize()
            ratio.obj.Divide( denom.weighted )
            ratio.obj.Draw('same')
            self.ratios.append( ratio )        
        self.DrawLegend( ratio=True )
        self.DrawRatioLines(denom, xmin,xmax)
        gPad.Update()
        self.lastDraw = 'DrawNormalizedRatioStack'
        self.lastDrawArgs = [ opt ]


    def DrawRatioLines(self, hist, xmin, xmax, frac=0.2):
        '''Draw a line at y = 1, at 1+frac, and at 1-frac.

        hist is used to get the y axis range.'''
        if xmin is None:
            xmin = hist.obj.GetXaxis().GetXmin()
        if xmax is None:
            xmax = hist.obj.GetXaxis().GetXmax()
        line = TLine()
        line.DrawLine(xmin, 1, xmax, 1)
        line.DrawLine(xmin, 1+frac, xmax, 1+frac)
        line.DrawLine(xmin, 1-frac, xmax, 1-frac)
        
                
    def DrawStack(self, opt='',
                  xmin=None, xmax=None, ymin=None, ymax=None):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.'''
        self._BuildStack(self._SortedHistograms(), ytitle='Events')
        self.stack.Draw(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax )
        for hist in self.nostack:
            hist.Draw('same')
        self.DrawLegend()
        gPad.Update()
        self.lastDraw = 'DrawStack'
        self.lastDrawArgs = [ opt ]


    def DrawNormalizedStack(self, opt='',
                            xmin=None, xmax=None, ymin=0.001, ymax=None ):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.
        all histograms out of the stack, and the stack itself, are shown as PDFs.'''
        self._BuildStack(self._SortedHistograms(),ytitle='p.d.f.')
        self.stack.DrawNormalized(opt,
                        xmin=xmin, xmax=xmax,
                        ymin=ymin, ymax=ymax )
        for hist in self.nostack:
            hist.obj.DrawNormalized('same')
        self.DrawLegend()
        gPad.Update()
        self.lastDraw = 'DrawNormalizedStack'
        self.lastDrawArgs = [ opt ]

    def Rebin(self, factor):
        '''Rebin, and redraw.'''
        # the dispatching technique is not too pretty,
        # but keeping a self.lastDraw function initialized to one of the Draw functions
        # when calling it creates a problem in deepcopy.
        for hist in self.histos:
            hist.Rebin(factor)
        if self.lastDraw == 'DrawStack':
            self.DrawStack( *self.lastDrawArgs)
        elif self.lastDraw == 'DrawNormalizedStack':
            self.DrawNormalizedStack( *self.lastDrawArgs)
        elif self.lastDraw == 'DrawRatioStack':
            self.DrawRatioStack( *self.lastDrawArgs)
        elif self.lastDraw == 'DrawNormalizedRatioStack':
            self.DrawNormalizedRatioStack( *self.lastDrawArgs)
        elif self.lastDraw == 'Draw':
            self.Draw(*self.lastDrawArgs)
        elif self.lastDraw == 'DrawNormalized':
            self.DrawNormalized(*self.lastDrawArgs)
        elif self.lastDraw == 'DrawRatio':
            self.DrawRatio(*self.lastDrawArgs)


    def _BuildStack(self, hists, ytitle=None):
        '''build a stack from a list of Histograms.

        The histograms for which Histogram.stack is False are put in self.nostack'''
        self.stack = None
        self.stack = Stack(self.name+'_stack', ytitle=ytitle)
        self.nostack = []
        for hist in hists:
            if hist.stack:
                self.stack.Add( hist )
            else:
                self.nostack.append(hist)


    def __str__(self):
        if self.stack is None:
            self._BuildStack(self._SortedHistograms(), ytitle='Events')
        tmp = [' '.join(['DataMCPlot: ', self.name])]
        tmp.append( 'Histograms:' )
        for hist in self._SortedHistograms( reverse=True ):
            tmp.append( ' '.join(['\t', str(hist)]) )
        tmp.append( 'Stack yield = {integ:7.1f}'.format( integ=self.stack.integral ) )
        return '\n'.join( tmp ) 

