import copy


class Histogram( object ):
    '''Histogram + a few things.

    This class does not inherit from a ROOT class as we could want to use it
    with a TH1D, TH1F, and even a 2D at some point.

    Histograms contain the original ROOT histogram, obj, and a weighted version,
    weigthed, originally set equal to obj (weight == 1).
    - layer : can be used to order histograms
    - stack : to decide whether the histogram
    should be stacked or not (see the Stack class for more information)
    - name  : user defined histogram. Useful when manipulating several histograms with
    the same GetName(), coming from different TDirectories.
    '''

    def __init__(self, name, obj, layer=0, stack=True):
        # name is a user defined name
        self.name = name 
        self.obj = obj
        # self.weighted = copy.deepcopy(self.obj)
        self.layer = layer
        self.stack = stack
        # after construction, weighted histogram = base histogram
        self.SetWeight(1)
        
    def __str__(self):
        return self.name + ' / ' + self.obj.GetName() + ', L=' + str(self.layer)

    def SetWeight(self, weight):
        '''Set the weight and create the weighted histogram.'''
        self.weighted = copy.deepcopy(self.obj)
        self.weight = weight
        self.weighted.Scale(weight)

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

    def Integral(self, weighted=True):
        '''Returns the integral of the weighted (or original) histogram.'''
        if weighted is True:
            return self.weighted.Integral()
        else:
            return self.obj.Integral()

    def DrawNormalized(self):
        '''Draw a normalized version of this histogram.

        The original and weighted histograms stay untouched.'''
        self.obj.DrawNormalized()

    def Normalize(self):
        '''Sets the weight to normalize the weighted histogram to 1.

        In other words, the original histogram stays untouched.'''
        self.SetWeight( 1/self.weighted.Integral() )

