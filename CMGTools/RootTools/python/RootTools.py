"""
Load the libraries needed to use RootTools  
"""
# import ROOT as rt

def loadLibs():
    print 'loading FWLite.'
    #load the libaries needed
    from ROOT import gROOT,gSystem
    gSystem.Load("libFWCoreFWLite")
    gROOT.ProcessLine('AutoLibraryLoader::enable();')
    gSystem.Load("libFWCoreFWLite")
    gSystem.Load("libCintex")
    gROOT.ProcessLine('ROOT::Cintex::Cintex::Enable();')
        
    #now the RootTools stuff
    gSystem.Load("libCMGToolsRootTools")

loadLibs()

#now expose the C++ classes with a little documentation
from ROOT import Aliases as CAliases
Aliases = CAliases
Aliases.__doc__ = """
A class for managing branch aliases in TTree instances
"""

from ROOT import Chain as CChain
Chain = CChain
Chain.__doc__ = """
An extention of TChain so that it can take a glob in its constructor
"""

from ROOT import DataSet as CDataSet
DataSet = CDataSet
DataSet.__doc__ = """
Holds a single TTree, with some automatic normalisation support
"""

from ROOT import DataSets as CDataSets
DataSets = CDataSets
DataSets.__doc__ = """
Holds collection of DataSet objects
"""

from ROOT import EventSummary as CEventSummary
EventSummary = CEventSummary
EventSummary.__doc__ = """
Provides a summary of the contents of an edm file (runs, lumis etc)
"""

from ROOT import Plot as CPlot
Plot = CPlot
Plot.__doc__ = """
A TH1 or stack with specific style
"""

from ROOT import Samples as CSamples
Samples = CSamples
Samples.__doc__ = """
Manage different samples. Makes Data/MC comparison easier
"""

from ROOT import StyleAndOpt as CStyleAndOpt
StyleAndOpt = CStyleAndOpt
StyleAndOpt.__doc__ = """
Stores various style options for the Samples class
"""

from ROOT import Styles as CStyles
Styles = CStyles
Styles.__doc__ = """
Stores various style options for the Samples class
"""

from ROOT import TmpHistName as CTmpHistName
TmpHistName = CTmpHistName
TmpHistName.__doc__ = """
Utility class to generate unique histogram names given a label
"""

class stliter(object):
    """
Defines a python iterator for stl types    
    """
    def __init__(self, stl):
        self.stl = stl
        self.index = 0
    
    def __iter__(self):
        return self
    def next(self):
        if self.index < self.stl.size() - 1:
            self.index += 1
        else:
            raise StopIteration()
        return self.stl.at(self.index)
    
    def __len__(self):
        return self.stl.size()
