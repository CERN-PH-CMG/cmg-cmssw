import copy
from ROOT import THStack

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
            self.obj.Add(hist.weighted)
        self.obj.Draw( opt )
        self.updateTitles()

    def DrawNormalized(self, opt ):
        if len( self.hists )==0:
            return
        integral = 0
        self.normHists = []
        self.obj = THStack(self.name,'')
        for hist in self.hists:
            normHist = copy.deepcopy(hist.weighted)
            normHist.Scale( 1/self.integral )
            self.normHists.append( normHist )
            self.obj.Add( normHist)
        self.obj.Draw( opt )
        self.updateTitles()

    def Normalize(self):
        for hist in self.hists:
            hist.weighted.Scale( 1/self.integral )        

    def Divide(self, otherHist):
        for hist in self.hists:
            hist.weighted.Divide(otherHist)
    
    def updateTitles( self ):
        if len( self.hists )==0:
            return        
        self.obj.GetXaxis().SetTitle( self.hists[0].obj.GetXaxis().GetTitle() )
        self.obj.GetYaxis().SetTitle( self.hists[0].obj.GetYaxis().GetTitle() )
        
