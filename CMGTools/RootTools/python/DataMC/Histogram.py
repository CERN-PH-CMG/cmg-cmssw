import copy


class Histogram( object ):
    '''Histogram + a few things.

    This class does not inherit from a ROOT class as we could want to use it
    with a TH1D, TH1F, and even a 2D at some point.

    Histogram contains the original ROOT histogram, obj, and a weighted version,
    weigthed, originally set equal to obj (weight == 1).
    - layer : can be used to order histograms
    - stack : to decide whether the histogram
    should be stacked or not (see the Stack class for more information)
    - name  : user defined histogram. Useful when manipulating several histograms with
    the same GetName(), coming from different TDirectories.
    '''

    def __init__(self, name, obj, layer=0., legendLine=None, stack=True):
        # name is a user defined name
        self.name = name
        if legendLine is None:
            self.legendLine = name
        else:
            self.legendLine = legendLine
        self.obj = obj
        # self.weighted = copy.deepcopy(self.obj)
        self.layer = layer
        self.stack = stack
        self.on = True
        # after construction, weighted histogram = base histogram
        self.SetWeight(1)
        
    def __str__(self):
        fmt = '{self.name:<10} / {hname:<10},\t Layer ={self.layer:8.1f}, w = {weighted:8.1f}, u = {unweighted:8.1f}'
        tmp = fmt.format(self=self,
                         hname = self.obj.GetName(),
                         weighted = self.Yield(weighted=True),
                         unweighted = self.Yield(weighted=False) )
        return tmp

    def Yield(self, weighted=True):
        hist = self.weighted
        if not weighted:
            hist = self.obj
        return hist.Integral( 0, hist.GetNbinsX()+1)

    def Rebin(self, factor):
        self.obj.Rebin( factor )
        self.weighted.Rebin(factor)
    
    def SetWeight(self, weight):
        '''Set the weight and create the weighted histogram.'''
        self.weighted = copy.deepcopy(self.obj)
        self.weight = weight
        self.weighted.Scale(weight)

    def Scale(self, scale):
        '''Scale the histogram (multiply the weight by scale)'''
        self.SetWeight( self.weight * scale )

    def SetStyle(self, style):
        '''Set the style for the original and weighted histograms.'''
        style.formatHisto( self.obj )
        style.formatHisto( self.weighted )

    def AddEntry(self, legend, legendLine=None):
        '''By default the legend entry is set to the name of the histogram.'''
        if legendLine == None:
            legendLine = self.name
        legend.AddEntry(self.obj, legendLine)

    def Draw(self, opt='', weighted=True):
        '''Draw the weighted (or original) histogram.'''
        if weighted is True:
            self.weighted.Draw(opt)
        else:
            self.obj.Draw(opt)

    def GetXaxis(self, opt='', weighted=True):
        '''All these functions could be written in a clever and compact way'''
        if weighted is True:
            return self.weighted.GetXaxis()
        else:
            return self.obj.GetXaxis()  

    def GetYaxis(self, opt='', weighted=True):
        '''All these functions could be written in a clever and compact way'''
        if weighted is True:
            return self.weighted.GetYaxis()
        else:
            return self.obj.GetYaxis()  

    def GetMaximum(self, opt='', weighted=True):
        '''All these functions could be written in a clever and compact way'''
        if weighted is True:
            return self.weighted.GetMaximum()
        else:
            return self.obj.GetMaximum()  
       
    def Add(self, other, coeff=1):
        self.obj.Add( other.obj, coeff )
        self.weighted.Add( other.weighted, coeff )
        return self

    def Integral(self, weighted=True, xmin=None, xmax=None ):
        if type( weighted ) is not bool:
            raise ValueError('weighted should be a boolean')
        if xmin is not None:
            bmin = self.obj.FindFixBin( xmin )
        else:
            bmin = None
        if xmax is not None:
            bmax = self.obj.FindFixBin( xmax ) - 1 
        else:
            bmax = None
        hist = self.weighted
        if weighted is False:
            hist = self.obj
        if bmin is None and bmax is None:
            return hist.Integral(0, hist.GetNbinsX()+1)
        elif bmin is not None and bmax is not None:
            if (xmax - xmin) % self.obj.GetBinWidth(1) != 0:
                raise ValueError('boundaries should define an integer number of bins. nbins=%d, xmin=%3.3f, xmax=%3.3f' % (self.obj.GetNbinsX(), self.obj.GetXaxis().GetXmin(), self.obj.GetXaxis().GetXmax()) )
            return hist.Integral(bmin, bmax)
        else:
            raise ValueError('if specifying one boundary, you must specify the other')


    def DrawNormalized(self):
        '''Draw a normalized version of this histogram.

        The original and weighted histograms stay untouched.'''
        self.obj.DrawNormalized()

    def Normalize(self):
        '''Sets the weight to normalize the weighted histogram to 1.

        In other words, the original histogram stays untouched.'''
        self.SetWeight( 1/self.weighted.Integral() )

