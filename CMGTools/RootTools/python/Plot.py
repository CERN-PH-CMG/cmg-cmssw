from CMGTools.RootTools.Legend import *
from ROOT import TH1, THStack

from operator import itemgetter, attrgetter
import copy

class Histogram:
    def __init__(self, name, obj, layer=0, stack=True):
        self.name = name 
        self.obj = obj
        self.layer = layer
        self.stack = stack
    
    def __str__(self):
        return self.name + ' / ' + self.obj.GetName() + ', L=' + str(self.layer)

    def AddEntry(self, legend, legendLine=None):
        if legendLine == None:
            legendLine = self.name
        legend.AddEntry(self.obj, legendLine)

    def Draw(self):
        self.obj.Draw()

    def DrawNormalized(self):
        self.obj.DrawNormalized()

    def Normalize(self):
        self.obj.Scale( 1/self.obj.Integral() )


class Stack:
    def __init__(self, name):
        self.name = name
        self.hists = []
        self.integral = 0
        
    def Add(self, hist):
        self.hists.append( copy.deepcopy(hist) )
        self.integral += hist.Integral()
        
    def Draw(self, opt=''):
        if len( self.hists )==0:
            return
        self.obj = THStack(self.name,'')
        for hist in self.hists:
            self.obj.Add(hist)
        self.obj.Draw( opt )
        self.updateTitles()

    def DrawNormalized(self, opt ):
        if len( self.hists )==0:
            return
        integral = 0
        self.normHists = []
        self.obj = THStack(self.name,'')
        for hist in self.hists:
            normHist = copy.deepcopy(hist)
            normHist.Scale( 1/self.integral )
            self.normHists.append( normHist )
            self.obj.Add( normHist)
        self.obj.Draw( opt )
        self.updateTitles()

    def Normalize(self):
        for hist in self.hists:
            hist.Scale( 1/self.integral )        

    def Divide(self, otherHist):
        for hist in self.hists:
            hist.Divide(otherHist)
    
    def updateTitles( self ):
        if len( self.hists )==0:
            return        
        self.obj.GetXaxis().SetTitle( self.hists[0].GetXaxis().GetTitle() )
        self.obj.GetYaxis().SetTitle( self.hists[0].GetYaxis().GetTitle() )
        

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
            hist.obj.Draw( same + opt)
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
            hist.obj.Draw('same')
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
                self.stack.Add( hist.obj )
            else:
                self.nostack.append(hist)
    
    def __str__(self):
        tmp =  'Plot: ' + self.name + '\n' 
        for hist in self.sortedHistograms():
            tmp += '\t' + str(hist) + '\n'
        return tmp

