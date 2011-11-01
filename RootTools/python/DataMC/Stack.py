import copy
from ROOT import THStack

class Stack:
    '''Attempt to overcome the defficiencies of the THStack class.

    Contains:
    - hists    : a list of Histogram (from this package,
    we\'re not taking about ROOT histograms here
    - integral : the integral of the stack.'''
    def __init__(self, name):
        self.name = name
        self.hists = []
        self.integral = 0
        
    def Add(self, hist):
        '''Add an Histogram.'''
        # one MUST do a deepcopy here. 
        self.hists.append( copy.deepcopy(hist) )
        self.integral += hist.Integral()
        
    def Draw(self, opt=''):
        '''Draw the stack. opt are the ROOT options'''
        if len( self.hists )==0:
            return
        self.obj = THStack(self.name,'')
        for hist in self.hists:
            self.obj.Add(hist.weighted)
        self.obj.Draw( opt )
        self._updateTitles()

    def DrawNormalized(self, opt ):
        '''Draw a normalized version of the stack (integral=1).'''
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
        self._updateTitles()

    def Normalize(self):
        '''Normalize the stack.

        All histograms in the stack are scaled
        so that the integral of the stack is 1.'''
        for hist in self.hists:
            hist.weighted.Scale( 1/self.integral )        

    def Divide(self, otherHist):
        '''Divide the stack by an histogram.'''
        for hist in self.hists:
            hist.weighted.Divide(otherHist)
    
    def _updateTitles( self ):
        '''Update the axis titles of the stack to the titles of the first histogram in the stack.'''
        if len( self.hists )==0:
            return        
        self.obj.GetXaxis().SetTitle( self.hists[0].obj.GetXaxis().GetTitle() )
        self.obj.GetYaxis().SetTitle( self.hists[0].obj.GetYaxis().GetTitle() )
        
