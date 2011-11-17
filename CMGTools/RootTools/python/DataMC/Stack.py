import copy
from ROOT import THStack, gPad
from CMGTools.RootTools.Style import sBlue,sBlack

class Stack:
    '''Attempt to overcome the defficiencies of the THStack class.

    Contains:
    - hists    : a list of Histogram (from this package,
    we\'re not taking about ROOT histograms here
    - integral : the integral of the stack.'''

    STAT_ERRORS = True
    STYLE = copy.copy(sBlack)
    STYLE.markerStyle = 1 
    
    def __init__(self, name):
        self.name = name
        self.hists = []
        self.integral = 0
        self.totalHist = None
        self.statErrors = Stack.STAT_ERRORS
        self.style = Stack.STYLE
        
    def Add(self, hist):
        '''Add an Histogram.'''
        # one MUST do a deepcopy here. 
        self.hists.append( copy.deepcopy(hist) )
        self.integral += hist.Integral()
        
    def Draw(self, opt='', ymin = None, ymax=None):
        '''Draw the stack. opt are the ROOT options'''
        if len( self.hists )==0:
            return
        self.obj = THStack(self.name,'')
        self.totalHist = None
        for hist in self.hists:
            self.obj.Add(hist.weighted)
            if self.totalHist is None:
                self.totalHist = copy.deepcopy( hist )
            else: 
                self.totalHist.Add( hist )
        self.SetStyle(self.style)
        # drawing the first histogram in the stack
        # as a support histo.
        # otherwise, can't change y axis range (ROOT!@#!)
        # we draw it as hist so that the markers don't appear.
        self.hists[0].Draw('hist')
        self.obj.Draw( opt+'same' )
        # need to redraw the axes, which are now "under"
        # the stacked histograms. 
        self.hists[0].Draw('axissame')
        if ymin is None:
            ymin = 0.1
        if ymax is None:
            ymax = self.totalHist.GetMaximum()*1.1
        self.hists[0].GetYaxis().SetRangeUser( ymin, ymax )
        self._DrawStatErrors()
        self._updateTitles()

    def _DrawStatErrors(self):
        '''Draw statistical errors if statErrors is True.'''
        if self.statErrors is False:
            return
        self.totalHist.Draw('same')
        
    def SetStyle(self, style ):
        '''Set the style for the total histogram'''
        self.style = style 
        self.totalHist.SetStyle( self.style )
        if gPad:
            gPad.Update()

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
        
