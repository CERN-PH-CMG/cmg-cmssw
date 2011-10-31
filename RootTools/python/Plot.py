from operator import itemgetter, attrgetter
import copy

from ROOT import TH1, THStack, TLegend

from CMGTools.RootTools.Histogram import Histogram
from CMGTools.RootTools.Stack import Stack



class Plot:
    def __init__(self, name):
        self.histosDict = {}
        self.histos = []
        self.name = name
        self.legend = TLegend(0.5,0.7,1,1)
        self.legendOn = True
        self.stack = None
        
    def addHistogram(self, name, histo, layer=0, legendLine=None):
        tmp = Histogram(name, histo, layer)
        self.histos.append( tmp )
        self.histosDict[name] = tmp
        tmp.AddEntry( self.legend, legendLine)
        
    def sortedHistograms(self):
        byLayer = sorted( self.histos, key=attrgetter('layer') )
        return byLayer

    def histogram(self, name):
        return self.histosDict[name]

    def normalize(self):
        for hist in self.histos:
            hist.obj.Scale( 1/hist.obj.Integral() )

##     def normalizeStack(self):
##         if self.stack == None:
##             return
##         for hist in self.nostack:
##             hist.obj.Scale( 1/hist.obj.Integral() )
##         self.stackIntegral = stack.integral

    def hist(self, histName):
        return self.histosDict[histName]

    def DrawNormalized(self, opt=''):
        same = ''
        for hist in self.sortedHistograms():
            hist.obj.DrawNormalized( same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()

    def Draw(self, opt = ''):
        same = ''
        for hist in self.sortedHistograms():
            hist.Draw( same + opt)
            if same == '':
                same = 'same'
        self.DrawLegend()

    def DrawLegend(self):
        if self.legendOn:
            self.legend.Draw('same')
                
    def DrawRatio(self):
        same = ''
        denom = None
        self.ratios = []
        for hist in self.sortedHistograms():
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

    def DrawRatioStack(self,opt=''):
        denom = None
        histForRatios = []
        for hist in self.sortedHistograms():
            if denom == None:
                denom = hist
                continue
            histForRatios.append( hist )
        self.buildStack( histForRatios )
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
                
    def DrawNormalizedRatioStack(self,opt=''):
        denom = None
        histForRatios = []
        for hist in self.sortedHistograms():
            if denom == None:
                denom = copy.deepcopy(hist)
                continue
            histForRatios.append( hist )
        self.buildStack( histForRatios )
        self.stack.Normalize()
        denom.Normalize()
        self.stack.Divide( denom.obj )
        self.stack.Draw(opt)
        self.ratios = []
        for hist in self.nostack:
            print 'nostack ', hist
            ratio = copy.deepcopy( hist )
            ratio.Normalize()
            ratio.obj.Divide( denom.obj )
            ratio.obj.Draw('same')
            self.ratios.append( ratio )        
        self.DrawLegend()
                
    def DrawStack(self, opt=''):
        self.buildStack(self.sortedHistograms())
        self.stack.Draw( opt )
        for hist in self.nostack:
            hist.Draw('same')
        self.DrawLegend()

    def DrawNormalizedStack(self, opt=''):
        self.buildStack(self.sortedHistograms())
        self.stack.DrawNormalized( opt )
        for hist in self.nostack:
            hist.obj.DrawNormalized('same')
        self.DrawLegend()

    def buildStack(self, hists):
        self.stack = None
        self.stack = Stack(self.name+'_stack')
        self.nostack = []
        for hist in hists:
            if hist.stack:
                self.stack.Add( hist )
            else:
                self.nostack.append(hist)
    
    def __str__(self):
        tmp =  'Plot: ' + self.name + '\n' 
        for hist in self.sortedHistograms():
            tmp += '\t' + str(hist) + '\n'
        return tmp

