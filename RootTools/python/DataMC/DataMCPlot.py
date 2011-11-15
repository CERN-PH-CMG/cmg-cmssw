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
        
    def AddHistogram(self, name, histo, layer=0, legendLine = None):
        '''Add a ROOT histogram, with a given name.

        Histograms will be drawn by increasing layer.'''
        tmp = Histogram(name, histo, layer, legendLine)
        self.histos.append( tmp )
        self.histosDict[name] = tmp
        # tmp.AddEntry( self.legend, legendLine)
        
    def _SortedHistograms(self):
        '''Returns the histogram dictionary, sorted by increasing layer,
        excluding histograms which are not "on".

        This function is used in all the Draw functions.'''
        byLayer = sorted( self.histos, key=attrgetter('layer') )
        byLayerOn = [ hist for hist in byLayer if (hist.on is True) ]
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

    def Draw(self, opt = ''):
        '''All histograms are drawn.'''
        same = ''
        for hist in self._SortedHistograms():
            hist.Draw( same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()
        gPad.Update()


    def CreateLegend(self):
        if self.legend is None:
            self.legend = TLegend( *self.legendBorders )
        else:
            self.legend.Clear()
        for hist in self._SortedHistograms():
            hist.AddEntry( self.legend )
            

    def DrawLegend(self):
        '''Draw the legend.'''
        if self.legendOn:
            self.CreateLegend()
            self.legend.Draw('same')

                
    def DrawRatio(self):
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
        self.DrawLegend()
        gPad.Update()

    def DrawRatioStack(self,opt=''):
        '''Draw ratios.

        The stack is considered as a single histogram.'''
        denom = None
        histForRatios = []
        for hist in self._SortedHistograms():
            if denom == None:
                denom = hist
                continue
            histForRatios.append( hist )
        self._BuildStack( histForRatios )
        self.stack.Divide( denom.obj )
        self.stack.Draw(opt)
        self.ratios = []
        for hist in self.nostack:
            print 'nostack ', hist
            ratio = copy.deepcopy( hist )
            ratio.obj.Divide( denom.obj )
            ratio.obj.Draw('same')
            self.ratios.append( ratio )
        self.DrawLegend()
        self.DrawRatioLines(denom)
        gPad.Update()
                
    def DrawNormalizedRatioStack(self,opt=''):
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
        self._BuildStack( histForRatios )
        self.stack.Normalize()
        denom.Normalize()
        self.stack.Divide( denom.weighted )
        self.stack.Draw(opt)
        self.ratios = []
        for hist in self.nostack:
            print 'nostack ', hist
            ratio = copy.deepcopy( hist )
            ratio.Normalize()
            ratio.obj.Divide( denom.weighted )
            ratio.obj.Draw('same')
            self.ratios.append( ratio )        
        self.DrawLegend()
        self.DrawRatioLines(denom)
        gPad.Update()


    def DrawRatioLines(self, hist, frac=0.2):
        '''Draw a line at y = 1, at 1+frac, and at 1-frac.

        hist is used to get the y axis range.'''
        xmin = hist.obj.GetXaxis().GetXmin()
        xmax = hist.obj.GetXaxis().GetXmax()
        line = TLine()
        line.DrawLine(xmin, 1, xmax, 1)
        line.DrawLine(xmin, 1+frac, xmax, 1+frac)
        line.DrawLine(xmin, 1-frac, xmax, 1-frac)
        
                
    def DrawStack(self, opt=''):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.'''
        self._BuildStack(self._SortedHistograms())
        self.stack.Draw( opt )
        for hist in self.nostack:
            hist.Draw('same')
        self.DrawLegend()
        gPad.Update()


    def DrawNormalizedStack(self, opt=''):
        '''Draw all histograms, some of them in a stack.

        if Histogram.stack is True, the histogram is put in the stack.
        all histograms out of the stack, and the stack itself, are shown as PDFs.'''
        self._BuildStack(self._SortedHistograms())
        self.stack.DrawNormalized( opt )
        for hist in self.nostack:
            hist.obj.DrawNormalized('same')
        self.DrawLegend()
        gPad.Update()


    def Rebin(self, factor):
        for hist in self.histos:
            hist.Rebin(factor)


    def _BuildStack(self, hists):
        '''build a stack from a list of Histograms.

        The histograms for which Histogram.stack is False are put in self.nostack'''
        self.stack = None
        self.stack = Stack(self.name+'_stack')
        self.nostack = []
        for hist in hists:
            if hist.stack:
                self.stack.Add( hist )
            else:
                self.nostack.append(hist)
    
    def __str__(self):
        tmp =  'DataMCPlot: ' + self.name + '\n' 
        for hist in self._SortedHistograms():
            tmp += '\t' + str(hist) + '\n'
        return tmp

